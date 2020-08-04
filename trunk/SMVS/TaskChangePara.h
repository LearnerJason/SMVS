#pragma once

typedef struct STypeData{

	double S1HBegin;
	double S1HEnd;
	double S1HLight;
	double S1HTestBegin;
	double S1HTestEnd;

	double S2HBegin;
	double S2HEnd;
	double S2HLight;
	double S2HTestBegin;
	double S2HTestEnd;
}TypeData;

using namespace YExcel;

class CTaskChangePara
{
private:
	
	BasicExcelWorksheet* m_pSheet;

	//Station1#--外观检测工位（4个通道）
	vector<double> m_vecS1HBegin;
	vector<double> m_vecS1HEnd;
	vector<double> m_vecS1HLight;
	vector<double> m_vecS1HTestBegin;
	vector<double> m_vecS1HTestEnd;

	//
	vector<double> m_vecS2HBegin;
	vector<double> m_vecS2HEnd;
	vector<double> m_vecS2HLight;
	vector<double> m_vecS2HTestBegin;
	vector<double> m_vecS2HTestEnd;

	double m_diameter;
	vector<CString> m_vecExpppType;

private:
	bool InitData();
	bool GetVectorData(vector<double> &m_vec,int row1, int col1,int row2,int col2);
	bool GetVectorData(vector<CString> &m_vec,int row1,int col1,int row2,int col2);
	bool GetData(double& data,int row,int col);
public:
	CTaskChangePara(void);
	~CTaskChangePara(void);
public:
	TypeData FindTypePara(double CapHeight);
	double GetLightHeight1(double CapHeight1);
	double GetLightHeight2(double CapHeight2);
	vector<CString> GetExpppType();
	double GetCapDiameter();

};

