#include "StdAfx.h"
#include "DuiAnimationTabLayout.h"

namespace DuiLib
{


    IMPLEMENT_DUICONTROL(CDuiAnimationTabLayout)

    CDuiAnimationTabLayout::CDuiAnimationTabLayout(void)
#pragma warning(disable: 4355)
        : IAnimation(this)
#pragma warning(default: 4355)
        , m_bIsVerticalDirection(FALSE)
        , m_bControlVisibleFlag(FALSE)
        , m_nPositiveDirection(1)
        , m_pCurrentControl(NULL)
    {
    }


    CDuiAnimationTabLayout::~CDuiAnimationTabLayout(void)
    {
    }


    CDuiString CDuiAnimationTabLayout::GetClass() const
    {
        return DUI_CTR_ANIMATION_TABLAYOUT;
    }

    LPVOID CDuiAnimationTabLayout::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_ANIMATION_TABLAYOUT) == 0)
        {
            return static_cast<CDuiAnimationTabLayout*>(this);
        }
        return CDuiTabLayout::GetInterface(pstrName);
    }

    void CDuiAnimationTabLayout::AnimationSwitch()
    {
        m_rcItemOld = GetPos();
        if(!m_bIsVerticalDirection)
        {
            m_rcCurPos.top = GetPos().top;
            m_rcCurPos.bottom = GetPos().bottom;
            m_rcCurPos.left = GetPos().left - (GetPos().right - GetPos().left) * m_nPositiveDirection + 52 * m_nPositiveDirection;
            m_rcCurPos.right = GetPos().right - (GetPos().right - GetPos().left) * m_nPositiveDirection + 52 * m_nPositiveDirection;
        }
        else
        {
            m_rcCurPos.left = GetPos().left;
            m_rcCurPos.right = GetPos().right;
            m_rcCurPos.top = GetPos().top - (GetPos().bottom - GetPos().top) * m_nPositiveDirection;
            m_rcCurPos.bottom = GetPos().bottom - (GetPos().bottom - GetPos().top) * m_nPositiveDirection;
        }

        StopAnimation(TIMER_ID_ANIMATION_TABLAYOUT);
        StartAnimation(TAB_ANIMATION_ELLAPSE, TAB_ANIMATION_FRAME_COUNT, TIMER_ID_ANIMATION_TABLAYOUT);
    }

    BOOL CDuiAnimationTabLayout::SelectItem(int iIndex)
    {
        if(iIndex < 0 || iIndex >= GetCount())
        {
            return FALSE;
        }
        if(iIndex == GetCurSel())
        {
            return TRUE;
        }
        if(iIndex > GetCurSel())
        {
            m_nPositiveDirection = -1;
        }
        if(iIndex < GetCurSel())
        {
            m_nPositiveDirection = 1;
        }

        int iOldSel = GetCurSel();
        SetCurSel(iIndex);
        for(int it = 0; it < GetCount(); it++)
        {
            if(it == iIndex)
            {
                GetItemAt(it)->SetVisible(TRUE);
                GetItemAt(it)->SetFocus();
                m_bControlVisibleFlag = FALSE;
                m_pCurrentControl = GetItemAt(it);
            }
            else
            {
                GetItemAt(it)->SetVisible(FALSE);
            }
        }
        NeedParentUpdate();
        if(NULL != m_pCurrentControl)
        {
            m_pCurrentControl->SetVisible(FALSE);
        }

        AnimationSwitch();

        if(GetManager() != NULL)
        {
            GetManager()->SetNextTabControl();
            GetManager()->SendNotify(this, DUI_MSGTYPE_TABSELECT, GetCurSel(), iOldSel);
        }
        return TRUE;
    }

    void CDuiAnimationTabLayout::OnAnimationStart(int nAnimationID, BOOL bFirstLoop)
    {

    }

    void CDuiAnimationTabLayout::OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID)
    {
        if(!m_bControlVisibleFlag)
        {
            m_bControlVisibleFlag = TRUE;
            m_pCurrentControl->SetVisible(TRUE);
        }

        int iStepLen = 0;
        if(!m_bIsVerticalDirection)
        {
            iStepLen = (m_rcItemOld.right - m_rcItemOld.left) * m_nPositiveDirection / nTotalFrame;
            if(nCurFrame != nTotalFrame)
            {
                m_rcCurPos.left = m_rcCurPos.left + iStepLen;
                m_rcCurPos.right = m_rcCurPos.right + iStepLen;
            }
            else
            {
                m_rcCurPos = m_rcItemOld;
            }
        }
        else
        {
            iStepLen = (m_rcItemOld.bottom - m_rcItemOld.top) * m_nPositiveDirection / nTotalFrame;
            if(nCurFrame != nTotalFrame)
            {
                m_rcCurPos.top = m_rcCurPos.top + iStepLen;
                m_rcCurPos.bottom = m_rcCurPos.bottom + iStepLen;
            }
            else
            {
                m_rcCurPos = m_rcItemOld;
            }
        }
        SetPos(m_rcCurPos);
    }

    void CDuiAnimationTabLayout::OnAnimationStop(int nAnimationID)
    {
        SetPos(m_rcItemOld);
        NeedParentUpdate();
    }

    void CDuiAnimationTabLayout::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("animationdirection")) == 0 && _tcsicmp(pstrValue, _T("vertical")) == 0)
        {
            m_bIsVerticalDirection = TRUE;    // pstrValue = "vertical" or "horizontal"
        }
        return CDuiTabLayout::SetAttribute(pstrName, pstrValue);
    }

    void CDuiAnimationTabLayout::DoEvent(CDuiEvent& event)
    {
        if(event.Type == DUIEVENT_TIMER)
        {
            int nTimerID = (int)event.wParam;
            OnAnimationElapse(nTimerID);
            return ;
        }
        CDuiTabLayout::DoEvent(event);
    }

}