#include "StdAfx.h"
#include "DuiControl.h"

namespace DuiLib
{

    IMPLEMENT_DUICONTROL(CDuiControl)

    CDuiControl::CDuiControl(void)
        : m_chShortcut('\0')
        , m_bUpdateNeeded(TRUE)
        , m_bVisible(TRUE)
        , m_bInternVisible(TRUE)
        , m_bSetPos(FALSE)
        , m_bFloat(FALSE)
        , m_bColorHSL(FALSE)
        , m_bDragEnabled(FALSE)
        , m_bDropEnabled(FALSE)
        , m_bResourceText(FALSE)
        , m_bEnabled(TRUE)
        , m_bMouseEnabled(TRUE)
        , m_bKeyboardEnabled(TRUE)
        , m_bFocused(FALSE)
        , m_bMenuUsed(FALSE)
        , m_wCursor(0)
        , m_nBorderSize(0)
        , m_nBorderStyle(0)
        , m_nTooltipWidth(300)
        , m_dwBackColor(0)
        , m_dwBackColor2(0)
        , m_dwBackColor3(0)
        , m_dwForeColor(0)
        , m_dwBorderColor(0)
        , m_dwFocusBorderColor(0)
        , m_sBkImage(_T(""))
        , m_sForeImage(_T(""))
        , m_sName(_T(""))
        , m_sText(_T(""))
        , m_sToolTip(_T(""))
        , m_sUserData(_T(""))
        , m_sVirtualWnd(_T(""))
        , m_pParent(NULL)
        , m_pManager(NULL)
        , m_hInstance(NULL)
    {
        ZeroMemory(&m_rcItem, sizeof(m_rcItem));
        ZeroMemory(&m_cXY, sizeof(m_cXY));
        ZeroMemory(&m_cxyFixed, sizeof(m_cxyFixed));
        ZeroMemory(&m_piFloatPercent, sizeof(m_piFloatPercent));
        ZeroMemory(&m_rcPadding, sizeof(m_rcPadding));
        ZeroMemory(&m_rcBorderSize, sizeof(m_rcBorderSize));
        ZeroMemory(&m_cxyBorderRound, sizeof(m_cxyBorderRound));
        ZeroMemory(&m_cxyMin, sizeof(m_cxyMin));
        ZeroMemory(&m_rcPaint, sizeof(m_rcPaint));
        m_cxyMax.cx = m_cxyMax.cy = 9999;
    }


    CDuiControl::~CDuiControl(void)
    {
    }

    LPCTSTR CDuiControl::GetClass() const
    {
        return DUI_CTR_CONTROL;
    }

    LPVOID CDuiControl::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_CONTROL) == 0)
        {
            return this;
        }
        return NULL;
    }

    UINT CDuiControl::GetControlFlags() const
    {
        return 0;
    }

    CDuiControl* CDuiControl::GetParent() const
    {
        return m_pParent;
    }

    RECT& CDuiControl::GetPaintRect()
    {
        return m_rcPaint;
    }

    void CDuiControl::Invalidate()
    {
        if(!IsVisible())
        {
            return;
        }
        RECT invalidateRc = m_rcItem;
        CDuiControl* pParent = this;
        RECT rcTemp;
        RECT rcParent;
        while(pParent = pParent->GetParent())
        {
            rcTemp = invalidateRc;
            rcParent = pParent->GetPos();
            if(!::IntersectRect(&invalidateRc, &rcTemp, &rcParent))
            {
                return;
            }
        }
        if(m_pManager != NULL)
        {
            m_pManager->Invalidate(invalidateRc);
        }
    }

    BOOL CDuiControl::IsUpdateNeeded() const
    {
        return m_bUpdateNeeded;
    }

    void CDuiControl::NeedParentUpdate()
    {
        if(GetParent())
        {
            GetParent()->NeedUpdate();
            GetParent()->Invalidate();
        }
        else
        {
            NeedUpdate();
        }
        if(m_pManager != NULL)
        {
            m_pManager->NeedUpdate();
        }
    }

    void CDuiControl::NeedUpdate()
    {
        if(!IsVisible())
        {
            return;
        }
        m_bUpdateNeeded = TRUE;
        Invalidate();
        if(m_pManager != NULL)
        {
            m_pManager->NeedUpdate();
        }
    }


    void CDuiControl::SetBkColor(DWORD dwBackColor)
    {
        if(m_dwBackColor == dwBackColor)
        {
            return;
        }
        m_dwBackColor = dwBackColor;
        Invalidate();
    }

    void CDuiControl::SetBkColor2(DWORD dwBackColor)
    {
        if(m_dwBackColor2 == dwBackColor)
        {
            return;
        }
        m_dwBackColor2 = dwBackColor;
        Invalidate();
    }

    void CDuiControl::SetBkColor3(DWORD dwBackColor)
    {
        if(m_dwBackColor3 == dwBackColor)
        {
            return;
        }
        m_dwBackColor3 = dwBackColor;
        Invalidate();
    }

    void CDuiControl::SetForeColor(DWORD dwForeColor)
    {
        if(m_dwForeColor == dwForeColor)
        {
            return;
        }
        m_dwForeColor = dwForeColor;
        Invalidate();
    }

    void CDuiControl::SetBkImage(LPCTSTR pStrImage)
    {
        if(m_pManager)
        {
            m_pManager->RemoveImage(pStrImage);
        }
        if(m_sBkImage == pStrImage)
        {
            return;
        }
        m_sBkImage = pStrImage;
        Invalidate();
    }

    void CDuiControl::SetForeImage(LPCTSTR pStrImage)
    {
        if(m_sForeImage == pStrImage)
        {
            return;
        }
        m_sForeImage = pStrImage;
        Invalidate();
    }

    void CDuiControl::SetFocusBorderColor(DWORD dwBorderColor)
    {
        if(m_dwFocusBorderColor == dwBorderColor)
        {
            return;
        }
        m_dwFocusBorderColor = dwBorderColor;
        Invalidate();
    }

    void CDuiControl::SetColorHSL(BOOL bColorHSL)
    {
        if(m_bColorHSL == bColorHSL)
        {
            return;
        }
        m_bColorHSL = bColorHSL;
        Invalidate();
    }

    void CDuiControl::SetBorderRound(SIZE cxyRound)
    {
        m_cxyBorderRound = cxyRound;
        Invalidate();
    }

    void CDuiControl::SetBorderSize(int nSize)
    {
        if(m_nBorderSize == nSize)
        {
            return;
        }
        m_nBorderSize = nSize;
        Invalidate();
    }

    void CDuiControl::SetBorderSize(RECT rc)
    {
        m_rcBorderSize = rc;
        Invalidate();
    }

    void CDuiControl::SetBorderColor(DWORD dwBorderColor)
    {
        if(m_dwBorderColor == dwBorderColor)
        {
            return;
        }
        m_dwBorderColor = dwBorderColor;
        Invalidate();
    }

    void CDuiControl::SetLeftBorderSize(int nSize)
    {
        m_rcBorderSize.left = nSize;
        Invalidate();
    }

    void CDuiControl::SetTopBorderSize(int nSize)
    {
        m_rcBorderSize.top = nSize;
        Invalidate();
    }

    void CDuiControl::SetRightBorderSize(int nSize)
    {
        m_rcBorderSize.right = nSize;
        Invalidate();
    }

    void CDuiControl::SetBottomBorderSize(int nSize)
    {
        m_rcBorderSize.bottom = nSize;
        Invalidate();
    }

    void CDuiControl::SetBorderStyle(int nStyle)
    {
        m_nBorderStyle = nStyle;
        Invalidate();
    }

    CDuiString CDuiControl::GetVirtualWnd() const
    {
        CDuiString str;
        if(!m_sVirtualWnd.IsEmpty())
        {
            str = m_sVirtualWnd;
        }
        else
        {
            CDuiControl* pParent = GetParent();
            if(pParent != NULL)
            {
                str = pParent->GetVirtualWnd();
            }
            else
            {
                str = _T("");
            }
        }
        return str;
    }

    void CDuiControl::SetVirtualWnd(LPCTSTR pstrValue)
    {
        m_sVirtualWnd = pstrValue;
        m_pManager->UsedVirtualWnd(TRUE);
    }


    CDuiString CDuiControl::GetName() const
    {
        return m_sName;
    }

    void CDuiControl::SetName(LPCTSTR pstrName)
    {
        m_sName = pstrName;
    }

    CDuiString CDuiControl::GetText() const
    {
        return m_sText;
    }

    void CDuiControl::SetText(LPCTSTR pstrText)
    {
        if(m_sText == pstrText)
        {
            return;
        }
        m_sText = pstrText;
        Invalidate();
    }

    void CDuiControl::SetDragEnable(BOOL bDrag)
    {
        m_bDragEnabled = bDrag;
    }

    void CDuiControl::SetDropEnable(BOOL bDrop)
    {
        m_bDropEnabled = bDrop;
    }

    void CDuiControl::SetResourceText(BOOL bResource)
    {
        if(m_bResourceText == bResource)
        {
            return;
        }
        m_bResourceText = bResource;
        Invalidate();
    }

    CDuiString CDuiControl::GetToolTip() const
    {
        //if(!IsResourceText())
        {
            return m_sToolTip;
        }
        //return CResourceManager::GetInstance()->GetText(m_sToolTip);
    }

    void CDuiControl::SetToolTip(LPCTSTR pstrText)
    {
        CDuiString strTemp(pstrText);
        strTemp.Replace(_T("<n>"), _T("\r\n"));
        m_sToolTip = strTemp;
    }

    int CDuiControl::GetToolTipWidth(void)
    {
        if(m_pManager != NULL)
        {
            return m_pManager->GetDPIObj()->Scale(m_nTooltipWidth);
        }
        return m_nTooltipWidth;
    }

    void CDuiControl::SetCursor(WORD wCursor)
    {
        m_wCursor = wCursor;
        Invalidate();
    }

    void CDuiControl::SetShortcut(TCHAR ch)
    {
        m_chShortcut = ch;
    }

    BOOL CDuiControl::IsContextMenuUsed() const
    {
        return m_bMenuUsed;
    }

    void CDuiControl::SetContextMenuUsed(BOOL bMenuUsed)
    {
        m_bMenuUsed = bMenuUsed;
    }

    void CDuiControl::SetUserData(LPCTSTR pstrText)
    {
        m_sUserData = pstrText;
    }

    const RECT& CDuiControl::GetPos() const
    {
        return m_rcItem;
    }

    void CDuiControl::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        if(rc.right < rc.left)
        {
            rc.right = rc.left;
        }
        if(rc.bottom < rc.top)
        {
            rc.bottom = rc.top;
        }
        CDuiRect invalidateRc = m_rcItem;
        if(::IsRectEmpty(&invalidateRc))
        {
            invalidateRc = rc;
        }
        m_rcItem = rc;
        if(m_pManager == NULL)
        {
            return;
        }
        if(!m_bSetPos)
        {
            m_bSetPos = TRUE;
            if(OnSize)
            {
                OnSize(this);
            }
            m_bSetPos = FALSE;
        }
        m_bUpdateNeeded = FALSE;
        if(bNeedInvalidate && IsVisible())
        {
            invalidateRc.Join(m_rcItem);
            CDuiControl* pParent = this;
            RECT rcTemp;
            RECT rcParent;
            while(pParent = pParent->GetParent())
            {
                if(!pParent->IsVisible())
                {
                    return;
                }
                rcTemp = invalidateRc;
                rcParent = pParent->GetPos();
                if(!::IntersectRect(&invalidateRc, &rcTemp, &rcParent))
                {
                    return;
                }
            }
            m_pManager->Invalidate(invalidateRc);
        }
    }

    SIZE CDuiControl::GetFixedXY() const
    {
        return GetManager()->GetDPIObj()->Scale(m_cXY);
    }

    void CDuiControl::SetFixedXY(SIZE szXY)
    {
        m_cXY.cx = szXY.cx;
        m_cXY.cy = szXY.cy;
        if(!m_bFloat)
        {
            NeedParentUpdate();
        }
        else
        {
            NeedUpdate();
        }
    }

    int CDuiControl::GetFixedWidth() const
    {
        if(GetManager())
        {
            return GetManager()->GetDPIObj()->Scale(m_cxyFixed.cx);
        }
        return m_cxyFixed.cx;
    }

    void CDuiControl::SetFixedWidth(int cx)
    {
        if(cx < 0)
        {
            return;
        }
        m_cxyFixed.cx = cx;
        if(!m_bFloat)
        {
            NeedParentUpdate();
        }
        else
        {
            NeedUpdate();
        }
    }

    int CDuiControl::GetFixedHeight() const
    {
        if(GetManager())
        {
            return GetManager()->GetDPIObj()->Scale(m_cxyFixed.cy);
        }
        return m_cxyFixed.cy;
    }

    void CDuiControl::SetFixedHeight(int cy)
    {
        if(cy < 0)
        {
            return;
        }
        m_cxyFixed.cy = cy;
        if(!m_bFloat)
        {
            NeedParentUpdate();
        }
        else
        {
            NeedUpdate();
        }
    }

    RECT CDuiControl::GetPadding() const
    {
        return m_rcPadding;
    }

    void CDuiControl::SetPadding(RECT rcPadding)
    {
        m_rcPadding = rcPadding;
        NeedParentUpdate();
    }

    int CDuiControl::GetMinWidth() const
    {
        return GetManager()->GetDPIObj()->Scale(m_cxyMin.cx);
    }

    void CDuiControl::SetMinWidth(int cx)
    {
        if(m_cxyMin.cx == cx)
        {
            return;
        }
        if(cx < 0)
        {
            return;
        }
        m_cxyMin.cx = cx;
        if(!m_bFloat)
        {
            NeedParentUpdate();
        }
        else
        {
            NeedUpdate();
        }
    }

    int CDuiControl::GetMaxWidth() const
    {
        return GetManager()->GetDPIObj()->Scale(m_cxyMax.cx);
    }

    void CDuiControl::SetMaxWidth(int cx)
    {
        if(m_cxyMax.cx == cx)
        {
            return;
        }
        if(cx < 0)
        {
            return;
        }
        m_cxyMax.cx = cx;
        if(!m_bFloat)
        {
            NeedParentUpdate();
        }
        else
        {
            NeedUpdate();
        }
    }

    int CDuiControl::GetMinHeight() const
    {
        return GetManager()->GetDPIObj()->Scale(m_cxyMin.cy);
    }

    void CDuiControl::SetMinHeight(int cy)
    {
        if(m_cxyMin.cy == cy)
        {
            return;
        }
        if(cy < 0)
        {
            return;
        }
        m_cxyMin.cy = cy;
        if(!m_bFloat)
        {
            NeedParentUpdate();
        }
        else
        {
            NeedUpdate();
        }
    }

    int CDuiControl::GetMaxHeight() const
    {
        return GetManager()->GetDPIObj()->Scale(m_cxyMax.cy);
    }

    void CDuiControl::SetMaxHeight(int cy)
    {
        if(m_cxyMax.cy == cy)
        {
            return;
        }
        if(cy < 0)
        {
            return;
        }
        m_cxyMax.cy = cy;
        if(!m_bFloat)
        {
            NeedParentUpdate();
        }
        else
        {
            NeedUpdate();
        }
    }

    TPercentInfo CDuiControl::GetFloatPercent() const
    {
        return m_piFloatPercent;
    }

    SIZE CDuiControl::EstimateSize(SIZE szAvailable)
    {
        return GetManager()->GetDPIObj()->Scale(m_cxyFixed);
    }

    RECT CDuiControl::GetRelativePos() const
    {
        CDuiControl* pParent = GetParent();
        if(pParent != NULL)
        {
            RECT rcParentPos = pParent->GetPos();
            CDuiRect rcRelativePos(m_rcItem);
            rcRelativePos.Offset(-rcParentPos.left, -rcParentPos.top);
            return rcRelativePos;
        }
        else
        {
            return CDuiRect(0, 0, 0, 0);
        }
    }

    BOOL CDuiControl::IsVisible() const
    {
        return m_bVisible && m_bInternVisible;
    }

    void CDuiControl::SetVisible(BOOL bVisible /*= TRUE*/)
    {
        if(m_bVisible == bVisible)
        {
            return;
        }
        BOOL v = IsVisible();
        m_bVisible = bVisible;
        if(m_bFocused)
        {
            m_bFocused = FALSE;
        }
        if(!bVisible && m_pManager && m_pManager->GetFocus() == this)
        {
            m_pManager->SetFocus(NULL) ;
        }
        if(IsVisible() != v)
        {
            NeedParentUpdate();
        }
    }

    void CDuiControl::SetEnabled(BOOL bEnable /*= TRUE*/)
    {
        if(m_bEnabled == bEnable)
        {
            return;
        }
        m_bEnabled = bEnable;
        Invalidate();
    }

    BOOL CDuiControl::IsEnabled() const
    {
        return m_bEnabled;
    }

    BOOL CDuiControl::IsMouseEnabled() const
    {
        return m_bMouseEnabled;
    }

    void CDuiControl::SetMouseEnabled(BOOL bEnable /*= TRUE*/)
    {
        m_bMouseEnabled = bEnable;
    }

    void CDuiControl::SetKeyboardEnabled(BOOL bEnable /*= TRUE*/)
    {
        m_bKeyboardEnabled = bEnable ;
    }

    void CDuiControl::SetInternVisible(BOOL bVisible /*= TRUE*/)
    {
        m_bInternVisible = bVisible;
        if(!bVisible && m_pManager && m_pManager->GetFocus() == this)
        {
            m_pManager->SetFocus(NULL) ;
        }
    }


    BOOL CDuiControl::IsFocused() const
    {
        return m_bFocused;
    }

    void CDuiControl::SetFocused(BOOL bFocused)
    {
        m_bFocused = bFocused;
    }

    void CDuiControl::SetFocus()
    {
        if(m_pManager != NULL)
        {
            m_pManager->SetFocus(this);
        }
    }

    BOOL CDuiControl::IsFloat() const
    {
        return m_bFloat;
    }

    void CDuiControl::SetFloat(BOOL bFloat /*= TRUE*/)
    {
        if(m_bFloat == bFloat)
        {
            return;
        }
        m_bFloat = bFloat;
        NeedParentUpdate();
    }

    void CDuiControl::SetFloatPercent(TPercentInfo piFloatPercent)
    {
        m_piFloatPercent = piFloatPercent;
        NeedParentUpdate();
    }

    CDuiControl* CDuiControl::ApplyAttributeList(LPCTSTR pstrValue)
    {
        // ������ʽ��
        if(m_pManager != NULL)
        {
            LPCTSTR pStyle = m_pManager->GetStyle(pstrValue);
            if(pStyle != NULL)
            {
                return ApplyAttributeList(pStyle);
            }
        }
        CDuiString sXmlData = pstrValue;
        sXmlData.Replace(_T("&quot;"), _T("\""));
        LPCTSTR pstrList = sXmlData.GetData();
        // ������ʽ����
        CDuiString sItem;
        CDuiString sValue;
        while(*pstrList != _T('\0'))
        {
            sItem.SetEmpty();
            sValue.SetEmpty();
            while(*pstrList != _T('\0') && *pstrList != _T('='))
            {
                LPTSTR pstrTemp = ::CharNext(pstrList);
                while(pstrList < pstrTemp)
                {
                    sItem += *pstrList++;
                }
            }
            ASSERT(*pstrList == _T('='));
            if(*pstrList++ != _T('='))
            {
                return this;
            }
            ASSERT(*pstrList == _T('\"'));
            if(*pstrList++ != _T('\"'))
            {
                return this;
            }
            while(*pstrList != _T('\0') && *pstrList != _T('\"'))
            {
                LPTSTR pstrTemp = ::CharNext(pstrList);
                while(pstrList < pstrTemp)
                {
                    sValue += *pstrList++;
                }
            }
            ASSERT(*pstrList == _T('\"'));
            if(*pstrList++ != _T('\"'))
            {
                return this;
            }
            SetAttribute(sItem, sValue);
            if(*pstrList++ != _T(' ') && *pstrList++ != _T(','))
            {
                return this;
            }
        }
        return this;
    }

    void CDuiControl::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        // �Ƿ���ʽ��
        if(m_pManager != NULL)
        {
            LPCTSTR pStyle = m_pManager->GetStyle(pstrValue);
            if(pStyle != NULL)
            {
                ApplyAttributeList(pStyle);
                return;
            }
        }
        if(_tcsicmp(pstrName, _T("pos")) == 0)
        {
            RECT rcPos = { 0 };
            LPTSTR pstr = NULL;
            rcPos.left = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            rcPos.top = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcPos.right = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcPos.bottom = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SIZE szXY = {rcPos.left >= 0 ? rcPos.left : rcPos.right, rcPos.top >= 0 ? rcPos.top : rcPos.bottom};
            SetFixedXY(szXY);
            SetFixedWidth(rcPos.right - rcPos.left);
            SetFixedHeight(rcPos.bottom - rcPos.top);
        }
        else if(_tcsicmp(pstrName, _T("float")) == 0)
        {
            CDuiString nValue = pstrValue;
            // ��̬������Ա���
            if(nValue.Find(',') < 0)
            {
                SetFloat(_tcsicmp(pstrValue, _T("true")) == 0);
            }
            else
            {
                TPercentInfo piFloatPercent = { 0 };
                LPTSTR pstr = NULL;
                piFloatPercent.left		= _tcstod(pstrValue, &pstr);
                ASSERT(pstr);
                piFloatPercent.top		= _tcstod(pstr + 1, &pstr);
                ASSERT(pstr);
                piFloatPercent.right	= _tcstod(pstr + 1, &pstr);
                ASSERT(pstr);
                piFloatPercent.bottom	= _tcstod(pstr + 1, &pstr);
                ASSERT(pstr);
                SetFloatPercent(piFloatPercent);
                SetFloat(TRUE);
            }
        }
        else if(_tcsicmp(pstrName, _T("padding")) == 0)
        {
            RECT rcPadding = { 0 };
            LPTSTR pstr = NULL;
            rcPadding.left = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            rcPadding.top = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcPadding.right = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetPadding(rcPadding);
        }
        else if(_tcsicmp(pstrName, _T("bkcolor")) == 0 || _tcsicmp(pstrName, _T("bkcolor1")) == 0)
        {
            while(*pstrValue > _T('\0') && *pstrValue <= _T(' '))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("bkcolor2")) == 0)
        {
            while(*pstrValue > _T('\0') && *pstrValue <= _T(' '))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetBkColor2(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("bkcolor3")) == 0)
        {
            while(*pstrValue > _T('\0') && *pstrValue <= _T(' '))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetBkColor3(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("forecolor")) == 0)
        {
            while(*pstrValue > _T('\0') && *pstrValue <= _T(' '))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetForeColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("bordercolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetBorderColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("focusbordercolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetFocusBorderColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("bordersize")) == 0)
        {
            CDuiString nValue = pstrValue;
            if(nValue.Find(',') < 0)
            {
                SetBorderSize(_ttoi(pstrValue));
                RECT rcPadding = {0};
                SetBorderSize(rcPadding);
            }
            else
            {
                RECT rcPadding = { 0 };
                LPTSTR pstr = NULL;
                rcPadding.left = _tcstol(pstrValue, &pstr, 10);
                ASSERT(pstr);
                rcPadding.top = _tcstol(pstr + 1, &pstr, 10);
                ASSERT(pstr);
                rcPadding.right = _tcstol(pstr + 1, &pstr, 10);
                ASSERT(pstr);
                rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10);
                ASSERT(pstr);
                SetBorderSize(rcPadding);
            }
        }
        else if(_tcsicmp(pstrName, _T("borderround")) == 0)
        {
            SIZE cxyRound = { 0 };
            LPTSTR pstr = NULL;
            cxyRound.cx = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            cxyRound.cy = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetBorderRound(cxyRound);
        }
        else if(_tcsicmp(pstrName, _T("colorhsl")) == 0)
        {
            SetColorHSL(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("leftbordersize")) == 0)
        {
            SetLeftBorderSize(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("topbordersize")) == 0)
        {
            SetTopBorderSize(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("rightbordersize")) == 0)
        {
            SetRightBorderSize(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("bottombordersize")) == 0)
        {
            SetBottomBorderSize(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("borderstyle")) == 0)
        {
            SetBorderStyle(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("bkimage")) == 0)
        {
            SetBkImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("foreimage")) == 0)
        {
            SetForeImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("width")) == 0)
        {
            SetFixedWidth(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("height")) == 0)
        {
            SetFixedHeight(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("minwidth")) == 0)
        {
            SetMinWidth(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("minheight")) == 0)
        {
            SetMinHeight(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("maxwidth")) == 0)
        {
            SetMaxWidth(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("maxheight")) == 0)
        {
            SetMaxHeight(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("name")) == 0)
        {
            SetName(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("drag")) == 0)
        {
            SetDragEnable(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("drop")) == 0)
        {
            SetDropEnable(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("resourcetext")) == 0)
        {
            SetResourceText(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("text")) == 0)
        {
            SetText(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("tooltip")) == 0)
        {
            SetToolTip(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("userdata")) == 0)
        {
            SetUserData(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("enabled")) == 0)
        {
            SetEnabled(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("mouse")) == 0)
        {
            SetMouseEnabled(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("keyboard")) == 0)
        {
            SetKeyboardEnabled(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("visible")) == 0)
        {
            SetVisible(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("float")) == 0)
        {
            SetFloat(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("shortcut")) == 0)
        {
            SetShortcut(pstrValue[0]);
        }
        else if(_tcsicmp(pstrName, _T("menu")) == 0)
        {
            SetContextMenuUsed(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("virtualwnd")) == 0)
        {
            SetVirtualWnd(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("cursor")) == 0 && pstrValue)
        {
            if(_tcsicmp(pstrValue, _T("arrow")) == 0)
            {
                SetCursor(DUI_ARROW);
            }
            else if(_tcsicmp(pstrValue, _T("ibeam")) == 0)
            {
                SetCursor(DUI_IBEAM);
            }
            else if(_tcsicmp(pstrValue, _T("wait")) == 0)
            {
                SetCursor(DUI_WAIT);
            }
            else if(_tcsicmp(pstrValue, _T("cross")) == 0)
            {
                SetCursor(DUI_CROSS);
            }
            else if(_tcsicmp(pstrValue, _T("uparrow")) == 0)
            {
                SetCursor(DUI_UPARROW);
            }
            else if(_tcsicmp(pstrValue, _T("size")) == 0)
            {
                SetCursor(DUI_SIZE);
            }
            else if(_tcsicmp(pstrValue, _T("icon")) == 0)
            {
                SetCursor(DUI_ICON);
            }
            else if(_tcsicmp(pstrValue, _T("sizenwse")) == 0)
            {
                SetCursor(DUI_SIZENWSE);
            }
            else if(_tcsicmp(pstrValue, _T("sizenesw")) == 0)
            {
                SetCursor(DUI_SIZENESW);
            }
            else if(_tcsicmp(pstrValue, _T("sizewe")) == 0)
            {
                SetCursor(DUI_SIZEWE);
            }
            else if(_tcsicmp(pstrValue, _T("sizens")) == 0)
            {
                SetCursor(DUI_SIZENS);
            }
            else if(_tcsicmp(pstrValue, _T("sizeall")) == 0)
            {
                SetCursor(DUI_SIZEALL);
            }
            else if(_tcsicmp(pstrValue, _T("no")) == 0)
            {
                SetCursor(DUI_NO);
            }
            else if(_tcsicmp(pstrValue, _T("hand")) == 0)
            {
                SetCursor(DUI_HAND);
            }
        }
        else if(_tcsicmp(pstrName, _T("innerstyle")) == 0)
        {
            CDuiString sXmlData = pstrValue;
            sXmlData.Replace(_T("&quot;"), _T("\""));
            LPCTSTR pstrList = sXmlData.GetData();
            CDuiString sItem;
            CDuiString sValue;
            while(*pstrList != _T('\0'))
            {
                sItem.SetEmpty();
                sValue.SetEmpty();
                while(*pstrList != _T('\0') && *pstrList != _T('='))
                {
                    LPTSTR pstrTemp = ::CharNext(pstrList);
                    while(pstrList < pstrTemp)
                    {
                        sItem += *pstrList++;
                    }
                }
                ASSERT(*pstrList == _T('='));
                if(*pstrList++ != _T('='))
                {
                    return;
                }
                ASSERT(*pstrList == _T('\"'));
                if(*pstrList++ != _T('\"'))
                {
                    return;
                }
                while(*pstrList != _T('\0') && *pstrList != _T('\"'))
                {
                    LPTSTR pstrTemp = ::CharNext(pstrList);
                    while(pstrList < pstrTemp)
                    {
                        sValue += *pstrList++;
                    }
                }
                ASSERT(*pstrList == _T('\"'));
                if(*pstrList++ != _T('\"'))
                {
                    return;
                }
                SetAttribute(sItem, sValue);
                if(*pstrList++ != _T(' ') && *pstrList++ != _T(','))
                {
                    return;
                }
            }
        }
    }

    DWORD CDuiControl::GetAdjustColor(DWORD dwColor)
    {
        if(!m_bColorHSL)
        {
            return dwColor;
        }
        short H, S, L;
        CDuiPaintManager::GetHSL(&H, &S, &L);
        return CRenderEngine::AdjustColor(dwColor, H, S, L);
    }

    BOOL CDuiControl::DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify /*= NULL*/)
    {
        return CRenderEngine::DrawImageString(hDC, m_pManager, m_rcItem, m_rcPaint, pStrImage, pStrModify, m_hInstance);
    }

    void CDuiControl::PaintBkColor(HDC hDC)
    {
        if(m_dwBackColor != 0)
        {
            if(m_dwBackColor2 != 0)
            {
                if(m_dwBackColor3 != 0)
                {
                    RECT rc = m_rcItem;
                    rc.bottom = (rc.bottom + rc.top) / 2;
                    CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), TRUE, 8);
                    rc.top = rc.bottom;
                    rc.bottom = m_rcItem.bottom;
                    CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor2), GetAdjustColor(m_dwBackColor3), TRUE, 8);
                }
                else
                {
                    CRenderEngine::DrawGradient(hDC, m_rcItem, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), TRUE, 16);
                }
            }
            else if(m_dwBackColor >= 0xFF000000)
            {
                CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwBackColor));
            }
            else
            {
                CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwBackColor));
            }
        }
    }

    void CDuiControl::PaintBkImage(HDC hDC)
    {
        if(m_sBkImage.IsEmpty())
        {
            return;
        }
        if(!DrawImage(hDC, (LPCTSTR)m_sBkImage))
        {
        }
    }

    void CDuiControl::PaintStatusImage(HDC hDC)
    {
        return;
    }

    void CDuiControl::PaintForeColor(HDC hDC)
    {
        CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwForeColor));
    }

    void CDuiControl::PaintForeImage(HDC hDC)
    {
        if(m_sForeImage.IsEmpty())
        {
            return;
        }
        DrawImage(hDC, m_sForeImage.GetData());
    }

    void CDuiControl::PaintText(HDC hDC)
    {
        return;
    }

    void CDuiControl::PaintBorder(HDC hDC)
    {
        int nBorderSize = GetManager()->GetDPIObj()->Scale(m_nBorderSize);
        SIZE cxyBorderRound = GetManager()->GetDPIObj()->Scale(m_cxyBorderRound);
        RECT rcBorderSize = GetManager()->GetDPIObj()->Scale(m_rcBorderSize);
        if(m_dwBorderColor != 0 || m_dwFocusBorderColor != 0)
        {
            //��Բ�Ǳ߿�
            if(nBorderSize > 0 && (cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0))
            {
                if(IsFocused() && m_dwFocusBorderColor != 0)
                {
                    CRenderEngine::DrawRoundRect(hDC, m_rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(m_dwFocusBorderColor), m_nBorderStyle);
                }
                else
                {
                    CRenderEngine::DrawRoundRect(hDC, m_rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(m_dwBorderColor), m_nBorderStyle);
                }
            }
            else
            {
                if(IsFocused() && m_dwFocusBorderColor != 0 && m_nBorderSize > 0)
                {
                    CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, GetAdjustColor(m_dwFocusBorderColor), m_nBorderStyle);
                }
                else if(rcBorderSize.left > 0 || rcBorderSize.top > 0 || rcBorderSize.right > 0 || rcBorderSize.bottom > 0)
                {
                    RECT rcBorder;
                    if(rcBorderSize.left > 0)
                    {
                        rcBorder		= m_rcItem;
                        rcBorder.right	= rcBorder.left;
                        CRenderEngine::DrawLine(hDC, rcBorder, rcBorderSize.left, GetAdjustColor(m_dwBorderColor), m_nBorderStyle);
                    }
                    if(rcBorderSize.top > 0)
                    {
                        rcBorder		= m_rcItem;
                        rcBorder.bottom	= rcBorder.top;
                        CRenderEngine::DrawLine(hDC, rcBorder, rcBorderSize.top, GetAdjustColor(m_dwBorderColor), m_nBorderStyle);
                    }
                    if(rcBorderSize.right > 0)
                    {
                        rcBorder		= m_rcItem;
                        rcBorder.right -= 1;
                        rcBorder.left	= rcBorder.right;
                        CRenderEngine::DrawLine(hDC, rcBorder, rcBorderSize.right, GetAdjustColor(m_dwBorderColor), m_nBorderStyle);
                    }
                    if(rcBorderSize.bottom > 0)
                    {
                        rcBorder		= m_rcItem;
                        rcBorder.bottom -= 1;
                        rcBorder.top	= rcBorder.bottom;
                        CRenderEngine::DrawLine(hDC, rcBorder, rcBorderSize.bottom, GetAdjustColor(m_dwBorderColor), m_nBorderStyle);
                    }
                }
                else if(nBorderSize > 0)
                {
                    CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, GetAdjustColor(m_dwBorderColor), m_nBorderStyle);
                }
            }
        }
    }

    void CDuiControl::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        if(!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem))
        {
            return;
        }
        // ����ѭ�򣺱�����ɫ->����ͼ->״̬ͼ->�ı�->�߿�
        SIZE cxyBorderRound = GetManager()->GetDPIObj()->Scale(m_cxyBorderRound);
        RECT rcBorderSize = GetManager()->GetDPIObj()->Scale(m_rcBorderSize);
        if(cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0)
        {
            CRenderClip roundClip;
            CRenderClip::GenerateRoundClip(hDC, m_rcPaint,  m_rcItem, cxyBorderRound.cx, cxyBorderRound.cy, roundClip);
            PaintBkColor(hDC);
            PaintBkImage(hDC);
            PaintStatusImage(hDC);
            PaintForeColor(hDC);
            PaintForeImage(hDC);
            PaintText(hDC);
            PaintBorder(hDC);
        }
        else
        {
            PaintBkColor(hDC);
            PaintBkImage(hDC);
            PaintStatusImage(hDC);
            PaintForeColor(hDC);
            PaintForeImage(hDC);
            PaintText(hDC);
            PaintBorder(hDC);
        }
    }

    void CDuiControl::DoPostPaint(HDC hDC, const RECT& rcPaint)
    {
    }

    void CDuiControl::Init()
    {
    }


    CDuiPaintManager* CDuiControl::GetManager() const
    {
        return m_pManager;
    }

    void CDuiControl::SetManager(CDuiPaintManager* pManager, CDuiControl* pParent, BOOL bInit /*= TRUE*/)
    {
        m_pManager = pManager;
        m_pParent = pParent;
        if(bInit && m_pParent)
        {
            Init();
        }
    }

    void CDuiControl::Event(TEventUI& event)
    {
        if(OnEvent(&event))
        {
            DoEvent(event);
        }
    }

    void CDuiControl::DoEvent(TEventUI& event)
    {
        if(event.Type == UIEVENT_SETCURSOR)
        {
            if(GetCursor())
            {
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(GetCursor())));
            }
            else
            {
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
            }
            return;
        }
        if(event.Type == UIEVENT_SETFOCUS)
        {
            m_bFocused = TRUE;
            Invalidate();
            return;
        }
        if(event.Type == UIEVENT_KILLFOCUS)
        {
            m_bFocused = FALSE;
            Invalidate();
            return;
        }
        if(event.Type == UIEVENT_TIMER)
        {
            m_pManager->SendNotify(this, DUI_MSGTYPE_TIMER, event.wParam, event.lParam);
            return;
        }
        if(event.Type == UIEVENT_CONTEXTMENU)
        {
            if(IsContextMenuUsed())
            {
                m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
                return;
            }
        }
        if(m_pParent != NULL)
        {
            m_pParent->DoEvent(event);
        }
    }

    CDuiControl* CDuiControl::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
    {
        if((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible())
        {
            return NULL;
        }
        if((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled())
        {
            return NULL;
        }
        if((uFlags & UIFIND_HITTEST) != 0 && (!m_bMouseEnabled || !::PtInRect(&m_rcItem, * static_cast<LPPOINT>(pData))))
        {
            return NULL;
        }
        return Proc(this, pData);
    }
}
