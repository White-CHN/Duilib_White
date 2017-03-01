#include "StdAfx.h"
#include "DuiButton.h"


namespace DuiLib
{

    IMPLEMENT_DUICONTROL(CDuiButton)

    CDuiButton::CDuiButton(void)
        : m_iBindTabIndex(-1)
        , m_uButtonState(0)
        , m_dwHotTextColor(0)
        , m_dwPushedTextColor(0)
        , m_dwFocusedTextColor(0)
        , m_dwHotBkColor(0)
        , m_dwPushedBkColor(0)
    {
    }


    CDuiButton::~CDuiButton(void)
    {
    }

    CDuiString CDuiButton::GetClass() const
    {
        return DUI_CTR_BUTTON;
    }

    LPVOID CDuiButton::GetInterface(CDuiString strName)
    {
        if(strName == DUI_CTR_BUTTON)
        {
            return static_cast<CDuiButton*>(this);
        }
        return CDuiLabel::GetInterface(strName);
    }

    UINT CDuiButton::GetControlFlags() const
    {
        return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
    }

    CDuiString CDuiButton::GetNormalImage() const
    {
        return m_sNormalImage;
    }

    void CDuiButton::SetNormalImage(LPCTSTR pStrImage)
    {
        if(m_sNormalImage == pStrImage)
        {
            return;
        }
        m_sNormalImage = pStrImage;
        Invalidate();
    }

    CDuiString CDuiButton::GetHotImage() const
    {
        return m_sHotImage;
    }

    void CDuiButton::SetHotImage(LPCTSTR pStrImage)
    {
        if(m_sHotImage == pStrImage)
        {
            return;
        }
        m_sHotImage = pStrImage;
        Invalidate();
    }

    CDuiString CDuiButton::GetPushedImage() const
    {
        return m_sPushedImage;
    }

    void CDuiButton::SetPushedImage(LPCTSTR pStrImage)
    {
        if(m_sPushedImage == pStrImage)
        {
            return;
        }
        m_sPushedImage = pStrImage;
        Invalidate();
    }

    CDuiString CDuiButton::GetFocusedImage() const
    {
        return m_sFocusedImage;
    }

    void CDuiButton::SetFocusedImage(LPCTSTR pStrImage)
    {
        if(m_sFocusedImage == pStrImage)
        {
            return;
        }
        m_sFocusedImage = pStrImage;
        Invalidate();
    }

    CDuiString CDuiButton::GetDisabledImage() const
    {
        return m_sDisabledImage;
    }

    void CDuiButton::SetDisabledImage(LPCTSTR pStrImage)
    {
        if(m_sDisabledImage == pStrImage)
        {
            return;
        }
        m_sDisabledImage = pStrImage;
        Invalidate();
    }

    CDuiString CDuiButton::GetHotForeImage() const
    {
        return m_sHotForeImage;
    }

    void CDuiButton::SetHotForeImage(LPCTSTR pStrImage)
    {
        if(m_sHotForeImage == pStrImage)
        {
            return;
        }
        m_sHotForeImage = pStrImage;
        Invalidate();
    }

    int CDuiButton::GetBindTabLayoutIndex()
    {
        return m_iBindTabIndex;
    }

    void CDuiButton::BindTabIndex(int iBindTabIndex)
    {
        if(iBindTabIndex >= 0)
        {
            m_iBindTabIndex	= iBindTabIndex;
        }
    }

    LPCTSTR CDuiButton::GetBindTabLayoutName()
    {
        return m_sBindTabLayoutName;
    }

    void CDuiButton::BindTabLayoutName(LPCTSTR pTabLayoutName)
    {
        if(pTabLayoutName)
        {
            m_sBindTabLayoutName = pTabLayoutName;
        }
    }

    void CDuiButton::BindTriggerTabSel(int iSetSelectIndex /*= -1*/)
    {

    }

    void CDuiButton::RemoveBindTabIndex()
    {

    }

    UINT CDuiButton::GetButtonState()
    {
        return m_uButtonState;
    }

    void CDuiButton::SetButtonStste(UINT uButtonState)
    {
        m_uButtonState = uButtonState;
    }

    DWORD CDuiButton::GetHotBkColor() const
    {
        return m_dwHotBkColor;
    }

    void CDuiButton::SetHotBkColor(DWORD dwColor)
    {
        m_dwHotBkColor = dwColor;
    }

    DWORD CDuiButton::GetPushedBkColor() const
    {
        return m_dwPushedBkColor;
    }

    void CDuiButton::SetPushedBkColor(DWORD dwColor)
    {
        m_dwPushedBkColor = dwColor;
    }

    DWORD CDuiButton::GetHotTextColor() const
    {
        return m_dwHotTextColor;
    }

    void CDuiButton::SetHotTextColor(DWORD dwColor)
    {
        m_dwHotTextColor = dwColor;
    }

    DWORD CDuiButton::GetPushedTextColor() const
    {
        return m_dwPushedTextColor;
    }

    void CDuiButton::SetPushedTextColor(DWORD dwColor)
    {
        m_dwPushedTextColor = dwColor;
    }

    DWORD CDuiButton::GetFocusedTextColor() const
    {
        return m_dwFocusedTextColor;
    }

    void CDuiButton::SetFocusedTextColor(DWORD dwColor)
    {
        m_dwFocusedTextColor = dwColor;
    }

    void CDuiButton::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("normalimage")) == 0)
        {
            SetNormalImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("hotimage")) == 0)
        {
            SetHotImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("pushedimage")) == 0)
        {
            SetPushedImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("focusedimage")) == 0)
        {
            SetFocusedImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("disabledimage")) == 0)
        {
            SetDisabledImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("hotforeimage")) == 0)
        {
            SetHotForeImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("bindtabindex")) == 0)
        {
            BindTabIndex(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("bindtablayoutname")) == 0)
        {
            BindTabLayoutName(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("hotbkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetHotBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("pushedbkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetPushedBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("hottextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetHotTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("pushedtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetPushedTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("focusedtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetFocusedTextColor(clrColor);
        }
        else
        {
            CDuiLabel::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiButton::PaintText(HDC hDC)
    {
        if(IsFocused())
        {
            m_uButtonState |= UISTATE_FOCUSED;
        }
        else
        {
            m_uButtonState &= ~ UISTATE_FOCUSED;
        }
        if(!IsEnabled())
        {
            m_uButtonState |= UISTATE_DISABLED;
        }
        else
        {
            m_uButtonState &= ~ UISTATE_DISABLED;
        }

        if(GetTextColor() == 0)
        {
            SetTextColor(GetManager()->GetDefaultFontColor());
        }
        if(GetDisabledTextColor() == 0)
        {
            SetDisabledTextColor(GetManager()->GetDefaultDisabledColor());
        }

        CDuiString sText = GetText();
        if(sText.IsEmpty())
        {
            return;
        }

        RECT rcTextPadding = GetTextPadding();
        if(GetManager())
        {
            GetManager()->GetDPIObj()->Scale(&rcTextPadding);
        }
        int nLinks = 0;
        RECT rc = GetPos();
        rc.left += rcTextPadding.left;
        rc.right -= rcTextPadding.right;
        rc.top += rcTextPadding.top;
        rc.bottom -= rcTextPadding.bottom;

        DWORD clrColor = IsEnabled() ? GetTextColor() : GetDisabledTextColor();

        if(((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0))
        {
            clrColor = GetPushedTextColor();
        }
        else if(((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0))
        {
            clrColor = GetHotTextColor();
        }
        else if(((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0))
        {
            clrColor = GetFocusedTextColor();
        }

        if(IsShowHtml())
        {
            CRenderEngine::DrawHtmlText(hDC, GetManager(), rc, sText, clrColor,
                                        NULL, NULL, nLinks, GetTextStyle());
        }
        else
        {
            CRenderEngine::DrawText(hDC, GetManager(), rc, sText, clrColor,
                                    GetFont(), GetTextStyle());
        }
    }

    void CDuiButton::PaintBkColor(HDC hDC)
    {
        if((m_uButtonState & UISTATE_PUSHED) != 0)
        {
            if(m_dwPushedBkColor != 0)
            {
                CRenderEngine::DrawColor(hDC, GetPaintRect(), GetAdjustColor(m_dwPushedBkColor));
                return;
            }
        }
        else if((m_uButtonState & UISTATE_HOT) != 0)
        {
            if(m_dwHotBkColor != 0)
            {
                CRenderEngine::DrawColor(hDC, GetPaintRect(), GetAdjustColor(m_dwHotBkColor));
                return;
            }
        }
        CDuiLabel::PaintBkColor(hDC);
    }

    void CDuiButton::PaintStatusImage(HDC hDC)
    {
        if(IsFocused())
        {
            m_uButtonState |= UISTATE_FOCUSED;
        }
        else
        {
            m_uButtonState &= ~ UISTATE_FOCUSED;
        }
        if(!IsEnabled())
        {
            m_uButtonState |= UISTATE_DISABLED;
        }
        else
        {
            m_uButtonState &= ~ UISTATE_DISABLED;
        }
        if(!::IsWindowEnabled(GetManager()->GetPaintWindow()))
        {
            m_uButtonState &= UISTATE_DISABLED;
        }
        if((m_uButtonState & UISTATE_DISABLED) != 0)
        {
            if(!m_sDisabledImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sDisabledImage))
                {

                }
                else
                {
                    return;
                }
            }
        }
        else if((m_uButtonState & UISTATE_PUSHED) != 0)
        {
            if(!m_sPushedImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sPushedImage))
                {

                }
                else
                {
                    return;
                }
            }
        }
        else if((m_uButtonState & UISTATE_HOT) != 0)
        {
            if(!m_sHotImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sHotImage))
                {
                }
                else
                {
                    return;
                }
            }
        }
        else if((m_uButtonState & UISTATE_FOCUSED) != 0)
        {
            if(!m_sFocusedImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sFocusedImage))
                {
                }
                else
                {
                    return;
                }
            }
        }

        if(!m_sNormalImage.IsEmpty())
        {
            if(!DrawImage(hDC, (LPCTSTR)m_sNormalImage))
            {
            }
        }
    }

    void CDuiButton::PaintForeImage(HDC hDC)
    {
        if((m_uButtonState & UISTATE_PUSHED) != 0)
        {
            if(!m_sPushedForeImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sPushedForeImage))
                {

                }
                else
                {
                    return;
                }
            }
        }
        else if((m_uButtonState & UISTATE_HOT) != 0)
        {
            if(!m_sHotForeImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sHotForeImage))
                {
                }
                else
                {
                    return;
                }
            }
        }
        if(!GetForeImage().IsEmpty())
        {
            if(!DrawImage(hDC, (LPCTSTR)GetForeImage()))
            {
            }
        }
    }

    BOOL CDuiButton::Activate()
    {
        if(!CDuiLabel::Activate())
        {
            return FALSE;
        }
        if(GetManager() != NULL)
        {
            GetManager()->SendNotify(this, DUI_MSGTYPE_CLICK);
            BindTriggerTabSel();
        }
        return TRUE;
    }

    void CDuiButton::SetEnabled(BOOL bEnable /*= TRUE*/)
    {
        CDuiControl::SetEnabled(bEnable);
        if(!IsEnabled())
        {
            m_uButtonState = 0;
        }
    }

    void CDuiButton::SetVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiControl::SetVisible(bVisible);
        if(!IsVisible())
        {
            m_uButtonState = 0;
        }
    }

    void CDuiButton::DoEvent(CDuiEvent& event)
    {
        if(!IsMouseEnabled() && event.Type > DUIEVENT__MOUSEBEGIN && event.Type < DUIEVENT__MOUSEEND)
        {
            if(GetParent() != NULL)
            {
                GetParent()->DoEvent(event);
            }
            else
            {
                CDuiLabel::DoEvent(event);
            }
            return;
        }

        if(event.Type == DUIEVENT_SETFOCUS)
        {
            Invalidate();
        }
        if(event.Type == DUIEVENT_KILLFOCUS)
        {
            Invalidate();
        }
        if(event.Type == DUIEVENT_KEYDOWN)
        {
            if(IsKeyboardEnabled())
            {
                if(event.chKey == VK_SPACE || event.chKey == VK_RETURN)
                {
                    Activate();
                    return;
                }
            }
        }
        if(event.Type == DUIEVENT_BUTTONDOWN || event.Type == DUIEVENT_DBLCLICK)
        {
            if(::PtInRect(&GetPos(), event.ptMouse) && IsEnabled())
            {
                m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
                Invalidate();
            }
            return;
        }
        if(event.Type == DUIEVENT_MOUSEMOVE)
        {
            if((m_uButtonState & UISTATE_CAPTURED) != 0)
            {
                if(::PtInRect(&GetPos(), event.ptMouse))
                {
                    m_uButtonState |= UISTATE_PUSHED;
                }
                else
                {
                    m_uButtonState &= ~UISTATE_PUSHED;
                }
                Invalidate();
            }
            return;
        }
        if(event.Type == DUIEVENT_BUTTONUP)
        {
            if((m_uButtonState & UISTATE_CAPTURED) != 0)
            {
                m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
                Invalidate();
                if(::PtInRect(&GetPos(), event.ptMouse))
                {
                    Activate();
                }
            }
            return;
        }
        if(event.Type == DUIEVENT_CONTEXTMENU)
        {
            if(IsContextMenuUsed())
            {
                GetManager()->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
            }
            return;
        }
        if(event.Type == DUIEVENT_MOUSEENTER)
        {
            if(IsEnabled())
            {
                m_uButtonState |= UISTATE_HOT;
                Invalidate();
            }
        }
        if(event.Type == DUIEVENT_MOUSELEAVE)
        {
            if(IsEnabled())
            {
                m_uButtonState &= ~UISTATE_HOT;
                Invalidate();
            }
        }
        if(event.Type == DUIEVENT_SYSKEYDOWN)
        {
            m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
            Invalidate();
            return;
        }
        if(event.Type == DUIEVENT_SYSKEYUP)
        {
            m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
            Invalidate();
            return;
        }
        CDuiLabel::DoEvent(event);
    }



}