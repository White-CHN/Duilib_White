#include "StdAfx.h"
#include "DuiSlider.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiSlider)

    CDuiSlider::CDuiSlider(void)
        : m_bSendMove(FALSE)
        , m_nStep(1)
        , m_uButtonState(0)
    {
        m_szThumb = CDuiSize(10, 10);
    }


    CDuiSlider::~CDuiSlider(void)
    {
    }

    LPCTSTR CDuiSlider::GetClass() const
    {
        return DUI_CTR_SLIDER;
    }

    LPVOID CDuiSlider::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_SLIDER) == 0)
        {
            return static_cast<CDuiSlider*>(this);
        }
        return CDuiProgress::GetInterface(pstrName);
    }

    LPCTSTR CDuiSlider::GetThumbImage() const
    {
        return m_sThumbImage;
    }

    void CDuiSlider::SetThumbImage(LPCTSTR pStrImage)
    {
        m_sThumbImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiSlider::GetThumbHotImage() const
    {
        return m_sThumbHotImage;
    }

    void CDuiSlider::SetThumbHotImage(LPCTSTR pStrImage)
    {
        m_sThumbHotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiSlider::GetThumbPushedImage() const
    {
        return m_sThumbPushedImage;
    }

    void CDuiSlider::SetThumbPushedImage(LPCTSTR pStrImage)
    {
        m_sThumbPushedImage = pStrImage;
        Invalidate();
    }

    void CDuiSlider::SetThumbSize(SIZE szXY)
    {
        m_szThumb = szXY;
    }

    int CDuiSlider::GetChangeStep()
    {
        return m_nStep;
    }

    void CDuiSlider::SetChangeStep(int step)
    {
        m_nStep = step;
    }

    void CDuiSlider::SetCanSendMove(BOOL bCanSend)
    {
        m_bSendMove = bCanSend;
    }

    void CDuiSlider::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("thumbimage")) == 0)
        {
            SetThumbImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("thumbhotimage")) == 0)
        {
            SetThumbHotImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("thumbpushedimage")) == 0)
        {
            SetThumbPushedImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("thumbsize")) == 0)
        {
            SIZE szXY = {0};
            LPTSTR pstr = NULL;
            szXY.cx = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            szXY.cy = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetThumbSize(szXY);
        }
        else if(_tcsicmp(pstrName, _T("step")) == 0)
        {
            SetChangeStep(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("sendmove")) == 0)
        {
            SetCanSendMove(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else
        {
            CDuiProgress::SetAttribute(pstrName, pstrValue);
        }
    }

    RECT CDuiSlider::GetThumbRect() const
    {
        CDuiRect rcThumb;
        CDuiSize szThumb = m_szThumb;
        if(GetManager() != NULL)
        {
            GetManager()->GetDPIObj()->Scale(&szThumb);
        }
        if(IsHorizontal())
        {
            int left = GetPos().left + (GetPos().right - GetPos().left - szThumb.cx) * (GetValue() - GetMinValue()) / (GetMaxValue() - GetMinValue());
            int top = (GetPos().bottom + GetPos().top - szThumb.cy) / 2;
            rcThumb = CDuiRect(left, top, left + szThumb.cx, top + szThumb.cy);
        }
        else
        {
            int left = (GetPos().right + GetPos().left - szThumb.cx) / 2;
            int top = GetPos().bottom - szThumb.cy - (GetPos().bottom - GetPos().top - szThumb.cy) * (GetValue() - GetMinValue()) / (GetMaxValue() - GetMinValue());
            rcThumb = CDuiRect(left, top, left + szThumb.cx, top + szThumb.cy);
        }
        if(GetManager() != NULL)
        {
            GetManager()->GetDPIObj()->Scale(&rcThumb);
        }
        return rcThumb;
    }

    void CDuiSlider::PaintForeImage(HDC hDC)
    {
        CDuiProgress::PaintForeImage(hDC);

        RECT rcThumb = GetThumbRect();
        rcThumb.left -= GetPos().left;
        rcThumb.top -= GetPos().top;
        rcThumb.right -= GetPos().left;
        rcThumb.bottom -= GetPos().top;

        if(GetManager())
        {
            GetManager()->GetDPIObj()->ScaleBack(&rcThumb);
        }

        if((m_uButtonState & UISTATE_CAPTURED) != 0)
        {
            if(!m_sThumbPushedImage.IsEmpty())
            {
                m_sImageModify.SetEmpty();
                m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
                if(!DrawImage(hDC, (LPCTSTR)m_sThumbPushedImage, (LPCTSTR)m_sImageModify)) {}
                else
                {
                    return;
                }
            }
        }
        else if((m_uButtonState & UISTATE_HOT) != 0)
        {
            if(!m_sThumbHotImage.IsEmpty())
            {
                m_sImageModify.SetEmpty();
                m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
                if(!DrawImage(hDC, (LPCTSTR)m_sThumbHotImage, (LPCTSTR)m_sImageModify)) {}
                else
                {
                    return;
                }
            }
        }

        if(!m_sThumbImage.IsEmpty())
        {
            m_sImageModify.SetEmpty();
            m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
            if(!DrawImage(hDC, (LPCTSTR)m_sThumbImage, (LPCTSTR)m_sImageModify)) {}
            else
            {
                return;
            }
        }
    }

    void CDuiSlider::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(GetParent() != NULL)
            {
                GetParent()->DoEvent(event);
            }
            else
            {
                CDuiProgress::DoEvent(event);
            }
            return;
        }
        if(event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
        {
            if(IsEnabled())
            {
                m_uButtonState |= UISTATE_CAPTURED;

                int nValue;
                if(IsHorizontal())
                {
                    if(event.ptMouse.x >= GetPos().right - m_szThumb.cx / 2)
                    {
                        nValue = GetMaxValue();
                    }
                    else if(event.ptMouse.x <= GetPos().left + m_szThumb.cx / 2)
                    {
                        nValue = GetMinValue();
                    }
                    else
                    {
                        nValue = GetMinValue() + (GetMaxValue() - GetMinValue()) * (event.ptMouse.x - GetPos().left - m_szThumb.cx / 2) / (GetPos().right - GetPos().left - m_szThumb.cx);
                    }
                }
                else
                {
                    if(event.ptMouse.y >= GetPos().bottom - m_szThumb.cy / 2)
                    {
                        nValue = GetMinValue();
                    }
                    else if(event.ptMouse.y <= GetPos().top + m_szThumb.cy / 2)
                    {
                        nValue = GetMaxValue();
                    }
                    else
                    {
                        nValue = GetMinValue() + (GetMaxValue() - GetMinValue()) * (GetPos().bottom - event.ptMouse.y - m_szThumb.cy / 2) / (GetPos().bottom - GetPos().top - m_szThumb.cy);
                    }
                }
                SetValue(nValue);
            }
            return;
        }
        if(event.Type == UIEVENT_BUTTONUP || event.Type == UIEVENT_RBUTTONUP)
        {
            if(IsEnabled())
            {
                int nValue = 0;
                if((m_uButtonState & UISTATE_CAPTURED) != 0)
                {
                    m_uButtonState &= ~UISTATE_CAPTURED;
                }
                if(IsHorizontal())
                {
                    if(event.ptMouse.x >= GetPos().right - m_szThumb.cx / 2)
                    {
                        nValue = GetMaxValue();
                    }
                    else if(event.ptMouse.x <= GetPos().left + m_szThumb.cx / 2)
                    {
                        nValue = GetMinValue();
                    }
                    else
                    {
                        nValue = GetMinValue() + (GetMaxValue() - GetMinValue()) * (event.ptMouse.x - GetPos().left - m_szThumb.cx / 2) / (GetPos().right - GetPos().left - m_szThumb.cx);
                    }
                }
                else
                {
                    if(event.ptMouse.y >= GetPos().bottom - m_szThumb.cy / 2)
                    {
                        nValue = GetMinValue();
                    }
                    else if(event.ptMouse.y <= GetPos().top + m_szThumb.cy / 2)
                    {
                        nValue = GetMaxValue();
                    }
                    else
                    {
                        nValue = GetMinValue() + (GetMaxValue() - GetMinValue()) * (GetPos().bottom - event.ptMouse.y - m_szThumb.cy / 2) / (GetPos().bottom - GetPos().top - m_szThumb.cy);
                    }
                }
                SetValue(nValue);
                GetManager()->SendNotify(this, DUI_MSGTYPE_VALUECHANGED);
                return;
            }
        }
        if(event.Type == UIEVENT_CONTEXTMENU)
        {
            return;
        }
        if(event.Type == UIEVENT_SCROLLWHEEL)
        {
            if(IsEnabled())
            {
                switch(LOWORD(event.wParam))
                {
                    case SB_LINEUP:
                        SetValue(GetValue() + GetChangeStep());
                        GetManager()->SendNotify(this, DUI_MSGTYPE_VALUECHANGED);
                        return;
                    case SB_LINEDOWN:
                        SetValue(GetValue() - GetChangeStep());
                        GetManager()->SendNotify(this, DUI_MSGTYPE_VALUECHANGED);
                        return;
                }
            }
        }
        if(event.Type == UIEVENT_MOUSEMOVE)
        {
            if((m_uButtonState & UISTATE_CAPTURED) != 0)
            {
                if(IsHorizontal())
                {
                    if(event.ptMouse.x >= GetPos().right - m_szThumb.cx / 2)
                    {
                        SetValue(GetMaxValue());
                    }
                    else if(event.ptMouse.x <= GetPos().left + m_szThumb.cx / 2)
                    {
                        SetValue(GetMinValue());
                    }
                    else
                    {
                        SetValue(GetMinValue() + (GetMaxValue() - GetMinValue()) * (event.ptMouse.x - GetPos().left - m_szThumb.cx / 2) / (GetPos().right - GetPos().left - m_szThumb.cx));
                    }
                }
                else
                {
                    if(event.ptMouse.y >= GetPos().bottom - m_szThumb.cy / 2)
                    {
                        SetValue(GetMinValue());
                    }
                    else if(event.ptMouse.y <= GetPos().top + m_szThumb.cy / 2)
                    {
                        SetValue(GetMaxValue());
                    }
                    else
                    {
                        SetValue(GetMinValue() + (GetMaxValue() - GetMinValue()) * (GetPos().bottom - event.ptMouse.y - m_szThumb.cy / 2) / (GetPos().bottom - GetPos().top - m_szThumb.cy));
                    }
                }
                if(m_bSendMove)
                {
                    UpdateText();
                    GetManager()->SendNotify(this, DUI_MSGTYPE_VALUECHANGED_MOVE);
                }
                Invalidate();
            }

            POINT pt = event.ptMouse;
            RECT rcThumb = GetThumbRect();
            if(IsEnabled() && ::PtInRect(&rcThumb, event.ptMouse))
            {
                m_uButtonState |= UISTATE_HOT;
                Invalidate();
            }
            else
            {
                m_uButtonState &= ~UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_SETCURSOR)
        {
            RECT rcThumb = GetThumbRect();
            if(IsEnabled())
            {
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
                return;
            }
        }
        if(event.Type == UIEVENT_MOUSELEAVE)
        {
            if(IsEnabled())
            {
                m_uButtonState &= ~UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        CDuiProgress::DoEvent(event);
    }

}