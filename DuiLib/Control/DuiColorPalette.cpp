#include "StdAfx.h"
#include "DuiColorPalette.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiColorPalette)

    CDuiColorPalette::CDuiColorPalette(void)
    {
    }


    CDuiColorPalette::~CDuiColorPalette(void)
    {
    }

    LPCTSTR CDuiColorPalette::GetClass() const
    {
        return DUI_CTR_COLORPALETTE;
    }

    LPVOID CDuiColorPalette::GetInterface(LPCTSTR pstrName)
    {
        if(_tcscmp(pstrName, DUI_CTR_COLORPALETTE) == 0)
        {
            return static_cast<CDuiColorPalette*>(this);
        }
        return CDuiControl::GetInterface(pstrName);
    }

    void CDuiColorPalette::SetPalletHeight(int nHeight)
    {
        m_nPalletHeight = nHeight;
    }
    int	 CDuiColorPalette::GetPalletHeight() const
    {
        return m_nPalletHeight;
    }
    void CDuiColorPalette::SetBarHeight(int nHeight)
    {
        if(nHeight > 150)
        {
            nHeight = 150; //限制最大高度，由于当前设计，nheight超出190，程序会因越界访问崩溃
        }
        m_nBarHeight = nHeight;
    }
    int  CDuiColorPalette::GetBarHeight() const
    {
        return m_nBarHeight;
    }

    void CDuiColorPalette::SetThumbImage(LPCTSTR pszImage)
    {
        if(m_strThumbImage != pszImage)
        {
            m_strThumbImage = pszImage;
            NeedUpdate();
        }
    }

    CDuiString CDuiColorPalette::GetThumbImage() const
    {
        return m_strThumbImage;
    }

    void CDuiColorPalette::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcscmp(pstrName, _T("palletheight")) == 0)
        {
            SetPalletHeight(_ttoi(pstrValue));
        }
        else if(_tcscmp(pstrName, _T("barheight")) == 0)
        {
            SetBarHeight(_ttoi(pstrValue));
        }
        else if(_tcscmp(pstrName, _T("thumbimage")) == 0)
        {
            SetThumbImage(pstrValue);
        }
        else
        {
            CDuiControl::SetAttribute(pstrName, pstrValue);
        }
    }

}