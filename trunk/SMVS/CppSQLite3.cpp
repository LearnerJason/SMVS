////////////////////////////////////////////////////////////////////////////////
// CppSQLite3 - A C++ wrapper around the SQLite3 embedded database library.
#include "stdafx.h"
#include "CppSQLite3.h"
#include <cstdlib>

// Named constant for passing to CppSQLite3Exception when passing it a string
// that cannot be deleted.
static const bool DONT_DELETE_MSG=false;

////////////////////////////////////////////////////////////////////////////////
int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out);
int sqlite3_decode_binary(const unsigned char *in, unsigned char *out);

CppSQLite3Exception::CppSQLite3Exception(const int nErrCode,char* strErrMsg,bool bDeleteMsg/*=true*/):m_nErrCode(nErrCode)
{
	m_strErrMsg = sqlite3_mprintf("%s[%d]: %s",errorCodeAsString(nErrCode),nErrCode,strErrMsg ? strErrMsg : "");

	if (bDeleteMsg && strErrMsg)
	{
		sqlite3_free(strErrMsg);
	}
}
								
CppSQLite3Exception::CppSQLite3Exception(const CppSQLite3Exception&  e):m_nErrCode(e.m_nErrCode)
{
	m_strErrMsg = 0;
	if (e.m_strErrMsg)
	{
		m_strErrMsg = sqlite3_mprintf("%s", e.m_strErrMsg);
	}
}

const char* CppSQLite3Exception::errorCodeAsString(int nErrCode)
{
	switch (nErrCode)
	{
		case SQLITE_OK          : return "SQLITE_OK";
		case SQLITE_ERROR       : return "SQLITE_ERROR";
		case SQLITE_INTERNAL    : return "SQLITE_INTERNAL";
		case SQLITE_PERM        : return "SQLITE_PERM";
		case SQLITE_ABORT       : return "SQLITE_ABORT";
		case SQLITE_BUSY        : return "SQLITE_BUSY";
		case SQLITE_LOCKED      : return "SQLITE_LOCKED";
		case SQLITE_NOMEM       : return "SQLITE_NOMEM";
		case SQLITE_READONLY    : return "SQLITE_READONLY";
		case SQLITE_INTERRUPT   : return "SQLITE_INTERRUPT";
		case SQLITE_IOERR       : return "SQLITE_IOERR";
		case SQLITE_CORRUPT     : return "SQLITE_CORRUPT";
		case SQLITE_NOTFOUND    : return "SQLITE_NOTFOUND";
		case SQLITE_FULL        : return "SQLITE_FULL";
		case SQLITE_CANTOPEN    : return "SQLITE_CANTOPEN";
		case SQLITE_PROTOCOL    : return "SQLITE_PROTOCOL";
		case SQLITE_EMPTY       : return "SQLITE_EMPTY";
		case SQLITE_SCHEMA      : return "SQLITE_SCHEMA";
		case SQLITE_TOOBIG      : return "SQLITE_TOOBIG";
		case SQLITE_CONSTRAINT  : return "SQLITE_CONSTRAINT";
		case SQLITE_MISMATCH    : return "SQLITE_MISMATCH";
		case SQLITE_MISUSE      : return "SQLITE_MISUSE";
		case SQLITE_NOLFS       : return "SQLITE_NOLFS";
		case SQLITE_AUTH        : return "SQLITE_AUTH";
		case SQLITE_FORMAT      : return "SQLITE_FORMAT";
		case SQLITE_RANGE       : return "SQLITE_RANGE";
		case SQLITE_ROW         : return "SQLITE_ROW";
		case SQLITE_DONE        : return "SQLITE_DONE";
		case CPPSQLITE_ERROR    : return "CPPSQLITE_ERROR";
		default: return "UNKNOWN_ERROR";
	}
}

CppSQLite3Exception::~CppSQLite3Exception()
{
	if (m_strErrMsg)
	{
		sqlite3_free(m_strErrMsg);
		m_strErrMsg = nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////

CppSQLite3Buffer::CppSQLite3Buffer()
{
	m_strBuffer = nullptr;
}

CppSQLite3Buffer::~CppSQLite3Buffer()
{
	clear();
}

void CppSQLite3Buffer::clear()
{
	if(m_strBuffer!= nullptr)
	{
		sqlite3_free(m_strBuffer);
		m_strBuffer = nullptr;
	}
}

const char* CppSQLite3Buffer::format(const char* szFormat, ...)
{
	clear();
	va_list va;
	va_start(va, szFormat);
	m_strBuffer = sqlite3_vmprintf(szFormat, va);
	va_end(va);
	return m_strBuffer;
}
////////////////////////////////////////////////////////////////////////////////
CppSQLite3Binary::CppSQLite3Binary() :
						m_strBuffer(0),
						m_nBinaryLen(0),
						m_nBufferLen(0),
						m_nEncodedLen(0),
						m_bEncoded(false)
{
}

CppSQLite3Binary::~CppSQLite3Binary()
{
	clear();
}

void CppSQLite3Binary::setBinary(const unsigned char* pBuf, int nLen)
{
	m_strBuffer = allocBuffer(nLen);
	memcpy(m_strBuffer, pBuf, nLen);
}

void CppSQLite3Binary::setEncoded(const unsigned char* pBuf)
{
	clear();
	m_nEncodedLen = (int)strlen((const char*)pBuf);
	m_nBufferLen = m_nEncodedLen + 1; // Allow for NULL terminator

	m_strBuffer = (unsigned char*)malloc(m_nBufferLen);

	if (!m_strBuffer)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Cannot allocate memory",DONT_DELETE_MSG);
	}

	memcpy(m_strBuffer, pBuf, m_nBufferLen);
	m_bEncoded = true;
}

const unsigned char* CppSQLite3Binary::getEncoded()
{
	if (!m_bEncoded)
	{
		unsigned char* ptmp = (unsigned char*)malloc(m_nBinaryLen);
		memcpy(ptmp, m_strBuffer, m_nBinaryLen);
		m_nEncodedLen = sqlite3_encode_binary(ptmp, m_nBinaryLen, m_strBuffer);
		free(ptmp);
		m_bEncoded = true;
	}

	return m_strBuffer;
}

const unsigned char* CppSQLite3Binary::getBinary()
{
	if (m_bEncoded)
	{
		// in/out buffers can be the same
		m_nBinaryLen = sqlite3_decode_binary(m_strBuffer, m_strBuffer);

		if (m_nBinaryLen == -1)
		{
			throw CppSQLite3Exception(CPPSQLITE_ERROR,"Cannot decode binary",DONT_DELETE_MSG);
		}
		m_bEncoded = false;
	}
	return m_strBuffer;
}

int CppSQLite3Binary::getBinaryLength()
{
	getBinary();
	return m_nBinaryLen;
}

unsigned char* CppSQLite3Binary::allocBuffer(int nLen)
{
	clear();

	// Allow extra space for encoded binary as per comments in
	// SQLite encode.c See bottom of this file for implementation
	// of SQLite functions use 3 instead of 2 just to be sure ;-)
	m_nBinaryLen = nLen;
	m_nBufferLen = 3 + (257*nLen)/254;

	m_strBuffer = (unsigned char*)malloc(m_nBufferLen);

	if (!m_strBuffer)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Cannot allocate memory", DONT_DELETE_MSG);
	}
	m_bEncoded = false;
	return m_strBuffer;
}

void CppSQLite3Binary::clear()
{
	if (m_strBuffer != nullptr)
	{
		m_nBinaryLen = 0;
		m_nBufferLen = 0;
		free(m_strBuffer);
		m_strBuffer = nullptr;
	}
}

CppSQLite3Query::CppSQLite3Query()
{
	m_ptrStmt = 0;
	m_bEof = true;
	m_nCols = 0;
	m_bOwnVM = false;
}

CppSQLite3Query::CppSQLite3Query(const CppSQLite3Query& rQuery)
{
	m_ptrStmt = rQuery.m_ptrStmt;
	// Only one object can own the VM
	const_cast<CppSQLite3Query&>(rQuery).m_ptrStmt = 0;
	m_bEof = rQuery.m_bEof;
	m_nCols = rQuery.m_nCols;
	m_bOwnVM = rQuery.m_bOwnVM;
}

CppSQLite3Query::CppSQLite3Query(sqlite3* pDB, sqlite3_stmt* pVM, bool bEof, bool bOwnVM/*=true*/)
{
	m_ptrSqlite3 = pDB;
	m_ptrStmt = pVM;
	m_bEof = bEof;
	m_nCols = sqlite3_column_count(m_ptrStmt);
	m_bOwnVM = bOwnVM;
}

CppSQLite3Query::~CppSQLite3Query()
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
}

CppSQLite3Query& CppSQLite3Query::operator=(const CppSQLite3Query& rQuery)
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
	m_ptrStmt = rQuery.m_ptrStmt;
	// Only one object can own the VM
	const_cast<CppSQLite3Query&>(rQuery).m_ptrStmt = 0;
	m_bEof = rQuery.m_bEof;
	m_nCols = rQuery.m_nCols;
	m_bOwnVM = rQuery.m_bOwnVM;
	return *this;
}

int CppSQLite3Query::numFields()
{
	checkVM();
	return m_nCols;
}

const char* CppSQLite3Query::fieldValue(int nField)
{
	checkVM();

	if (nField < 0 || nField > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
	}

	return (const char*)sqlite3_column_text(m_ptrStmt, nField);
}

const char* CppSQLite3Query::fieldValue(const char* szField)
{
	int nField = fieldIndex(szField);
	return (const char*)sqlite3_column_text(m_ptrStmt, nField);
}

int CppSQLite3Query::getIntField(int nField, int nNullValue/*=0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return sqlite3_column_int(m_ptrStmt, nField);
	}
}

int CppSQLite3Query::getIntField(const char* szField, int nNullValue/*=0*/)
{
	int nField = fieldIndex(szField);
	return getIntField(nField, nNullValue);
}

sqlite_int64 CppSQLite3Query::getInt64Field(int nField, sqlite_int64 nNullValue/*=0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return sqlite3_column_int64(m_ptrStmt, nField);
	}
}

sqlite_int64 CppSQLite3Query::getInt64Field(const char* szField, sqlite_int64 nNullValue/*=0*/)
{
	int nField = fieldIndex(szField);
	return getInt64Field(nField, nNullValue);
}

double CppSQLite3Query::getFloatField(int nField, double fNullValue/*=0.0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return fNullValue;
	}
	else
	{
		return sqlite3_column_double(m_ptrStmt, nField);
	}
}

double CppSQLite3Query::getFloatField(const char* szField, double fNullValue/*=0.0*/)
{
	int nField = fieldIndex(szField);
	return getFloatField(nField, fNullValue);
}

const char* CppSQLite3Query::getStringField(int nField, const char* szNullValue/*=""*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return szNullValue;
	}
	else
	{
		return (const char*)sqlite3_column_text(m_ptrStmt, nField);
	}
}

const char* CppSQLite3Query::getStringField(const char* szField, const char* szNullValue/*=""*/)
{
	int nField = fieldIndex(szField);
	return getStringField(nField, szNullValue);
}

const unsigned char* CppSQLite3Query::getBlobField(int nField, int& nLen)
{
	checkVM();

	if (nField < 0 || nField > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
	}

	nLen = sqlite3_column_bytes(m_ptrStmt, nField);
	return (const unsigned char*)sqlite3_column_blob(m_ptrStmt, nField);
}

const unsigned char* CppSQLite3Query::getBlobField(const char* szField, int& nLen)
{
	int nField = fieldIndex(szField);
	return getBlobField(nField, nLen);
}

bool CppSQLite3Query::fieldIsNull(int nField)
{
	return (fieldDataType(nField) == SQLITE_NULL);
}

bool CppSQLite3Query::fieldIsNull(const char* szField)
{
	int nField = fieldIndex(szField);
	return (fieldDataType(nField) == SQLITE_NULL);
}

int CppSQLite3Query::fieldIndex(const char* szField)
{
	checkVM();

	if (szField)
	{
		for (int nField = 0; nField < m_nCols; nField++)
		{
			const char* szTemp = sqlite3_column_name(m_ptrStmt, nField);

			if (strcmp(szField, szTemp) == 0)
			{
				return nField;
			}
		}
	}
	throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field name requested", DONT_DELETE_MSG);
}

const char* CppSQLite3Query::fieldName(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
	}

	return sqlite3_column_name(m_ptrStmt, nCol);
}

const char* CppSQLite3Query::fieldDeclType(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
	}

	return sqlite3_column_decltype(m_ptrStmt, nCol);
}

int CppSQLite3Query::fieldDataType(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,  "Invalid field index requested", DONT_DELETE_MSG);
	}

	return sqlite3_column_type(m_ptrStmt, nCol);
}

bool CppSQLite3Query::eof()
{
	checkVM();
	return m_bEof;
}

void CppSQLite3Query::NextRow()
{
	checkVM();

	int nRet = sqlite3_step(m_ptrStmt);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		m_bEof = true;
	}
	else if (nRet == SQLITE_ROW)
	{
		// more rows, nothing to do
	}
	else
	{
		nRet = sqlite3_finalize(m_ptrStmt);
		m_ptrStmt = 0;
		const char* szError = sqlite3_errmsg(m_ptrSqlite3);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}


void CppSQLite3Query::finalize()
{
	if (m_ptrStmt && m_bOwnVM)
	{
		int nRet = sqlite3_finalize(m_ptrStmt);
		m_ptrStmt = 0;
		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(m_ptrSqlite3);
			throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
}

void CppSQLite3Query::checkVM()
{
	if (m_ptrStmt == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Null Virtual Machine pointer",DONT_DELETE_MSG);
	}
}

CppSQLite3Table::CppSQLite3Table()
{
	m_pStrResults = 0;
	m_nRows = 0;
	m_nCols = 0;
	m_nCurrentRow = 0;
}

CppSQLite3Table::CppSQLite3Table(const CppSQLite3Table& rTable)
{
	m_pStrResults = rTable.m_pStrResults;
	// Only one object can own the results
	const_cast<CppSQLite3Table&>(rTable).m_pStrResults = 0;
	m_nRows = rTable.m_nRows;
	m_nCols = rTable.m_nCols;
	m_nCurrentRow = rTable.m_nCurrentRow;
}

CppSQLite3Table::CppSQLite3Table(char** paszResults, int nRows, int nCols)
{
	m_pStrResults = paszResults;
	m_nRows = nRows;
	m_nCols = nCols;
	m_nCurrentRow = 0;
}

CppSQLite3Table::~CppSQLite3Table()
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
}

CppSQLite3Table& CppSQLite3Table::operator=(const CppSQLite3Table& rTable)
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
	m_pStrResults = rTable.m_pStrResults;
	// Only one object can own the results
	const_cast<CppSQLite3Table&>(rTable).m_pStrResults = 0;
	m_nRows = rTable.m_nRows;
	m_nCols = rTable.m_nCols;
	m_nCurrentRow = rTable.m_nCurrentRow;
	return *this;
}

void CppSQLite3Table::finalize()
{
	if (m_pStrResults)
	{
		sqlite3_free_table(m_pStrResults);
		m_pStrResults = 0;
	}
}

int CppSQLite3Table::numFields()
{
	checkResults();
	return m_nCols;
}

int CppSQLite3Table::numRows()
{
	checkResults();
	return m_nRows;
}

const char* CppSQLite3Table::fieldValue(int nField)
{
	checkResults();

	if (nField < 0 || nField > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Invalid field index requested",DONT_DELETE_MSG);
	}

	int nIndex = (m_nCurrentRow*m_nCols) + m_nCols + nField;
	return m_pStrResults[nIndex];
}

const char* CppSQLite3Table::fieldValue(const char* szField)
{
	checkResults();

	if (szField)
	{
		for (int nField = 0; nField < m_nCols; nField++)
		{
			if (strcmp(szField, m_pStrResults[nField]) == 0)
			{
				int nIndex = (m_nCurrentRow*m_nCols) + m_nCols + nField;
				return m_pStrResults[nIndex];
			}
		}
	}

	throw CppSQLite3Exception(CPPSQLITE_ERROR,"Invalid field name requested",DONT_DELETE_MSG);
}

int CppSQLite3Table::getIntField(int nField, int nNullValue/*=0*/)
{
	if (fieldIsNull(nField))
	{
		return nNullValue;
	}
	else
	{
		return atoi(fieldValue(nField));
	}
}

int CppSQLite3Table::getIntField(const char* szField, int nNullValue/*=0*/)
{
	if (fieldIsNull(szField))
	{
		return nNullValue;
	}
	else
	{
		return atoi(fieldValue(szField));
	}
}

double CppSQLite3Table::getFloatField(int nField, double fNullValue/*=0.0*/)
{
	if (fieldIsNull(nField))
	{
		return fNullValue;
	}
	else
	{
		return atof(fieldValue(nField));
	}
}

double CppSQLite3Table::getFloatField(const char* szField, double fNullValue/*=0.0*/)
{
	if (fieldIsNull(szField))
	{
		return fNullValue;
	}
	else
	{
		return atof(fieldValue(szField));
	}
}

const char* CppSQLite3Table::getStringField(int nField, const char* szNullValue/*=""*/)
{
	if (fieldIsNull(nField))
	{
		return szNullValue;
	}
	else
	{
		return fieldValue(nField);
	}
}

const char* CppSQLite3Table::getStringField(const char* szField, const char* szNullValue/*=""*/)
{
	if (fieldIsNull(szField))
	{
		return szNullValue;
	}
	else
	{
		return fieldValue(szField);
	}
}

bool CppSQLite3Table::fieldIsNull(int nField)
{
	checkResults();
	return (fieldValue(nField) == 0);
}

bool CppSQLite3Table::fieldIsNull(const char* szField)
{
	checkResults();
	return (fieldValue(szField) == 0);
}

const char* CppSQLite3Table::fieldName(int nCol)
{
	checkResults();

	if (nCol < 0 || nCol > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Invalid field index requested",DONT_DELETE_MSG);
	}

	return m_pStrResults[nCol];
}

void CppSQLite3Table::setRow(int nRow)
{
	checkResults();

	if (nRow < 0 || nRow > m_nRows-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Invalid row index requested",DONT_DELETE_MSG);
	}
	m_nCurrentRow = nRow;
}

void CppSQLite3Table::checkResults()
{
	if (m_pStrResults == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Null Results pointer",DONT_DELETE_MSG);
	}
}

CppSQLite3Statement::CppSQLite3Statement()
{
	m_ptrSqlite3 = 0;
	m_ptrStmt = 0;
}

CppSQLite3Statement::CppSQLite3Statement(const CppSQLite3Statement& rStatement)
{
	m_ptrSqlite3 = rStatement.m_ptrSqlite3;
	m_ptrStmt = rStatement.m_ptrStmt;
	// Only one object can own VM
	const_cast<CppSQLite3Statement&>(rStatement).m_ptrStmt = 0;
}


CppSQLite3Statement::CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM)
{
	m_ptrSqlite3 = pDB;
	m_ptrStmt = pVM;
}

CppSQLite3Statement::~CppSQLite3Statement()
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
}

CppSQLite3Statement& CppSQLite3Statement::operator=(const CppSQLite3Statement& rStatement)
{
	m_ptrSqlite3 = rStatement.m_ptrSqlite3;
	m_ptrStmt = rStatement.m_ptrStmt;
	// Only one object can own VM
	const_cast<CppSQLite3Statement&>(rStatement).m_ptrStmt = 0;
	return *this;
}

int CppSQLite3Statement::execDML()
{
	checkDB();
	checkVM();

	const char* szError=0;

	int nRet = sqlite3_step(m_ptrStmt);

	if (nRet == SQLITE_DONE)
	{
		int nRowsChanged = sqlite3_changes(m_ptrSqlite3);

		nRet = sqlite3_reset(m_ptrStmt);

		if (nRet != SQLITE_OK)
		{
			szError = sqlite3_errmsg(m_ptrSqlite3);
			throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}

		return nRowsChanged;
	}
	else
	{
		nRet = sqlite3_reset(m_ptrStmt);
		szError = sqlite3_errmsg(m_ptrSqlite3);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}

CppSQLite3Query CppSQLite3Statement::execQuery()
{
	checkDB();
	checkVM();

	int nRet = sqlite3_step(m_ptrStmt);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		return CppSQLite3Query(m_ptrSqlite3, m_ptrStmt, true/*eof*/, false);
	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		return CppSQLite3Query(m_ptrSqlite3, m_ptrStmt, false/*eof*/, false);
	}
	else
	{
		nRet = sqlite3_reset(m_ptrStmt);
		const char* szError = sqlite3_errmsg(m_ptrSqlite3);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::bind(int nParam, const char* szValue)
{
	checkVM();
	int nRes = sqlite3_bind_text(m_ptrStmt, nParam, szValue, -1, SQLITE_TRANSIENT);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,"Error binding string param",DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::bind(int nParam, const int nValue)
{
	checkVM();
	int nRes = sqlite3_bind_int(m_ptrStmt, nParam, nValue);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,"Error binding int param",DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::bind(int nParam, const double dValue)
{
	checkVM();
	int nRes = sqlite3_bind_double(m_ptrStmt, nParam, dValue);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,"Error binding double param",DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::bind(int nParam, const unsigned char* blobValue, int nLen)
{
	checkVM();
	int nRes = sqlite3_bind_blob(m_ptrStmt, nParam,
								(const void*)blobValue, nLen, SQLITE_TRANSIENT);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,"Error binding blob param",DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::bindNull(int nParam)
{
	checkVM();
	int nRes = sqlite3_bind_null(m_ptrStmt, nParam);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,"Error binding NULL param",DONT_DELETE_MSG);
	}
}

int CppSQLite3Statement::bindParameterIndex(const char* szParam)
{
	checkVM();

	int nParam = sqlite3_bind_parameter_index(m_ptrStmt, szParam);

int nn = sqlite3_bind_parameter_count(m_ptrStmt);
const char* sz1 = sqlite3_bind_parameter_name(m_ptrStmt, 1);
const char* sz2 = sqlite3_bind_parameter_name(m_ptrStmt, 2);

	if (!nParam)
	{
		char buf[128];
		sprintf_s(buf, "Parameter '%s' is not valid for this statement", szParam);
		throw CppSQLite3Exception(CPPSQLITE_ERROR, buf, DONT_DELETE_MSG);
	}

	return nParam;
}

void CppSQLite3Statement::bind(const char* szParam, const char* szValue)
{
	int nParam = bindParameterIndex(szParam);
	bind(nParam, szValue);
}

void CppSQLite3Statement::bind(const char* szParam, const int nValue)
{
	int nParam = bindParameterIndex(szParam);
	bind(nParam, nValue);
}

void CppSQLite3Statement::bind(const char* szParam, const double dwValue)
{
	int nParam = bindParameterIndex(szParam);
	bind(nParam, dwValue);
}

void CppSQLite3Statement::bind(const char* szParam, const unsigned char* blobValue, int nLen)
{
	int nParam = bindParameterIndex(szParam);
	bind(nParam, blobValue, nLen);
}

void CppSQLite3Statement::bindNull(const char* szParam)
{
	int nParam = bindParameterIndex(szParam);
	bindNull(nParam);
}

void CppSQLite3Statement::reset()
{
	if (m_ptrStmt)
	{
		int nRet = sqlite3_reset(m_ptrStmt);

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(m_ptrSqlite3);
			throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
}

void CppSQLite3Statement::finalize()
{
	if (m_ptrStmt)
	{
		int nRet = sqlite3_finalize(m_ptrStmt);
		m_ptrStmt = 0;

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(m_ptrSqlite3);
			throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
}

void CppSQLite3Statement::checkDB()
{
	if (m_ptrSqlite3 == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Database not open",DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::checkVM()
{
	if (m_ptrStmt == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Null Virtual Machine pointer",DONT_DELETE_MSG);
	}
}

//////////////////////////////////////////////////////////////////////////////

CppSQLite3DB::CppSQLite3DB()
{
	m_ptrSqlite3 = nullptr;
	m_nBusyTimeoutMs = 20000; // 60 seconds
}

CppSQLite3DB::CppSQLite3DB(const CppSQLite3DB& db)
{
	m_ptrSqlite3 = db.m_ptrSqlite3;
	m_nBusyTimeoutMs = 20000; // 60 seconds
}

CppSQLite3DB::~CppSQLite3DB()
{
	try
	{
		close();
	}
	catch (...)
	{
	}
}

CppSQLite3DB& CppSQLite3DB::operator=(const CppSQLite3DB& db)
{
	m_ptrSqlite3 = db.m_ptrSqlite3;
	m_nBusyTimeoutMs = 20000; // 60 seconds
	return *this;
}

void CppSQLite3DB::open(const char* szFile)
{
	int nRet = sqlite3_open(szFile, &m_ptrSqlite3);
	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(m_ptrSqlite3);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
	setBusyTimeout(m_nBusyTimeoutMs);
}

void CppSQLite3DB::close()
{
	if (m_ptrSqlite3)
	{
		if (sqlite3_close(m_ptrSqlite3) == SQLITE_OK)
		{
			m_ptrSqlite3 = nullptr;
		}
		else
		{
			throw CppSQLite3Exception(CPPSQLITE_ERROR,"Unable to close database",DONT_DELETE_MSG);
		}
	}
}

CppSQLite3Statement CppSQLite3DB::compileStatement(const char* szSQL)
{
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);
	return CppSQLite3Statement(m_ptrSqlite3, pVM);
}

bool CppSQLite3DB::tableExists(const char* szTable)
{
	char szSQL[256];
	sprintf_s(szSQL,"select count(*) from sqlite_master where type='table' and name='%s'", szTable);
	int nRet = -1;
	try
	{
		nRet = execScalar(szSQL);
	}
	catch(CppSQLite3Exception&)
	{
		return false;
	}
	return (nRet > 0);
}

int CppSQLite3DB::execDML(const char* szSQL)
{
	checkDB();

	char* szError = 0;

	int nRet = sqlite3_exec(m_ptrSqlite3, szSQL, 0, 0, &szError);

	if (nRet != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRet, szError);
	}
	return nRet;
}

CppSQLite3Query CppSQLite3DB::execQuery(const char* szSQL)
{
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);

	int nRet = sqlite3_step(pVM);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		return CppSQLite3Query(m_ptrSqlite3, pVM, true/*eof*/);
	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		return CppSQLite3Query(m_ptrSqlite3, pVM, false/*eof*/);
	}
	else
	{
		nRet = sqlite3_finalize(pVM);
		const char* szError= sqlite3_errmsg(m_ptrSqlite3);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}

int CppSQLite3DB::execScalar(const char* szSQL, int nNullValue/*=0*/)
{
	CppSQLite3Query q = execQuery(szSQL);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Invalid scalar query",DONT_DELETE_MSG);
	}

	return q.getIntField(0, nNullValue);
}

CppSQLite3Table CppSQLite3DB::getTable(const char* szSQL)
{
	checkDB();

	char* szError = 0;
	char** paszResults = 0;
	int nRows(0);
	int nCols(0);

	int nRet = sqlite3_get_table(m_ptrSqlite3, szSQL, &paszResults, &nRows, &nCols, &szError);

	if (nRet == SQLITE_OK)
	{
		return CppSQLite3Table(paszResults, nRows, nCols);
	}
	else
	{
		throw CppSQLite3Exception(nRet, szError);
	}
}

sqlite_int64 CppSQLite3DB::lastRowId()
{
	return sqlite3_last_insert_rowid(m_ptrSqlite3);
}

void CppSQLite3DB::setBusyTimeout(int nMillisecs)
{
	m_nBusyTimeoutMs = nMillisecs;
	sqlite3_busy_timeout(m_ptrSqlite3, m_nBusyTimeoutMs);
}

void CppSQLite3DB::checkDB()
{
	if (!m_ptrSqlite3)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,"Database not open",DONT_DELETE_MSG);
	}
}

sqlite3_stmt* CppSQLite3DB::compile(const char* szSQL)
{
	checkDB();

	const char* szTail=0;
	sqlite3_stmt* pVM;

	int nRet = sqlite3_prepare_v2(m_ptrSqlite3, szSQL, -1, &pVM, &szTail);

	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(m_ptrSqlite3);
		throw CppSQLite3Exception(nRet,(char*)szError, DONT_DELETE_MSG);
	}

	return pVM;
}

bool CppSQLite3DB::IsAutoCommitOn()
{
	checkDB();
	return sqlite3_get_autocommit(m_ptrSqlite3) ? true : false;
}

int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out)
{
  int i, j, e, m;
  int cnt[256];
  if( n<=0 )
  {
    out[0] = 'x';
    out[1] = 0;
    return 1;
  }
  memset(cnt, 0, sizeof(cnt));
  for(i=n-1; i>=0; i--)
  {
	  cnt[in[i]]++;
  }
  m = n;
  for(i=1; i<256; i++)
  {
    int sum;
    if( i=='\'' )
		continue;
    sum = cnt[i] + cnt[(i+1)&0xff] + cnt[(i+'\'')&0xff];
    if( sum<m )
	{
      m = sum;
      e = i;
      if( m==0 ) 
		  break;
    }
  }
  out[0] = e;
  j = 1;
  for(i=0; i<n; i++)
  {
    int c = (in[i] - e)&0xff;
    if( c==0 )
	{
      out[j++] = 1;
      out[j++] = 1;
    }
	else if( c==1 )
	{
      out[j++] = 1;
      out[j++] = 2;
    }
	else if( c=='\'' )
	{
      out[j++] = 1;
      out[j++] = 3;
    }
	else
	{
      out[j++] = c;
    }
  }
  out[j] = 0;
  return j;
}

/*
** Decode the string "in" into binary data and write it into "out".
** This routine reverses the encoding created by sqlite3_encode_binary().
** The output will always be a few bytes less than the input.  The number
** of bytes of output is returned.  If the input is not a well-formed
** encoding, -1 is returned.
**
** The "in" and "out" parameters may point to the same buffer in order
** to decode a string in place.
*/
int sqlite3_decode_binary(const unsigned char *in, unsigned char *out){
  int i, c, e;
  e = *(in++);
  i = 0;
  while( (c = *(in++))!=0 )
  {
    if( c==1 )
	{
      c = *(in++);
      if( c==1 )
	  {
        c = 0;
      }
	  else if( c==2 )
	  {
        c = 1;
      }
	  else if( c==3 )
	  {
        c = '\'';
      }
	  else
	  {
        return -1;
      }
    }
    out[i++] = (c + e)&0xff;
  }
  return i;
}
