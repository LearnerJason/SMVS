// RangeSlider.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RangeSlider.h"
#include "memdc.h"

#include <algorithm>
using namespace std;

// CRangeSlider
//��������֮����С������߼����
const int MINLP = 1;

//ʹ���м����ַŴ�ʱ�ķŴ�����
const double FACTOR_ZOOMIN = 1.2;

//ʹ���м�������Сʱ����С����
const double FACTOR_ZOOMOUT = 0.8;

IMPLEMENT_DYNAMIC(CRangeSlider, CWnd)

CRangeSlider::CRangeSlider()
{
	m_nMin = 0;
	m_nMax = 100;
	m_nThumbMin = 0;
	m_nThumbMax = 50;
	m_bThumbMinChanged = true;
	m_bThumbMaxChanged = true;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nMargin = 0;
	m_nPt = 0;
	m_bDragMin = false;
	m_bDragMax = false;
	m_bEnableMouseWheel = true;//false;
	m_bVertical = false;
	m_bSingleSlider = false;
	m_bCurrentBlock = false;
}

CRangeSlider::~CRangeSlider()
{
}

//��������
BOOL CRangeSlider::CreateWnd(CWnd* pWndParent, const CRect& winRect, UINT nID /* = 0x00 */)
{
	BOOL nResult = FALSE;
	do 
	{
		if ( NULL == pWndParent)
		{
			nResult = FALSE;
		}
		else
		{
			//����Ϊmsctls_trackbar32����ʹ����뿪�ؼ�����Ȼ���Լ������ƶ�״̬
			nResult =  Create(_T("msctls_trackbar32"), _T(""), WS_VISIBLE | WS_CHILD | SS_NOTIFY, winRect, pWndParent ,nID);
		}
	} while (false);

	return nResult;
}

//���ÿؼ��Ƿ�Ϊ��ֱ
void CRangeSlider::SetVertical(bool bVertical /* = true */)
{
	m_bVertical = bVertical;
	if ( bVertical)
	{
		ModifyStyle(NULL, TBS_VERT);//�����ֱ���ԣ�����ʹ�ؼ���ֱʱ����뿪�ؼ�ʱ�������
	}
	else
	{
		ModifyStyle(TBS_VERT, NULL);//ȥ����ֱ����
	}
}

void CRangeSlider::SetChannelMin(int nChannelMin)
{
	m_nMin = nChannelMin;
	m_bThumbMaxChanged = true;
	m_bThumbMinChanged = true;
}

int CRangeSlider::GetChannelMin()
{
	return m_nMin;
}
void CRangeSlider::SetChannelMax(int nChannelMax)
{
	m_nMax = nChannelMax;
	m_bThumbMaxChanged = true;
	m_bThumbMinChanged = true;
}

int CRangeSlider::GetChannelMax()
{
	return m_nMax;
}

void CRangeSlider::SetThumbMin(int nThumbMin)
{
	m_nThumbMin = nThumbMin;
}

int CRangeSlider::GetThumbMin()
{
	return m_nThumbMin;
}

void CRangeSlider::SetThumbMax(int nThumbMax)
{
	m_nThumbMax = nThumbMax;
}

int CRangeSlider::GetThumbMax()
{
	return m_nThumbMax;
}

void CRangeSlider::SetThumbMargin(int nMargin)
{
	m_nMargin = nMargin;
}

void CRangeSlider::SendStopSliderMessage()
{
	m_bDragMax = false;
	m_bDragMin = false;
	m_nPt = 0;
}
//����channelͼƬ��Դ
void CRangeSlider::LoadChannelImage(UINT nRecourceID, const CString& cstrResourceType)
{
	m_imageChannel.Destroy();
	LoadImageFromResource(nRecourceID, cstrResourceType, &m_imageChannel);
	DealAlphaChannel(m_imageChannel);
}

void CRangeSlider::LoadChannelImage(const CString& cstrImageFilePath)
{
	m_imageChannel.Destroy();
	m_imageChannel.Load(cstrImageFilePath);
	DealAlphaChannel(m_imageChannel);
}
//����thumbͼƬ��Դ
void CRangeSlider::LoadThumbImage(UINT nRecourceID, const CString& cstrResourceType)
{
	m_imageThumb.Destroy();
	LoadImageFromResource(nRecourceID, cstrResourceType, &m_imageThumb);
	DealAlphaChannel(m_imageThumb);
}

void CRangeSlider::LoadThumbImage(const CString& cstrImageFilePath)
{
	m_imageThumb.Destroy();
	m_imageThumb.Load(cstrImageFilePath);
	DealAlphaChannel(m_imageThumb);
}

//����Դ����ͼƬ
void CRangeSlider::LoadImageFromResource(UINT nRecourceID, const CString& cstrResourceType, CImage* pImage)
{
	if (NULL == pImage)
	{
		return;
	}

	pImage->Destroy();
	
	HINSTANCE hInst = ::AfxGetInstanceHandle();
	if ("bmp" == cstrResourceType)
	{
		pImage->LoadFromResource(hInst, nRecourceID);
	}
	else
	{
		HRSRC hRsrc = ::FindResource(hInst,MAKEINTRESOURCE(nRecourceID),cstrResourceType); // type
		if (!hRsrc)
			return ;

		BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);  
		if (!lpRsrc) 
		{
			return ;  
		}
		
		// Allocate global memory on which to create stream  
		DWORD imageSize = ::SizeofResource(hInst, hRsrc);  
		HGLOBAL hBuffer = ::GlobalAlloc(GMEM_FIXED, imageSize);  

		BYTE* pBuffer = (BYTE*)GlobalLock(hBuffer);  
		memcpy(pBuffer,lpRsrc,imageSize);  

		IStream* pstm = NULL;  
		::CreateStreamOnHGlobal(hBuffer,FALSE,&pstm);  

		// load from stream  
		pImage->Load(pstm);  

		// free/release stuff  
		::GlobalUnlock(hBuffer);
		::GlobalFree(hBuffer);

		pstm->Release();  

		FreeResource(lpRsrc);
	}
}

//��͸��ͼƬ���д���
void CRangeSlider::DealAlphaChannel(ATL::CImage& image)
{
	if (image.GetBPP() == 32)
	{
		for (int i = 0; i < image.GetWidth(); i++)
		{
			for (int j = 0; j < image.GetHeight(); j++)
			{
				unsigned char* pucColor = reinterpret_cast<unsigned char*>(image.GetPixelAddress(i,j));
				pucColor[0] = pucColor[0] * pucColor[3] / 255;
				pucColor[1] = pucColor[1] * pucColor[3] / 255;
				pucColor[2] = pucColor[2] * pucColor[3] / 255;
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CRangeSlider, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CRangeSlider ��Ϣ�������
void CRangeSlider::IncreaseStep(int iStepValue)
{
	if (!m_bCurrentBlock)
	{
		if (m_nThumbMin >= m_nThumbMax)
		{
			return;
		}
		m_nThumbMin += iStepValue;
		m_bThumbMinChanged = true;
	}
	else
	{
		if (m_nThumbMax>= m_nMax)
		{
			return;
		}
		m_nThumbMax += iStepValue;
		m_bThumbMaxChanged = true;
	}
	Invalidate();
	::PostMessage(GetParent()->m_hWnd,
		WM_RANGESLIDER_RANGECHANGED,
		GetDlgCtrlID(),//���ݴ���ID��������Ϣʱ��ȡ��Ӧ�Ĵ��ڣ������ͨ���ӿڻ�ȡ������߼�ֵ
		NULL);
}

void CRangeSlider::DecreaseStep(int iStepValue)
{
	if (!m_bCurrentBlock)
	{
		if (m_nThumbMin <= m_nMin)
		{
			return;
		}
		m_nThumbMin -=  iStepValue;
		m_bThumbMinChanged = true;

		
//		nCurPosLP = m_nThumbMin;
	}
	else
	{
		if (m_nThumbMax <= m_nThumbMin)
		{
			return;
		}
		m_nThumbMax -= iStepValue;
		m_bThumbMaxChanged = true;
		

//		nCurPosLP = m_nThumbMax;
	}
	Invalidate();
	::PostMessage(GetParent()->m_hWnd,
		WM_RANGESLIDER_RANGECHANGED,
		GetDlgCtrlID(),//���ݴ���ID��������Ϣʱ��ȡ��Ӧ�Ĵ��ڣ������ͨ���ӿڻ�ȡ������߼�ֵ
		NULL);
}

//����������ʱ����Ϣ��Ӧ����
void CRangeSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nPt = 0;//��굱ǰ�����λ��
	int nMinDP = Pos2Pixel(m_nThumbMin);//��Сֵ��Ӧ������λ��
	int nMaxDP = Pos2Pixel(m_nThumbMax);//���ֵ��Ӧ������λ��
	if (m_bVertical)
	{
		nPt = point.y;//�ؼ���ֱʱ��ֻ���������λ�õ�y��������Сֵ�������ֵ֮����������
	}
	else
	{
		nPt = point.x;//�ؼ�ˮƽʱ��ֻ���������λ�õ�x��������Сֵ�������ֵ֮����������
	}

	if (nPt < nMinDP && nPt > nMinDP - m_nMargin)//�����Сֵ�����ϵ�ʱ�����϶���Сֵ���ܣ�����¼���������Сֵ������������
	{
		m_bDragMin = true;
		m_nPt = nPt - nMinDP;
		m_bCurrentBlock = false;
	}
	else if (nPt > nMaxDP && nPt < nMaxDP + m_nMargin)//������ֵ�����ϵ�ʱ�����϶����ֵ���ܣ�����¼����������ֵ������������
	{
		if(m_bSingleSlider == false)
		{
			m_bDragMax = true;
			m_nPt = nPt - nMaxDP;
		}
		else
		{
			m_bDragMax = false;
		}
		m_bCurrentBlock = true;
	}
	else
	{
	}
	CWnd::OnLButtonDown(nFlags, point);
}

//����������ʱ����Ϣ��Ӧ����
void CRangeSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDragMax = false;
	m_bDragMin = false;
	m_nPt = 0;

	::PostMessage(GetParent()->m_hWnd,
		WM_RANGESLIDER_RANGECHANGED,
		GetDlgCtrlID(),//���ݴ���ID��������Ϣʱ��ȡ��Ӧ�Ĵ��ڣ������ͨ���ӿڻ�ȡ������߼�ֵ
		NULL);
	CWnd::OnLButtonUp(nFlags, point);
}

//����ƶ�ʱ����Ϣ��Ӧ����
void CRangeSlider::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rcClient;
	this->GetClientRect(rcClient);
	if(!rcClient.PtInRect(point))
	{
		m_bDragMax = false;
		m_bDragMin = false;
		m_nPt = 0;
	}
	int nCurPos = 0;
	if (m_bVertical)
	{
		nCurPos = point.y;
	}
	else
	{
		nCurPos = point.x;
	}

	//����������µ��߼�λ��
	int nCurPosLP = Pixel2Pos(nCurPos - m_nPt);

	//��Ҫͬʱ�϶���Сֵ��������ֵ����
	if (m_bDragMin && m_bDragMax)
	{
		int deltaLP = m_nThumbMax - m_nThumbMin;//��¼�϶�ǰ�������������߼�����

		//���紦��
		if (nCurPosLP < m_nMin)
		{
			nCurPosLP = m_nMin;
		}
		else if (nCurPosLP + deltaLP > m_nMax)
		{
			nCurPosLP = m_nMax - deltaLP;
		}

		//���������ƶ����µ�λ��
		m_nThumbMin = nCurPosLP;
		m_bThumbMinChanged = true;

		m_nThumbMax = nCurPosLP + deltaLP;
		m_bThumbMaxChanged = true;
		Invalidate();
	}
	//��Ҫ�϶���Сֵ
	else if (m_bDragMin)
	{
		//���紦��
		if (nCurPosLP > m_nThumbMax - MINLP)
		{
			nCurPosLP = m_nThumbMax - MINLP;
		}
		else if (nCurPosLP < m_nMin)
		{
			nCurPosLP = m_nMin;
		}

		//��Сֵ�����ƶ����µ�λ��
		m_nThumbMin = nCurPosLP;
		m_bThumbMinChanged = true;

		Invalidate();
	}
	//��Ҫ�϶����ֵ
	else if (m_bDragMax)
	{
		//���紦��
		if (nCurPosLP > m_nMax)
		{
			nCurPosLP = m_nMax;
		}
		else if (nCurPosLP < m_nThumbMin + MINLP)
		{
			nCurPosLP = m_nThumbMin + MINLP;
		}

		m_nThumbMax = nCurPosLP;
		m_bThumbMaxChanged = true;
		Invalidate();
	}

	//CWnd::OnMouseMove(nFlags, point);
}

//����м�����ʱ����Ϣ��Ӧ����
//BOOL CRangeSlider::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	if(nFlags == MK_CONTROL)
//		return TRUE;
//
//	if (m_bEnableMouseWheel)
//	{
//		int nHalf = double(m_nThumbMax - m_nThumbMin)/2 + 0.5;//(m_nThumbMax - m_nThumbMin)/2;
//		if (zDelta > 0)
//		{
//			//�Ŵ�
//			m_nThumbMin = m_nThumbMin - nHalf * (FACTOR_ZOOMIN - 1) - 0.5;//ע��double������ת��Ϊint������ʱ��ɵ���ʧ��+ 0.5���
//			if(!m_bSingleSlider)
//				m_nThumbMax = m_nThumbMax + nHalf * (FACTOR_ZOOMIN - 1) + 0.5;
//		}
//		else
//		{
//			//��С
//			m_nThumbMin = m_nThumbMin + nHalf * (1 - FACTOR_ZOOMOUT) + 0.5;
//			if(!m_bSingleSlider)
//				m_nThumbMax = m_nThumbMax - nHalf * (1 - FACTOR_ZOOMOUT) - 0.5;
//		}
//
//		//���紦��
//		if (m_nThumbMax - m_nThumbMin < MINLP)
//		{
//			m_nThumbMax = m_nThumbMin + MINLP;
//		}
//		if ( m_nThumbMin < m_nMin)
//		{
//			m_nThumbMin = m_nMin;
//		}
//		if (m_nThumbMax > m_nMax)
//		{
//			m_nThumbMax = m_nMax;
//		}
//		m_bThumbMinChanged = true;
//		m_bThumbMaxChanged = true;
//
//		::PostMessage(
//			GetParent()->m_hWnd,
//			WM_RANGESLIDER_RANGECHANGED,
//			GetDlgCtrlID(),//���ݴ���ID��������Ϣʱ��ȡ��Ӧ�Ĵ��ڣ������ͨ���ӿڻ�ȡ������߼�ֵ
//			NULL);
//
//		Invalidate();
//	}
//
//	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
//}

//��ͼ����
void CRangeSlider::OnPaint()
{
	CPaintDC dcOrigin(this);

	CRect winRect;
	GetClientRect(&winRect);

	m_nWidth = winRect.Width();
	m_nHeight = winRect.Height();

	//ʹ��˫���������˸
	CMemoryDC dc(&dcOrigin,&winRect, TRUE);

	//��ͼǰ���з�Χ���
	if (m_nMin > m_nMax)
	{
		swap(m_nMin, m_nMax);
	}
	if (m_nThumbMin > m_nThumbMax)
	{
		swap(m_nThumbMin, m_nThumbMax);
	}
	if (m_nThumbMin < m_nMin)
	{
		m_nThumbMin = m_nMin;
	}
	if (m_nThumbMax > m_nMax)
	{
		m_nThumbMax = m_nMax;
	}

	//���ݿؼ��Ƿ���ֱȷ��channel��thumb������λ�úͳߴ�
	//channel
	int nChannelDestX = 0;
	int nChannelDestY = 0;

	int nChannelSrcX = 0;
	int nChannelSrcY = 0;

	int nChannelWidth = m_imageChannel.GetWidth();
	int nChannelHeight = m_imageChannel.GetHeight();

	//thumb
	int nThumbWidth_half = 0;
	int nThumbHeight_half = 0;

	//thumb�ֳ����λ��ƣ��ֱ����Դ�������ڻ���
	int nThumbDestX_min = 0;
	int nThumbDestY_min = 0;
	int nThumbDestX_max = 0;
	int nThumbDestY_max = 0;

	int nThumbSrcX_min = 0;
	int nThumbSrcY_min = 0;
	int nThumbSrcX_max = 0;
	int nThumbSrcY_max = 0;

	int nThumbSpanDP_half = ( Pos2Pixel(m_nThumbMax) - Pos2Pixel(m_nThumbMin) ) / 2; //�����������һ��

	if (m_bVertical)//�ؼ���ֱ
	{
		if (m_nMargin <= 0)
		{
			m_nMargin = m_imageThumb.GetWidth();
		}

		nChannelDestX = m_nWidth / 2 - m_imageChannel.GetWidth() / 2;

		nThumbWidth_half = m_imageThumb.GetWidth();
		nThumbHeight_half = nThumbSpanDP_half + m_nMargin;

		nThumbDestX_min = m_nWidth / 2 - m_imageThumb.GetWidth() / 2;
		nThumbDestY_min = Pos2Pixel(m_nThumbMin) - m_nMargin;

		nThumbDestX_max = nThumbDestX_min;
		//�������ֵ����������Сֵ����λ���������°�Σ������϶���Сֵ����ʱ���ֶ�������
		nThumbDestY_max = Pos2Pixel(m_nThumbMax) - double(nThumbSpanDP_half) - 0.5;

		//�����°�ε�ʱ����ȡ��Դ���°��
		nThumbSrcY_max = m_imageThumb.GetHeight() - nThumbSpanDP_half - m_nMargin;
	}
	else//�ؼ�Ϊˮƽ
	{
		if (m_nMargin <= 0)
		{
			m_nMargin = m_imageThumb.GetHeight();
		}

		nChannelDestY = m_nHeight / 2 - m_imageChannel.GetHeight() / 2;

		nThumbWidth_half = nThumbSpanDP_half + m_nMargin;
		nThumbHeight_half = m_imageThumb.GetHeight();

		nThumbDestX_min = Pos2Pixel(m_nThumbMin) - m_nMargin;
		nThumbDestY_min = m_nHeight / 2 - m_imageThumb.GetHeight() / 2;

		//�������ֵ����������Сֵ����λ���������Ұ�Σ������϶���Сֵ����ʱ���ֶ�������
		nThumbDestX_max = int(Pos2Pixel(m_nThumbMax) - double(nThumbSpanDP_half) - 0.5);
		nThumbDestY_max = nThumbDestY_min;

		//�����Ұ�ε�ʱ����ȡ��Դ���Ұ��
		nThumbSrcX_max = m_imageThumb.GetWidth() - nThumbSpanDP_half - m_nMargin;
	}

	//����channel
	m_imageChannel.Draw(dc->m_hDC,
		nChannelDestX, 
		nChannelDestY, 
		nChannelWidth, 
		nChannelHeight,
		nChannelSrcX, 
		nChannelSrcY, 
		nChannelWidth, 
		nChannelHeight);

	//����thumb���ȴ���Сֵ���鴦���м����һ��
	m_imageThumb.Draw(dc->m_hDC, 
		nThumbDestX_min,
		nThumbDestY_min,
		nThumbWidth_half,
		nThumbHeight_half,
		nThumbSrcX_min,
		nThumbSrcY_min,
		nThumbWidth_half,
		nThumbHeight_half);

	//����thumb���ٴ����ֵ���鴦���м����һ��
	m_imageThumb.Draw(dc->m_hDC, 
		nThumbDestX_max,
		nThumbDestY_max,
		nThumbWidth_half,
		nThumbHeight_half,
		nThumbSrcX_max,
		nThumbSrcY_max,
		nThumbWidth_half,
		nThumbHeight_half);
}

void CRangeSlider::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

BOOL CRangeSlider::OnEraseBkgnd(CDC* pDC)
{
	//����TRUE������ˢ��Ƶ��ʱ������˸
	return TRUE;
}

bool CRangeSlider::GetThumbMinChangedFlag()
{
	return m_bThumbMinChanged;
}

bool CRangeSlider::GetThumbMaxChangedFlag()
{
	return m_bThumbMaxChanged;
}

void CRangeSlider::SetThumbMinChangedFlag(bool flag)
{
	m_bThumbMinChanged = flag;
}

void CRangeSlider::SetThumbMaxChangedFlag(bool flag)
{
	m_bThumbMaxChanged = flag;
}
