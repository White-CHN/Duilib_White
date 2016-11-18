#include "StdAfx.h"
#include "DuiContainer.h"

namespace DuiLib
{

    IMPLEMENT_DUICONTROL(CDuiContainer)

    CDuiContainer::CDuiContainer(void)
        : m_bAutoDestroy(TRUE)
        , m_bDelayedDestroy(TRUE)
        , m_bMouseChildEnabled(TRUE)
        , m_iChildPadding(0)
        , m_nScrollStepSize(0)
        , m_iChildAlign(DT_LEFT)
        , m_iChildVAlign(DT_TOP)
        , m_pVerticalScrollBar(NULL)
        , m_pHorizontalScrollBar(NULL)
    {
        ZeroMemory(&m_rcInset, sizeof(m_rcInset));
    }


    CDuiContainer::~CDuiContainer(void)
    {
        m_bDelayedDestroy = FALSE;
        RemoveAll();
        DUI_FREE_POINT(m_pVerticalScrollBar);
        DUI_FREE_POINT(m_pHorizontalScrollBar);
    }

    LPCTSTR CDuiContainer::GetClass() const
    {
        return DUI_CTR_CONTAINER;
    }

    LPVOID CDuiContainer::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, _T("IContainer")) == 0)
        {
            return static_cast<IContainer*>(this);
        }
        else if(_tcsicmp(pstrName, DUI_CTR_CONTAINER) == 0)
        {
            return static_cast<CDuiContainer*>(this);
        }
        return __super::GetInterface(pstrName);
    }



    RECT CDuiContainer::GetInset() const
    {
        return GetManager()->GetDPIObj()->Scale(m_rcInset);
    }

    void CDuiContainer::SetInset(RECT rcInset)
    {
        m_rcInset = rcInset;
        NeedUpdate();
    }

    int CDuiContainer::GetChildPadding() const
    {
        if(GetManager())
        {
            return GetManager()->GetDPIObj()->Scale(m_iChildPadding);
        }
        return m_iChildPadding;
    }

    void CDuiContainer::SetChildPadding(int iPadding)
    {
        m_iChildPadding = iPadding;
        NeedUpdate();
    }

    UINT CDuiContainer::GetChildAlign() const
    {
        return m_iChildAlign;
    }

    UINT CDuiContainer::GetChildVAlign() const
    {
        return m_iChildVAlign;
    }

    BOOL CDuiContainer::IsMouseChildEnabled() const
    {
        return m_bMouseChildEnabled;
    }

    void CDuiContainer::SetMouseChildEnabled(BOOL bEnable /*= TRUE*/)
    {
        m_bMouseChildEnabled = bEnable;
    }

    SIZE CDuiContainer::GetScrollPos() const
    {
        SIZE sz = {0, 0};
        if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
        {
            sz.cy = m_pVerticalScrollBar->GetScrollPos();
        }
        if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
        {
            sz.cx = m_pHorizontalScrollBar->GetScrollPos();
        }
        return sz;
    }

    SIZE CDuiContainer::GetScrollRange() const
    {
        SIZE sz = {0, 0};
        if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
        {
            sz.cy = m_pVerticalScrollBar->GetScrollRange();
        }
        if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
        {
            sz.cx = m_pHorizontalScrollBar->GetScrollRange();
        }
        return sz;
    }

    int CDuiContainer::GetScrollStepSize() const
    {
        if(GetManager())
        {
            return GetManager()->GetDPIObj()->Scale(m_nScrollStepSize);
        }

        return m_nScrollStepSize;
    }

    void CDuiContainer::SetScrollStepSize(int nSize)
    {
        if(nSize > 0)
        {
            m_nScrollStepSize = nSize;
        }
    }

    void CDuiContainer::SetScrollPos(SIZE szPos, BOOL bMsg /*= TURE*/)
    {
        int cx = 0;
        int cy = 0;
        if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
        {
            int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
            m_pVerticalScrollBar->SetScrollPos(szPos.cy);
            cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
        }

        if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
        {
            int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
            m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
            cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
        }

        if(cx == 0 && cy == 0)
        {
            return;
        }

        RECT rcPos;
        for(int it2 = 0; it2 < m_items.GetSize(); it2++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(m_items[it2]);
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                continue;
            }

            rcPos = pControl->GetPos();
            rcPos.left -= cx;
            rcPos.right -= cx;
            rcPos.top -= cy;
            rcPos.bottom -= cy;
            pControl->SetPos(rcPos);
        }

        Invalidate();

        if(m_pVerticalScrollBar)
        {
            // 发送滚动消息
            if(GetManager() != NULL && bMsg)
            {
                int nPage = (m_pVerticalScrollBar->GetScrollPos() + m_pVerticalScrollBar->GetLineSize()) / m_pVerticalScrollBar->GetLineSize();
                GetManager()->SendNotify(this, DUI_MSGTYPE_SCROLL, (WPARAM)nPage);
            }
        }
    }

    void CDuiContainer::LineUp()
    {
        int cyLine = GetScrollStepSize();
        if(cyLine == 0)
        {
            cyLine = 8;
            if(GetManager())
            {
                cyLine = GetManager()->GetDefaultFontInfo()->tm.tmHeight + 8;
            }
        }

        SIZE sz = GetScrollPos();
        sz.cy -= cyLine;
        SetScrollPos(sz);
    }

    void CDuiContainer::LineDown()
    {
        int cyLine = GetScrollStepSize();
        if(cyLine == 0)
        {
            cyLine = 8;
            if(GetManager())
            {
                cyLine = GetManager()->GetDefaultFontInfo()->tm.tmHeight + 8;
            }
        }

        SIZE sz = GetScrollPos();
        sz.cy += cyLine;
        SetScrollPos(sz);
    }

    void CDuiContainer::LineLeft()
    {
        int nScrollStepSize = GetScrollStepSize();
        int cxLine = nScrollStepSize == 0 ? 8 : nScrollStepSize;

        SIZE sz = GetScrollPos();
        sz.cx -= cxLine;
        SetScrollPos(sz);
    }

    void CDuiContainer::LineRight()
    {
        int nScrollStepSize = GetScrollStepSize();
        int cxLine = nScrollStepSize == 0 ? 8 : nScrollStepSize;

        SIZE sz = GetScrollPos();
        sz.cx += cxLine;
        SetScrollPos(sz);
    }

    void CDuiContainer::PageUp()
    {
        SIZE sz = GetScrollPos();
        int iOffset = GetPos().bottom - GetPos().top - m_rcInset.top - m_rcInset.bottom;
        if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
        {
            iOffset -= m_pHorizontalScrollBar->GetFixedHeight();
        }
        sz.cy -= iOffset;
        SetScrollPos(sz);
    }

    void CDuiContainer::PageDown()
    {
        SIZE sz = GetScrollPos();
        int iOffset = GetPos().bottom - GetPos().top - m_rcInset.top - m_rcInset.bottom;
        if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
        {
            iOffset -= m_pHorizontalScrollBar->GetFixedHeight();
        }
        sz.cy += iOffset;
        SetScrollPos(sz);
    }

    void CDuiContainer::PageLeft()
    {
        SIZE sz = GetScrollPos();
        int iOffset = GetPos().right - GetPos().left - m_rcInset.left - m_rcInset.right;
        if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
        {
            iOffset -= m_pVerticalScrollBar->GetFixedWidth();
        }
        sz.cx -= iOffset;
        SetScrollPos(sz);
    }

    void CDuiContainer::PageRight()
    {
        SIZE sz = GetScrollPos();
        int iOffset = GetPos().right - GetPos().left - m_rcInset.left - m_rcInset.right;
        if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
        {
            iOffset -= m_pVerticalScrollBar->GetFixedWidth();
        }
        sz.cx += iOffset;
        SetScrollPos(sz);
    }

    void CDuiContainer::HomeUp()
    {
        SIZE sz = GetScrollPos();
        sz.cy = 0;
        SetScrollPos(sz);
    }

    void CDuiContainer::EndDown()
    {
        SIZE sz = GetScrollPos();
        sz.cy = GetScrollRange().cy;
        SetScrollPos(sz);
    }

    void CDuiContainer::HomeLeft()
    {
        SIZE sz = GetScrollPos();
        sz.cx = 0;
        SetScrollPos(sz);
    }

    void CDuiContainer::EndRight()
    {
        SIZE sz = GetScrollPos();
        sz.cx = GetScrollRange().cx;
        SetScrollPos(sz);
    }

    CDuiScrollBar* CDuiContainer::GetVerticalScrollBar() const
    {
        return m_pVerticalScrollBar;
    }

    CDuiScrollBar* CDuiContainer::GetHorizontalScrollBar() const
    {
        return m_pHorizontalScrollBar;
    }

    CDuiControl* CDuiContainer::GetItemAt(int iIndex) const
    {
        if(iIndex < 0 || iIndex >= m_items.GetSize())
        {
            return NULL;
        }
        return static_cast<CDuiControl*>(m_items[iIndex]);
    }

    int CDuiContainer::GetItemIndex(CDuiControl* pControl) const
    {
        for(int i = 0; i < m_items.GetSize(); i++)
        {
            if(static_cast<CDuiControl*>(m_items[i]) == pControl)
            {
                return i;
            }
        }
        return -1;
    }

    BOOL CDuiContainer::SetItemIndex(CDuiControl* pControl, int iIndex)
    {
        for(int it = 0; it < m_items.GetSize(); it++)
        {
            if(static_cast<CDuiControl*>(m_items[it]) == pControl)
            {
                NeedUpdate();
                m_items.Remove(it);
                return m_items.InsertAt(iIndex, pControl);
            }
        }
        return FALSE;
    }

    int CDuiContainer::GetCount() const
    {
        return m_items.GetSize();
    }

    BOOL CDuiContainer::Add(CDuiControl* pControl)
    {
        if(pControl == NULL)
        {
            return FALSE;
        }
        if(GetManager() != NULL)
        {
            GetManager()->InitControls(pControl, this);
        }
        if(IsVisible())
        {
            NeedUpdate();
        }
        else
        {
            pControl->SetInternVisible(FALSE);
        }
        return m_items.Add(pControl);
    }

    BOOL CDuiContainer::AddAt(CDuiControl* pControl, int iIndex)
    {
        if(pControl == NULL)
        {
            return false;
        }
        if(GetManager() != NULL)
        {
            GetManager()->InitControls(pControl, this);
        }
        if(IsVisible())
        {
            NeedUpdate();
        }
        else
        {
            pControl->SetInternVisible(false);
        }
        return m_items.InsertAt(iIndex, pControl);
    }

    BOOL CDuiContainer::Remove(CDuiControl* pControl)
    {
        if(pControl == NULL)
        {
            return false;
        }
        for(int it = 0; it < m_items.GetSize(); it++)
        {
            if(static_cast<CDuiControl*>(m_items[it]) == pControl)
            {
                NeedUpdate();
                if(m_bAutoDestroy)
                {
                    if(m_bDelayedDestroy && GetManager())
                    {
                        GetManager()->AddDelayedCleanup(pControl);
                    }
                    else
                    {
                        DUI_FREE_POINT(pControl);
                    }
                }
                return m_items.Remove(it);
            }
        }
        return false;
    }

    BOOL CDuiContainer::RemoveAt(int iIndex)
    {
        CDuiControl* pControl = GetItemAt(iIndex);
        if(pControl != NULL)
        {
            return Remove(pControl);
        }
        return FALSE;
    }

    void CDuiContainer::RemoveAll()
    {
        for(int it = 0; m_bAutoDestroy && it < m_items.GetSize(); it++)
        {
            CDuiControl* pItem = static_cast<CDuiControl*>(m_items[it]);
            if(m_bDelayedDestroy && GetManager())
            {
                GetManager()->AddDelayedCleanup(pItem);
            }
            else
            {
                DUI_FREE_POINT(pItem);
            }
        }
        m_items.Empty();
        NeedUpdate();
    }

    void CDuiContainer::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        __super::SetPos(rc, bNeedInvalidate);
        if(m_items.IsEmpty())
        {
            return;
        }
        rc = GetPos();
        rc.left += m_rcInset.left;
        rc.top += m_rcInset.top;
        rc.right -= m_rcInset.right;
        rc.bottom -= m_rcInset.bottom;
        if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
        {
            rc.left -= m_pHorizontalScrollBar->GetScrollPos();
            rc.right -= m_pHorizontalScrollBar->GetScrollPos();
            rc.right += m_pHorizontalScrollBar->GetScrollRange();
            rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
        }
        if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
        {
            rc.left -= m_pHorizontalScrollBar->GetScrollPos();
            rc.right -= m_pHorizontalScrollBar->GetScrollPos();
            rc.right += m_pHorizontalScrollBar->GetScrollRange();
            rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
        }
        for(int i = 0; i < m_items.GetSize(); i++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(m_items[i]);
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                SetFloatPos(i);
            }
            else
            {
                SIZE sz = { rc.right - rc.left, rc.bottom - rc.top };
                if(sz.cx < pControl->GetMinWidth())
                {
                    sz.cx = pControl->GetMinWidth();
                }
                if(sz.cx > pControl->GetMaxWidth())
                {
                    sz.cx = pControl->GetMaxWidth();
                }
                if(sz.cy < pControl->GetMinHeight())
                {
                    sz.cy = pControl->GetMinHeight();
                }
                if(sz.cy > pControl->GetMaxHeight())
                {
                    sz.cy = pControl->GetMaxHeight();
                }
                RECT rcCtrl = { rc.left, rc.top, rc.left + sz.cx, rc.top + sz.cy };
                pControl->SetPos(rcCtrl, FALSE);
            }
        }
    }

    void CDuiContainer::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        RECT rcTemp = { 0 };
        if(!::IntersectRect(&rcTemp, &rcPaint, &GetPos()))
        {
            return;
        }
        CRenderClip clip;
        CRenderClip::GenerateClip(hDC, rcTemp, clip);
        __super::DoPaint(hDC, rcPaint);
        if(m_items.GetSize() > 0)
        {
            RECT rcInset = GetInset();
            RECT rc = GetPos();
            rc.left += rcInset.left;
            rc.top += rcInset.top;
            rc.right -= rcInset.right;
            rc.bottom -= rcInset.bottom;
            if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
            {
                rc.right -= m_pVerticalScrollBar->GetFixedWidth();
            }
            if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
            {
                rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
            }
            if(!::IntersectRect(&rcTemp, &rcPaint, &rc))
            {
                for(int it = 0; it < m_items.GetSize(); it++)
                {
                    CDuiControl* pControl = static_cast<CDuiControl*>(m_items[it]);
                    if(!pControl->IsVisible())
                    {
                        continue;
                    }
                    if(!::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()))
                    {
                        continue;
                    }
                    if(pControl ->IsFloat())
                    {
                        if(!::IntersectRect(&rcTemp, &GetPos(), &pControl->GetPos()))
                        {
                            continue;
                        }
                        pControl->DoPaint(hDC, rcPaint);
                    }
                }
            }
            else
            {
                CRenderClip childClip;
                CRenderClip::GenerateClip(hDC, rcTemp, childClip);
                for(int it = 0; it < m_items.GetSize(); it++)
                {
                    CDuiControl* pControl = static_cast<CDuiControl*>(m_items[it]);
                    if(!pControl->IsVisible())
                    {
                        continue;
                    }
                    if(!::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()))
                    {
                        continue;
                    }
                    if(pControl ->IsFloat())
                    {
                        if(!::IntersectRect(&rcTemp, &GetPos(), &pControl->GetPos()))
                        {
                            continue;
                        }
                        CRenderClip::UseOldClipBegin(hDC, childClip);
                        pControl->DoPaint(hDC, rcPaint);
                        CRenderClip::UseOldClipEnd(hDC, childClip);
                    }
                    else
                    {
                        if(!::IntersectRect(&rcTemp, &rc, &pControl->GetPos()))
                        {
                            continue;
                        }
                        pControl->DoPaint(hDC, rcPaint);
                    }
                }
            }
        }

        if(m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible())
        {
            if(::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()))
            {
                m_pVerticalScrollBar->DoPaint(hDC, rcPaint);
            }
        }
        if(m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible())
        {
            if(::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos()))
            {
                m_pHorizontalScrollBar->DoPaint(hDC, rcPaint);
            }
        }
    }

    void CDuiContainer::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(GetParent() != NULL)
            {
                GetParent()->DoEvent(event);
            }
            else
            {
                CDuiControl::DoEvent(event);
            }
            return;
        }
        if(event.Type == UIEVENT_SETFOCUS)
        {
            SetFocused(TRUE);
            return;
        }
        else if(event.Type == UIEVENT_KILLFOCUS)
        {
            SetFocused(FALSE);
            return;
        }
        else if(event.Type == UIEVENT_KEYDOWN)
        {
            if(m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() && m_pVerticalScrollBar->IsEnabled())
            {
                switch(event.chKey)
                {
                    case VK_DOWN:
                        LineDown();
                        return;
                    case VK_UP:
                        LineUp();
                        return;
                    case VK_NEXT:
                        PageDown();
                        return;
                    case VK_PRIOR:
                        PageUp();
                        return;
                    case VK_HOME:
                        HomeUp();
                        return;
                    case VK_END:
                        EndDown();
                        return;
                }
            }
            if(m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() && m_pHorizontalScrollBar->IsEnabled())
            {
                switch(event.chKey)
                {
                    case VK_RIGHT:
                        LineRight();
                        return;
                    case VK_LEFT:
                        LineLeft();
                        return;
                    case VK_NEXT:
                        PageRight();
                        return;
                    case VK_PRIOR:
                        PageLeft();
                        return;
                    case VK_HOME:
                        HomeLeft();
                        return;
                    case VK_END:
                        EndRight();
                        return;
                }
            }
        }
        else if(event.Type == UIEVENT_SCROLLWHEEL)
        {
            if(m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() && m_pVerticalScrollBar->IsEnabled())
            {
                switch(LOWORD(event.wParam))
                {
                    case SB_LINEUP:
                        LineUp();
                        return;
                    case SB_LINEDOWN:
                        LineDown();
                        return;
                }
            }
            if(m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() && m_pHorizontalScrollBar->IsEnabled())
            {
                switch(LOWORD(event.wParam))
                {
                    case SB_LINEUP:
                        LineLeft();
                        return;
                    case SB_LINEDOWN:
                        LineRight();
                        return;
                }
            }
        }
        CDuiControl::DoEvent(event);
    }

    void CDuiContainer::EnableScrollBar(BOOL bEnableVertical /*= TRUE*/, bool bEnableHorizontal /*= FALSE*/)
    {
        if(bEnableVertical && !m_pVerticalScrollBar)
        {
            m_pVerticalScrollBar = new CDuiScrollBar;
            m_pVerticalScrollBar->SetOwner(this);
            m_pVerticalScrollBar->SetManager(GetManager(), NULL, FALSE);
            if(GetManager())
            {
                LPCTSTR pDefaultAttributes = GetManager()->GetDefaultAttributeList(_T("VScrollBar"));
                if(pDefaultAttributes)
                {
                    m_pVerticalScrollBar->ApplyAttributeList(pDefaultAttributes);
                }
            }
        }
        else if(!bEnableVertical && m_pVerticalScrollBar)
        {
            DUI_FREE_POINT(m_pVerticalScrollBar);
        }
        if(bEnableHorizontal && !m_pHorizontalScrollBar)
        {
            m_pHorizontalScrollBar = new CDuiScrollBar;
            m_pHorizontalScrollBar->SetHorizontal(TRUE);
            m_pHorizontalScrollBar->SetOwner(this);
            m_pHorizontalScrollBar->SetManager(GetManager(), NULL, FALSE);
            if(GetManager())
            {
                LPCTSTR pDefaultAttributes = GetManager()->GetDefaultAttributeList(_T("HScrollBar"));
                if(pDefaultAttributes)
                {
                    m_pHorizontalScrollBar->ApplyAttributeList(pDefaultAttributes);
                }
            }
        }
        else if(!bEnableHorizontal && m_pHorizontalScrollBar)
        {
            DUI_FREE_POINT(m_pHorizontalScrollBar);
        }

        NeedUpdate();
    }

    void CDuiContainer::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("inset")) == 0)
        {
            RECT rcInset = { 0 };
            LPTSTR pstr = NULL;
            rcInset.left = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            rcInset.top = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcInset.right = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcInset.bottom = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetInset(rcInset);
        }
        else if(_tcsicmp(pstrName, _T("mousechild")) == 0)
        {
            SetMouseChildEnabled(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("vscrollbar")) == 0)
        {
            EnableScrollBar(_tcsicmp(pstrValue, _T("true")) == 0, GetHorizontalScrollBar() != NULL);
        }
        else if(_tcsicmp(pstrName, _T("vscrollbarstyle")) == 0)
        {
            m_sVerticalScrollBarStyle = pstrValue;
            EnableScrollBar(TRUE, GetHorizontalScrollBar() != NULL);
            if(GetVerticalScrollBar())
            {
                LPCTSTR pStyle = GetManager()->GetStyle(m_sVerticalScrollBarStyle);
                if(pStyle)
                {
                    GetVerticalScrollBar()->ApplyAttributeList(pStyle);
                }
                else
                {
                    GetVerticalScrollBar()->ApplyAttributeList(pstrValue);
                }
            }
        }
        else if(_tcsicmp(pstrName, _T("hscrollbar")) == 0)
        {
            EnableScrollBar(GetVerticalScrollBar() != NULL, _tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("hscrollbarstyle")) == 0)
        {
            m_sHorizontalScrollBarStyle = pstrValue;
            EnableScrollBar(TRUE, GetHorizontalScrollBar() != NULL);
            if(GetHorizontalScrollBar())
            {
                LPCTSTR pStyle = GetManager()->GetStyle(m_sHorizontalScrollBarStyle);
                if(pStyle)
                {
                    GetHorizontalScrollBar()->ApplyAttributeList(pStyle);
                }
                else
                {
                    GetHorizontalScrollBar()->ApplyAttributeList(pstrValue);
                }
            }
        }
        else if(_tcsicmp(pstrName, _T("childpadding")) == 0)
        {
            SetChildPadding(_ttoi(pstrValue));
        }
        else if(_tcscmp(pstrName, _T("childalign")) == 0)
        {
            if(_tcscmp(pstrValue, _T("left")) == 0)
            {
                m_iChildAlign = DT_LEFT;
            }
            else if(_tcscmp(pstrValue, _T("center")) == 0)
            {
                m_iChildAlign = DT_CENTER;
            }
            else if(_tcscmp(pstrValue, _T("right")) == 0)
            {
                m_iChildAlign = DT_RIGHT;
            }
        }
        else if(_tcscmp(pstrName, _T("childvalign")) == 0)
        {
            if(_tcscmp(pstrValue, _T("top")) == 0)
            {
                m_iChildVAlign = DT_TOP;
            }
            else if(_tcscmp(pstrValue, _T("vcenter")) == 0)
            {
                m_iChildVAlign = DT_VCENTER;
            }
            else if(_tcscmp(pstrValue, _T("bottom")) == 0)
            {
                m_iChildVAlign = DT_BOTTOM;
            }
        }
        else if(_tcsicmp(pstrName, _T("scrollstepsize")) == 0)
        {
            SetScrollStepSize(_ttoi(pstrValue));
        }
        else
        {
            CDuiControl::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiContainer::SetManager(CDuiPaintManager* pManager, CDuiControl* pParent, BOOL bInit /*= TRUE*/)
    {
        for(int i = 0; i < m_items.GetSize(); i++)
        {
            static_cast<CDuiControl*>(m_items[i])->SetManager(pManager, this, bInit);
        }

        if(m_pVerticalScrollBar != NULL)
        {
            m_pVerticalScrollBar->SetManager(pManager, this, bInit);
        }
        if(m_pHorizontalScrollBar != NULL)
        {
            m_pHorizontalScrollBar->SetManager(pManager, this, bInit);
        }
        CDuiControl::SetManager(pManager, pParent, bInit);
    }

    CDuiControl* CDuiContainer::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
    {
        if((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible())
        {
            return NULL;
        }
        if((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled())
        {
            return NULL;
        }
        if((uFlags & UIFIND_HITTEST) != 0 && !::PtInRect(&GetPos(), *(static_cast<LPPOINT>(pData))))
        {
            return NULL;
        }
        if((uFlags & UIFIND_UPDATETEST) != 0 && Proc(this, pData) != NULL)
        {
            return NULL;
        }
        CDuiControl* pResult = NULL;
        if((uFlags & UIFIND_ME_FIRST) != 0)
        {
            if((uFlags & UIFIND_HITTEST) == 0 || IsMouseEnabled())
            {
                pResult = Proc(this, pData);
            }
        }
        if(pResult == NULL && m_pVerticalScrollBar != NULL)
        {
            if((uFlags & UIFIND_HITTEST) == 0 || IsMouseEnabled())
            {
                pResult = m_pVerticalScrollBar->FindControl(Proc, pData, uFlags);
            }
        }
        if(pResult == NULL && m_pHorizontalScrollBar != NULL)
        {
            if((uFlags & UIFIND_HITTEST) == 0 || IsMouseEnabled())
            {
                pResult = m_pHorizontalScrollBar->FindControl(Proc, pData, uFlags);
            }
        }
        if(pResult != NULL)
        {
            return pResult;
        }
        if((uFlags & UIFIND_HITTEST) == 0 || IsMouseChildEnabled())
        {
            RECT rc = GetPos();
            rc.left += m_rcInset.left;
            rc.top += m_rcInset.top;
            rc.right -= m_rcInset.right;
            rc.bottom -= m_rcInset.bottom;
            if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
            {
                rc.right -= m_pVerticalScrollBar->GetFixedWidth();
            }
            if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
            {
                rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
            }
            if((uFlags & UIFIND_TOP_FIRST) != 0)
            {
                for(int it = m_items.GetSize() - 1; it >= 0; it--)
                {
                    pResult = static_cast<CDuiControl*>(m_items[it])->FindControl(Proc, pData, uFlags);
                    if(pResult != NULL)
                    {
                        if((uFlags & UIFIND_HITTEST) != 0 && !pResult->IsFloat() && !::PtInRect(&rc, *(static_cast<LPPOINT>(pData))))
                        {
                            continue;
                        }
                        else
                        {
                            return pResult;
                        }
                    }
                }
            }
            else
            {
                for(int it = 0; it < m_items.GetSize(); it++)
                {
                    pResult = static_cast<CDuiControl*>(m_items[it])->FindControl(Proc, pData, uFlags);
                    if(pResult != NULL)
                    {
                        if((uFlags & UIFIND_HITTEST) != 0 && !pResult->IsFloat() && !::PtInRect(&rc, *(static_cast<LPPOINT>(pData))))
                        {
                            continue;
                        }
                        else
                        {
                            return pResult;
                        }
                    }
                }
            }
        }
        pResult = NULL;
        if(pResult == NULL && (uFlags & UIFIND_ME_FIRST) == 0)
        {
            if((uFlags & UIFIND_HITTEST) == 0 || IsMouseEnabled())
            {
                pResult = Proc(this, pData);
            }
        }
        return pResult;
    }

    void CDuiContainer::SetFloatPos(int iIndex)
    {
        // 因为CControlUI::SetPos对float的操作影响，这里不能对float组件添加滚动条的影响
        if(iIndex < 0 || iIndex >= m_items.GetSize())
        {
            return;
        }
        CDuiControl* pControl = static_cast<CDuiControl*>(m_items[iIndex]);
        if(!pControl->IsVisible() || !pControl->IsFloat())
        {
            return;
        }
        SIZE szXY = pControl->GetFixedXY();
        SIZE sz = {pControl->GetFixedWidth(), pControl->GetFixedHeight()};
        TPercentInfo rcPercent = pControl->GetFloatPercent();
        LONG width = GetPos().right - GetPos().left;
        LONG height = GetPos().bottom - GetPos().top;
        RECT rcCtrl = { 0 };
        rcCtrl.left = (LONG)(width * rcPercent.left) + szXY.cx + GetPos().left;
        rcCtrl.top = (LONG)(height * rcPercent.top) + szXY.cy + GetPos().top;
        rcCtrl.right = (LONG)(width * rcPercent.right) + szXY.cx + sz.cx + GetPos().left;
        rcCtrl.bottom = (LONG)(height * rcPercent.bottom) + szXY.cy + sz.cy + GetPos().top;
        pControl->SetPos(rcCtrl, false);
    }

    void CDuiContainer::ProcessScrollBar(RECT rc, int cxRequired, int cyRequired)
    {
        // by 冰下海 2015/08/16
        while(m_pHorizontalScrollBar)
        {
            // Scroll needed
            if(cxRequired > rc.right - rc.left && !m_pHorizontalScrollBar->IsVisible())
            {
                m_pHorizontalScrollBar->SetVisible(TRUE);
                m_pHorizontalScrollBar->SetScrollRange(cxRequired - (rc.right - rc.left));
                m_pHorizontalScrollBar->SetScrollPos(0);
                SetPos(GetPos());
                break;
            }
            // No scrollbar required
            if(!m_pHorizontalScrollBar->IsVisible())
            {
                break;
            }
            // Scroll not needed anymore?
            int cxScroll = cxRequired - (rc.right - rc.left);
            if(cxScroll <= 0)
            {
                m_pHorizontalScrollBar->SetVisible(FALSE);
                m_pHorizontalScrollBar->SetScrollPos(0);
                m_pHorizontalScrollBar->SetScrollRange(0);
                SetPos(GetPos());
            }
            else
            {
                RECT rcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight() };
                m_pHorizontalScrollBar->SetPos(rcScrollBarPos);
                if(m_pHorizontalScrollBar->GetScrollRange() != cxScroll)
                {
                    int iScrollPos = m_pHorizontalScrollBar->GetScrollPos();
                    m_pHorizontalScrollBar->SetScrollRange(::abs(cxScroll)); // if scrollpos>range then scrollpos=range
                    if(iScrollPos > m_pHorizontalScrollBar->GetScrollPos())
                    {
                        SetPos(GetPos());
                    }
                }
            }
            break;
        }
        while(m_pVerticalScrollBar)
        {
            // Scroll needed
            if(cyRequired > rc.bottom - rc.top && !m_pVerticalScrollBar->IsVisible())
            {
                m_pVerticalScrollBar->SetVisible(TRUE);
                m_pVerticalScrollBar->SetScrollRange(cyRequired - (rc.bottom - rc.top));
                m_pVerticalScrollBar->SetScrollPos(0);
                SetPos(GetPos());
                break;
            }
            // No scrollbar required
            if(!m_pVerticalScrollBar->IsVisible())
            {
                break;
            }
            // Scroll not needed anymore?
            int cyScroll = cyRequired - (rc.bottom - rc.top);
            if(cyScroll <= 0)
            {
                m_pVerticalScrollBar->SetVisible(FALSE);
                m_pVerticalScrollBar->SetScrollPos(0);
                m_pVerticalScrollBar->SetScrollRange(0);
                SetPos(GetPos());
                break;
            }
            RECT rcScrollBarPos = { rc.right, rc.top, rc.right + m_pVerticalScrollBar->GetFixedWidth(), rc.bottom };
            m_pVerticalScrollBar->SetPos(rcScrollBarPos);
            if(m_pVerticalScrollBar->GetScrollRange() != cyScroll)
            {
                int iScrollPos = m_pVerticalScrollBar->GetScrollPos();
                m_pVerticalScrollBar->SetScrollRange(::abs(cyScroll)); // if scrollpos>range then scrollpos=range
                if(iScrollPos > m_pVerticalScrollBar->GetScrollPos())
                {
                    SetPos(GetPos());
                }
            }
            break;
        }
    }

}