#include "StdAfx.h"
#include "WndEffect.h"



// ����֧�ֵĶ���
const char* CWndEffect::m_animation =  "����ת\0���ҷ�ת\0���·�ת\0���Ϸ�ת\0������\0���󻬶�\0���һ���\0���ϻ���\0���»���\0ˮƽ��״����\0��ֱ��״����\0ˮƽ��Ҷ\0��ֱ��Ҷ\0���ȸ�ˮƽ��Ҷ\0���ȿ�ֱ��Ҷ\0�������\0���Ҳ���\0���ϲ���\0���²���\0������Ի���\0���ҹ��Ի���\0���Ϲ��Ի���\0���¹��Ի���\0����/����\0�Ŵ�\0��С\0�ڳ�/����\0©��\0����\0��������\0��������\0��������\0��������\0���󽥱����\0���ҽ������\0���Ͻ������\0���½������\0�����Ƽ�\0�����Ƽ�\0�����Ƽ�\0�����Ƽ�\0�ܽ�\0Ļ\0��������\0��������\0��������\0��������\0����������\0����������\0����������\0����������\0�ռ�����\0�ռ�����\0�ռ�����\0�ռ�����\0�ֿ鷭ת\0�ռ�������ת\0�ռ�������ת\0�ռ�������ת\0�ռ�������ת\0��/����\0����\0��ɢ\0��ɫ\0���½����Ử��\0���½����Ử��\0���Ͻ����Ử��\0���Ͻ����Ử��\0�������Ử��\0�������Ử��\0�������Ử��\0�������Ử��\0���β���\0�糵\0��ҳ\0ģ������/��\0ˮ��\0��β\0��ֱ�۵�\0ˮƽ�۵�\0\0";


void ClearContainerData(InternalAnimationParam& pElem)
{
    // ɾ���������ڴ�
    delete []pElem.bmpDataCopy;
    pElem.bmpDataCopy = NULL;
    // ɾ����Ч��
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
    // ������Դ
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

    // ���зǿգ�ִ�ж���
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
    // ��ѭ�������в��ܲ���
    for(m_itAnimation = m_animationContainer.begin(); m_itAnimation != m_animationContainer.end(); m_itAnimation++)
    {
        IEffect* pEffect = (IEffect*)m_itAnimation->pEffect;
        if((!m_itAnimation->frameNow))
        {
            // ��һ֡
            m_itAnimation->frameNow++;
            pEffect->ComputeOneFrame(&*m_itAnimation);
            // ֪ͨ����ģ��
            iDrawEffect->OnUiEffectBegin(m_itAnimation->param.effectKey, m_itAnimation->param.animationEffect);
        }
        else if(timeElapse / m_itAnimation->param.animationFrequency >= m_itAnimation->frameNow)
        {
            // ������һ֡���������¼���
            m_itAnimation->frameNow++;
            pEffect->ComputeOneFrame(&*m_itAnimation);
        }

    }
}

void CWndEffect::CleanFinishedAnimation(IUIEffectCallBack* iDrawEffect)
{
    // ������ɺ�ɾ����ɵĶ���
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