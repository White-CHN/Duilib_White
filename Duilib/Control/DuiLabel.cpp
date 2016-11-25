#include "StdAfx.h"
#include "DuiLabel.h"


namespace DuiLib
{

    IMPLEMENT_DUICONTROL(CDuiLabel)

    CDuiLabel::CDuiLabel(void)
        : m_bShowHtml(FALSE)
        , m_bAutoCalcWidth(FALSE)
        , m_iFont(-1)
        , m_dwTextColor(0)
        , m_dwDisabledTextColor(0)
        , m_uTextStyle(DT_VCENTER | DT_SINGLELINE)
    {
        ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
    }


    CDuiLabel::~CDuiLabel(void)
    {
    }

    LPCTSTR CDuiLabel::GetClass() const
    {
        return DUI_CTR_LABEL;
    }

    LPVOID CDuiLabel::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_LABEL) == 0)
        {
            return static_cast<CDuiLabel*>(this);
        }
        return __super::GetInterface(pstrName);
    }

    UINT CDuiLabel::GetControlFlags() const
    {
        return IsEnabled() ? UIFLAG_SETCURSOR : 0;
    }

    int CDuiLabel::GetFont() const
    {
        return m_iFont;
    }

    void CDuiLabel::SetFont(int index)
    {
        if(m_iFont == index)
        {
            return;
        }
        m_iFont = index;
        Invalidate();
    }

    UINT CDuiLabel::GetTextStyle() const
    {
        return m_uTextStyle;
    }

    DWORD CDuiLabel::GetTextColor() const
    {
        return m_dwTextColor;
    }

    void CDuiLabel::SetTextColor(DWORD dwTextColor)
    {
        if(m_dwTextColor == dwTextColor)
        {
            return;
        }
        m_dwTextColor = dwTextColor;
        Invalidate();
    }

    DWORD CDuiLabel::GetDisabledTextColor() const
    {
        return m_dwDisabledTextColor;
    }

    void CDuiLabel::SetDisabledTextColor(DWORD dwTextColor)
    {
        if(m_dwDisabledTextColor == dwTextColor)
        {
            return;
        }
        m_dwDisabledTextColor = dwTextColor;
        Invalidate();
    }

    RECT CDuiLabel::GetTextPadding() const
    {
        return m_rcTextPadding;
    }

    void CDuiLabel::SetTextPadding(RECT rc)
    {
        if(m_rcTextPadding.bottom == rc.bottom &&
                m_rcTextPadding.top == rc.top &&
                m_rcTextPadding.right == rc.right &&
                m_rcTextPadding.left == rc.left)
        {
            return;
        }
        m_rcTextPadding = rc;
        Invalidate();
    }

    BOOL CDuiLabel::IsShowHtml()
    {
        return m_bShowHtml;
    }

    void CDuiLabel::SetShowHtml(BOOL bShowHtml /*= TRUE*/)
    {
        if(m_bShowHtml == bShowHtml)
        {
            return;
        }
        m_bShowHtml = bShowHtml;
        Invalidate();
    }

    void CDuiLabel::SetText(LPCTSTR pstrText)
    {
        __super::SetText(pstrText);
        if(GetAutoCalcWidth())
        {
            NeedParentUpdate();
        }
    }

    BOOL CDuiLabel::GetAutoCalcWidth() const
    {
        return m_bAutoCalcWidth;
    }

    void CDuiLabel::SetAutoCalcWidth(BOOL bAutoCalcWidth)
    {
        m_bAutoCalcWidth = bAutoCalcWidth;
    }

    SIZE CDuiLabel::EstimateSize(SIZE szAvailable)
    {
        if(m_bAutoCalcWidth)
        {
            CDuiString sText = GetText();
            RECT rcText = {0, 0, szAvailable.cx, szAvailable.cy};
            int nLinks = 0;
            if(m_bShowHtml)
            {
                CRenderEngine::DrawHtmlText(GetManager()->GetPaintDC(), GetManager(), rcText, sText, m_dwTextColor, NULL, NULL, nLinks, DT_CALCRECT | m_uTextStyle);
            }
            else
            {
                CRenderEngine::DrawText(GetManager()->GetPaintDC(), GetManager(), rcText, sText, m_dwTextColor, m_iFont, DT_CALCRECT | m_uTextStyle);
            }
            SetFixedWidth(MulDiv(rcText.right - rcText.left + GetManager()->GetDPIObj()->Scale(m_rcTextPadding.left) + GetManager()->GetDPIObj()->Scale(m_rcTextPadding.right), 100, GetManager()->GetDPIObj()->GetScale()));
        }
        if(GetFixedHeight() == 0)
        {
            return CDuiSize(GetManager()->GetDPIObj()->Scale(GetFixedWidth()), GetManager()->GetFontInfo(GetFont())->tm.tmHeight + 4);
        }
        return __super::EstimateSize(szAvailable);
    }

    void CDuiLabel::DoEvent(TEventUI& event)
    {
        if(event.Type == UIEVENT_SETFOCUS)
        {
            SetFocused(TRUE);
            return;
        }
        if(event.Type == UIEVENT_KILLFOCUS)
        {
            SetFocused(FALSE);
            return;
        }
        __super::DoEvent(event);
    }

    void CDuiLabel::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("align")) == 0)
        {
            if(_tcsstr(pstrValue, _T("left")) != NULL)
            {
                m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
                m_uTextStyle |= DT_LEFT;
            }
            if(_tcsstr(pstrValue, _T("center")) != NULL)
            {
                m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
                m_uTextStyle |= DT_CENTER;
            }
            if(_tcsstr(pstrValue, _T("right")) != NULL)
            {
                m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
                m_uTextStyle |= DT_RIGHT;
            }
        }
        else if(_tcsicmp(pstrName, _T("valign")) == 0)
        {
            if(_tcsstr(pstrValue, _T("top")) != NULL)
            {
                m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER | DT_WORDBREAK);
                m_uTextStyle |= (DT_TOP | DT_SINGLELINE);
            }
            if(_tcsstr(pstrValue, _T("vcenter")) != NULL)
            {
                m_uTextStyle &= ~(DT_TOP | DT_BOTTOM | DT_WORDBREAK);
                m_uTextStyle |= (DT_VCENTER | DT_SINGLELINE);
            }
            if(_tcsstr(pstrValue, _T("bottom")) != NULL)
            {
                m_uTextStyle &= ~(DT_TOP | DT_VCENTER | DT_WORDBREAK);
                m_uTextStyle |= (DT_BOTTOM | DT_SINGLELINE);
            }
        }
        else if(_tcsicmp(pstrName, _T("endellipsis")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_uTextStyle |= DT_END_ELLIPSIS;
            }
            else
            {
                m_uTextStyle &= ~DT_END_ELLIPSIS;
            }
        }
        else if(_tcsicmp(pstrName, _T("wordbreak")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_uTextStyle &= ~DT_SINGLELINE;
                m_uTextStyle |= DT_WORDBREAK | DT_EDITCONTROL;
            }
            else
            {
                m_uTextStyle &= ~DT_WORDBREAK & ~DT_EDITCONTROL;
                m_uTextStyle |= DT_SINGLELINE;
            }
        }
        else if(_tcsicmp(pstrName, _T("noprefix")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_uTextStyle |= DT_NOPREFIX;
            }
            else
            {
                m_uTextStyle = m_uTextStyle & ~DT_NOPREFIX;
            }
        }
        else if(_tcsicmp(pstrName, _T("font")) == 0)
        {
            SetFont(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("textcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("disabledtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetDisabledTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("textpadding")) == 0)
        {
            RECT rcTextPadding = { 0 };
            LPTSTR pstr = NULL;
            rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetTextPadding(rcTextPadding);
        }
        else if(_tcsicmp(pstrName, _T("showhtml")) == 0)
        {
            SetShowHtml(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("autocalcwidth")) == 0)
        {
            SetAutoCalcWidth(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else
        {
            __super::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiLabel::PaintText(HDC hDC)
    {
        if(m_dwTextColor == 0)
        {
            m_dwTextColor = GetManager()->GetDefaultFontColor();
        }
        if(m_dwDisabledTextColor == 0)
        {
            m_dwDisabledTextColor = GetManager()->GetDefaultDisabledColor();
        }
        RECT rc = GetPos();
        RECT rcTextPadding = GetTextPadding();
        GetManager()->GetDPIObj()->Scale(&rcTextPadding);
        rc.left += rcTextPadding.left;
        rc.right -= rcTextPadding.right;
        rc.top += rcTextPadding.top;
        rc.bottom -= rcTextPadding.bottom;
        CDuiString sText = GetText();
        if(sText.IsEmpty())
        {
            return;
        }
        int nLinks = 0;
        if(IsEnabled())
        {
            if(m_bShowHtml)
            {
                CRenderEngine::DrawHtmlText(hDC, GetManager(), rc, sText, m_dwTextColor,
                                            NULL, NULL, nLinks, m_uTextStyle);
            }
            else
            {
                CRenderEngine::DrawText(hDC, GetManager(), rc, sText, m_dwTextColor,
                                        m_iFont, m_uTextStyle);
            }
        }
        else
        {
            if(m_bShowHtml)
            {
                CRenderEngine::DrawHtmlText(hDC, GetManager(), rc, sText, m_dwDisabledTextColor,
                                            NULL, NULL, nLinks, m_uTextStyle);
            }
            else
            {
                CRenderEngine::DrawText(hDC, GetManager(), rc, sText, m_dwDisabledTextColor,
                                        m_iFont, m_uTextStyle);
            }
        }
    }

}