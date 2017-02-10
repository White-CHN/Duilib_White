#pragma once
class CAlphaBlendImpl : public IImageProcess
{
    // alpha »ìºÏ
    BOOL AlphaBlend(BYTE* desData, int desStride, int desLeft, int desTop, int desWidth, int desHeight, BYTE* srcData, int srcStride, int srcLeft, int srcTop, int srcWidth, int srcHeight, BYTE sourceAlpha = 255, int interpolationMode = 0);
    // Í¼Æ¬Ëõ·Å
    BOOL ImageScale(BYTE* DataDes, int desWidth, int desHeight, BYTE* DataSrc, int srcStride, int srcLeft, int srcTop, int srcWidth, int srcHeight, int interpolationMode = 0);
};

