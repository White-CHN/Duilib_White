#include "StdAfx.h"
#include "DuiGroupBox.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiGroupBox)

    CDuiGroupBox::CDuiGroupBox(void)
        : m_iFont(-1)
        , m_dwTextColor(0)
        , m_dwDisabledTextColor(0)
        , m_uTextStyle(DT_SINGLELINE | DT_VCENTER | DT_CENTER)
    {
        SetInset(CDuiRect(20, 25, 20, 20));
    }


    CDuiGroupBox::~CDuiGroupBox(void)
    {
    }

    LPCTSTR CDuiGroupBox::GetClass() const
    {
        return DUI_CTR_GROUPBOX;
    }

    LPVOID CDuiGroupBox::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_GROUPBOX) == 0)
        {
            return static_cast<CDuiGroupBox*>(this);
        }
        return CDuiVerticalLayout::GetInterface(pstrName);
    }

    DWORD CDuiGroupBox::GetTextColor() const
    {
        return m_dwTextColor;
    }

    void CDuiGroupBox::SetTextColor(DWORD dwTextColor)
    {
        if(m_dwTextColor == dwTextColor)
        {
            return;
        }
        m_dwTextColor = dwTextColor;
        Invalidate();
    }

    DWORD CDuiGroupBox::GetDisabledTextColor() const
    {
        return m_dwDisabledTextColor;
    }

    void CDuiGroupBox::SetDisabledTextColor(DWORD dwTextColor)
    {
        if(m_dwDisabledTextColor == dwTextColor)
        {
            return;
        }
        m_dwDisabledTextColor = dwTextColor;
        Invalidate();
    }

    int CDuiGroupBox::GetFont() const
    {
        return m_iFont;
    }

    void CDuiGroupBox::SetFont(int index)
    {
        if(m_iFont == index)
        {
            return;
        }
        m_iFont = index;
        Invalidate();
    }

    void CDuiGroupBox::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("textcolor")) == 0)
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
        else if(_tcsicmp(pstrName, _T("font")) == 0)
        {
            SetFont(_ttoi(pstrValue));
        }

        CDuiVerticalLayout::SetAttribute(pstrName, pstrValue);
    }

    void CDuiGroupBox::PaintText(HDC hDC)
    {
        CDuiString sText = GetText();
        if(sText.IsEmpty())
        {
            return;
        }

        if(m_dwTextColor == 0)
        {
            m_dwTextColor = GetManager()->GetDefaultFontColor();
        }
        if(m_dwDisabledTextColor == 0)
        {
            m_dwDisabledTextColor = GetManager()->GetDefaultDisabledColor();
        }

        CDuiRect rcText = GetPos();
        rcText.Deflate(5, 5);
        SIZE szAvailable = { rcText.right - rcText.left, rcText.bottom - rcText.top };
        SIZE sz = CalcrectSize(szAvailable);

        //¼ÆËãÎÄ×ÖÇøÓò
        rcText.left = rcText.left + GROUPBOX_TEXT_OFFSET;
        rcText.top = rcText.top - 7;
        rcText.right = rcText.left + sz.cx;
        rcText.bottom = rcText.top + sz.cy;
        if(IsEnabled())
        {
            CRenderEngine::DrawText(hDC, GetManager(), rcText, sText, m_dwTextColor, m_iFont, m_uTextStyle,
                                    GetAdjustColor(GetBkColor()), FALSE);
        }
        else
        {
            CRenderEngine::DrawText(hDC, GetManager(), rcText, sText, m_dwDisabledTextColor, m_iFont, m_uTextStyle,
                                    GetAdjustColor(GetBkColor()), FALSE);
        }
    }

    void CDuiGroupBox::PaintBorder(HDC hDC)
    {
        if(GetBorderSize() > 0)
        {
            CDuiRect rcItem = GetPos();
            rcItem.Deflate(5, 5);
            if(GetBorderRound().cx > 0 || GetBorderRound().cy > 0)  //»­Ô²½Ç±ß¿ò
            {
                if(IsFocused() && GetFocusBorderColor() != 0)
                {
                    CRenderEngine::DrawRoundRect(hDC, rcItem, GetBorderSize(), GetBorderRound().cx, GetBorderRound().cy, GetAdjustColor(GetFocusBorderColor()));
                }
                else
                {
                    CRenderEngine::DrawRoundRect(hDC, rcItem, GetBorderSize(), GetBorderRound().cx, GetBorderRound().cy, GetAdjustColor(GetBorderColor()));
                }
            }
            else
            {
                if(IsFocused() && GetFocusBorderColor() != 0)
                {
                    CRenderEngine::DrawRect(hDC, rcItem, GetBorderSize(), GetAdjustColor(GetFocusBorderColor()));
                }
                else
                {
                    CRenderEngine::DrawRect(hDC, rcItem, GetBorderSize(), GetAdjustColor(GetBorderColor()));
                }
            }
        }
        PaintText(hDC);
    }

    SIZE CDuiGroupBox::CalcrectSize(SIZE szAvailable)
    {
        RECT rcText = { 0, 0, MAX(szAvailable.cx, GetFixedWidth()), 20 };
        rcText.left += m_rcTextPadding.left;
        rcText.right -= m_rcTextPadding.right;

        CDuiString sText = GetText();

        CRenderEngine::DrawText(GetManager()->GetPaintDC(), GetManager(), rcText, sText, m_dwTextColor, m_iFont, DT_CALCRECT | m_uTextStyle);
        SIZE cXY = {rcText.right - rcText.left + m_rcTextPadding.left + m_rcTextPadding.right,
                    rcText.bottom - rcText.top + m_rcTextPadding.top + m_rcTextPadding.bottom
                   };

        return cXY;
    }

}
