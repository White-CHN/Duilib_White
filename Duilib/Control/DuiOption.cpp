#include "StdAfx.h"
#include "DuiOption.h"


namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiOption)
    CDuiOption::CDuiOption(void)
        : m_bSelected(FALSE)
        , m_dwSelectedBkColor(0)
        , m_dwSelectedTextColor(0)
    {
    }


    CDuiOption::~CDuiOption(void)
    {
        if(!m_sGroupName.IsEmpty() && GetManager())
        {
            GetManager()->RemoveOptionGroup(m_sGroupName, this);
        }
    }

    LPCTSTR CDuiOption::GetClass() const
    {
        return DUI_CTR_OPTION;
    }

    LPVOID CDuiOption::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_OPTION) == 0)
        {
            return static_cast<CDuiOption*>(this);
        }
        return __super::GetInterface(pstrName);
    }

    BOOL CDuiOption::Activate()
    {
        if(!__super::Activate())
        {
            return FALSE;
        }
        if(!m_sGroupName.IsEmpty())
        {
            Selected(TRUE);
        }
        else
        {
            Selected(!m_bSelected);
        }

        return TRUE;
    }

    void CDuiOption::SetGroup(LPCTSTR pStrGroupName /*= NULL*/)
    {
        if(pStrGroupName == NULL)
        {
            if(m_sGroupName.IsEmpty())
            {
                return;
            }
            m_sGroupName.SetEmpty();
        }
        else
        {
            if(m_sGroupName == pStrGroupName)
            {
                return;
            }
            if(!m_sGroupName.IsEmpty() && GetManager())
            {
                GetManager()->RemoveOptionGroup(m_sGroupName, this);
            }
            m_sGroupName = pStrGroupName;
        }

        if(!m_sGroupName.IsEmpty())
        {
            if(GetManager())
            {
                GetManager()->AddOptionGroup(m_sGroupName, this);
            }
        }
        else
        {
            if(GetManager())
            {
                GetManager()->RemoveOptionGroup(m_sGroupName, this);
            }
        }

        Selected(m_bSelected);
    }

    BOOL CDuiOption::IsSelected() const
    {
        return m_bSelected;
    }

    void CDuiOption::Selected(BOOL bSelected)
    {
        if(m_bSelected == bSelected)
        {
            return;
        }

        m_bSelected = bSelected;
        UINT uButtonState = GetButtonState();
        if(m_bSelected)
        {
            uButtonState |= UISTATE_SELECTED;
        }
        else
        {
            uButtonState &= ~UISTATE_SELECTED;
        }
        SetButtonStste(uButtonState);
        if(GetManager() != NULL)
        {
            if(!m_sGroupName.IsEmpty())
            {
                if(m_bSelected)
                {
                    CStdPtrArray* aOptionGroup = GetManager()->GetOptionGroup(m_sGroupName);
                    for(int i = 0; i < aOptionGroup->GetSize(); i++)
                    {
                        CDuiOption* pControl = static_cast<CDuiOption*>(aOptionGroup->GetAt(i));
                        if(pControl != this)
                        {
                            pControl->Selected(FALSE);
                        }
                    }
                    GetManager()->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
                }
            }
            else
            {
                GetManager()->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
            }
        }

        Invalidate();
    }

    LPCTSTR CDuiOption::GetSelectedImage()
    {
        return m_sSelectedImage;
    }

    void CDuiOption::SetSelectedImage(LPCTSTR pStrImage)
    {
        m_sSelectedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiOption::GetSelectedHotImage()
    {
        return m_sSelectedHotImage;
    }

    void CDuiOption::SetSelectedHotImage(LPCTSTR pStrImage)
    {
        m_sSelectedHotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiOption::GetSelectedPushedImage()
    {
        return m_sSelectedPushedImage;
    }

    void CDuiOption::SetSelectedPushedImage(LPCTSTR pStrImage)
    {
        m_sSelectedPushedImage = pStrImage;
        Invalidate();
    }

    void CDuiOption::SetSelectedTextColor(DWORD dwTextColor)
    {
        m_dwSelectedTextColor = dwTextColor;
    }

    DWORD CDuiOption::GetSelectedTextColor()
    {
        if(m_dwSelectedTextColor == 0)
        {
            m_dwSelectedTextColor = GetManager()->GetDefaultFontColor();
        }
        return m_dwSelectedTextColor;
    }

    void CDuiOption::SetSelectedBkColor(DWORD dwBkColor)
    {
        m_dwSelectedBkColor = dwBkColor;
    }

    DWORD CDuiOption::GetSelectBkColor()
    {
        return m_dwSelectedBkColor;
    }

    LPCTSTR CDuiOption::GetSelectedForedImage()
    {
        return m_sSelectedForeImage;
    }

    void CDuiOption::SetSelectedForedImage(LPCTSTR pStrImage)
    {
        m_sSelectedForeImage = pStrImage;
        Invalidate();
    }

    void CDuiOption::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("group")) == 0)
        {
            SetGroup(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("selected")) == 0)
        {
            Selected(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("selectedimage")) == 0)
        {
            SetSelectedImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("selectedhotimage")) == 0)
        {
            SetSelectedHotImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("selectedpushedimage")) == 0)
        {
            SetSelectedPushedImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("selectedforeimage")) == 0)
        {
            SetSelectedForedImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("selectedbkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("selectedtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedTextColor(clrColor);
        }
        else
        {
            __super::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiOption::PaintBkColor(HDC hDC)
    {
        if(IsSelected())
        {
            if(m_dwSelectedBkColor != 0)
            {
                CRenderEngine::DrawColor(hDC, GetPaintRect(), GetAdjustColor(m_dwSelectedBkColor));
            }
        }
        else
        {
            return __super::PaintBkColor(hDC);
        }
    }

    void CDuiOption::PaintStatusImage(HDC hDC)
    {
        if(IsSelected())
        {
            if((GetButtonState() & UISTATE_PUSHED) != 0 && !m_sSelectedPushedImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sSelectedPushedImage)) {}
                else
                {
                    return;
                }
            }
            else if((GetButtonState() & UISTATE_HOT) != 0 && !m_sSelectedHotImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sSelectedHotImage)) {}
                else
                {
                    return;
                }
            }

            if(!m_sSelectedImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sSelectedImage)) {}
            }
        }
        else
        {
            __super::PaintStatusImage(hDC);
        }
    }

    void CDuiOption::PaintForeImage(HDC hDC)
    {
        if(IsSelected())
        {
            if(!m_sSelectedForeImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sSelectedForeImage)) {}
                else
                {
                    return;
                }
            }
        }
        __super::PaintForeImage(hDC);
    }

    void CDuiOption::PaintText(HDC hDC)
    {
        if((GetButtonState() & UISTATE_SELECTED) != 0)
        {
            if(GetTextColor() == 0)
            {
                SetTextColor(GetManager()->GetDefaultFontColor());
            }
            if(m_dwSelectedTextColor == 0)
            {
                m_dwSelectedTextColor = GetTextColor();
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
            int nLinks = 0;
            RECT rc = GetPos();
            RECT rcTextPadding = GetTextPadding();
            if(GetManager())
            {
                GetManager()->GetDPIObj()->Scale(&rcTextPadding);
            }
            rc.left += rcTextPadding.left;
            rc.right -= rcTextPadding.right;
            rc.top += rcTextPadding.top;
            rc.bottom -= rcTextPadding.bottom;

            if(IsShowHtml())
            {
                CRenderEngine::DrawHtmlText(hDC, GetManager(), rc, sText, IsEnabled() ? m_dwSelectedTextColor : GetDisabledTextColor(),
                                            NULL, NULL, nLinks, GetTextStyle());
            }
            else
            {
                CRenderEngine::DrawText(hDC, GetManager(), rc, sText, IsEnabled() ? m_dwSelectedTextColor : GetDisabledTextColor(),
                                        GetFont(), GetTextStyle());
            }
        }
        else
        {
            __super::PaintText(hDC);
        }
    }
}