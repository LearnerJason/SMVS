#pragma once
enum DisplayStyle{
	DISPlayStyle_1X1 = 0, //单通道显示
	DISPlayStyle_1X2,
	DISPlayStyle_1X4,
	DISPlayStyle_2X2,
	DISPlayStyle_2X3
};
class CScreenDisplayOp
{

public:
	CScreenDisplayOp(void);
	~CScreenDisplayOp(void);
	void init();

	void SetCurRect(size_t iWndCount ,int iWidth=0,int iHeight=0,CRect rect = CRect(0,0,0,0));//设置分辨率
	bool GetIndexRect(int ID,CRect& Rect);
	int GetRectIndex(CPoint point);
	CRect GetPosRect();
	void SetCurDisplayStyle(DisplayStyle em_PlayStyple = DISPlayStyle_1X1);
	int  GetCurDisplayWndCount();
private:
//	DisplayStyle m_DisplayStyle; //显示风格
	CRect m_PosRect;		     //显示分辨率
	CRect m_DefaultDisPlayRect;  //最大显示区域
	int m_width;
	int m_height;

	DisplayStyle m_em_DIsplayStyle;

};

