#include "StdAfx.h"
#include "DuiFadeButton.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiFadeButton)

    CDuiFadeButton::CDuiFadeButton(void)
#pragma warning(disable: 4355)
        : IAnimation(this)
#pragma warning(default: 4355)
        , m_bFadeAlpha(0)
        , m_bMouseHove(FALSE)
        , m_bMouseLeave(FALSE)
    {
    }


    CDuiFadeButton::~CDuiFadeButton(void)
    {
        StopAnimation();
    }


    CDuiString CDuiFadeButton::GetClass() const
    {
        return DUI_CTR_FADEBUTTON;
    }

    LPVOID CDuiFadeButton::GetInterface(LPCTSTR pstrName)
    {
        if(_tcscmp(pstrName, DUI_CTR_FADEBUTTON) == 0)
        {
            return static_cast<CDuiFadeButton*>(this);
        }
        return CDuiButton::GetInterface(pstrName);
    }

    void CDuiFadeButton::SetNormalImage(LPCTSTR pStrImage)
    {
        m_sLastImage = pStrImage;
        CDuiButton::SetNormalImage(pStrImage);
    }

    void CDuiFadeButton::OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID)
    {
        m_bFadeAlpha = (BYTE)((nCurFrame / (double)nTotalFrame) * 255);
        m_bFadeAlpha = m_bFadeAlpha == 0 ? 10 : m_bFadeAlpha;
        Invalidate();
    }

    void CDuiFadeButton::OnTimer(int nTimerID)
    {
        OnAnimationElapse(nTimerID);
    }

    void CDuiFadeButton::DoEvent(TEventUI& event)
    {
        if(event.Type == UIEVENT_MOUSEENTER && !IsAnimationRunning(TIMER_ID_FADEBUTTON_IN))
        {
            m_bFadeAlpha = 0;
            m_bMouseHove = TRUE;
            StopAnimation(TIMER_ID_FADEBUTTON_OUT);
            StartAnimation(FADE_ELLAPSE, FADE_FRAME_COUNT, TIMER_ID_FADEBUTTON_IN);
            Invalidate();
            return;
        }
        if(event.Type == UIEVENT_MOUSELEAVE && !IsAnimationRunning(TIMER_ID_FADEBUTTON_OUT))
        {
            m_bFadeAlpha = 0;
            m_bMouseLeave = TRUE;
            StopAnimation(TIMER_ID_FADEBUTTON_IN);
            StartAnimation(FADE_ELLAPSE, FADE_FRAME_COUNT, TIMER_ID_FADEBUTTON_OUT);
            Invalidate();
            return;
        }
        if(event.Type == UIEVENT_TIMER)
        {
            OnTimer(event.wParam);
        }
        CDuiButton::DoEvent(event);
    }

    void CDuiFadeButton::PaintStatusImage(HDC hDC)
    {
        UINT uButtonState = GetButtonState();
        if(IsFocused())
        {
            uButtonState |= UISTATE_FOCUSED;
        }
        else
        {
            uButtonState &= ~ UISTATE_FOCUSED;
        }
        if(!IsEnabled())
        {
            uButtonState |= UISTATE_DISABLED;
        }
        else
        {
            uButtonState &= ~ UISTATE_DISABLED;
        }
        SetButtonStste(uButtonState);
        if((uButtonState & UISTATE_DISABLED) != 0)
        {
            if(!GetDisabledImage().IsEmpty())
            {
                if(!DrawImage(hDC, GetDisabledImage().GetData())) {}
                else
                {
                    return;
                }
            }
        }
        else if((uButtonState & UISTATE_PUSHED) != 0)
        {
            if(!GetPushedImage().IsEmpty())
            {
                if(!DrawImage(hDC, GetPushedImage().GetData())) {}
                else
                {
                    return;
                }
            }
        }
        else if((uButtonState & UISTATE_FOCUSED) != 0)
        {
            if(!GetFocusedImage().IsEmpty())
            {
                if(!DrawImage(hDC, GetFocusedImage().GetData())) {}
                else
                {
                    return;
                }
            }
        }

        if(!GetNormalImage().IsEmpty())
        {
            if(IsAnimationRunning(TIMER_ID_FADEBUTTON_IN) || IsAnimationRunning(TIMER_ID_FADEBUTTON_OUT))
            {
                if(m_bMouseHove)
                {
                    m_bMouseHove = FALSE;
                    m_sLastImage = GetHotImage();
                    if(!DrawImage(hDC, GetNormalImage().GetData())) {}
                    return;
                }

                if(m_bMouseLeave)
                {
                    m_bMouseLeave = FALSE;
                    m_sLastImage = GetNormalImage();
                    if(!DrawImage(hDC, GetHotImage().GetData())) {}
                    return;
                }

                m_sOldImage = GetNormalImage();
                m_sNewImage = GetHotImage();
                if(IsAnimationRunning(TIMER_ID_FADEBUTTON_OUT))
                {
                    m_sOldImage = GetHotImage();
                    m_sNewImage = GetNormalImage();
                }
                CDuiString sFadeOut, sFadeIn;
                sFadeOut.Format(_T("fade='%d'"), 255 - m_bFadeAlpha);
                sFadeIn.Format(_T("fade='%d'"), m_bFadeAlpha);
                if(!DrawImage(hDC, (LPCTSTR)m_sOldImage, sFadeOut)) {}
                if(!DrawImage(hDC, (LPCTSTR)m_sNewImage, sFadeIn)) {}
                return;
            }
            else
            {
                if(m_sLastImage.IsEmpty())
                {
                    m_sLastImage = GetNormalImage();
                }
                if(!DrawImage(hDC, (LPCTSTR)m_sLastImage)) {}
                return;
            }
        }
    }

}