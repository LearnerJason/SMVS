#include "stdafx.h"
#include "CanOutside360_Core.h"

BOOL CanCalib:: HIsCalibImage(Hobject MultiImage, HTuple TargePtSizeInPixel, HTuple MinPtNum)
{
	BOOL Result = TRUE;
	HTuple ImageObjNum, CalibPtNum;
	Hobject Image, DotImage;
	Hobject DotRegion, DotRegionFillUp, ConnectedDotRegion, SelectDotRegions; 
	count_obj(MultiImage, &ImageObjNum);
	for (int ImageObjIndex = 1; ImageObjIndex <= ImageObjNum; ImageObjIndex++){
		select_obj(MultiImage, &Image, ImageObjIndex);
		dots_image(Image, &DotImage, 5, "dark", 2);
		threshold(DotImage, &DotRegion, 60, 255);
		fill_up(DotRegion, &DotRegionFillUp);
		connection(DotRegionFillUp, &ConnectedDotRegion);
		select_shape(ConnectedDotRegion, &SelectDotRegions, ((HTuple("circularity").Append("inner_radius")).Append("outer_radius")),
					"and", ((HTuple(0.3).Append(TargePtSizeInPixel*0.8)).Append(TargePtSizeInPixel)), ((HTuple(1).Append(TargePtSizeInPixel*2)).Append(TargePtSizeInPixel*2)));
		count_obj(SelectDotRegions, &CalibPtNum);

		if (CalibPtNum[0].I()  < MinPtNum){
			Result = FALSE;
		}
	}
	return Result;
}
