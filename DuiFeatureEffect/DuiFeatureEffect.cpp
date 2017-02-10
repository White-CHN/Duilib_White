// DuiFeatureEffect.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "WndEffect.h"
#include "AlphaBlendImpl.h"

IUIEffect* GetAnimation(void)
{
    // 创建对象实例
    IUIEffect* pEffect =  dynamic_cast<IUIEffect*>(new CWndEffect());
    return pEffect != NULL ? pEffect : NULL;
}

BOOL ReleaseAnimation(IUIEffect*& pUIEffect)
{
    // 释放，置空
    CWndEffect* pWndEffect =  dynamic_cast<CWndEffect*>(pUIEffect);
    delete pWndEffect;
    pUIEffect = NULL;

    return TRUE;
}

DWORD GetSurportAnimationType(const char*& strAnimationType)
{
    return CWndEffect::InitSurportAnimationType(strAnimationType);
}

IImageProcess* GetImageProcess(void)
{
    // 创建对象实例
    IImageProcess* pProcess =  dynamic_cast<IImageProcess*>(new CAlphaBlendImpl());
    return pProcess != NULL ? pProcess : NULL;
}

BOOL ReleaseImageProcess(IImageProcess*& pProcess)
{
    // 释放，置空
    IImageProcess* pImageProcess =  dynamic_cast<CAlphaBlendImpl*>(pProcess);
    delete pImageProcess;
    pImageProcess = NULL;

    return TRUE;
}
