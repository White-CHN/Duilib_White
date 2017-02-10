#include "StdAfx.h"
#include "WndEffect.h"



// 所有支持的动画
const char* CWndEffect::m_animation =  "向左翻转\0向右翻转\0向下翻转\0向上翻转\0马赛克\0向左滑动\0向右滑动\0向上滑动\0向下滑动\0水平带状滑动\0垂直带状滑动\0水平百叶\0垂直百叶\0不等高水平百叶\0不等宽垂直百叶\0向左擦除\0向右擦除\0向上擦除\0向下擦除\0向左惯性滑动\0向右惯性滑动\0向上惯性滑动\0向下惯性滑动\0淡出/淡入\0放大\0缩小\0摆出/摆入\0漏斗\0噪声\0左立方体\0右立方体\0上立方体\0下立方体\0向左渐变擦除\0向右渐变擦除\0向上渐变擦除\0向下渐变擦除\0向左推挤\0向右推挤\0向上推挤\0向下推挤\0溶解\0幕\0右三棱柱\0左三棱柱\0上三棱柱\0下三棱柱\0左内立方体\0右内立方体\0上内立方体\0下内立方体\0空间左移\0空间右移\0空间上移\0空间下移\0分块翻转\0空间向左旋转\0空间向右旋转\0空间向上旋转\0空间向下旋转\0开/关门\0漩涡\0打散\0褪色\0左下角阻尼滑动\0右下角阻尼滑动\0左上角阻尼滑动\0右上角阻尼滑动\0向左阻尼滑动\0向右阻尼滑动\0向上阻尼滑动\0向下阻尼滑动\0环形擦除\0风车\0卷页\0模糊淡入/出\0水波\0拖尾\0垂直折叠\0水平折叠\0\0";


void ClearContainerData(InternalAnimationParam& pElem)
{
    // 删除拷贝的内存
    delete []pElem.bmpDataCopy;
    pElem.bmpDataCopy = NULL;
    // 删除特效类
    pElem.pEffect->ReleaseEffectParam();
    delete dynamic_cast<CEffect*>(pElem.pEffect);
    pElem.pEffect = NULL;
}


CWndEffect::CWndEffect(void)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}


CWndEffect::~CWndEffect(void)
{
    // 清理资源
    for_each(m_animationContainer.begin(), m_animationContainer.end(), ClearContainerData);
    m_animationContainer.clear();

    if(m_gdiplusToken != 0)
    {
        Gdiplus::GdiplusShutdown(m_gdiplusToken);
    }
}

BOOL CWndEffect::AppendAnimation(AnimationParam& aParam)
{
    if(aParam.animationEffect > ANIMATIONNUM + 1 || aParam.animationEffect <= 1)
    {
        return FALSE;
    }

    InternalAnimationParam internalParam;
    memcpy(&internalParam.param, &aParam, sizeof(aParam));
    DWORD dataSize = internalParam.param.bmpSize.cx * internalParam.param.bmpSize.cy * 4;
    internalParam.bmpDataCopy = new BYTE[dataSize];

    if(internalParam.bmpDataCopy == NULL)
    {
        return FALSE;
    }

    memcpy(internalParam.bmpDataCopy, aParam.pBmpData, dataSize);
    internalParam.frameNow = 0;
    internalParam.bLastFrame = FALSE;

    IEffect* pEffect = CEffectFactory::CreateEffect(aParam.animationEffect);
    internalParam.pEffect = pEffect;
    pEffect->InitEffectParam(&internalParam);

    m_animationContainer.push_back(internalParam);

    return TRUE;
}

BOOL CWndEffect::DependAnimation(WPARAM effectKey)
{
    BOOL bFind = FALSE;
    for(m_itAnimation = m_animationContainer.begin(); m_itAnimation != m_animationContainer.end(); m_itAnimation++)
    {
        if(m_itAnimation->param.effectKey == effectKey)
        {
            delete []m_itAnimation->bmpDataCopy;
            CEffect* pEffect = dynamic_cast<CEffect*>(m_itAnimation->pEffect);
            pEffect->ReleaseEffectParam();
            delete pEffect;
            m_animationContainer.erase(m_itAnimation);

            bFind = TRUE;
        }
    }

    return  bFind;
}

BOOL CWndEffect::ClearAllAnimation()
{
    for_each(m_animationContainer.begin(), m_animationContainer.end(), ClearContainerData);
    m_animationContainer.clear();

    return TRUE;
}

BOOL CWndEffect::Animation(IUIEffectCallBack* iDrawEffect, DWORD frameSpin)
{
    if(iDrawEffect == NULL)
    {
        return FALSE;
    }

    DWORD time_start = clock();;
    DWORD delta_time = 0;

    // 队列非空，执行动画
    while(m_animationContainer.size() != 0)
    {
        ComputeAnimation(iDrawEffect, delta_time);

        iDrawEffect->OnUiEffectDraw();

        CleanFinishedAnimation(iDrawEffect);

        Sleep(1);

        delta_time = clock() - time_start;
    }

    return TRUE;
}

DWORD CWndEffect::InitSurportAnimationType(const char*& strAnimationType)
{
    strAnimationType = m_animation;
    return ANIMATIONNUM;
}

void CWndEffect::ComputeAnimation(IUIEffectCallBack* iDrawEffect, DWORD timeElapse)
{
    // 此循环过程中不能插入
    for(m_itAnimation = m_animationContainer.begin(); m_itAnimation != m_animationContainer.end(); m_itAnimation++)
    {
        IEffect* pEffect = (IEffect*)m_itAnimation->pEffect;
        if((!m_itAnimation->frameNow))
        {
            // 第一帧
            m_itAnimation->frameNow++;
            pEffect->ComputeOneFrame(&*m_itAnimation);
            // 通知绘制模块
            iDrawEffect->OnUiEffectBegin(m_itAnimation->param.effectKey, m_itAnimation->param.animationEffect);
        }
        else if(timeElapse / m_itAnimation->param.animationFrequency >= m_itAnimation->frameNow)
        {
            // 符合下一帧条件，重新计算
            m_itAnimation->frameNow++;
            pEffect->ComputeOneFrame(&*m_itAnimation);
        }

    }
}

void CWndEffect::CleanFinishedAnimation(IUIEffectCallBack* iDrawEffect)
{
    // 绘制完成后删除完成的动画
    int sizeVec = m_animationContainer.size();
    for(int i = 0; i < sizeVec; i++)
    {
        if(m_animationContainer[i].bLastFrame)
        {
            iDrawEffect->OnUiEffectEnd(m_animationContainer[i].param.effectKey, m_animationContainer[i].param.animationEffect);
            delete []m_animationContainer[i].bmpDataCopy;
            CEffect* pEffect = dynamic_cast<CEffect*>(m_animationContainer[i].pEffect);
            pEffect->ReleaseEffectParam();
            delete pEffect;
            m_animationContainer.erase(m_animationContainer.begin() + i);
            sizeVec = m_animationContainer.size();
        }

    }
}