#include "StdAfx.h"
#include "DuiAnimation.h"


namespace DuiLib
{

    CAnimationData::CAnimationData(int nElipse, int nFrame, int nID, BOOL bLoop)
        : m_nAnimationID(nID)
        , m_nElapse(nElipse)
        , m_nTotalFrame(nFrame)
        , m_nCurFrame(0)
        , m_bLoop(bLoop)
        , m_bFirstLoop(TRUE)
    {
    }


    /////////////////////////////////////////////////////////////////////////////////

    IAnimation::IAnimation(CDuiControl* pOwner)
        : m_pControl(pOwner)
    {
    }

    IAnimation::~IAnimation(void)
    {
        CAnimationData* pData = NULL;
        for(vector<CAnimationData*>::iterator it = m_arAnimations.begin(); it != m_arAnimations.end();)
        {
            pData = *it;
            if(pData != NULL)
            {
                it = m_arAnimations.erase(it);
                m_pControl->KillTimer(pData->m_nAnimationID);
                DUI_FREE_POINT(pData);
            }
            else
            {
                ++it;
            }
        }
    }

    BOOL IAnimation::StartAnimation(int nElapse, int nTotalFrame, int nAnimationID /*= 0*/, BOOL bLoop /*= FALSE*/)
    {
        CAnimationData* pData = GetAnimationDataByID(nAnimationID);
        if(NULL != pData
                || nElapse <= 0
                || nTotalFrame <= 0
                || NULL == m_pControl)
        {
            ASSERT(FALSE);
            return FALSE;
        }
        CAnimationData* pAnimation = new CAnimationData(nElapse, nTotalFrame, nAnimationID, bLoop);
        if(m_pControl->SetTimer(nAnimationID, nElapse))
        {
            m_arAnimations.push_back(pAnimation);
            return TRUE;
        }
        return FALSE;
    }

    void IAnimation::StopAnimation(int nAnimationID /*= 0*/)
    {
        if(m_pControl == NULL || nAnimationID < 0)
        {
            return;
        }
        CAnimationData* pData = GetAnimationDataByID(nAnimationID);
        if(pData != NULL)
        {
            m_pControl->KillTimer(nAnimationID);
            m_arAnimations.erase(std::remove(m_arAnimations.begin(), m_arAnimations.end(), pData), m_arAnimations.end());
            DUI_FREE_POINT(pData);
        }
    }

    BOOL IAnimation::IsAnimationRunning(int nAnimationID)
    {
        CAnimationData* pData = GetAnimationDataByID(nAnimationID);
        return NULL != pData;
    }

    int IAnimation::GetCurrentFrame(int nAnimationID /*= 0*/)
    {
        CAnimationData* pData = GetAnimationDataByID(nAnimationID);
        if(NULL == pData)
        {
            ASSERT(FALSE);
            return -1;
        }
        return pData->m_nCurFrame;
    }

    BOOL IAnimation::SetCurrentFrame(int nFrame, int nAnimationID /*= 0*/)
    {
        CAnimationData* pData = GetAnimationDataByID(nAnimationID);
        if(NULL != pData)
        {
            if(nFrame >= 0 && nFrame <= pData->m_nTotalFrame)
            {
                pData->m_nCurFrame = nFrame;
                return TRUE;
            }
        }
        return FALSE;
    }

    void IAnimation::OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID)
    {

    }

    void IAnimation::OnAnimationStart(int nAnimationID, BOOL bFirstLoop)
    {

    }

    void IAnimation::OnAnimationStop(int nAnimationID)
    {

    }

    void IAnimation::OnAnimationElapse(int nAnimationID)
    {
        if(m_pControl == NULL)
        {
            return;
        }

        CAnimationData* pData = GetAnimationDataByID(nAnimationID);
        if(NULL == pData)
        {
            return;
        }
        int nCurFrame = pData->m_nCurFrame;
        if(nCurFrame == 0)
        {
            OnAnimationStart(nAnimationID, pData->m_bFirstLoop);
            pData->m_bFirstLoop = FALSE;
        }
        OnAnimationStep(pData->m_nTotalFrame, nCurFrame, nAnimationID);
        if(nCurFrame >= pData->m_nTotalFrame)
        {
            OnAnimationStop(nAnimationID);
            if(pData->m_bLoop)
            {
                pData->m_nCurFrame = 0;
            }
            else
            {
                m_pControl->KillTimer(nAnimationID);
                m_arAnimations.erase(std::remove(m_arAnimations.begin(), m_arAnimations.end(), pData), m_arAnimations.end());
                DUI_FREE_POINT(pData);
            }
        }
        if(NULL != pData)
        {
            ++(pData->m_nCurFrame);
        }
    }

    CAnimationData* IAnimation::GetAnimationDataByID(int nAnimationID)
    {
        CAnimationData* pRet = NULL;
        for(size_t i = 0; i < m_arAnimations.size(); ++i)
        {
            if(m_arAnimations[i]->m_nAnimationID == nAnimationID)
            {
                pRet = m_arAnimations[i];
                break;
            }
        }

        return pRet;
    }

}