// DuiFeatureEffect.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "WndEffect.h"
#include "AlphaBlendImpl.h"

IUIEffect* GetAnimation(void)
{
    // ��������ʵ��
    IUIEffect* pEffect =  dynamic_cast<IUIEffect*>(new CWndEffect());
    return pEffect != NULL ? pEffect : NULL;
}

BOOL ReleaseAnimation(IUIEffect*& pUIEffect)
{
    // �ͷţ��ÿ�
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
    // ��������ʵ��
    IImageProcess* pProcess =  dynamic_cast<IImageProcess*>(new CAlphaBlendImpl());
    return pProcess != NULL ? pProcess : NULL;
}

BOOL ReleaseImageProcess(IImageProcess*& pProcess)
{
    // �ͷţ��ÿ�
    IImageProcess* pImageProcess =  dynamic_cast<CAlphaBlendImpl*>(pProcess);
    delete pImageProcess;
    pImageProcess = NULL;

    return TRUE;
}
