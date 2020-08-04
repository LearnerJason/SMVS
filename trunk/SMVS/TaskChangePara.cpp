#include "StdAfx.h"
#include "TaskChangePara.h"



CTaskChangePara::CTaskChangePara(void)
{
	m_pSheet = NULL;
#ifdef 	CAP_PRODUCT

	BasicExcel e;//

	if (!e.Load("CapPara.xls"))
	{
		AfxMessageBox(_T("Program cannot open CapPara.xls"));
		return;
	}
	else
	{
		int i = 0;
		m_pSheet = e.GetWorksheet(i);
	}
	InitData();

	
#endif

}
bool CTaskChangePara::InitData()
{
	GetVectorData(m_vecS1HBegin,	4,1,21,1);
	GetVectorData(m_vecS1HEnd,		4,2,21,2);
	GetVectorData(m_vecS1HLight,	4,3,21,3);
	GetVectorData(m_vecS1HTestBegin,4,4,21,4);
	GetVectorData(m_vecS1HTestEnd,	4,5,21,5);

	//
	GetVectorData(m_vecS2HBegin,	4,6,12,6);
	GetVectorData(m_vecS2HEnd,		4,7,12,7);
	GetVectorData(m_vecS2HLight,	4,8,12,8);
	GetVectorData(m_vecS2HTestBegin,4,9,48,9);
	GetVectorData(m_vecS2HTestEnd,	4,10,48,10);

	GetVectorData(m_vecExpppType,	4,12,48,12);

	GetData(m_diameter,4,11);

	return true;
}

bool CTaskChangePara::GetData(double& data,int row,int col)
{
	ASSERT(m_pSheet);
	size_t nMaxRows = m_pSheet->GetTotalRows();
	size_t nMaxCols = m_pSheet->GetTotalCols();
	BasicExcelCell* pCell = m_pSheet->Cell(row-1, col-1);

	switch (pCell->Type())
	{
	case BasicExcelCell::UNDEFINED:
		return true;
		break;

	case BasicExcelCell::INT:
		//	str.Format(_T("%d"), pCell->GetInteger());
		break;

	case BasicExcelCell::DOUBLE:
		data = pCell->GetDouble();
		break;

	case BasicExcelCell::STRING:
		{
#ifdef UNICODE
			ULONG nLength = strlen(pCell->GetString());
			TCHAR* pszBuffer = new TCHAR[nLength + 1];
			_mbstowcsz(pszBuffer, pCell->GetString(), nLength + 1);
			delete[] pszBuffer;

#else
			str.Format(_T("%s"), pCell->GetString());
#endif
			break;
		}
	case BasicExcelCell::WSTRING:
		{
#ifndef UNICODE
			int nLength = wcslen(pCell->GetWString()) * 2;
			TCHAR* pszBuffer = new TCHAR[nLength + 1];
			_wcstombsz(pszBuffer, pCell->GetWString(), nLength + 1);
			str.Format(_T("%s"), pszBuffer);
			delete[] pszBuffer;
#else
	
#endif
			break;
		}
	}


}
bool CTaskChangePara::GetVectorData(vector<CString> &m_vec,int row1,int col1,int row2,int col2)
{ 
	ASSERT(m_pSheet);
	m_vec.clear();

	// 	ASSERT(row1<=0);
	// 	ASSERT(row2<=0);
	// 	ASSERT(col1<=0);
	// 	ASSERT(col2<=0);

	size_t nMaxRows = m_pSheet->GetTotalRows();
	size_t nMaxCols = m_pSheet->GetTotalCols();

	int j = row1-1;//
	while (j < nMaxRows)
	{
		CString str;
		BasicExcelCell* pCell = m_pSheet->Cell(j, col1-1);

		switch (pCell->Type())
		{
		case BasicExcelCell::UNDEFINED:
			return true;
			break;

		case BasicExcelCell::INT:
			//	str.Format(_T("%d"), pCell->GetInteger());
			break;

		case BasicExcelCell::DOUBLE:
			break;

		case BasicExcelCell::STRING:
			{
#ifdef UNICODE
				ULONG nLength = strlen(pCell->GetString());
				TCHAR* pszBuffer = new TCHAR[nLength + 1];
				_mbstowcsz(pszBuffer, pCell->GetString(), nLength + 1);
				str.Format(_T("%s"), pszBuffer);
				m_vec.push_back(str);
				delete[] pszBuffer;
				
#else
				str.Format(_T("%s"), pCell->GetString());
#endif
				break;
			}
		case BasicExcelCell::WSTRING:
			{
#ifndef UNICODE
				int nLength = wcslen(pCell->GetWString()) * 2;
				TCHAR* pszBuffer = new TCHAR[nLength + 1];
				_wcstombsz(pszBuffer, pCell->GetWString(), nLength + 1);
				str.Format(_T("%s"), pszBuffer);
				delete[] pszBuffer;
#else
				str.Format(_T("%s"), pCell->GetWString());
				m_vec.push_back(str);
#endif
				break;
			}
		}
		j++;
	}

	return true;

}
bool CTaskChangePara::GetVectorData(vector<double> &m_vec,int row1,int col1,int row2,int col2)
{ 
	ASSERT(m_pSheet);
	m_vec.clear();

// 	ASSERT(row1<=0);
// 	ASSERT(row2<=0);
// 	ASSERT(col1<=0);
// 	ASSERT(col2<=0);
	
	size_t nMaxRows = m_pSheet->GetTotalRows();
	size_t nMaxCols = m_pSheet->GetTotalCols();

	int j = row1-1;//
	while (j < nMaxRows)
		{
			BasicExcelCell* pCell = m_pSheet->Cell(j, col1-1);

			switch (pCell->Type())
			{
			case BasicExcelCell::UNDEFINED:
					return true;
				break;

			case BasicExcelCell::INT:
			//	str.Format(_T("%d"), pCell->GetInteger());
				break;

			case BasicExcelCell::DOUBLE:
				m_vec.push_back(pCell->GetDouble());
				break;

			case BasicExcelCell::STRING:
				{
#ifdef UNICODE
					ULONG nLength = strlen(pCell->GetString());
					TCHAR* pszBuffer = new TCHAR[nLength + 1];
					_mbstowcsz(pszBuffer, pCell->GetString(), nLength + 1);
					//			pListCtrl->SetItemText(nRow, nCol, str);
					delete[] pszBuffer;
#else
					str.Format(_T("%s"), pCell->GetString());
#endif
					break;
				}
			case BasicExcelCell::WSTRING:
				{
#ifndef UNICODE
					int nLength = wcslen(pCell->GetWString()) * 2;
					TCHAR* pszBuffer = new TCHAR[nLength + 1];
					_wcstombsz(pszBuffer, pCell->GetWString(), nLength + 1);
					str.Format(_T("%s"), pszBuffer);
					delete[] pszBuffer;
#else
#endif
					break;
				}
			}
			j++;
		}
	
	return true;

}
double CTaskChangePara::GetLightHeight1(double CapHeight)
{
	for (size_t i = 0;  i < m_vecS1HBegin.size(); i++)
	{
		if (m_vecS1HBegin[i]<=CapHeight && m_vecS1HEnd[i]> CapHeight)
		{
			return m_vecS1HLight[i];
		}
	}
}

vector<CString> CTaskChangePara::GetExpppType()
{
	return m_vecExpppType;
}
double CTaskChangePara::GetCapDiameter()
{
	return m_diameter;
}

double CTaskChangePara::GetLightHeight2(double CapHeight)
{
	for (size_t i = 0;  i < m_vecS2HBegin.size(); i++)
	{
		if (m_vecS2HBegin[i]<=CapHeight && m_vecS2HEnd[i]> CapHeight)
		{
			return  m_vecS2HLight[i];
		}
	}
}

TypeData CTaskChangePara::FindTypePara(double CapHeight)
{
	TypeData ReParaData;
	for (size_t i = 0;  i < m_vecS1HBegin.size(); i++)
	{
		if (m_vecS1HBegin[i]<=CapHeight && m_vecS1HEnd[i]> CapHeight)
		{
			ReParaData.S1HBegin = m_vecS1HBegin[i];
			ReParaData.S1HEnd = m_vecS1HEnd[i];
			ReParaData.S1HLight = m_vecS1HLight[i];
			break;
		}
	}

	for (size_t i = 0;  i < m_vecS1HTestBegin.size(); i++)
	{
		if (m_vecS1HTestBegin[i]<=CapHeight && m_vecS1HTestEnd[i]> CapHeight)
		{
			ReParaData.S1HTestBegin = m_vecS1HTestBegin[i];
			ReParaData.S1HTestEnd = m_vecS1HTestEnd[i];
			break;
		}
	}

	for (size_t i = 0;  i < m_vecS2HBegin.size(); i++)
	{
		if (m_vecS2HBegin[i]<=CapHeight && m_vecS2HEnd[i]> CapHeight)
		{
			ReParaData.S2HBegin = m_vecS2HBegin[i];
			ReParaData.S2HEnd = m_vecS2HEnd[i];
			ReParaData.S2HLight = m_vecS2HLight[i];
			break;
		}
	}

	for (size_t i = 0;  i < m_vecS2HTestBegin.size(); i++)
	{
		if (m_vecS2HTestBegin[i]<=CapHeight && m_vecS2HTestEnd[i]> CapHeight)
		{
			ReParaData.S2HTestBegin = m_vecS2HTestBegin[i];
			ReParaData.S2HTestEnd = m_vecS2HTestEnd[i];
			break;
		}
	}

	return ReParaData;
}

CTaskChangePara::~CTaskChangePara(void)
{
}
