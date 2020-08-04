#include "StdAfx.h"
#include "ScreenDisPlayOp.h"

#pragma warning(disable : 4244)
void CScreenDisplayOp::SetCurRect(size_t iWndCount,int iWidth,int iHeight,CRect rect)
{
	init();
	if (iWidth != 0)
	{
		m_width = iWidth;
		m_height = iHeight;
	}
	if(rect!= CRect(0,0,0,0))
	{
		m_DefaultDisPlayRect = rect;
	}	
	m_PosRect.left = m_DefaultDisPlayRect.left;
	m_PosRect.top = m_DefaultDisPlayRect.top;
	if (iWndCount == 1 )
	{
		m_em_DIsplayStyle = DISPlayStyle_1X1;
		m_PosRect.right = m_PosRect.left + m_width;
		m_PosRect.bottom = m_PosRect.top + m_height;

		// 		if ((float)m_width / m_height >= (float)m_DefaultDisPlayRect.Width()/ m_DefaultDisPlayRect.Height())
		// 		{
		// 			m_PosRect.right = m_DefaultDisPlayRect.right;
		// 			m_PosRect.bottom = m_PosRect.top + (float)m_DefaultDisPlayRect.Width() /m_width * m_height;
		// 		}
		// 		else
		// 		{
		// 			m_PosRect.bottom = m_DefaultDisPlayRect.bottom;
		// 			m_PosRect.right = m_PosRect.left + (float)m_DefaultDisPlayRect.Height() /m_height * m_width ;
		// 		}
	}
	else if (iWndCount == 2)
	{
		m_em_DIsplayStyle = DISPlayStyle_1X2;
		if ((float)m_width*2  / m_height >= (float)m_DefaultDisPlayRect.Width()/ m_DefaultDisPlayRect.Height())
		{
			m_PosRect.right = m_DefaultDisPlayRect.right;
			m_PosRect.bottom = m_PosRect.top + (float)m_DefaultDisPlayRect.Width()/2 /m_width * m_height;
		}
		else
		{
			m_PosRect.bottom = m_DefaultDisPlayRect.bottom;
			m_PosRect.right = m_PosRect.left + (float)m_DefaultDisPlayRect.Height() /m_height * m_width *2;
		}
	}
	else if (iWndCount == 3 ||iWndCount == 4)
	{
		m_em_DIsplayStyle = DISPlayStyle_2X2;
		if ((float)m_width  / m_height >= (float)m_DefaultDisPlayRect.Width()/ m_DefaultDisPlayRect.Height())
		{
			m_PosRect.right = m_DefaultDisPlayRect.right;
			m_PosRect.bottom = m_PosRect.top + (float)m_DefaultDisPlayRect.Width() /m_width * m_height;
		}
		else
		{
			m_PosRect.bottom = m_DefaultDisPlayRect.bottom;
			m_PosRect.right = m_PosRect.left + (float)m_DefaultDisPlayRect.Height() /m_height * m_width;
		}
	}
	else
	{
		m_em_DIsplayStyle = DISPlayStyle_2X3;
		if ((float)m_width  / m_height >= (float)m_DefaultDisPlayRect.Width()/ m_DefaultDisPlayRect.Height())
		{
			m_PosRect.right = m_DefaultDisPlayRect.right;
			m_PosRect.bottom = m_PosRect.top + (float)m_DefaultDisPlayRect.Width() /m_width * m_height;
		}
		else
		{
			m_PosRect.bottom = m_DefaultDisPlayRect.bottom;
			m_PosRect.right = m_PosRect.left + (float)m_DefaultDisPlayRect.Height() /m_height * m_width;
		}
// 		if ((float)m_width *3 / (m_height *2) >= (float)m_DefaultDisPlayRect.Width()/ m_DefaultDisPlayRect.Height())
// 		{
// 			m_PosRect.right = m_DefaultDisPlayRect.right;
// 			m_PosRect.bottom = m_PosRect.top + (float)m_DefaultDisPlayRect.Width()/3 /m_width * m_height*2;
// 		}
// 		else
// 		{
// 			m_PosRect.bottom = m_DefaultDisPlayRect.bottom;
// 			m_PosRect.right = m_PosRect.left + (float)m_DefaultDisPlayRect.Height()/2 /m_height * m_width *3;
// 		}
	}
}
CRect CScreenDisplayOp::GetPosRect()
{
	return m_PosRect;
}
void CScreenDisplayOp::SetCurDisplayStyle(DisplayStyle em_PlayStyple)
{

}

int CScreenDisplayOp::GetCurDisplayWndCount()
{
	switch(m_em_DIsplayStyle)
	{
	case DISPlayStyle_1X1:
		return 1;
		break;
	case DISPlayStyle_1X2:
		return 2;
		break;
	case DISPlayStyle_1X4:
		return 4;
		break;
	case DISPlayStyle_2X2:
		return 4;
		break;
	case DISPlayStyle_2X3:
		return 6;
		break;
	default:
		return -1;
		break;
	}
}
//…Ë÷√∑÷±Ê¬ 
int CScreenDisplayOp::GetRectIndex(CPoint point)
{
	int i,j;
	int index =-1;
	switch(m_em_DIsplayStyle)
	{
		case DISPlayStyle_1X1:
			index = 0;
			break;
		case DISPlayStyle_1X2:
			j = (point.x - m_PosRect.left)/(m_PosRect.Width()/2);
			index = j;
			break;
		case DISPlayStyle_1X4:
			j = (point.x - m_PosRect.left)/(m_PosRect.Width()/4);
			index = j;
			break;
		case DISPlayStyle_2X2:
			 i = (point.y - m_PosRect.top)/(m_PosRect.Height()/2);
			 j = (point.x - m_PosRect.left)/(m_PosRect.Width()/2);
			 index = i * 2 + j;
			break;
		case DISPlayStyle_2X3:
			i = (point.y - m_PosRect.top)/(m_PosRect.Height()/2);
			j = (point.x - m_PosRect.left)/(m_PosRect.Width()/3);
			index = i * 3 + j;//
			break;
		default:
			break;
	}
	return index;
	
}
bool CScreenDisplayOp::GetIndexRect(int ID,CRect& Rect)
{
	int x1,y1,x2,y2;
	float fwidth,fheight;
	switch(m_em_DIsplayStyle)
	{
	case DISPlayStyle_1X1:
		Rect = m_PosRect;
		return true;
		break;
	case DISPlayStyle_1X2:
		fwidth = (float)m_PosRect.Width()/2;
		fheight = (float)m_PosRect.Height();
		x1 = m_PosRect.left + fwidth*(ID);
		y1 = m_PosRect.top;
		x2 = m_PosRect.left + fwidth*(ID+1);
		y2 = m_PosRect.bottom;
		Rect = CRect(x1,y1,x2,y2);
		break;
	case DISPlayStyle_1X4:
		fwidth = (float)m_PosRect.Width()/4;
		fheight = (float)m_PosRect.Height()/4;
		x1 = m_PosRect.left + fwidth*(ID);
		y1 = m_PosRect.top;
		x2 = m_PosRect.left + fwidth*(ID+1);
		y2 = m_PosRect.bottom;
		Rect = CRect(x1,y1,x2,y2);
		break;
		case DISPlayStyle_2X2:
			fwidth = (float)m_PosRect.Width()/2;
			fheight = (float)m_PosRect.Height()/2;
			x1 = m_PosRect.left + fwidth*(ID%2)+0.5;
			y1 = m_PosRect.top+fheight*(ID/2)+0.5;
			x2 = m_PosRect.left + fwidth*(ID%2+1)+0.5;
			y2 = m_PosRect.top+fheight*(ID/2+1)+0.5;
			Rect = CRect(x1,y1,x2,y2);
			break;
		case DISPlayStyle_2X3:
			fwidth = (float)m_PosRect.Width()/3;
			fheight = (float)m_PosRect.Height()/2;
			x1 = m_PosRect.left + fwidth*(ID%3);
			y1 = m_PosRect.top+fheight*(ID/3);
			x2 = m_PosRect.left + fwidth*(ID%3+1);
			y2 = m_PosRect.top+fheight*(ID/3+1);;
			Rect = CRect(x1,y1,x2,y2);
			break;
		default:
			break;		
	}
	
	Rect.left += 2;
	Rect.right -=2;
	Rect.top +=2;
	Rect.bottom -=2;

	return true;
}

CScreenDisplayOp::CScreenDisplayOp(void)
{
	int Rcleft = 64 + (5*2);
	
	CRect rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);
	float fRate = 1080.0f / rcWorkArea.Height();
	int titleAreaH = int(75.0f / fRate) + 64 + 5*2;

	m_DefaultDisPlayRect = CRect(Rcleft,titleAreaH,Rcleft+rcWorkArea.Width()-896-80/*1024*/,titleAreaH+rcWorkArea.Height()-436/*+644*/);

	m_em_DIsplayStyle = DISPlayStyle_1X1;
}

void CScreenDisplayOp::init()
{
	int Rcleft = 64 + (5*2);

	CRect rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);
	float fRate = 1080.0f / rcWorkArea.Height();
	int titleAreaH = int(75.0f / fRate) + 64 + 5*2;

	m_DefaultDisPlayRect = CRect(Rcleft,titleAreaH,Rcleft+rcWorkArea.Width()-896-80/*1024*/,titleAreaH+rcWorkArea.Height()-436/*+644*/);
}


CScreenDisplayOp::~CScreenDisplayOp(void)
{

}
