#include "StdAfx.h"
#include "AlphaBlendImpl.h"
#include "ImageProcess.h"


BOOL CAlphaBlendImpl::AlphaBlend(BYTE* desData, int desStride, int desLeft, int desTop, int desWidth, int desHeight, BYTE* srcData, int srcStride, int srcLeft, int srcTop, int srcWidth, int srcHeight, BYTE sourceAlpha, int interpolationMode)
{
    return CAlphaBlend::AlphaBlend(desData, desStride, desLeft, desTop, desWidth, desHeight, srcData, srcStride, srcLeft, srcTop, srcWidth, srcHeight, sourceAlpha, interpolationMode);
}

BOOL CAlphaBlendImpl::ImageScale(BYTE* DataDes, int desWidth, int desHeight, BYTE* DataSrc, int srcStride, int srcLeft, int srcTop, int srcWidth, int srcHeight, int interpolationMode)
{
    return CScale::ImageScale(DataDes, desWidth, desHeight, DataSrc, srcStride, srcLeft, srcTop, srcWidth, srcHeight, interpolationMode);
}
