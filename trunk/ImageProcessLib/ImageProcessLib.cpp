#include "stdafx.h"
#include "ImageProcessLib.h"
#define  CAMERA_NUM		4
#include <tchar.h>
#define  HAMMING (11)
#define  BOXCAR (10)
#define  ALL  (0)
#define  DARK  (-1)
#define  LIGHT (1)
/***********************************************************************
* �������ƣ�H_IsCalibImage
* �������ܣ�ѵ�������У��жϵ�ǰͼ���Ƿ�Ϊ���̸�ͼ��,���ݶ����ͼ��
* ���������MultiImage -- �����ͼ��Ϊ������ͼ��ϳ�
CalibPtRadiusInPixel -- �������̸��кڵ�뾶����λΪ���أ�
MinCalibPtNum -- ��С�����̸�ڵ���
* ���������CalibPtRegion -- ���̸�����Ϊ���������
* �� �� ֵ��BOOL������TRUE��ʾ��ǰͼ��Ϊ���̸�ͼ�񣬷��������̸�ͼ��
* ��    ����1.0
* ��    �ߣ�LukeChen
* ��    �ڣ�2015.10.10
************************************************************************/
BOOL CylinderCalibration:: H_IsCalibImage(Hobject MultiImage, Hobject* CalibPtRegion, HTuple CalibPtRadiusInPixel, HTuple MinCalibPtNum)
{
	BOOL Result = TRUE;
	HTuple ImageObjNum, CalibPtNum;
	Hobject Image, DotImage;
	Hobject DotRegion, DotRegionFillUp, ConnectedDotRegion, SelectDotRegions, SelectDotRegionsUnion; 

	//���ÿգ���������������ȷ
	gen_empty_obj(CalibPtRegion);

	count_obj(MultiImage, &ImageObjNum);
	for (int ImageObjIndex = 1; ImageObjIndex <= ImageObjNum; ImageObjIndex++)
	{
		select_obj(MultiImage, &Image, ImageObjIndex);
		dots_image(Image, &DotImage, 5, "dark", 2);
		threshold(DotImage, &DotRegion, 60, 255);
		fill_up(DotRegion, &DotRegionFillUp);
		connection(DotRegionFillUp, &ConnectedDotRegion);
		select_shape(ConnectedDotRegion, &SelectDotRegions, ((HTuple("circularity").Append("inner_radius")).Append("outer_radius")),
			"and", ((HTuple(0.3).Append(CalibPtRadiusInPixel*0.8)).Append(CalibPtRadiusInPixel)), ((HTuple(1).Append(CalibPtRadiusInPixel*2)).Append(CalibPtRadiusInPixel*2)));
		count_obj(SelectDotRegions, &CalibPtNum);

		if (CalibPtNum[0].I() < MinCalibPtNum){
			Result = FALSE;
		}
		else
		{
			union1(SelectDotRegions, &SelectDotRegionsUnion);
			concat_obj(*CalibPtRegion, SelectDotRegionsUnion, CalibPtRegion);
		}			
	}	
	return Result;
}


/***********************************************************************
* �������ƣ�H_EnhanceCalibImage
* �������ܣ��ԻҶȲ����ȵ�Բ�����̸�ͼ����Ȼ�
* ���������Image -- Բ�����̸�ͼ��
GrayClosingMaskRadius -- �Ҷ�ͼ�������ģ��뾶������ȥ������
Ĭ��ֵΪ3.5
UpperThreshold4BlackPt -- ��ֵ����������ֵ��������ȡ�ڵ�����
EnhancePara -- ͼ����ǿϵ����Ĭ��ֵΪ0.02
* ���������EnhancedImage -- ��ǿ������̸�ͼ��
MultImage -- ��ǿͼ�����õ�ӳ��
* �� �� ֵ��void
* ��    ����1.0
* ��    �ߣ�LukeChen
* ��    �ڣ�2015.10.14
************************************************************************/
void  CylinderCalibration:: H_EnhanceCalibImage(Hobject Image, Hobject* EnhancedImage, Hobject* MultImage, long GrayClosingMaskRadius, int UpperThreshold4BlackPt, double EnhancePara)
{
	gen_empty_obj(EnhancedImage);
	gen_empty_obj(MultImage);

	HTuple ImageWidth, ImageHeight;
	get_image_size(Image, &ImageWidth, &ImageHeight);
	HTuple CalibPtRegMean, CalibPtRegDev;
	Hobject ImageClosing, ThresholdReg, ThresholdRegErosion;
	Hobject ConnectedRegion, ConnectedRegDilation, ConnectedRegErosion;
	Hobject CalibPtBoundaryReg, InnerRectangle, CalibPtBoundaryRegIntersect;
	Hobject PaintedImage, PaintedImageReduced, ExpandImage, ImageMean, TargetWhiteImage;

	gray_closing_rect(Image, &ImageClosing, GrayClosingMaskRadius, GrayClosingMaskRadius);
	threshold(ImageClosing, &ThresholdReg, 0, UpperThreshold4BlackPt);
	erosion_circle(ThresholdReg, &ThresholdRegErosion, 1.5);

	connection(ThresholdRegErosion, &ConnectedRegion);
	dilation_circle(ConnectedRegion, &ConnectedRegDilation, 7.5);
	erosion_circle(ConnectedRegDilation, &ConnectedRegErosion, 2.5);
	//�����ɫԲ��ı߽磬���ڵõ����̸�ͼ��ĻҶȷֲ�
	difference(ConnectedRegDilation, ConnectedRegErosion, &CalibPtBoundaryReg);

	gen_rectangle1(&InnerRectangle, 5, 5, ImageHeight[0].I() -5, ImageWidth[0].I() - 5);
	intersection(CalibPtBoundaryReg, InnerRectangle, &CalibPtBoundaryRegIntersect);
	intensity(CalibPtBoundaryRegIntersect, ImageClosing, &CalibPtRegMean, &CalibPtRegDev);
	paint_region(ConnectedRegErosion, ImageClosing, &PaintedImage, CalibPtRegMean, "fill");

	reduce_domain(PaintedImage, InnerRectangle, &PaintedImageReduced);
	expand_domain_gray(PaintedImageReduced, &ExpandImage, 5);
	mean_image(ExpandImage, &ImageMean, 57, 57);
	gen_image_proto(ImageMean, &TargetWhiteImage, 255);
	div_image(TargetWhiteImage, ImageMean, MultImage, 1.0/EnhancePara, 0);
	mult_image(Image, *MultImage, EnhancedImage, EnhancePara, 0);
}

/***********************************************************************
* �������ƣ�H_IsImageWithSeam
* �������ܣ��ж����̸�ͼ�����Ƿ���ڽӷ졤
* ���������Image -- Բ�����̸�ͼ��
UpperThresholdForLine -- ��ֵ������
SeamDirection -- �ӷ췽��ˮƽ��Horizontal������ֱ(Vertical)
UpperInnerRadius -- �ӷ����������ڽ�Բ�뾶��Ĭ��Ϊ15
* ���������void
* �� �� ֵ��BOOL����,TRUE��ʾͼ���д��ڽӷ죬 FALSE��ʾͼ���в����ڽӷ�
* ��    ����1.0
* ��    �ߣ�LukeChen
* ��    �ڣ�2015.10.15
************************************************************************/
DLLEXPT BOOL CylinderCalibration:: H_IsImageWithSeam(Hobject Image, int UpperThresholdForLine, string SeamDirection, long UpperInnerRadius)
{
	BOOL Result = FALSE;

	HTuple ImageWidth, ImageHeight;
	Hobject CentralRect, ImageReduced, RegionForLine;
	Hobject RegErosion, RegErosionDilation;

	get_image_size(Image, &ImageWidth, &ImageHeight);
	gen_rectangle1(&CentralRect, (ImageHeight[0].I())/2.0 - 250, (ImageWidth[0].I())/2 - 500, (ImageHeight[0].I())/2.0 + 250, (ImageWidth[0].I())/2.0 + 550);
	reduce_domain(Image, CentralRect, &ImageReduced);
	threshold(ImageReduced, &RegionForLine, 0, UpperThresholdForLine);
	if (SeamDirection == "Horizontal")
	{
		erosion_rectangle1(RegionForLine, &RegErosion, 16, 1);
		dilation_rectangle1(RegErosion, &RegErosionDilation, 16, 2);
	} 
	else if (SeamDirection == "Vertical")
	{
		erosion_rectangle1(RegionForLine, &RegErosion, 1, 16);
		dilation_rectangle1(RegErosion, &RegErosionDilation, 2, 16);
	}
	else
	{
		//�ӷ췽��ƴд������
#ifdef _ENGLISH
		MessageBox(NULL,_T("Spell Error of the World for Value SeamDirection"), _T("Algorithm��H_IsImageWithSeam"), MB_OK|MB_ICONEXCLAMATION);
#else
		MessageBox(NULL,_T("�ӷ췽��ƴд��������!"), _T("���ӣ�H_IsImageWithSeam"), MB_OK|MB_ICONEXCLAMATION);
#endif
		return true;
	}
	Hobject ConnectedReg, SelectedReg;
	HTuple SeamNum;
	connection(RegErosionDilation, &ConnectedReg);
	HTuple tempNum;
	count_obj(ConnectedReg, &tempNum);

	select_shape(ConnectedReg, &SelectedReg, (HTuple("inner_radius").Append("outer_radius")), 
		"and", (HTuple(1).Append(320)), (HTuple(UpperInnerRadius).Append(900)));
	count_obj(SelectedReg, &SeamNum);
	if (SeamNum[0].I() >= 1)
	{
		Result = TRUE;
	}

	return  Result;
}

/***********************************************************************
* �������ƣ�H_ExtractCalibDots
* �������ܣ���ȡ���̸�ͼ���еĺ�ɫ�궨�������
* ���������Image -- Բ�����̸�ͼ��
MaskWidth -- �Ҷȸ�ʴģ����
ThresholdDyn -- ��̬�Ҷ���ֵ����������
HoleArea    --- ����Ŀ��׾�����
SmallSize   --- �������е�ģ��뾶
SelectDataMin --ɸѡ����ʱ�Ĳ������ޣ�����������ھ����⾶��Բ��
SelectDataMax --ɸѡ����ʱ�Ĳ�������
* ���������DotsRegion -- ��ȡ���ĺ�ɫУ��������
* �� �� ֵ��void
* ��    ����1.0
* ��    �ߣ�LukeChen
* ��    �ڣ�2015.10.17
************************************************************************/
DLLEXPT void CylinderCalibration:: H_ExtractCalibDots(Hobject Image, Hobject* DotsRegion, int MaskWidth, int DynThreshold, int MaxAreaOfHole, double OpeningMaskRadius, HTuple TargetFeatureMin, HTuple TargetFeatureMax)
{
	gen_empty_obj(DotsRegion);
	Hobject DotImage, DotImageScaleMax;
	Hobject ScaleImageMean, ImageErosion;
	//Ŀ��궨���ֱ������СΪ3
	int DotDiameter = 5;
	int PiexlShift = 2;
	int MeanImageMaskWidth = 3;
	dots_image(Image, &DotImage, DotDiameter, "dark", PiexlShift);
	scale_image_max(DotImage, &DotImageScaleMax);
	mean_image(DotImageScaleMax, &ScaleImageMean, MeanImageMaskWidth, MeanImageMaskWidth);
	gray_erosion_rect(ScaleImageMean, &ImageErosion, 1, MaskWidth);

	Hobject RegionDyhThreshold, ConnectedRegions, RegionFillUp;
	dyn_threshold(ScaleImageMean, ImageErosion, &RegionDyhThreshold, DynThreshold, "light");
	connection(RegionDyhThreshold, &ConnectedRegions);
	fill_up_shape(ConnectedRegions, &RegionFillUp, "area", 0, MaxAreaOfHole);

	Hobject RegionFillUpErosion, RegionErosionDilation;
	Hobject RegionIntersection, RegionOpening, ConnectedOpeningRegion, SelectedRegions;
	erosion_rectangle1(RegionFillUp, &RegionFillUpErosion, 7, 1);
	dilation_rectangle1(RegionFillUpErosion, &RegionErosionDilation, 9, 3);
	intersection(RegionFillUp, RegionErosionDilation, &RegionIntersection);
	opening_circle(RegionIntersection, &RegionOpening, OpeningMaskRadius+0.5);

	connection(RegionOpening, &ConnectedOpeningRegion);
	select_shape(ConnectedOpeningRegion, &SelectedRegions, ((HTuple("area").Append("inner_radius")).Append("outer_radius")).Append("circularity"), "and", TargetFeatureMin, TargetFeatureMax);
	union1(SelectedRegions, DotsRegion);
}



/***********************************************************************
* �������ƣ�H_RemoveNoiseReg
* �������ܣ�ȥ���궨�������е���������
* ���������Image -- Բ�����̸�ͼ��
DilationRectHeight -- ����ģ��߶�
InnerRadiusMin -- ��С�ھ�ֵ
InnerRadiusMax -- ����ھ�ֵ
OuterRadiusMin -- ��С�⾶ֵ
OuterRadiusMax -- ����⾶ֵ
DilationRadiusForExtractDotReg -- �궨�����С����
* ���������DotsRegionRevomedNoise -- ȥ��������ı궨������
* �� �� ֵ��void
* ��    ����1.0
* ��    �ߣ�LukeChen
* ��    �ڣ�2015.10.17
************************************************************************/
DLLEXPT void CylinderCalibration:: H_RemoveNoiseReg(Hobject DotsRegion, Hobject* DotsRegionRevomedNoise, int DilationRectHeight, int InnerRadiusMin, int InnerRadiusMax, int OuterRadiusMin, int OuterRadiusMax, double DilationRadiusForExtractDotReg)
{
	gen_empty_obj(DotsRegionRevomedNoise);

	Hobject DotsRegDilation, ConnectedRegion, SelectedRegion;
	Hobject RegionUnion, RegionUnionDilation, RegionDilationFillup;
	dilation_rectangle1(DotsRegion, &DotsRegDilation, 2, DilationRectHeight);
	connection(DotsRegDilation, &ConnectedRegion);
	select_shape(ConnectedRegion, &SelectedRegion, (HTuple("inner_radius")).Append("outer_radius"), "and",(HTuple(InnerRadiusMin)).Append(OuterRadiusMin), (HTuple(InnerRadiusMax)).Append(OuterRadiusMax));
	union1(SelectedRegion, &RegionUnion);
	dilation_circle(RegionUnion, &RegionUnionDilation, DilationRadiusForExtractDotReg);
	fill_up(RegionUnionDilation, &RegionDilationFillup);
	intersection(DotsRegion, RegionDilationFillup, DotsRegionRevomedNoise);
}

/***********************************************************************
* �������ƣ�H_GetRowConnectedDotsRegion
* �������ܣ���ȡ���̸��������
* ���������DotsRegion -- ���̸�����
MinDistBetweenDots -- �궨���ľ���
OpeningCircleRadius -- �������ģ��뾶
InnerRadiusMin -- ��С�ھ�ֵ
InnerRadiusMax -- ����ھ�ֵ
OuterRadiusMin -- ��С�⾶ֵ
OuterRadiusMax -- ����⾶ֵ			
* ���������RowDotsRegion -- �����ĺ������̸�����
* �� �� ֵ��void
* ��    ����1.0
* ��    �ߣ�LukeChen
* ��    �ڣ�2015.10.19
************************************************************************/
DLLEXPT void CylinderCalibration:: H_GetRowConnectedDotsRegion(Hobject DotsRegion, Hobject* RowDotsRegion, int MinDistBetweenDots, double OpeningCircleRadius, double InnerRadiusMin, double InnerRadiusMax, double OuterRadiusMin, double OuterRadiusMax)
{
	gen_empty_obj(RowDotsRegion);
	Hobject DotsRegionDilation, ConnectedReg;
	Hobject ConnectedRegOpening, OpeningRegConnected, SelectedRegion;
	dilation_rectangle1(DotsRegion, &DotsRegionDilation, MinDistBetweenDots, 1);
	connection(DotsRegionDilation, &ConnectedReg);
	opening_circle(ConnectedReg, &ConnectedRegOpening, OpeningCircleRadius);
	connection(ConnectedRegOpening, &OpeningRegConnected);
	select_shape(OpeningRegConnected, &SelectedRegion, (HTuple("inner_radius")).Append("outer_radius"), "and",(HTuple(InnerRadiusMin)).Append(OuterRadiusMin), (HTuple(InnerRadiusMax)).Append(OuterRadiusMax));
	sort_region(SelectedRegion, RowDotsRegion, "first_point","true", "row");
}

/***********************************************************************
* �������ƣ�H_GetColConnectedDotsRegion
* �������ܣ���ȡ���������м䲿��
* ���������DotsRegion -- ���̸�����
ImageWidth -- ԭʼͼ����
ImageHeight -- ԭʼͼ��߶�
MinDistBetweenDots -- �궨���ľ���					
* ���������ColCentralDotsRegion -- �������������̸��м�����
* �� �� ֵ��void
* ��    ����1.0
* ��    �ߣ�LukeChen
* ��    �ڣ�2015.10.19
************************************************************************/
DLLEXPT void CylinderCalibration:: H_GetColConnectedDotsRegion(Hobject DotsRegion, Hobject* ColCentralDotsRegion, int ImageWidth, int ImageHeight, int MinDistBetweenDots)
{
	gen_empty_obj(ColCentralDotsRegion);
	Hobject ColRegion, CentralRect, CentralRectIntersection;
	Hobject ConnectedRectRegion, DotsRegionConnected;

	dilation_rectangle1(DotsRegion, &ColRegion, 1, MinDistBetweenDots);
	gen_rectangle1(&CentralRect, ImageHeight/2.0 - 200, 0, ImageHeight/2.0 + 200, ImageWidth -1);
	intersection(ColRegion, CentralRect, &CentralRectIntersection);
	connection(CentralRectIntersection, &ConnectedRectRegion);

	HTuple ContLength, ContLengthMean;
	contlength(ConnectedRectRegion, &ContLength);
	tuple_mean(ContLength, &ContLengthMean);
	select_shape(ConnectedRectRegion, ColCentralDotsRegion, "contlength", "and", ContLengthMean * 0.8, 99999);
}

/***********************************************************************
* �������ƣ�H_CreateRectifyMapUsePoints
* �������ܣ�������ȡ����У��Բ������λ�ã����ɷ������̸��ٻ��У��ӳ��
* ���������DotsRegion -- ���̸�����
RowDotsRegion -- �������̸�����
ColCentralDotsRegion -- �������̸��м�����
ImageWidth -- ԭʼͼ����
ImageHeight -- ԭʼͼ��߶�
GridSpacing -- Ŀ�귽��ʵ�ʿ��
Sigma -- connect_grid_points������Ϊ��˹�������� 
MaxDist -- connect_grid_points������У������������߳���
* ���������RectifyMap -- У��ӳ��
* �� �� ֵ��void
* ��    ����1.0
* ��    �ߣ�LukeChen
* ��    �ڣ�2015.10.19
************************************************************************/
DLLEXPT void CylinderCalibration:: H_CreateRectifyMapUsePoints(Hobject DotsRegion, Hobject RowDotsRegion, Hobject ColCentralDotsRegion, Hobject* RectifyMap, int ImageWidth, int ImageHeight, int GridSpacing, double Sigma, double MaxDist)
{
	gen_empty_obj(RectifyMap);

	HTuple RowNum, ColNum;
	count_obj(RowDotsRegion, &RowNum);
	count_obj(ColCentralDotsRegion, &ColNum);

	HTuple MatrixGridPtRow, MatrixGridPtCol;
	create_matrix(RowNum[0].I() + 10, ColNum[0].I() + 10, 0, &MatrixGridPtRow);
	create_matrix(RowNum[0].I() + 10, ColNum[0].I() + 10, 0, &MatrixGridPtCol);

	//��connect_grid_points�����õ�
	HTuple GridPtRows, GridPtCols;
	GridPtRows = HTuple();
	GridPtCols = HTuple();

	//����λ����Ϣ������
	for (int RowIndex = 1; RowIndex <= RowNum; RowIndex++)
	{
		Hobject ObjectSelected, SingleRowRect, SingleRowRectDilation;
		Hobject SingleRowRegion, RowRegionConnected, SelectedRegion;
		select_obj(RowDotsRegion, &ObjectSelected, RowIndex);
		shape_trans(ObjectSelected, &SingleRowRect, "rectangle2");	
		dilation_rectangle1(SingleRowRect, &SingleRowRectDilation, 5, 2);
		intersection(SingleRowRectDilation, DotsRegion, &SingleRowRegion);
		connection(SingleRowRegion, &RowRegionConnected);
		select_shape(RowRegionConnected, &SelectedRegion, "area", "and", 30, 99999);

		Hobject RegionSorted;
		HTuple Row, Col, Area;
		sort_region(SelectedRegion, &RegionSorted, "upper_left", "true", "column");
		area_center(RegionSorted, &Area, &Row, &Col);
		for (int ColIndex = 0; ColIndex <= ColNum - 1; ColIndex++)
		{
			set_value_matrix(MatrixGridPtRow, RowIndex, ColIndex, Row[ColIndex]);
			set_value_matrix(MatrixGridPtCol, RowIndex, ColIndex, Col[ColIndex]);
			GridPtRows.Append(Row[ColIndex]);
			GridPtCols.Append(Col[ColIndex]);
		}
	}

	Hobject image, GrayImage;
	gen_image_const(&image, "byte", ImageWidth, ImageHeight);
	//���ɻҶ�ֵΪ128��ͼ��
	scale_image(image, &GrayImage, 1, 128);

	HTuple SquarePtX, SquarePtY;
	tuple_gen_const(CAMERA_NUM+1, 0, &SquarePtX);
	tuple_gen_const(CAMERA_NUM+1, 0, &SquarePtY);

	BOOL IsBlackColor = TRUE;

	for (int RowIndexNew = 1; RowIndexNew <= RowNum - 1; RowIndexNew++)
	{
		if (RowIndexNew % 2 == 1)
		{
			IsBlackColor = FALSE;
		}
		else
		{
			IsBlackColor = TRUE;
		}

		for (int ColIndexNew = 0; ColIndexNew <= ColNum -2; ColIndexNew++)
		{
			double ValueX = 0.0;
			double ValueY = 0.0;
			//���Ͻ�Ϊ��ʼ�㣬��ʱ�뷽������˳��
			get_value_matrix(MatrixGridPtRow, RowIndexNew, ColIndexNew, &ValueY);
			get_value_matrix(MatrixGridPtCol, RowIndexNew, ColIndexNew, &ValueX);
			SquarePtX[3] = ValueX;
			SquarePtY[3] = ValueY;

			get_value_matrix(MatrixGridPtRow, RowIndexNew, ColIndexNew+1, &ValueY);
			get_value_matrix(MatrixGridPtCol, RowIndexNew, ColIndexNew+1, &ValueX);
			SquarePtX[0] = ValueX;
			SquarePtY[0] = ValueY;

			get_value_matrix(MatrixGridPtRow, RowIndexNew+1, ColIndexNew+1, &ValueY);
			get_value_matrix(MatrixGridPtCol, RowIndexNew+1, ColIndexNew+1, &ValueX);
			SquarePtX[1] = ValueX;
			SquarePtY[1] = ValueY;

			get_value_matrix(MatrixGridPtRow, RowIndexNew+1, ColIndexNew, &ValueY);
			get_value_matrix(MatrixGridPtCol, RowIndexNew+1, ColIndexNew, &ValueX);
			SquarePtX[2] = ValueX;
			SquarePtY[2] = ValueY;

			SquarePtX[4] = SquarePtX[0];
			SquarePtY[4] = SquarePtY[0];

			Hobject ContourPt, SquareRegion;
			gen_contour_polygon_xld(&ContourPt, SquarePtY, SquarePtX);
			paint_xld(ContourPt, GrayImage, &GrayImage, 255*(1 - IsBlackColor));
			IsBlackColor = !IsBlackColor;
		}
	}
	Hobject ConeectingLines, Mesh;
	connect_grid_points(GrayImage, &ConeectingLines, GridPtRows, GridPtCols, Sigma, MaxDist);
	gen_grid_rectification_map(GrayImage, ConeectingLines, RectifyMap, &Mesh,GridSpacing, 0, GridPtRows, GridPtCols, "coord_map_sub_pix");
}


DLLEXPT bool CylinderCalibration:: H_IsCalibImage_new(Hobject Image, Hobject* CanRegion, HTuple* InnerRadius, HTuple* OuterRadius)
{
	bool result = true;
	gen_empty_obj(CanRegion);

	//select the first obj
	Hobject Img;
	Hobject SearchROI, ImgReduced;
	Hobject ImgClosing, ImgAbsDiff;
	Hobject Reg, ConnectedReg, SelectedReg;
	Hobject RegionTrans,RegErosion,RegIntersection;

	HTuple  ChannelNum;
	HTuple  ImgWidth, ImgHeight;
	HTuple  MaskRadius = 11; //�Ҷȿ�����ģ����

	HTuple ObjNum;
	
	count_obj(Image, &ObjNum);


	for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
	{
		select_obj(Image, &Img, ObjIndex);
		count_channels(Img, &ChannelNum);
		if (ChannelNum[0].I() == 3)
		{
			rgb1_to_gray(Img, &Img);
		}
		get_image_size(Img, &ImgWidth, &ImgHeight);
		gen_rectangle2(&SearchROI, ImgHeight*0.5, ImgWidth*0.5, 0, ImgWidth*0.3, ImgHeight*0.4);
		reduce_domain(Img, SearchROI, &ImgReduced);
		gray_closing_rect(ImgReduced, &ImgClosing, MaskRadius, MaskRadius);
		abs_diff_image(ImgReduced, ImgClosing, &ImgAbsDiff, 1);
		threshold(ImgAbsDiff, &Reg, 50, 255); //��abs_diff_image��ǿ2ʱ�� ��С��ֵ�ʵ�����
		connection(Reg, &ConnectedReg);
		select_shape(ConnectedReg, &SelectedReg, "area", "and", 5, 99999);

		HTuple RegNumTemp;
		count_obj(SelectedReg, &RegNumTemp);
		if (RegNumTemp[0].I() == 0)
		{
			result = false;
			return result;
		}

		union1(SelectedReg, &SelectedReg);
		shape_trans(SelectedReg, &RegionTrans, "rectangle1");

		HTuple RegTransW, RegTransH, InnerR, OuterR, GridPtNum;

		region_features(RegionTrans, "width", &RegTransW);
		region_features(RegionTrans, "height", &RegTransH);
		erosion_rectangle1(RegionTrans, &RegErosion, RegTransW*0.25, RegTransH*0.4);

		intersection(SelectedReg, RegErosion, &RegIntersection);
		connection(RegIntersection, &RegIntersection);

		region_features(RegIntersection, "inner_radius", &InnerR);
		region_features(RegIntersection, "outer_radius", &OuterR);
		count_obj(RegIntersection, &GridPtNum);

		HTuple InnerRMean, InnerRDevia;
		HTuple OuterRMean, OuterRDevia;

		tuple_mean(InnerR, &InnerRMean);
		tuple_deviation(InnerR, &InnerRDevia);

		tuple_mean(OuterR, &OuterRMean);
		tuple_deviation(OuterR, &OuterRDevia);

		if (GridPtNum[0].I() > 10 && 1.0*OuterRMean/InnerRMean < 2.5 && InnerRDevia< 1 && OuterRDevia < 1)
		{
			(*InnerRadius).Append(InnerRMean);
			(*OuterRadius).Append(OuterRMean);

			Hobject CanRegionTemp;
			dilation_rectangle1(RegionTrans, &CanRegionTemp, ImgWidth, 1);
			concat_obj(*CanRegion, CanRegionTemp, CanRegion);
		}
		else
			result = false;
	}

	return result;
}

void CylinderCalibration:: H_ExtractCalibDots_new(Hobject Image, Hobject CanRegion, Hobject* DotsRegion, HTuple InnerRs, HTuple OuterRs, HTuple* ObjNum)
{
	gen_empty_obj(DotsRegion);

	//�ɼ��ݶ����������
	//HTuple ObjNum;
	count_obj(Image, ObjNum);

	Hobject SelectedImg, SelectedImgReduced, ImgClosing, ImgAbsDiff;
	Hobject Reg, ConnectedReg;
	Hobject CanRegionErosion;	


	HTuple  ImgWidth, ImgHeight;
	HTuple  MaskRadius = 11; //�Ҷȿ�����ģ����

	for (int ObjIndex = 1; ObjIndex <= (*ObjNum); ObjIndex++)
	{
		select_obj(Image, &SelectedImg, ObjIndex);

		HTuple ObjChannelNum;
		count_channels(SelectedImg, &ObjChannelNum);
		if (ObjChannelNum[0].I() == 3)
		{
			rgb1_to_gray(SelectedImg, &SelectedImg);
		}

		HTuple InnerR = InnerRs[ObjIndex-1];
		HTuple OuterR = OuterRs[ObjIndex-1];

		HTuple StdArea4CalibPt = InnerR*InnerR*4;
		HTuple AreaMin = 0.10*StdArea4CalibPt;
		HTuple AreaMax = 2*StdArea4CalibPt;

		HTuple InnerRMin = InnerR*0.3;
		HTuple InnerRMax = InnerR*1.5;

		HTuple OuterRMin = OuterR*0.5;
		HTuple OuterRMax = OuterR*1.5;

		HTuple CircularityMin = 0.25;
		HTuple CircularityMax = 1.0;

		HTuple FeatureMin = ((HTuple(AreaMin).Append(InnerRMin)).Append(OuterRMin)).Append(CircularityMin);
		HTuple FeatureMax = ((HTuple(AreaMax).Append(InnerRMax)).Append(OuterRMax)).Append(CircularityMax);
		HTuple Feature = ((HTuple("area").Append("inner_radius")).Append("outer_radius")).Append("circularity");	


		Hobject SelectedCanReg;
		select_obj(CanRegion, &SelectedCanReg, ObjIndex);
		erosion_rectangle1(SelectedCanReg, &CanRegionErosion, InnerR*2, InnerR*6);

		HTuple CanRegHeight;
		region_features(CanRegionErosion, "height", &CanRegHeight);

		HTuple OuterRStd_Dilation = 0.8*CanRegHeight;
		HTuple OuterRMin_Dilation = 0.2*OuterRStd_Dilation;
		HTuple OuterRMax_Dilation = 2*OuterRStd_Dilation;

		reduce_domain(SelectedImg, CanRegionErosion, &SelectedImgReduced);
		get_image_size(SelectedImg, &ImgWidth, &ImgHeight);
		gray_closing_rect(SelectedImgReduced, &ImgClosing, MaskRadius, MaskRadius);
		abs_diff_image(SelectedImgReduced, ImgClosing, &ImgAbsDiff, 1);

		threshold(ImgAbsDiff, &Reg, 50, 255); //��abs_diff_image��ǿ2ʱ�� ��С��ֵ�ʵ�����
		connection(Reg, &ConnectedReg);

		Hobject SelectedReg, SelectedRegDilation, SelectedRegNew;
		select_shape(ConnectedReg, &SelectedReg, Feature, "and", FeatureMin, FeatureMax);

		//remove noise
		union1(SelectedReg, &SelectedReg);
		dilation_rectangle1(SelectedReg, &SelectedRegDilation, 1, InnerR*4);
		connection(SelectedRegDilation, &SelectedRegDilation);

		select_shape(SelectedRegDilation, &SelectedRegNew, HTuple("inner_radius").Append("outer_radius"), "and", InnerRMin.Append(OuterRMin_Dilation), (InnerRMax*2).Append(OuterRMax_Dilation));
		union1(SelectedRegNew, &SelectedRegNew);
		intersection(SelectedRegNew, SelectedReg, &SelectedReg);

		concat_obj(*DotsRegion, SelectedReg, DotsRegion);
	}		
}

void CylinderCalibration:: H_GetRowConnectedDotsRegion_new(Hobject& DotsRegion, Hobject* RowDotsRegion, HTuple InnerRs, HTuple OuterRs, HTuple ObjNum, HTuple* RowNum, HTuple* ErrorMsg)
{
	gen_empty_obj(RowDotsRegion);
	*ErrorMsg = HTuple();
	*RowNum = HTuple();

	Hobject RegionClosing, ConnectedReg, SortedReg;
	Hobject CentralReg, CentralDotsReg;
	HTuple RegNum, CentralColNum;
	Hobject RowDotsRegTemp;
	Hobject DotsRegionTemp;
	gen_empty_obj(&DotsRegionTemp);

	for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
	{
		Hobject SelectReg;
		select_obj(DotsRegion, &SelectReg, ObjIndex);

		HTuple InnerR = InnerRs[ObjIndex-1];
		HTuple OuterR = OuterRs[ObjIndex-1];

		closing_rectangle1(SelectReg, &RegionClosing, InnerR*6, 2);
		connection(RegionClosing, &ConnectedReg);
		sort_region(ConnectedReg, &SortedReg, "upper_left", "true", "row");
		count_obj(SortedReg, &RegNum);
		select_obj(SortedReg, &CentralReg, RegNum/2+RegNum%2);
		intersection(SelectReg, CentralReg, &CentralDotsReg);
		connection(CentralDotsReg, &CentralDotsReg);
		count_obj(CentralDotsReg, &CentralColNum);

		gen_empty_obj(&RowDotsRegTemp);
		Hobject SelectedSortReg, RegionIntersect;
		Hobject SingleConnectedReg;
		HTuple  ColNumPerRow;

		for (int RegIndex = 1; RegIndex <= RegNum; RegIndex++)
		{
			select_obj(SortedReg, &SelectedSortReg, RegIndex);
			intersection(SelectReg, SelectedSortReg, &RegionIntersect);
			connection(RegionIntersect, &SingleConnectedReg);
			count_obj(SingleConnectedReg, &ColNumPerRow);
			if (ColNumPerRow == CentralColNum)
			{
				concat_obj(RowDotsRegTemp, SelectedSortReg, &RowDotsRegTemp);
			}
		}

		union1(RowDotsRegTemp, &RowDotsRegTemp);
		concat_obj(*RowDotsRegion, RowDotsRegTemp, RowDotsRegion);

		//����DotsRegion
		Hobject RegIntersectTemp;

		intersection(RowDotsRegTemp, SelectReg, &RegIntersectTemp);

		if (ObjIndex < ObjNum)
		{
			concat_obj(DotsRegionTemp, RegIntersectTemp, &DotsRegionTemp);
		}
		else
		{
			concat_obj(DotsRegionTemp, RegIntersectTemp, &DotsRegion);
		}

		Hobject RegTemp;
		HTuple RowNumTemp;
		connection(RowDotsRegTemp, &RegTemp);
		count_obj(RegTemp, &RowNumTemp);

		(*RowNum).Append(RowNumTemp);
	}	

}


void CylinderCalibration:: H_GetColConnectedDotsRegion_new(Hobject DotsRegion, Hobject* ColDotsRegion, HTuple InnerRs, HTuple OuterRs, HTuple ObjNum, HTuple RowNum, HTuple* ColNum, HTuple* ErrorMsg)
{
	gen_empty_obj(ColDotsRegion);
	*ErrorMsg = HTuple();

	Hobject SelectedDotsReg, RegionDilation, SortedRegion;
	Hobject ObjSelected;
	Hobject ColDotsRegionTemp;
	Hobject RegIntersect;

	for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
	{
		select_obj(DotsRegion, &SelectedDotsReg, ObjIndex);

		HTuple InnerR = InnerRs[ObjIndex-1];
		HTuple OuterR = OuterRs[ObjIndex-1];

		dilation_rectangle1(SelectedDotsReg, &RegionDilation, 1, InnerR*4);
		connection(RegionDilation, &RegionDilation);
		sort_region(RegionDilation, &SortedRegion, "upper_left","false", "column");
		HTuple ColRegNum, CalibPtNumPerCol;
		count_obj(SortedRegion, &ColRegNum);


		gen_empty_obj(&ColDotsRegionTemp);

		for (int RegIndex = 1; RegIndex <= ColRegNum; RegIndex++)
		{
			select_obj(SortedRegion, &ObjSelected, RegIndex);
			intersection(SelectedDotsReg, ObjSelected, &RegIntersect);
			connection(RegIntersect, &RegIntersect);
			count_obj(RegIntersect, &CalibPtNumPerCol);
			if (CalibPtNumPerCol == RowNum[ObjIndex-1].I())
			{
				concat_obj(ColDotsRegionTemp, ObjSelected, &ColDotsRegionTemp);
			}
		}
		union1(ColDotsRegionTemp, &ColDotsRegionTemp);

		concat_obj(*ColDotsRegion, ColDotsRegionTemp, ColDotsRegion);

		Hobject RegTempConnected;
		HTuple  ColNumTemp;
		connection(ColDotsRegionTemp, &RegTempConnected);
		count_obj(RegTempConnected, &ColNumTemp);
		(*ColNum).Append(ColNumTemp);
	}

}

void CylinderCalibration:: H_CreateRectifyMapUsePoints(Hobject Image, Hobject DotsRegion, Hobject RowDotsRegion, Hobject ColDotsRegion, Hobject* Map, Hobject* MapRegInOrigImg, HTuple ObjNum, HTuple RowNum, HTuple ColNum, HTuple* ErrorMsg)
{
	gen_empty_obj(Map);
	gen_empty_obj(MapRegInOrigImg);
	*ErrorMsg = HTuple();

	Hobject SelectedDotsReg, SelectedRowDotsReg, SelectedColDotsReg;
	//Ӱ���ٶ�
	HTuple GridSpacing = 10;

	for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
	{
		Hobject SelectedImg;
		select_obj(Image, &SelectedImg, ObjIndex);
		HTuple ImgW, ImgH;
		get_image_size(SelectedImg, &ImgW, &ImgH);

		select_obj(DotsRegion, &SelectedDotsReg, ObjIndex);
		select_obj(RowDotsRegion, &SelectedRowDotsReg, ObjIndex);
		select_obj(ColDotsRegion, &SelectedColDotsReg, ObjIndex);

		Hobject ConnectedColDotsReg, SortedColRegion;

		connection(SelectedColDotsReg, &ConnectedColDotsReg);
		sort_region(ConnectedColDotsReg, &SortedColRegion, "upper_left","false", "column");

		HTuple GridPtRows = HTuple();
		HTuple GridPtCols = HTuple();

		Hobject SelectedSortReg, RegIntersect;
		Hobject RegIntersectConnected, SortedReg;


		for (int ColIndex = 1; ColIndex <= ColNum; ColIndex++)
		{
			select_obj(SortedColRegion, &SelectedSortReg, ColIndex);
			intersection(SelectedSortReg, SelectedDotsReg, &RegIntersect);
			connection(RegIntersect, &RegIntersectConnected);
			sort_region(RegIntersectConnected, &SortedReg, "first_point", "true", "row");

			HTuple PtNum, area, PtRow, PtCol;
			count_obj(SortedReg, &PtNum);
			if (PtNum == RowNum[ObjIndex-1].I())
			{
				area_center(SortedReg, &area, &PtRow, &PtCol);
				GridPtRows.Append(PtRow);
				GridPtCols.Append(PtCol);
			}
		}

		Hobject MapTemp, MapRegInOrigImgTemp;
		gen_arbitrary_distortion_map(&MapTemp, GridSpacing, GridPtRows, GridPtCols, RowNum[ObjIndex-1], ImgW,ImgH, "coord_map_sub_pix");
		H_GetMapOrginReg(MapTemp, &MapRegInOrigImgTemp);

		concat_obj(*Map, MapTemp, Map);
		concat_obj(*MapRegInOrigImg, MapRegInOrigImgTemp, MapRegInOrigImg);
	}

	/*write_image(*Map, "tiff", 0, "c:\\map.tiff");*/
}


//���Բ�α궨��ͼ����ȡ����ӳ��
DLLEXPT bool CylinderCalibration:: H_GetMap(Hobject CalibImg, Hobject CanRegion, Hobject* RectifyMap, Hobject* MapRegInOrigImg, HTuple InnerR, HTuple OuterR, HTuple* ErrorMsg)
{
	bool result = false;
	gen_empty_obj(RectifyMap);
	*ErrorMsg = HTuple();


	Hobject DotsRegion, RowDotsRegion, ColDotsRegion;
	HTuple  ObjNum, RowNum, ColNum;
	H_ExtractCalibDots_new(CalibImg, CanRegion, &DotsRegion, InnerR, OuterR, &ObjNum);
	H_GetRowConnectedDotsRegion_new(DotsRegion, &RowDotsRegion, InnerR, OuterR, ObjNum, &RowNum, ErrorMsg);
	H_GetColConnectedDotsRegion_new(DotsRegion, &ColDotsRegion, InnerR, OuterR, ObjNum, RowNum, &ColNum, ErrorMsg);
	H_CreateRectifyMapUsePoints(CalibImg, DotsRegion, RowDotsRegion, ColDotsRegion, RectifyMap, MapRegInOrigImg, ObjNum, RowNum, ColNum, ErrorMsg);

	if (*ErrorMsg == HTuple())
	{
		result = true;
	}

	return result;
}

DLLEXPT bool CylinderCalibration:: H_MapImgs(Hobject Images, Hobject Maps, Hobject* MappedImgs, HTuple* ErrorMsg)
{
	bool result = false;

	gen_empty_obj(MappedImgs);
	*ErrorMsg = HTuple();

	HTuple ImgNum, MapNum;
	count_obj(Images, &ImgNum);
	count_obj(Maps, &MapNum);

	if (ImgNum != MapNum)
	{
		(*ErrorMsg).Append("Map�ļ��뵱ǰͼ��������һ�£�");
		return result;
	}


	Hobject SelectedImg, SelectedMap, MappedImgTemp;


	for (int ObjIndex = 1; ObjIndex <= MapNum; ObjIndex++)
	{
		select_obj(Images, &SelectedImg, ObjIndex);
		select_obj(Maps,   &SelectedMap, ObjIndex);
		map_image(SelectedImg, SelectedMap, &MappedImgTemp);
		concat_obj(*MappedImgs, MappedImgTemp, MappedImgs);
		result = true;
	}


	return result;
}

//ƽ������
void  SmoothFeatureVec(HTuple FeatureVector, HTuple Sigma, HTuple* SmoothedFeatureVec)
{
	*SmoothedFeatureVec = HTuple();

	HTuple Function;
	HTuple SmoothedFunction;
	HTuple XValue;

	create_funct_1d_array(FeatureVector, &Function);
	smooth_funct_1d_gauss(Function, Sigma, &SmoothedFunction);

	//ע��ƽ������ֵΪС��ֵ
	funct_1d_to_pairs(SmoothedFunction, &XValue, SmoothedFeatureVec);
}

DLLEXPT void CylinderCalibration:: H_GetFeatureVector(Hobject MappedImgs, Hobject ProjROI, HTuple* FeatureVector)
{
	*FeatureVector = HTuple();

	HTuple ObjNum, RegNum;

	count_obj(MappedImgs, &ObjNum);
	count_obj(ProjROI, &RegNum);
	if (RegNum != ObjNum)
	{
		//Ĭ��ʹ������ͼ������
		get_domain(MappedImgs, &ProjROI);
	}

	Hobject SelectedImg, SelectedProjROI;
	Hobject ImgR, ImgG, ImgB, ImgH, ImgS, ImgV;
	HTuple  HorProj_H, VertProj_H, HorProj_S, VertProj_S, HorProj_V, VertProj_V;	
	HTuple HorProj_H_Smoothed, HorProj_S_Smoothed, HorProj_V_Smoothed;
	HTuple  Sigma = 2.0;		//���ܿ���Ϊ���ڲ���

	for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
	{
		select_obj(MappedImgs, &SelectedImg, ObjIndex);
		select_obj(ProjROI, &SelectedProjROI, ObjIndex);

		HTuple ChannelNum;
		count_channels(SelectedImg, &ChannelNum);

		if (ChannelNum[0].I() == 3)
		{
			decompose3(SelectedImg, &ImgR, &ImgG, &ImgB);
			trans_from_rgb(ImgR, ImgG, ImgB, &ImgH, &ImgS, &ImgV, "hsv");

			gray_projections(SelectedProjROI, ImgH, "simple", &HorProj_H, &VertProj_H);
			gray_projections(SelectedProjROI, ImgS, "simple", &HorProj_S, &VertProj_S);
			gray_projections(SelectedProjROI, ImgV, "simple", &HorProj_V, &VertProj_V);

			//��Ҫƽ��������Ӧ����߶��ϵĲ���


			SmoothFeatureVec(HorProj_H, Sigma, &HorProj_H_Smoothed);
			SmoothFeatureVec(HorProj_S, Sigma, &HorProj_S_Smoothed);
			SmoothFeatureVec(HorProj_V, Sigma, &HorProj_V_Smoothed);


			(*FeatureVector).Append(HorProj_H_Smoothed);
			(*FeatureVector).Append(HorProj_S_Smoothed);
			(*FeatureVector).Append(HorProj_V_Smoothed);
		}
		else
		{
			gray_projections(SelectedProjROI, SelectedImg, "simple", &HorProj_H, &VertProj_H);
			SmoothFeatureVec(HorProj_H, Sigma, &HorProj_H_Smoothed);
			(*FeatureVector).Append(HorProj_H_Smoothed);
		}		
	}
}

DLLEXPT void CylinderCalibration:: H_GetFeatureVector_FixReg(Hobject OriginImg, Hobject ModellingRegs, Hobject OriginPartitionROI_1, Hobject OriginPartitionROI_2,Hobject OriginPartitionUnionROI_1, Hobject OriginPartitionUnionROI_2, HTuple* FeatureVector)
{
	*FeatureVector = HTuple();

	HTuple ObjNum, RegNum;

	count_obj(OriginImg, &ObjNum);
	count_obj(ModellingRegs, &RegNum);
	if (RegNum != ObjNum)
	{
		//��ʾ����
		return;
	}

	Hobject SelectedImg, SelectedProjROI;
	Hobject ImgR, ImgG, ImgB, ImgH, ImgS, ImgV;
	HTuple  HorProj_H, VertProj_H, HorProj_S, VertProj_S, HorProj_V, VertProj_V;	
	HTuple HorProj_H_Smoothed, HorProj_S_Smoothed, HorProj_V_Smoothed;
	HTuple  Sigma = 2.0;		//���ܿ���Ϊ���ڲ���

	for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
	{
		select_obj(OriginImg, &SelectedImg, ObjIndex);
		select_obj(ModellingRegs, &SelectedProjROI, ObjIndex);

		Hobject ReducedImg;
//		reduce_domain(SelectedImg, SelectedProjROI, &ReducedImg);

		HTuple ChannelNum;
		count_channels(SelectedImg, &ChannelNum);
		
		
		Hobject CalcROI;
		Hobject  CalcROIUnion;
		if (ObjIndex == 1)
		{
			CalcROI = OriginPartitionROI_1;
			CalcROIUnion =OriginPartitionUnionROI_1;
		}
		else
		{
			CalcROI = OriginPartitionROI_2;
			CalcROIUnion =OriginPartitionUnionROI_2;
		}

		if (ChannelNum[0].I() == 3)
		{
			reduce_domain(SelectedImg, CalcROIUnion, &ReducedImg);
			decompose3(ReducedImg,&ImgR, &ImgG, &ImgB);
		    trans_from_rgb(ImgR, ImgG, ImgB, &ImgH, &ImgS, &ImgV, "hsv");

			intensity(CalcROI, ImgH, &HorProj_H, &VertProj_H);
			intensity(CalcROI, ImgS, &HorProj_S, &VertProj_S);
			intensity(CalcROI, ImgV, &HorProj_V, &VertProj_V);

			//��Ҫƽ��������Ӧ����߶��ϵĲ���
			SmoothFeatureVec(HorProj_H, Sigma, &HorProj_H_Smoothed);
			SmoothFeatureVec(HorProj_S, Sigma, &HorProj_S_Smoothed);
			SmoothFeatureVec(HorProj_V, Sigma, &HorProj_V_Smoothed);


			(*FeatureVector).Append(HorProj_H_Smoothed);
			(*FeatureVector).Append(HorProj_S_Smoothed);
			(*FeatureVector).Append(HorProj_V_Smoothed);
		}
		else
		{
			intensity(CalcROI, SelectedImg, &HorProj_H, &VertProj_H);
	//		gray_projections(SelectedProjROI, SelectedImg, "simple", &HorProj_H, &VertProj_H);
			SmoothFeatureVec(HorProj_H, Sigma, &HorProj_H_Smoothed);
			(*FeatureVector).Append(HorProj_H_Smoothed);
		}		
	}
}

DLLEXPT void CylinderCalibration:: H_GetParallelFeatureVector_FixReg(Hobject OriginImg, Hobject ModellingRegs, Hobject OriginPartitionROI_1, Hobject OriginPartitionROI_2,Hobject OriginPartitionUnionROI_1, Hobject OriginPartitionUnionROI_2, HTuple* FeatureVector)
{
	HTuple ObjNum, RegNum,FeatureVectors;
	count_obj(OriginImg, &ObjNum);
	count_obj(ModellingRegs, &RegNum);
	if (RegNum != ObjNum)
	{
		return ;
	}
	m_vROI.clear();
	for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
	{
		HobjectPacForCan tmp;
		Hobject SelectedImg,SelectedProjROI;
		select_obj(OriginImg, &SelectedImg, ObjIndex);
		tmp.m_crtSingleImage       = SelectedImg;
		tmp.m_objIndex             = ObjIndex;
		m_vROI.push_back(tmp);
	}
	Concurrency::concurrent_vector<HTuple> vFeatureVector;
	vFeatureVector.resize(2);
	//std::for_each(m_vROI.begin(),m_vROI.end(), [&](HobjectPacForCan& it)
	parallel_for_each(m_vROI.begin(),m_vROI.end(), [&](HobjectPacForCan& it)
	{
		Hobject SelectedImg, SelectedProjROI,ReducedImg;
		Hobject ImgR, ImgG, ImgB, ImgH, ImgS, ImgV;
		HTuple  HorProj_H, VertProj_H, HorProj_S, VertProj_S, HorProj_V, VertProj_V;	
		HTuple HorProj_H_Smoothed, HorProj_S_Smoothed, HorProj_V_Smoothed,FeatureVector;
		HTuple  Sigma = 2.0;		//���ܿ���Ϊ���ڲ���
		HTuple ChannelNum;
		count_channels(it.m_crtSingleImage, &ChannelNum);
		Hobject CalcROI,CalcROIUnion;
		if (it.m_objIndex == 1)
		{
			CalcROI = OriginPartitionROI_1;
			CalcROIUnion =OriginPartitionUnionROI_1;
		}
		else
		{
			CalcROI = OriginPartitionROI_2;
			CalcROIUnion =OriginPartitionUnionROI_2;
		}

		if (ChannelNum[0].I() == 3)
		{
			reduce_domain(it.m_crtSingleImage, CalcROIUnion, &ReducedImg);
			decompose3(ReducedImg,&ImgR, &ImgG, &ImgB);
			trans_from_rgb(ImgR, ImgG, ImgB, &ImgH, &ImgS, &ImgV, "hsv");

			intensity(CalcROI, ImgH, &HorProj_H, &VertProj_H);
			intensity(CalcROI, ImgS, &HorProj_S, &VertProj_S);
			intensity(CalcROI, ImgV, &HorProj_V, &VertProj_V);

			//��Ҫƽ��������Ӧ����߶��ϵĲ���
			SmoothFeatureVec(HorProj_H, Sigma, &HorProj_H_Smoothed);
			SmoothFeatureVec(HorProj_S, Sigma, &HorProj_S_Smoothed);
			SmoothFeatureVec(HorProj_V, Sigma, &HorProj_V_Smoothed);

			(FeatureVector).Append(HorProj_H_Smoothed);
			(FeatureVector).Append(HorProj_S_Smoothed);
			(FeatureVector).Append(HorProj_V_Smoothed);
		}
		else
		{
			intensity(CalcROI, it.m_crtSingleImage, &HorProj_H, &VertProj_H);
			SmoothFeatureVec(HorProj_H, Sigma, &HorProj_H_Smoothed);
			(FeatureVector).Append(HorProj_H_Smoothed);
		}		
		vFeatureVector[it.m_objIndex-1]=FeatureVector;
		//vFeatureVector.push_back(FeatureVector);
	});
	*FeatureVector = HTuple();
	std::for_each(vFeatureVector.begin(),vFeatureVector.end(),[FeatureVector](HTuple tup)
	{
		(*FeatureVector).Append(tup);
	});
}

DLLEXPT void CylinderCalibration:: H_Calc_Vector_Diff(HTuple ReferMatrix, HTuple FeatureVector, HTuple TrainedImgNum, HTuple* DiffValue, HTuple* DiffIndex, HTuple Method)
{
	*DiffIndex = -1;
	*DiffValue = -1;

	HTuple MatrixRowNum, MatrixColNum;	
	HTuple VectorLength;

	get_size_matrix(ReferMatrix, &MatrixRowNum, &MatrixColNum);
	tuple_length(FeatureVector, &VectorLength);


	if (MatrixRowNum != VectorLength)
	{
		//����ά�Ȳ�һ�£�
		return;
	} 
	else
	{
		HTuple ReferMatrixSub;
		get_sub_matrix(ReferMatrix, 0, 0, MatrixRowNum, TrainedImgNum, &ReferMatrixSub);

		HTuple MatrixID, MatrixRepeatedID, MatrixResultID;

		create_matrix(MatrixRowNum, 1, FeatureVector, &MatrixID);
		repeat_matrix(MatrixID, 1, TrainedImgNum, &MatrixRepeatedID);
		sub_matrix_mod(ReferMatrixSub, MatrixRepeatedID);
		abs_matrix_mod(ReferMatrixSub);

		if (Method == "Sqrt_sum")
		{
			pow_scalar_element_matrix_mod(ReferMatrixSub, 2);
			sum_matrix(ReferMatrixSub, "columns", &MatrixResultID);
			sqrt_matrix_mod(MatrixResultID);
		}

		HTuple Values, MinValue;

		get_full_matrix(MatrixResultID, &Values);
		tuple_min(Values, &MinValue);

		*DiffValue = 1.0*MinValue[0].D()/sqrt((double)(MatrixRowNum[0].I()));

		tuple_find(Values, MinValue, DiffIndex);

		clear_matrix(ReferMatrixSub);
		clear_matrix(MatrixID);
		clear_matrix(MatrixRepeatedID);
		clear_matrix(MatrixResultID);
	}
}

//��Ե���Map�ü�
DLLEXPT void CylinderCalibration:: H_CropMap(Hobject MapImg, Hobject* RegInOrigImg ,HTuple StartRowPercent, HTuple EndRowPercent, Hobject* MapImgNew)
{
	gen_empty_obj(RegInOrigImg);
	gen_empty_obj(MapImgNew);

	HTuple ImgWidth, ImgHeight;

	Hobject SelectedMap;
	Hobject RowVec, ColVec;

	select_obj(MapImg, &SelectedMap, 1);

	get_image_size(SelectedMap, &ImgWidth, &ImgHeight);
	vector_field_to_real(SelectedMap, &RowVec, &ColVec);

	HTuple StartRow, EndRow;

	tuple_int(1.0*ImgHeight*StartRowPercent /100, &StartRow);
	tuple_int(1.0*ImgHeight*EndRowPercent /100, &EndRow);


	Hobject RowVecPart, ColVecPart;

	crop_part(RowVec, &RowVecPart, StartRow, 0, ImgWidth, EndRow - StartRow);
	crop_part(ColVec, &ColVecPart, StartRow, 0, ImgWidth, EndRow - StartRow);

	real_to_vector_field(RowVecPart, ColVecPart, MapImgNew, "vector_field_absolute");

	//���ɷ�ӳ������

	H_GetMapOrginReg(*MapImgNew, RegInOrigImg);
}

//����Map��������ԭʼͼ���е�����
DLLEXPT void CylinderCalibration:: H_GetMapOrginReg(Hobject MapImg, Hobject* MapRegInOrigImg)
{
	gen_empty_obj(MapRegInOrigImg);

	Hobject RowVec, ColVec;
	Hobject ImgDomain, RegionBorder;
	HTuple  RegRows, RegCols;
	HTuple  RowGrayval, ColGrayval;

	vector_field_to_real(MapImg, &RowVec, &ColVec);
	get_domain(MapImg, &ImgDomain);
	boundary(ImgDomain, &RegionBorder,"inner_filled");
	get_region_points(RegionBorder, &RegRows, &RegCols);
	get_grayval(RowVec, RegRows, RegCols, &RowGrayval);
	get_grayval(ColVec, RegRows, RegCols, &ColGrayval);

	Hobject RegTemp;
	gen_region_points(&RegTemp, RowGrayval, ColGrayval);
	fill_up(RegTemp, MapRegInOrigImg);
}

DLLEXPT void CylinderCalibration:: H_GetMapOrginRegDualChannel(Hobject MapImg, Hobject* MapRegInOrigImgs)
{
	gen_empty_obj(MapRegInOrigImgs);

	HTuple ObjNum;
	count_obj(MapImg, &ObjNum);

	Hobject RegTemp;
	Hobject SelectMap;
	for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
	{
		select_obj(MapImg, &SelectMap, ObjIndex);
		H_GetMapOrginReg(SelectMap, &RegTemp);
		concat_obj(*MapRegInOrigImgs, RegTemp, MapRegInOrigImgs);
	}

}

DLLEXPT void CylinderCalibration:: H_GetSingleMapRegInOrigImg(Hobject SingleMap, Hobject* SingleMapRegInOrigImg)
{
	gen_empty_obj(SingleMapRegInOrigImg);

	HTuple ImgW, ImgH;
	Hobject MapRow, MapCol;
	Hobject ImgDomain, PartitionedReg;

	get_image_size(SingleMap, &ImgW, &ImgH);
	vector_field_to_real(SingleMap, &MapRow, &MapCol);
	get_domain(SingleMap, &ImgDomain);
	partition_rectangle(ImgDomain, &PartitionedReg, ImgW, 1); //single row

	Hobject SelectedPartitionReg, RegTemp;
	HTuple  RegRows, RegCols, Grayval_Row, Grayval_Col;

	for (int RegIndex = 1; RegIndex <= ImgH[0].I(); RegIndex++)
	{
		select_obj(PartitionedReg, &SelectedPartitionReg, RegIndex);
		get_region_points(SelectedPartitionReg, &RegRows, &RegCols);

		get_grayval(MapRow, RegRows, RegCols, &Grayval_Row);
		get_grayval(MapCol, RegRows, RegCols, &Grayval_Col);

		gen_region_points(&RegTemp, Grayval_Row, Grayval_Col);
		concat_obj(*SingleMapRegInOrigImg, RegTemp, SingleMapRegInOrigImg);
	}
}

DLLEXPT void CylinderCalibration:: H_GetMultiMapRegInOrigImg(Hobject Maps, Hobject* OriginPartitionROI_1, Hobject* OriginPartitionROI_2)
{
	gen_empty_obj(OriginPartitionROI_1);
	gen_empty_obj(OriginPartitionROI_2);

	HTuple ObjNum;
	count_obj(Maps, &ObjNum);
	if (ObjNum > 2)
	{
		return;
	}

	Hobject SelectedMap;
	for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
	{
		select_obj(Maps, &SelectedMap, ObjIndex);

		if (ObjIndex == 1)
		{
			H_GetSingleMapRegInOrigImg(SelectedMap, OriginPartitionROI_1);
		} 
		else
		{
			H_GetSingleMapRegInOrigImg(SelectedMap, OriginPartitionROI_2);
		}
	}
}

/***********************************************************************
* �������ƣ�H_Stitch2Img
* �������ܣ�ƴ������ͼ��
* ���������LeftImg -- ��ͼ��
RightImg -- ��ͼ��
HomMat2D_R2L -- ��ͼ����ͼ���ӳ��
MatchedColumn -- ��ͼ���ģ������ͼ��ƥ�䵽��λ��			 
* ���������StitchedImg -- ƴ�ӽ��ͼ��
* �� �� ֵ��void
* ��    ����1.0
* ��    �ߣ�LukeChen
* ��    �ڣ�2015.10.26
************************************************************************/
DLLEXPT void Stitching :: H_Stitch2Img(Hobject LeftImg, Hobject RightImg, Hobject* StitchedImg, HTuple HomMat2D_R2L, HTuple MatchedColumn)
{
	gen_empty_obj(StitchedImg);

	HTuple ImgWidth1, ImgHeight1;
	HTuple ImgTransWidth2, ImgTransHeight2;
	HTuple ColumnModel;
	Hobject LeftImgPart, RightImgPart;
	tuple_round(MatchedColumn, &ColumnModel);

	get_image_size(LeftImg, &ImgWidth1, &ImgHeight1);
	crop_part(LeftImg, &LeftImgPart, 0, 0, ColumnModel+10, ImgHeight1);

	//ƴ�ӽ��ͼ�������
	HTuple MaxWidth = 8*ImgWidth1;

	Hobject Img2AffinTrans;
	affine_trans_image(RightImg, &Img2AffinTrans, HomMat2D_R2L, "bilinear", "true");
	get_image_size(Img2AffinTrans,  &ImgTransWidth2, &ImgTransHeight2);
	crop_part(Img2AffinTrans, &RightImgPart, 0, ColumnModel, ImgTransWidth2- ColumnModel, ImgHeight1);


	Hobject TiledImg1, TiledImg2;
	Hobject Rect1, Rect2;
	HTuple  Rows1, Cols1, Rows2, Cols2;
	HTuple  Grayvals1, Grayvals2;
	//ӳ�䵽ͬһ�ߴ�ͼ��
	tile_images_offset(LeftImgPart, &TiledImg1, 0, 0, 0, 0,  ImgHeight1 -1, ColumnModel + 9, MaxWidth, ImgHeight1);
	gen_rectangle1(&Rect1, 0,0, ImgHeight1-1, ColumnModel+10);
	get_region_points(Rect1, &Rows1, &Cols1);
	get_grayval(TiledImg1, Rows1, Cols1, &Grayvals1);
	gen_image_const(&TiledImg1, "byte", MaxWidth, ImgHeight1);
	set_grayval(TiledImg1, Rows1, Cols1, Grayvals1);

	tile_images_offset(RightImgPart, &TiledImg2, 0, ColumnModel, 0, 0, ImgHeight1-1, ImgTransWidth2- ColumnModel, MaxWidth, ImgHeight1);
	gen_rectangle1(&Rect2, 0, ColumnModel, ImgHeight1-1, ImgTransWidth2-1);
	get_region_points(Rect2, &Rows2, &Cols2);
	get_grayval(TiledImg2, Rows2, Cols2, &Grayvals2);
	gen_image_const(&TiledImg2, "byte", MaxWidth, ImgHeight1);
	set_grayval(TiledImg2, Rows2, Cols2, Grayvals2);

	HTuple BlendingSeam = 10;
	HTuple BSLeft, BSRight;
	HTuple CoefOneLineLeft, RowExtLeft, ColExtLeft;
	H_ComputeBlendingFactor(BlendingSeam, &BSLeft, &BSRight);
	//������ߴ�ƴ��ͼ��
	CoefOneLineLeft.Append(HTuple(ColumnModel[0].I()-1, 1));
	CoefOneLineLeft.Append(BSRight);
	RowExtLeft = HTuple(CoefOneLineLeft.Num(), 0);
	tuple_find(RowExtLeft, 0, &ColExtLeft);

	Hobject ImageM1, ImageM1Zoomed, ConvertedTiledImg1, LeftImgToStitch;
	gen_image_const(&ImageM1, "real", MaxWidth, 1);
	set_grayval(ImageM1, RowExtLeft, ColExtLeft, CoefOneLineLeft);
	zoom_image_size(ImageM1, &ImageM1Zoomed, MaxWidth, ImgHeight1, "nearest_neighbor");
	convert_image_type(TiledImg1, &ConvertedTiledImg1, "real");
	mult_image(ConvertedTiledImg1, ImageM1Zoomed, &LeftImgToStitch, 1, 0);

	//�����ұߴ�ƴ��ͼ��
	HTuple CoefOneLineRight, RowExtRight, ColExtRight;
	CoefOneLineRight.Append(BSLeft);
	CoefOneLineRight.Append(HTuple(ImgTransWidth2-ColumnModel-1, 1));
	RowExtRight = HTuple(CoefOneLineRight.Num(),0);
	tuple_find(RowExtRight, 0, &ColExtRight);
	ColExtRight = ColExtRight + ColumnModel[0].I();

	Hobject ImageM2, ImageM1Zoomed2, ConvertedTiledImg2, RightImgToStitch;
	gen_image_const(&ImageM2, "real", MaxWidth, 1);
	set_grayval(ImageM2, RowExtRight, ColExtRight, CoefOneLineRight);
	zoom_image_size(ImageM2, &ImageM1Zoomed2, MaxWidth, ImgHeight1, "nearest_neighbor");
	convert_image_type(TiledImg2, &ConvertedTiledImg2, "real");
	mult_image(ConvertedTiledImg2, ImageM1Zoomed2, &RightImgToStitch, 1, 0);


	//ƴ������ͼ��
	Hobject AddedImg, AddedImgCroped, StitchedImgByte;
	add_image(LeftImgToStitch, RightImgToStitch, &AddedImg, 1, 0);
	crop_part(AddedImg, &AddedImgCroped, 0, 0, ImgTransWidth2-3, ImgHeight1);
	convert_image_type(AddedImgCroped, &StitchedImgByte, "byte");
	copy_image(StitchedImgByte, StitchedImg);
}

/***********************************************************************
* �������ƣ�H_ComputeBlendingFactor
* �������ܣ�����ӷ촦������ͼ��ı���ϵ����ʹƴ�Ӵ����ɸ�ƽ��
* ���������BlendingSeamWidth -- �ӷ���				 
* ���������BlendingFactorLeft -- ��ͼ�����ϵ��
BlendingFactorRight -- ��ͼ�����ϵ��
* �� �� ֵ��void
* ��    ����1.0
* ��    �ߣ�LukeChen
* ��    �ڣ�2015.10.26
************************************************************************/
void Stitching:: H_ComputeBlendingFactor(HTuple BlendingSeamWidth, HTuple* BlendingFactorLeft, HTuple* BlendingFactorRight)
{
	*BlendingFactorLeft = HTuple();

	HTuple BSEven = (BlendingSeamWidth/2)*2;
	if (BSEven > BlendingSeamWidth){
		BSEven += 2;
	}

	HTuple BSStep;
	if (BSEven >= 2){
		BSStep = 1.0/BSEven;
		*BlendingFactorLeft = HTuple(BSEven,BSStep).Cumul();
		*BlendingFactorRight = (1.0+BSStep)-(*BlendingFactorLeft);
	} 
	else{
		*BlendingFactorLeft = HTuple();
		*BlendingFactorRight = 1;
	}
}

/***********************************************************************
* �������ƣ�H_LocateCircleUseProfile
* �������ܣ�ͨ����Բ�η�ձ�Ե��λ
* ���������Image--���붨λͼ�� 
SearchRegion--������������
Radius--���Բ������뾶
MinScore--��ȡ��Ե������ԽСԽ������ȡ��Ե����ͬʱҲ������ȡ����������
* ���������LocatedCircle--�������
Pt_X,Pt_Y--���X��Y���꣨��λ����ʱ����������Ϊͼ�����ģ���
* �� �� ֵ��Bool
* ��    ����1.0
* ��    �ߣ�ZhangLei
* ��    �ڣ�2016.01.19
************************************************************************/
DLLEXPT  BOOL H_LocateCircleUseProfile(Hobject Image, Hobject SearchRegion, Hobject* LocatedCircle, double Radius, int MinScore, double* Pt_X, double* Pt_Y)
{
	gen_empty_obj(LocatedCircle);

	Hobject  ImageReduced, ModelImages, ModelRegions; 
	Hobject  RegionDilation, ConnectedRegions, RegionFillUp, RegionErosion;
	Hobject  SelectedRegions, RegionUnion;
	HTuple   RegionUnion_Class, CircleArea, CenterRow, CenterCol;
	reduce_domain (Image, SearchRegion, &ImageReduced);
	inspect_shape_model(ImageReduced, &ModelImages, &ModelRegions, 1, MinScore);
	dilation_circle(ModelRegions, &RegionDilation, 2.5);

	connection(RegionDilation, &ConnectedRegions);
	fill_up(ConnectedRegions, &RegionFillUp);
	erosion_circle(RegionFillUp, &RegionErosion, 2.5);
	opening_circle (RegionErosion, &RegionErosion, 70);
	double AreaMax,AreaMin;
	AreaMin=(Radius-8)*(Radius-8)*3.1416;
	AreaMax=(Radius+8)*(Radius+8)*3.1416;
	select_shape(RegionErosion, &SelectedRegions, "area", "and", AreaMin, AreaMax);//inner_radius
	union1(SelectedRegions, &(*LocatedCircle));
	get_obj_class(*LocatedCircle, &RegionUnion_Class);
	if (RegionUnion_Class==HTuple("region"))
	{
		area_center(*LocatedCircle, &CircleArea, &CenterRow, &CenterCol);

		*Pt_Y = CenterRow[0].D();
		*Pt_X = CenterCol[0].D();
	}
	else
	{
		HTuple Width, Height;
		get_image_size(Image, &Width, &Height);
		gen_circle(LocatedCircle, Height/2.0, Width/2.0, Radius);
		*Pt_Y = 0.5 * Height[0].D();
		*Pt_X = 0.5 * Width[0].D();
		return  FALSE;
	}
	return  TRUE;
}
/***********************************************************************
* �������ƣ�H_LocateCircleUseEdges
* �������ܣ�ͨ��Բ������Ե��ϣ��õ����ĵ�λ��
* ���������Image--���붨λͼ�� 
SearchRegion--������������
Radius--���Բ������뾶
ThresholdLow--��Ե��ȡ����edges_sub_pix����������ֵ��
ThresholdHigh--��Ե��ȡ����edges_sub_pix����������ֵ��
circleScore--circularity�����ڸò����ı�Ե���Բ��ơ�
* ���������LocatedCircle--�������
Pt_X,Pt_Y--���X��Y���꣨��λ����ʱ����������Ϊͼ�����ģ���
* �� �� ֵ��Bool
* ��    ����1.0
* ��    �ߣ�ZhangLei
* ��    �ڣ�2016.01.19
************************************************************************/
DLLEXPT  BOOL H_LocateCircleUseEdges(Hobject Image, Hobject SearchRegion, Hobject* LocatedCircle, double Radius, int ThresholdLow, int ThresholdHigh, double circleScore,double* Pt_X, double* Pt_Y)
{
	gen_empty_obj(LocatedCircle);
	// Local control variables 	
	Hobject  ImageReduced, Edges;
	Hobject  ContoursSplit, UnionContours, SelectedXLD;

	HTuple  Number;
	HTuple  Row, Column,RadiusFit, StartPhi, EndPhi, PointOrder, RadiusSub, Indices;
	HTuple  CenterRowXld, CenterColXld,RadiusSelect;

	reduce_domain(Image, SearchRegion, &ImageReduced);
	if (ThresholdLow>=ThresholdHigh)
	{
		ThresholdLow=ThresholdHigh-1;
		if (ThresholdLow<=0)
		{
			ThresholdLow=10;
			ThresholdHigh=20;
		}
	}
	edges_sub_pix(ImageReduced, &Edges, "canny", 1, ThresholdLow, ThresholdHigh);
	segment_contours_xld(Edges, &ContoursSplit, "lines_circles", 5, 4, 2);
	union_cocircular_contours_xld(ContoursSplit, &UnionContours, 0.5, 0.1, 0.2, 30, 10, 10, "true", 1);
	select_shape_xld(UnionContours, &SelectedXLD, (HTuple("contlength").Append("circularity")), "and", (HTuple(100).Append(circleScore)), (HTuple(99999).Append(1)));
	count_obj(SelectedXLD, &Number);
	if (0 != (Number>=1))
	{
		fit_circle_contour_xld(SelectedXLD, "atukey", -1, 2, 0, 3, 2, &Row, &Column, &RadiusFit, &StartPhi, &EndPhi, &PointOrder);
		RadiusSub = (RadiusFit-Radius).Abs();
		tuple_sort_index(RadiusSub, &Indices);
		CenterRowXld = Row[HTuple(Indices[0])];
		CenterColXld = Column[HTuple(Indices[0])];
		RadiusSelect=RadiusFit[HTuple(Indices[0])];
		gen_circle(LocatedCircle,CenterRowXld,CenterColXld,Radius);
		*Pt_Y = CenterRowXld[0].D();
		*Pt_X = CenterColXld[0].D();
	}
	else
	{
		HTuple Width, Height;
		get_image_size(Image, &Width, &Height);
		gen_circle(LocatedCircle, Height/2.0, Width/2.0, Radius);
		*Pt_Y = 0.5 * Height[0].D();
		*Pt_X = 0.5 * Width[0].D();
		return  FALSE;
	}
	return  TRUE;
}
/***********************************************************************
* �������ƣ�H_CurlOptimizeAlgo
* �������ܣ�ͨ������Ҷ�任�����������ݡ������ȱ��
* ���������Image--������ͼ�� 
SearchRegion--����������
CenterX��CenterY--���ͼ�����Ķ�λ���ꡣ
RoiWidth--����������
Radius--����������Բ�뾶
FreqStart--FFT�˲��任��ʼƵ��
FreqCutoff--FFT�˲��任��ֹƵ��
DefectType--ȱ������
WaveThreshold--ѡ��ȱ����ȡ��ֵ��ԽСԽ������ȡȱ�ݡ�
MinArea--ȱ����С���
* ���������DetectRegion--ȱ������
pDetectNumber--ȱ�����
DictSpecialData--�˲���������
*�����ʼ�ǶȲ���90�ȡ�ƫ�Ҵ��4.4�����ҡ�
* �� �� ֵ��Bool
* ��    ����1.0
* ��    �ߣ�ת��LukeChen����������
* ��    �ڣ�2015.10.26
************************************************************************/
void ZoningRing(Hobject Image, Hobject Ring, Hobject MiddleCircle, Hobject* ZoningReg, HTuple CenterRow, HTuple  CenterColumn, int RingWidth, HTuple* tuple_RegMean_DCoffset)
{
	//�Ի���������зָ�
	Hobject ImageReduced, Rectangle;
	HTuple  Rows, Cols, RowCenter, ColumnCenter;
	HTuple  Length, Phi, HalfHeight, HalfWidth;
	HTuple  tuple_RegMean,Mean, Deviation, MeanValue;

	Hobject ObjectSelected, RegionIntersection;
	reduce_domain(Image, Ring, &ImageReduced);

	get_region_contour(MiddleCircle, &Rows, &Cols);
	tuple_length(Rows, &Length);
	tuple_gen_const(Length, CenterRow, &RowCenter);
	tuple_gen_const(Length, CenterColumn, &ColumnCenter);
	line_orientation(RowCenter, ColumnCenter, Rows, Cols, &Phi);

	tuple_gen_const(Length, 0.5, &HalfHeight);
	tuple_gen_const(Length, RingWidth/2, &HalfWidth);
	gen_rectangle2(&Rectangle, Rows, Cols, Phi, HalfWidth, HalfHeight);
	copy_obj(Rectangle, ZoningReg, 1, Length);

	tuple_RegMean = HTuple();
	intensity(Rectangle, ImageReduced, &tuple_RegMean, &Deviation);
	tuple_mean(tuple_RegMean, &MeanValue);
	tuple_sub(tuple_RegMean, MeanValue, tuple_RegMean_DCoffset);
}
double Sinc(double n)
{
	if (0 == n)
	{
		return (double) 1;
	} 
	else
	{
		return (double)(sin(PI*n)/(PI*n));
	}
}
void Unit_Impulse_Response(int paraLength, double angFreqStart, double angFreqCutoff, int winType, std::vector<double>& vFilterParam)
{
	signed int Count = 0;

	for (Count = -(paraLength-1)/2; Count <= (paraLength-1)/2; Count++)
	{
		vFilterParam.push_back ((angFreqCutoff/PI)*Sinc((angFreqCutoff/PI)*(double)(Count)) - (angFreqStart/PI)*Sinc((angFreqStart/PI)*(double)(Count)));
	}

	switch (winType)
	{
	case HAMMING:
		for (Count = -(paraLength-1)/2; Count <= (paraLength-1)/2; Count++)
		{
			vFilterParam[Count + (paraLength-1)/2] *= (0.54 + 0.46 * cos((2*PI*Count)/(paraLength-1)));
		}
		break;
	case BOXCAR:  //���δ�
		break;
	default:
		break;
	}
}
double CalcSum(int index, std::vector<double>& vFilterParam, int ParaLength, std::vector<double>& vOutTemp)
{
	double dSum = 0.0;
	return std::inner_product(vFilterParam.begin(), vFilterParam.end(), vOutTemp.begin() + index, dSum);
}
void PhaseAdjust(std::vector<double>& vOutputData, int FilterWidth)
{
	int outputSz = (int)vOutputData.size();
	std::vector<double> vOutTemp1;
	vOutTemp1.reserve(outputSz);

	vOutTemp1.assign(vOutputData.begin() + (outputSz-FilterWidth),  vOutputData.begin() + outputSz);
	vOutTemp1.insert(vOutTemp1.end(), vOutputData.begin(), vOutputData.begin() + outputSz - FilterWidth);
	std::copy(vOutTemp1.begin(), vOutTemp1.begin() + outputSz, vOutputData.begin());
}
void BandPass_Filter(std::vector<double>& vFilterParam, int ParaLength, std::vector<double>& vInputData, std::vector<double>& vOutputData)
{
	std::vector<double> vOutTemp;
	int inputSz = (int)vInputData.size();
	vOutTemp = vInputData;
	vOutTemp.insert(vOutTemp.end(), vInputData.begin(), vInputData.end());

	int index = inputSz - inputSz % 8;
	for (int k = 0; k < index; k+=8)
	{
		vOutputData.push_back(CalcSum(k,   vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+1, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+2, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+3, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+4, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+5, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+6, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+7, vFilterParam, ParaLength, vOutTemp));
	}

	int nLeftover = inputSz % 8;
	for (int k = 0; k < nLeftover; k++)
	{
		vOutputData.push_back(CalcSum(k, vFilterParam, ParaLength, vOutTemp));
	}
	PhaseAdjust(vOutputData, (ParaLength-1)/2);	
}
void Tuple_Replace(HTuple& tuple, HTuple Indices, HTuple ReplaceValue)
{
	HTuple  Length, SelectedIndice;
	tuple_length(Indices, &Length);
	for (int i = 0; i < Length; i++)
	{
		tuple_select(Indices, i, &SelectedIndice);
		tuple[SelectedIndice] = ReplaceValue;
	}
}
DLLEXPT  BOOL H_CurlOptimizeAlgo(Hobject Image,Hobject SearchRegion, Hobject* DetectRegion,float CenterX,float CenterY,float RoiWidth, float Radius , double FreqStart, double FreqCutoff, int DefectType, float WaveThreshold, int MinArea, int* pDetectNumber,std::vector<float>&DictSpecialData)
{	//���ô�ͨ�˲����ľ���Ż��㷨

	double angFreqStart = FreqStart * 2 * PI;
	double angFreqCutoff = FreqCutoff * 2 * PI;
	double angFreqStart0 = angFreqStart * 0.5;
	double angFreqCutoff0 = angFreqCutoff * 1.2;
	if (FreqCutoff <= FreqStart)
	{
		return false;
	}
	std::vector<double>  vInputData;  
	std::vector<double>  vOutputData;
	std::vector<double>  vFilterParam;
	Hobject RegionDifference_ext, ImageReduced_ext;
	Hobject  Ring, ZoningReg, MiddleCircle;
	HTuple MeanDiffValues, ImageWidth, ImageHeight, Area, Row_test, Column_test;
	get_image_size (Image, &ImageWidth, &ImageHeight);
	RegionDifference_ext = SearchRegion;
	reduce_domain(Image, RegionDifference_ext, &ImageReduced_ext);
	float MidRadius;
	if(Radius-RoiWidth > 0)
	{
		MidRadius = Radius - RoiWidth/2;
	}
	else
	{
		MidRadius = Radius/2;
	}
	gen_circle (&MiddleCircle, CenterY, CenterX, MidRadius);
	ZoningRing(Image, RegionDifference_ext, MiddleCircle, &ZoningReg, CenterY, CenterX,(int)abs(RoiWidth), &MeanDiffValues);
	HTuple  tempLength;
	tuple_length(MeanDiffValues, &tempLength);

	int InputDataLength = tempLength[0].I();
	for (int i = 0; i < InputDataLength; i++)
	{
		vInputData.push_back(MeanDiffValues[i].D());
	}


	int ParamLength = 0;
	int WinType=11;
	switch(WinType)
	{
	case 10:
		ParamLength = (int)((1.8*PI)/(angFreqCutoff0 - angFreqCutoff)+0.5);
		break;
	case 11:
		ParamLength = (int)((6.6*PI)/(angFreqCutoff0 - angFreqCutoff)+0.5);
		if(ParamLength>=0.5*InputDataLength)
		{
			ParamLength = (int)0.5*InputDataLength;
		}
		break;
	default:
		ParamLength = (int)((6.6*PI)/(angFreqCutoff0 - angFreqCutoff)+0.5);
		break;
	}
	if (0 == ParamLength%2)
		ParamLength++;

	Unit_Impulse_Response(ParamLength, angFreqStart, angFreqCutoff, WinType, vFilterParam);
	BandPass_Filter(vFilterParam, ParamLength, vInputData, vOutputData);

	HTuple BandpassMeanValue = HTuple();
	for (int j = 0; j < InputDataLength; j++)
	{
		BandpassMeanValue.Append((HTuple)vOutputData[j]);
	}

	HTuple absBandpassMeanValue, DefectIndices, tempDiff, sgnBandpass;
	HTuple sgnDiff, IndicesTemp;
	Hobject DefectReg, DefectRegs, DeftectRegConnection;
	tuple_sgn(BandpassMeanValue, &sgnBandpass);

	switch(DefectType)
	{
	case ALL:
		break;
	case LIGHT:
		tuple_find(sgnBandpass, -1, &IndicesTemp);
		if(IndicesTemp[0].I()>-1)
		{
			Tuple_Replace(BandpassMeanValue, IndicesTemp, 0);
		}
		break;
	case DARK:
		tuple_find(sgnBandpass, 1, &IndicesTemp);
		if(IndicesTemp[0].I()>-1)
		{
			Tuple_Replace(BandpassMeanValue, IndicesTemp, 0);
		}
		break;
	default:
		break;
	}

	tuple_abs(BandpassMeanValue, &absBandpassMeanValue);
	tuple_sub(absBandpassMeanValue, WaveThreshold, &tempDiff);
	tuple_sgn(tempDiff, &sgnDiff);
	tuple_find(sgnDiff, 1, &DefectIndices);
	//�������
	HTuple  maxValue = 0;
	tuple_max(absBandpassMeanValue, &maxValue);

	float YRange = 100;
	float YScaleFactor = float(YRange/maxValue[0].D());

	switch(DefectType)
	{
	case ALL:
		DictSpecialData.push_back(WaveThreshold * YScaleFactor);
		DictSpecialData.push_back((-1)* WaveThreshold * YScaleFactor);
		break;
	case LIGHT:
		DictSpecialData.push_back(WaveThreshold * YScaleFactor);
		DictSpecialData.push_back(-999999.0f);
		break;
	case DARK:
		DictSpecialData.push_back(999999.0f);
		DictSpecialData.push_back((-1)* WaveThreshold * YScaleFactor);
		break;
	default:
		break;
	}
	for(int i = 0; i < InputDataLength; i++)
	{
		DictSpecialData.push_back(float(BandpassMeanValue[i].D()*YScaleFactor*(-1)));
	}
	//sector
	Hobject CircleRegion,ExtractRegion,ExtractRegionDilation;
	if(Radius-RoiWidth > 2)
	{
		Hobject Circle1,Circle2;
		gen_circle (&Circle1, CenterY, CenterX, Radius);
		gen_circle (&Circle2, CenterY, CenterX, Radius-RoiWidth);
		difference(Circle1,Circle2,&CircleRegion);
	}
	else
	{
		gen_circle (&CircleRegion, CenterY, CenterX, Radius);
	}
	difference(CircleRegion,SearchRegion,&ExtractRegion);
	dilation_circle(ExtractRegion,&ExtractRegionDilation,2.5);
	//////////////////////
	if (DefectIndices != -1)
	{
		HTuple DefectNum;
		select_obj(ZoningReg, &DefectReg, DefectIndices+1);

		Hobject DefectUnionReg,DefectUnionRegDiff;
		union1(DefectReg, &DefectUnionReg);
		//
		difference(DefectUnionReg,ExtractRegionDilation,&DefectUnionRegDiff);
		//
		connection(DefectUnionRegDiff, &DeftectRegConnection);
		fill_up(DeftectRegConnection, &DeftectRegConnection);
		select_shape(DeftectRegConnection, &DefectReg, "area", "and", MinArea, 99999);
		count_obj(DefectReg, &DefectNum);
		union1 (DefectReg, &DefectRegs);
		area_center (DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			*pDetectNumber = 0;
		}
		else
		{
			*pDetectNumber = Area[0].I();
		}
		dilation_circle (DefectRegs, &DefectRegs, 2.5);
		*DetectRegion= DefectRegs;
	}
	else
	{
		*pDetectNumber = 0; 
		Hobject newRgn;
		*DetectRegion= newRgn;
	}		
	return true;
}
/***********************************************************************
* �������ƣ�H_SelectRegionExcludeLinear
* �������ܣ�ͨ������ϸ����ĳ���������ڽ�Բ�뾶��ֵ���޳���״����
* ���������RegionForSelect--��������
InnerRadius--ѡ�������������ڽ�Բ�뾶�����ڸò�������������
LengthRadiusRatio--ѡ�����������򳤶����ڽ�Բ�뾶��ֵ��С�ڸò�������������
* ���������BlackPoints--�������
* �� �� ֵ��void
* ��    ����1.0
* ��    �ߣ�ZhangLei
* ��    �ڣ�2016.01.27
************************************************************************/
DLLEXPT   void H_SelectRegionExcludeLinear (Hobject RegionForSelect,Hobject *BlackPoints, HTuple InnerRadius, HTuple LengthRadiusRatio)
{
	// Local iconic variables 
	Hobject  ConnectedRegions, Skeleton, ObjectSelected,SelectRegionShape;
	HTuple  Area1,Row1,Col1, Row, Column, Radius;
	HTuple  ContLength, Quot, Length, Index1;
	gen_empty_obj(&SelectRegionShape);
	area_center(RegionForSelect,&Area1,&Row1,&Col1);
	if (Area1>=1)
	{
		connection(RegionForSelect, &ConnectedRegions);
		inner_circle(ConnectedRegions, &Row, &Column, &Radius);
		skeleton(ConnectedRegions, &Skeleton);
		contlength(Skeleton, &ContLength);
		tuple_div(ContLength, Radius, &Quot);
		tuple_length(Quot, &Length);
		for (int Index1=0; Index1<=Length[0].I()-1; Index1++)
		{
			if ((Quot[Index1]<LengthRadiusRatio)&((Radius[Index1])>=InnerRadius))
			{
				select_obj(ConnectedRegions, &ObjectSelected, Index1+1);
				concat_obj(SelectRegionShape, ObjectSelected,&SelectRegionShape);   
			}
		}
	}
	*BlackPoints=SelectRegionShape;
	return;
}
/***********************************************************************
* �������ƣ�H_ColorImageTrans
* �������ܣ�24λ��ɫͼ�񾭹������õ�����ɫ�ʿռ����ͨ��ͼ�񣬲�ѡ������һ��ͨ����Ϊ�����
* ���������ColorImage--�����ɫͼ��
TransType--ѡ���ɫͼ��任������
ChannelSelect--ѡ�����ͼ���š�
* ���������ImageSelect--����任���ĳһ��ͨ��ͼ��
* �� �� ֵ��bool
* ��    ����1.0
* ��    �ߣ�ZhangLei
* ��    �ڣ�2016.05.16
************************************************************************/
DLLEXPT   BOOL H_ColorImageTransAll(Hobject ColorImage, Hobject* ImageSelect, int TransType, int ChannelSelect)
{
	HTuple  Channels;
	count_channels(ColorImage, &Channels);
	//24λ��ɫͼƬ
	if(Channels[0].I()==3)
	{
		Hobject ImageChannel1,ImageChannel2,ImageChannel3,ImageTrans1,ImageTrans2,ImageTrans3;
		decompose3 (ColorImage, &ImageChannel1,&ImageChannel2,&ImageChannel3);
		switch (TransType)
		{
		case 0:
			{
				ImageTrans1=ImageChannel1;
				ImageTrans2=ImageChannel2;
				ImageTrans3=ImageChannel3;
				break;
			}
		case 1:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "hsv");
				break;
			}
		case 2:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "hsi");
				break;
			}
		case 3:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "yuv");
				break;
			}

		case 4:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "hls");
				break;
			}
		case 5:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "ihs");
				break;
			}
		case 6:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "cielab");
				break;
			}
		default:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "hsi");
				return 0;
			}
		}
		switch (ChannelSelect)
		{
		case 1:
			{
				*ImageSelect=ImageTrans1;
				break;
			}
		case 2:
			{
				*ImageSelect=ImageTrans2;
				break;
			}
		case 3:
			{
				*ImageSelect=ImageTrans3;
				break;
			}
		default:
			{
				*ImageSelect=ImageTrans1;
				return 0;
			}
		}
		return 1;
	}
	else
	{
		return 0;
	}

}
DLLEXPT   BOOL H_ColorImageTrans(Hobject ColorImage, Hobject* ImageSelect, Hobject RegionDetect, int TransType, int ChannelSelect)
{
	Hobject ColorImageReduced,ReginDilation;
	dilation_circle (RegionDetect, &ReginDilation,21);
	reduce_domain(ColorImage,ReginDilation,&ColorImageReduced);
	HTuple  Channels=1;
	count_channels(ColorImageReduced, &Channels);
	//24λ��ɫͼƬ
	if(Channels[0].I()==3)
	{
		Hobject ImageChannel1,ImageChannel2,ImageChannel3,ImageTrans1,ImageTrans2,ImageTrans3;
		decompose3 (ColorImageReduced, &ImageChannel1,&ImageChannel2,&ImageChannel3);
		switch (TransType)
		{
		case 0:
			{
				ImageTrans1=ImageChannel1;
				ImageTrans2=ImageChannel2;
				ImageTrans3=ImageChannel3;
				break;
			}
		case 1:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "hsv");
				break;
			}
		case 2:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "hsi");
				break;
			}
		case 3:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "yuv");
				break;
			}

		case 4:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "hls");
				break;
			}
		case 5:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "ihs");
				break;
			}
		case 6:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "cielab");
				break;
			}
		default:
			{
				trans_from_rgb (ImageChannel1, ImageChannel2, ImageChannel3, &ImageTrans1, &ImageTrans2, &ImageTrans3, "hsi");
				return 0;
			}
		}
		switch (ChannelSelect)
		{
		case 1:
			{
				*ImageSelect=ImageTrans1;
				break;
			}
		case 2:
			{
				*ImageSelect=ImageTrans2;
				break;
			}
		case 3:
			{
				*ImageSelect=ImageTrans3;
				break;
			}
		default:
			{
				*ImageSelect=ImageTrans1;
				return 0;
			}
		}
		return 1;
	}
	else
	{
		return 0;
	}

}

