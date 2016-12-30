#include "StdAfx.h"
#include "DuiCombo.h"

namespace DuiLib
{

    IMPLEMENT_DUICONTROL(CDuiListElement)

    CDuiListElement::CDuiListElement(void)
        : m_bSelected(FALSE)
        , m_iIndex(-1)
        , m_pOwner(NULL)
    {

    }

    CDuiListElement::~CDuiListElement(void)
    {

    }

    CDuiString CDuiListElement::GetClass() const
    {
        return DUI_CTR_LISTELEMENT;
    }

    LPVOID CDuiListElement::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, GET_CLASS_NAME(IListItem)) == 0)
        {
            return static_cast<IListItem*>(this);
        }
        if(_tcsicmp(pstrName, DUI_CTR_LISTELEMENT) == 0)
        {
            return static_cast<CDuiListElement*>(this);
        }
        return CDuiControl::GetInterface(pstrName);
    }

    UINT CDuiListElement::GetControlFlags() const
    {
        return UIFLAG_WANTRETURN;
    }

    void CDuiListElement::Invalidate()
    {
        if(!IsVisible())
        {
            return;
        }

        if(GetParent())
        {
            CDuiContainer* pParentContainer = static_cast<CDuiContainer*>(GetParent()->GetInterface(DUI_CTR_CONTAINER));
            if(pParentContainer)
            {
                RECT rc = pParentContainer->GetPos();
                RECT rcInset = pParentContainer->GetInset();
                rc.left += rcInset.left;
                rc.top += rcInset.top;
                rc.right -= rcInset.right;
                rc.bottom -= rcInset.bottom;
                CDuiScrollBar* pVerticalScrollBar = pParentContainer->GetVerticalScrollBar();
                if(pVerticalScrollBar && pVerticalScrollBar->IsVisible())
                {
                    rc.right -= pVerticalScrollBar->GetFixedWidth();
                }
                CDuiScrollBar* pHorizontalScrollBar = pParentContainer->GetHorizontalScrollBar();
                if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
                {
                    rc.bottom -= pHorizontalScrollBar->GetFixedHeight();
                }

                RECT invalidateRc = GetPos();
                if(!::IntersectRect(&invalidateRc, &GetPos(), &rc))
                {
                    return;
                }

                CDuiControl* pParent = GetParent();
                RECT rcTemp = {0};
                RECT rcParent = {0};
                while(pParent = pParent->GetParent())
                {
                    rcTemp = invalidateRc;
                    rcParent = pParent->GetPos();
                    if(!::IntersectRect(&invalidateRc, &rcTemp, &rcParent))
                    {
                        return;
                    }
                }

                if(GetManager() != NULL)
                {
                    GetManager()->Invalidate(invalidateRc);
                }
            }
            else
            {
                CDuiControl::Invalidate();
            }
        }
        else
        {
            CDuiControl::Invalidate();
        }
    }

    BOOL CDuiListElement::Activate()
    {
        if(!CDuiControl::Activate())
        {
            return FALSE;
        }
        if(GetManager() != NULL)
        {
            GetManager()->SendNotify(this, DUI_MSGTYPE_ITEMACTIVATE);
        }
        return TRUE;
    }

    int CDuiListElement::GetIndex() const
    {
        return m_iIndex;
    }

    void CDuiListElement::SetIndex(int iIndex)
    {
        m_iIndex = iIndex;
    }

    IListOwner* CDuiListElement::GetOwner()
    {
        return m_pOwner;
    }

    void CDuiListElement::SetOwner(CDuiControl* pOwner)
    {
        m_pOwner = static_cast<IListOwner*>(pOwner->GetInterface(GET_CLASS_NAME(IListOwner)));
    }

    BOOL CDuiListElement::IsSelected() const
    {
        return m_bSelected;
    }

    BOOL CDuiListElement::Select(BOOL bSelect /*= TRUE*/)
    {
        if(!IsEnabled())
        {
            return FALSE;
        }
        if(m_pOwner != NULL && m_bSelected)
        {
            m_pOwner->UnSelectItem(m_iIndex, TRUE);
        }
        if(bSelect == m_bSelected)
        {
            return TRUE;
        }
        m_bSelected = bSelect;
        if(bSelect && m_pOwner != NULL)
        {
            m_pOwner->SelectItem(m_iIndex);
        }
        Invalidate();

        return TRUE;
    }

    BOOL CDuiListElement::SelectMulti(BOOL bSelect /*= TRUE*/)
    {
        if(!IsEnabled())
        {
            return FALSE;
        }
        if(bSelect == m_bSelected)
        {
            return TRUE;
        }

        m_bSelected = bSelect;
        if(bSelect && m_pOwner != NULL)
        {
            m_pOwner->SelectMultiItem(m_iIndex);
        }
        Invalidate();
        return TRUE;
    }

    BOOL CDuiListElement::IsExpanded() const
    {
        return FALSE;
    }

    BOOL CDuiListElement::Expand(BOOL bExpand /*= TRUE*/)
    {
        return FALSE;
    }

    void CDuiListElement::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("selected")) == 0)
        {
            Select();
        }
        else
        {
            CDuiControl::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiListElement::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(m_pOwner != NULL)
            {
                m_pOwner->DoEvent(event);
            }
            else
            {
                CDuiControl::DoEvent(event);
            }
            return;
        }

        if(event.Type == UIEVENT_DBLCLICK)
        {
            if(IsEnabled())
            {
                Activate();
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_KEYDOWN && IsEnabled())
        {
            if(event.chKey == VK_RETURN)
            {
                Activate();
                Invalidate();
                return;
            }
        }
        // An important twist: The list-item will send the event not to its immediate
        // parent but to the "attached" list. A list may actually embed several components
        // in its path to the item, but key-presses etc. needs to go to the actual list.
        if(m_pOwner != NULL)
        {
            m_pOwner->DoEvent(event);
        }
        else
        {
            CDuiControl::DoEvent(event);
        }
    }

    ////////////////////////////////////////////////////


    IMPLEMENT_DUICONTROL(CDuiListLabelElement)

    CDuiListLabelElement::CDuiListLabelElement(void)
        : m_uButtonState(0)
    {

    }

    CDuiListLabelElement::~CDuiListLabelElement(void)
    {

    }

    CDuiString CDuiListLabelElement::GetClass() const
    {
        return DUI_CTR_LISTLABELELEMENT;
    }

    LPVOID CDuiListLabelElement::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_LISTLABELELEMENT) == 0)
        {
            return static_cast<CDuiListLabelElement*>(this);
        }
        return CDuiListElement::GetInterface(pstrName);
    }

    UINT CDuiListLabelElement::GetButtonState()
    {
        return m_uButtonState;
    }

    SIZE CDuiListLabelElement::EstimateSize(SIZE szAvailable)
    {
        if(GetOwner() == NULL)
        {
            return CDuiSize(0, 0);
        }
        CDuiString sText = GetText();

        ListInfo* pInfo = GetOwner()->GetListInfo();
        SIZE cXY = CDuiControl::EstimateSize(szAvailable);
        //按原比例缩小，因为DPI变化后，会产生空白区域，所以还是原来高度
        cXY.cy = MulDiv(cXY.cy + pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom, 100, GetManager()->GetDPIObj()->GetScale());
        if(cXY.cy == 0 && GetManager() != NULL)
        {
            cXY.cy = GetManager()->GetFontInfo(pInfo->nFont)->tm.tmHeight + 8;
            cXY.cy += pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
        }

        if(cXY.cx == 0 && GetManager() != NULL)
        {
            RECT rcText = { 0, 0, 9999, cXY.cy };
            if(pInfo->bShowHtml)
            {
                int nLinks = 0;
                CRenderEngine::DrawHtmlText(GetManager()->GetPaintDC(), GetManager(), rcText, sText, 0, NULL, NULL, nLinks, DT_SINGLELINE | DT_CALCRECT | pInfo->uTextStyle & ~DT_RIGHT & ~DT_CENTER);
            }
            else
            {
                CRenderEngine::DrawText(GetManager()->GetPaintDC(), GetManager(), rcText, sText, 0, pInfo->nFont, DT_SINGLELINE | DT_CALCRECT | pInfo->uTextStyle & ~DT_RIGHT & ~DT_CENTER);
            }
            cXY.cx = rcText.right - rcText.left + pInfo->rcTextPadding.left + pInfo->rcTextPadding.right;
        }

        return cXY;
    }

    void CDuiListLabelElement::DrawItemBk(HDC hDC, const RECT& rcItem)
    {
        ASSERT(GetOwner());
        if(GetOwner() == NULL)
        {
            return;
        }
        ListInfo* pInfo = GetOwner()->GetListInfo();
        DWORD iBackColor = 0;
        if(!pInfo->bAlternateBk || GetIndex() % 2 == 0)
        {
            iBackColor = pInfo->dwBkColor;
        }
        if((m_uButtonState & UISTATE_HOT) != 0)
        {
            iBackColor = pInfo->dwHotBkColor;
        }
        if(IsSelected())
        {
            iBackColor = pInfo->dwSelectedBkColor;
        }
        if(!IsEnabled())
        {
            iBackColor = pInfo->dwDisabledBkColor;
        }

        if(iBackColor != 0)
        {
            CRenderEngine::DrawColor(hDC, GetPos(), GetAdjustColor(iBackColor));
        }

        if(!IsEnabled())
        {
            if(!pInfo->sDisabledImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)pInfo->sDisabledImage)) {}
                else
                {
                    return;
                }
            }
        }
        if(IsSelected())
        {
            if(!pInfo->sSelectedImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)pInfo->sSelectedImage)) {}
                else
                {
                    return;
                }
            }
        }
        if((m_uButtonState & UISTATE_HOT) != 0)
        {
            if(!pInfo->sHotImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)pInfo->sHotImage)) {}
                else
                {
                    return;
                }
            }
        }

        if(!GetBkImage().IsEmpty())
        {
            if(!pInfo->bAlternateBk || GetIndex() % 2 == 0)
            {
                if(!DrawImage(hDC, GetBkImage().GetData()))
                {

                }
                else
                {
                    return;
                }
            }
        }

        if(GetBkImage().IsEmpty())
        {
            if(!pInfo->sBkImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)pInfo->sBkImage)) {}
                else
                {
                    return;
                }
            }
        }

        if(pInfo->dwLineColor != 0)
        {
            if(pInfo->bShowRowLine)
            {
                RECT rcLine = { GetPos().left, GetPos().bottom - 1, GetPos().right, GetPos().bottom - 1 };
                CRenderEngine::DrawLine(hDC, rcLine, 1, GetAdjustColor(pInfo->dwLineColor));
            }
            if(pInfo->bShowColumnLine)
            {
                for(int i = 0; i < pInfo->nColumns; i++)
                {
                    RECT rcLine = { pInfo->rcColumn[i].right - 1, GetPos().top, pInfo->rcColumn[i].right - 1, GetPos().bottom };
                    CRenderEngine::DrawLine(hDC, rcLine, 1, GetAdjustColor(pInfo->dwLineColor));
                }
            }
        }
    }

    void CDuiListLabelElement::DrawItemText(HDC hDC, const RECT& rcItem)
    {
        CDuiString sText = GetText();
        if(sText.IsEmpty())
        {
            return;
        }

        if(GetOwner() == NULL)
        {
            return;
        }
        ListInfo* pInfo = GetOwner()->GetListInfo();
        DWORD iTextColor = pInfo->dwTextColor;
        if((m_uButtonState & UISTATE_HOT) != 0)
        {
            iTextColor = pInfo->dwHotTextColor;
        }
        if(IsSelected())
        {
            iTextColor = pInfo->dwSelectedTextColor;
        }
        if(!IsEnabled())
        {
            iTextColor = pInfo->dwDisabledTextColor;
        }
        int nLinks = 0;
        RECT rcText = rcItem;
        rcText.left += pInfo->rcTextPadding.left;
        rcText.right -= pInfo->rcTextPadding.right;
        rcText.top += pInfo->rcTextPadding.top;
        rcText.bottom -= pInfo->rcTextPadding.bottom;

        if(pInfo->bShowHtml)
            CRenderEngine::DrawHtmlText(hDC, GetManager(), rcText, sText, iTextColor, \
                                        NULL, NULL, nLinks, pInfo->uTextStyle);
        else
            CRenderEngine::DrawText(hDC, GetManager(), rcText, sText, iTextColor, \
                                    pInfo->nFont, pInfo->uTextStyle);
    }

    void CDuiListLabelElement::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        if(!::IntersectRect(&GetPaintRect(), &rcPaint, &GetPos()))
        {
            return;
        }
        CDuiControl::DoPaint(hDC, rcPaint);
        DrawItemBk(hDC, GetPos());
        DrawItemText(hDC, GetPos());

    }

    void CDuiListLabelElement::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(GetOwner() != NULL)
            {
                GetOwner()->DoEvent(event);
            }
            else
            {
                CDuiListElement::DoEvent(event);
            }
            return;
        }

        if(event.Type == UIEVENT_BUTTONDOWN)
        {
            if(IsEnabled())
            {
                if((GetKeyState(VK_CONTROL) & 0x8000))
                {
                    SelectMulti(!IsSelected());
                }
                else
                {
                    Select();
                }
            }
            return;
        }
        if(event.Type == UIEVENT_BUTTONUP)
        {
            if(IsEnabled() && GetManager())
            {
                GetManager()->SendNotify(this, DUI_MSGTYPE_ITEMCLICK);
            }
            return;
        }

        if(event.Type == UIEVENT_MOUSEMOVE)
        {
            return;
        }

        if(event.Type == UIEVENT_MOUSEENTER)
        {
            if(IsEnabled())
            {
                m_uButtonState |= UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_MOUSELEAVE)
        {
            if((m_uButtonState & UISTATE_HOT) != 0)
            {
                m_uButtonState &= ~UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        CDuiListElement::DoEvent(event);
    }

    ////////////////////////////////////////////////
    CDuiComboWnd::CDuiComboWnd(void)
        : m_bHitItem(FALSE)
        , m_iOldSel(-1)
        ,  m_pOwner(NULL)
        ,  m_pLayout(NULL)
    {

    }

    CDuiComboWnd::~CDuiComboWnd(void)
    {

    }

    void CDuiComboWnd::Init(CDuiCombo* pOwner)
    {
        m_bHitItem = FALSE;
        m_pOwner = pOwner;
        m_pLayout = NULL;
        m_iOldSel = m_pOwner->GetCurSel();

        // Position the popup window in absolute space
        SIZE szDrop = m_pOwner->GetDropBoxSize();
        RECT rcOwner = pOwner->GetPos();
        RECT rc = rcOwner;
        rc.top = rc.bottom;		// 父窗口left、bottom位置作为弹出窗口起点
        rc.bottom = rc.top + szDrop.cy;	// 计算弹出窗口高度
        if(szDrop.cx > 0)
        {
            rc.right = rc.left + szDrop.cx;    // 计算弹出窗口宽度
        }

        SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
        int cyFixed = 0;
        for(int it = 0; it < pOwner->GetCount(); it++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(pOwner->GetItemAt(it));
            if(!pControl->IsVisible())
            {
                continue;
            }
            SIZE sz = pControl->EstimateSize(szAvailable);
            cyFixed += sz.cy;
        }
        cyFixed += 4; // CVerticalLayoutUI 默认的Inset 调整
        rc.bottom = rc.top + MIN(cyFixed, szDrop.cy);

        ::MapWindowRect(pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);

        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
        if(rc.bottom > rcWork.bottom)
        {
            rc.left = rcOwner.left;
            rc.right = rcOwner.right;
            if(szDrop.cx > 0)
            {
                rc.right = rc.left + szDrop.cx;
            }
            rc.top = rcOwner.top - MIN(cyFixed, szDrop.cy);
            rc.bottom = rcOwner.top;
            ::MapWindowRect(pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
        }

        HWND hWndParent = Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW, rc);
        // HACK: Don't deselect the parent's caption
        while(::GetParent(hWndParent) != NULL)
        {
            hWndParent = ::GetParent(hWndParent);
        }
        ::ShowWindow(GetHWND(), SW_SHOW);
        ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
    }

    LPCTSTR CDuiComboWnd::GetWindowClassName() const
    {
        return GET_CLASS_NAME(CDuiComboWnd);
    }

    void CDuiComboWnd::OnFinalMessage(HWND hWnd)
    {
        m_pOwner->EmptyComboWnd();
        UINT uButtonState = m_pOwner->GetButtonState();
        uButtonState &= ~ UISTATE_PUSHED;
        m_pOwner->SetButtonState(uButtonState);
        m_pOwner->Invalidate();
        CDuiWnd::OnFinalMessage(hWnd);
    }

#if(_WIN32_WINNT >= 0x0501)
    UINT CDuiComboWnd::GetClassStyle() const
    {
        return CDuiWnd::GetClassStyle() | CS_DROPSHADOW;
    }
#endif

    BOOL CDuiComboWnd::IsHitItem(POINT ptMouse)
    {
        CDuiControl* pControl = m_PaintManager.FindControl(ptMouse);
        if(pControl != NULL)
        {
            LPVOID pInterface = pControl->GetInterface(DUI_CTR_SCROLLBAR);
            if(pInterface)
            {
                return FALSE;
            }

            while(pControl != NULL)
            {
                IListItem* pListItem = (IListItem*)pControl->GetInterface(GET_CLASS_NAME(IListItem));
                if(pListItem != NULL)
                {
                    return TRUE;
                }
                pControl = pControl->GetParent();
            }
        }

        return FALSE;
    }

    void CDuiComboWnd::Scroll(int dx, int dy)
    {
        if(dx == 0 && dy == 0)
        {
            return;
        }
        SIZE sz = m_pLayout->GetScrollPos();
        m_pLayout->SetScrollPos(CDuiSize(sz.cx + dx, sz.cy + dy));
    }

    void CDuiComboWnd::EnsureVisible(int iIndex)
    {
        if(m_pOwner->GetCurSel() < 0)
        {
            return;
        }
        m_pLayout->FindSelectable(m_pOwner->GetCurSel(), FALSE);
        RECT rcItem = m_pLayout->GetItemAt(iIndex)->GetPos();
        RECT rcList = m_pLayout->GetPos();
        CDuiScrollBar* pHorizontalScrollBar = m_pLayout->GetHorizontalScrollBar();
        if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
        {
            rcList.bottom -= pHorizontalScrollBar->GetFixedHeight();
        }
        int iPos = m_pLayout->GetScrollPos().cy;
        if(rcItem.top >= rcList.top && rcItem.bottom < rcList.bottom)
        {
            return;
        }
        int dx = 0;
        if(rcItem.top < rcList.top)
        {
            dx = rcItem.top - rcList.top;
        }
        if(rcItem.bottom > rcList.bottom)
        {
            dx = rcItem.bottom - rcList.bottom;
        }
        Scroll(0, dx);
    }

    LRESULT CDuiComboWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        m_PaintManager.SetForceUseSharedRes(TRUE);
        m_PaintManager.Init(GetHWND());
        // The trick is to add the items to the new container. Their owner gets
        // reassigned by this operation - which is why it is important to reassign
        // the items back to the righfull owner/manager when the window closes.
        m_pLayout = new CDuiVerticalLayout;
        m_pLayout->SetManager(&m_PaintManager, NULL, TRUE);
        LPCTSTR pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(_T("VerticalLayout"));
        if(pDefaultAttributes)
        {
            m_pLayout->ApplyAttributeList(pDefaultAttributes);
        }
        m_pLayout->SetInset(CDuiRect(1, 1, 1, 1));
        m_pLayout->SetBkColor(0xFFFFFFFF);
        m_pLayout->SetBorderColor(0xFFC6C7D2);
        m_pLayout->SetBorderSize(1);
        m_pLayout->SetAutoDestroy(FALSE);
        m_pLayout->EnableScrollBar();
        m_pLayout->ApplyAttributeList(m_pOwner->GetDropBoxAttributeList());
        for(int i = 0; i < m_pOwner->GetCount(); i++)
        {
            m_pLayout->Add(m_pOwner->GetItemAt(i));
        }
        m_PaintManager.AttachDialog(m_pLayout);
        m_PaintManager.AddNotifier(this);
        return 0;
    }

    LRESULT CDuiComboWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), FALSE);
        RECT rcNull = { 0 };
        for(int i = 0; i < m_pOwner->GetCount(); i++)
        {
            static_cast<CDuiControl*>(m_pOwner->GetItemAt(i))->SetPos(rcNull);
        }
        m_pOwner->SetFocus();
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiComboWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt = { 0 };
        ::GetCursorPos(&pt);
        ::ScreenToClient(m_PaintManager.GetPaintWindow(), &pt);
        m_bHitItem = IsHitItem(pt);
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiComboWnd::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt = { 0 };
        ::GetCursorPos(&pt);
        ::ScreenToClient(m_PaintManager.GetPaintWindow(), &pt);
        if(m_bHitItem && IsHitItem(pt))
        {
            PostMessage(WM_KILLFOCUS);
        }
        m_bHitItem = FALSE;
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiComboWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        switch(wParam)
        {
            case VK_ESCAPE:
                m_pOwner->SelectItem(m_iOldSel, TRUE);
                EnsureVisible(m_iOldSel);
            case VK_RETURN:
                PostMessage(WM_KILLFOCUS);
                break;
            default:
                TEventUI event;
                event.Type = UIEVENT_KEYDOWN;
                event.chKey = (TCHAR)wParam;
                m_pOwner->DoEvent(event);
                EnsureVisible(m_pOwner->GetCurSel());
                bHandled = TRUE;
                return 0;
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiComboWnd::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        int zDelta = (int)(short) HIWORD(wParam);
        TEventUI event = { 0 };
        event.Type = UIEVENT_SCROLLWHEEL;
        event.wParam = MAKELPARAM(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0);
        event.lParam = lParam;
        event.dwTimestamp = ::GetTickCount();
        if(m_pOwner->GetScrollSelect())
        {
            m_pOwner->DoEvent(event);
            EnsureVisible(m_pOwner->GetCurSel());
        }
        else
        {
            m_pLayout->DoEvent(event);
        }
        bHandled = TRUE;
        return 0;
    }

    LRESULT CDuiComboWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(GetHWND() != (HWND) wParam)
        {
            PostMessage(WM_CLOSE);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiComboWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch(uMsg)
        {
            case WM_CREATE:
                lRes = OnCreate(uMsg, wParam, lParam, bHandled);
                break;
            case WM_CLOSE:
                lRes = OnClose(uMsg, wParam, lParam, bHandled);
                break;
            case WM_LBUTTONDOWN:
                lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled);
                break;
            case WM_LBUTTONUP:
                lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KEYDOWN:
                lRes = OnKeyDown(uMsg, wParam, lParam, bHandled);
                break;
            case WM_MOUSEWHEEL:
                lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KILLFOCUS:
                lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
                break;
            default:
                bHandled = FALSE;
                break;
        }
        if(bHandled)
        {
            return lRes;
        }
        if(m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
        {
            return lRes;
        }
        return CDuiWnd::HandleMessage(uMsg, wParam, lParam);
    }

    void CDuiComboWnd::Notify(TNotifyUI& msg)
    {
        if(msg.sType == DUI_MSGTYPE_WINDOWINIT)
        {
            EnsureVisible(m_iOldSel);
        }
    }

    ////////////////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiCombo)

    CDuiCombo::CDuiCombo(void)
        : m_pComboWnd(NULL)
        , m_bShowHtml(FALSE)
        , m_bScrollSelect(TRUE)
        , m_iCurSel(-1)
        , m_iFont(-1)
        , m_uTextStyle(DT_VCENTER | DT_SINGLELINE)
        , m_uButtonState(0)
        , m_dwTextColor(0)
        , m_dwDisabledTextColor(0)
    {
        m_szDropBox = CDuiSize(0, 150);

        m_ListInfo.bShowHtml = FALSE;
        m_ListInfo.bMultiExpandable = FALSE;
        m_ListInfo.bAlternateBk = FALSE;
        m_ListInfo.bShowRowLine = FALSE;
        m_ListInfo.bShowColumnLine = FALSE;
        m_ListInfo.nColumns = 0;
        m_ListInfo.nFont = -1;
        m_ListInfo.uTextStyle = DT_VCENTER | DT_SINGLELINE;
        m_ListInfo.dwTextColor = 0xFF000000;
        m_ListInfo.dwBkColor = 0;
        m_ListInfo.dwSelectedTextColor = 0xFF000000;
        m_ListInfo.dwSelectedBkColor = 0xFFC1E3FF;
        m_ListInfo.dwHotTextColor = 0xFF000000;
        m_ListInfo.dwHotBkColor = 0xFFE9F5FF;
        m_ListInfo.dwDisabledTextColor = 0xFFCCCCCC;
        m_ListInfo.dwDisabledBkColor = 0xFFFFFFFF;
        m_ListInfo.dwLineColor = 0;
    }


    CDuiCombo::~CDuiCombo(void)
    {

    }

    CDuiString CDuiCombo::GetClass() const
    {
        return DUI_CTR_COMBO;
    }

    LPVOID CDuiCombo::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_COMBO) == 0)
        {
            return static_cast<CDuiCombo*>(this);
        }
        if(_tcsicmp(pstrName, GET_CLASS_NAME(IListOwner)) == 0)
        {
            return static_cast<IListOwner*>(this);
        }
        return CDuiContainer::GetInterface(pstrName);
    }

    CDuiString CDuiCombo::GetText() const
    {
        if(m_iCurSel < 0)
        {
            return _T("");
        }
        CDuiControl* pControl = GetItemAt(m_iCurSel);
        if(pControl == NULL)
        {
            return _T("");
        }
        return pControl->GetText();
    }

    void CDuiCombo::EmptyComboWnd()
    {
        m_pComboWnd = NULL;
    }

    UINT CDuiCombo::GetButtonState() const
    {
        return m_uButtonState;
    }

    void CDuiCombo::SetButtonState(UINT uButtonState)
    {
        m_uButtonState = uButtonState;
    }

    UINT CDuiCombo::GetListType()
    {
        return LT_COMBO;
    }

    ListInfo* CDuiCombo::GetListInfo()
    {
        return &m_ListInfo;
    }

    int CDuiCombo::GetCurSel() const
    {
        return m_iCurSel;
    }

    BOOL CDuiCombo::SelectItem(int iIndex, BOOL bTakeFocus /*= FALSE*/)
    {
        if(iIndex == m_iCurSel)
        {
            return TRUE;
        }
        int iOldSel = m_iCurSel;
        if(m_iCurSel >= 0)
        {
            CDuiControl* pControl = GetItemAt(m_iCurSel);
            if(!pControl)
            {
                return FALSE;
            }
            IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem != NULL)
            {
                pListItem->Select(FALSE);
            }
            m_iCurSel = -1;
        }
        if(iIndex < 0)
        {
            return FALSE;
        }
        if(GetItems()->GetSize() == 0)
        {
            return FALSE;
        }
        if(iIndex >= GetItems()->GetSize())
        {
            iIndex = GetItems()->GetSize() - 1;
        }
        CDuiControl* pControl = GetItemAt(iIndex);
        if(!pControl || !pControl->IsEnabled())
        {
            return FALSE;
        }
        IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
        if(pListItem == NULL)
        {
            return FALSE;
        }
        m_iCurSel = iIndex;
        if(m_pComboWnd != NULL || bTakeFocus)
        {
            pControl->SetFocus();
        }
        pListItem->Select(TRUE);
        if(GetManager() != NULL)
        {
            GetManager()->SendNotify(this, DUI_MSGTYPE_ITEMSELECT, m_iCurSel, iOldSel);
        }
        Invalidate();

        return TRUE;
    }

    BOOL CDuiCombo::SelectMultiItem(int iIndex, BOOL bTakeFocus /*= FALSE*/)
    {
        return SelectItem(iIndex, bTakeFocus);
    }

    BOOL CDuiCombo::UnSelectItem(int iIndex, BOOL bOthers /*= FALSE*/)
    {
        return FALSE;
    }

    BOOL CDuiCombo::SetItemIndex(CDuiControl* pControl, int iIndex)
    {
        int iOrginIndex = GetItemIndex(pControl);
        if(iOrginIndex == -1)
        {
            return FALSE;
        }
        if(iOrginIndex == iIndex)
        {
            return TRUE;
        }

        IListItem* pSelectedListItem = NULL;
        if(m_iCurSel >= 0) pSelectedListItem =
                static_cast<IListItem*>(GetItemAt(m_iCurSel)->GetInterface(GET_CLASS_NAME(IListItem)));
        if(!CDuiContainer::SetItemIndex(pControl, iIndex))
        {
            return FALSE;
        }
        int iMinIndex = min(iOrginIndex, iIndex);
        int iMaxIndex = max(iOrginIndex, iIndex);
        for(int i = iMinIndex; i < iMaxIndex + 1; ++i)
        {
            CDuiControl* p = GetItemAt(i);
            IListItem* pListItem = static_cast<IListItem*>(p->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem != NULL)
            {
                pListItem->SetIndex(i);
            }
        }
        if(m_iCurSel >= 0 && pSelectedListItem != NULL)
        {
            m_iCurSel = pSelectedListItem->GetIndex();
        }
        return TRUE;
    }

    BOOL CDuiCombo::Add(CDuiControl* pControl)
    {
        IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
        if(pListItem != NULL)
        {
            pListItem->SetOwner(this);
            pListItem->SetIndex(GetItems()->GetSize());
        }
        return CDuiContainer::Add(pControl);
    }

    BOOL CDuiCombo::AddAt(CDuiControl* pControl, int iIndex)
    {
        if(!CDuiContainer::AddAt(pControl, iIndex))
        {
            return FALSE;
        }

        // The list items should know about us
        IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
        if(pListItem != NULL)
        {
            pListItem->SetOwner(this);
            pListItem->SetIndex(iIndex);
        }

        for(int i = iIndex + 1; i < GetCount(); ++i)
        {
            CDuiControl* p = GetItemAt(i);
            pListItem = static_cast<IListItem*>(p->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem != NULL)
            {
                pListItem->SetIndex(i);
            }
        }
        if(m_iCurSel >= iIndex)
        {
            m_iCurSel += 1;
        }
        return TRUE;
    }

    BOOL CDuiCombo::Remove(CDuiControl* pControl)
    {
        int iIndex = GetItemIndex(pControl);
        if(iIndex == -1)
        {
            return FALSE;
        }

        if(!CDuiContainer::RemoveAt(iIndex))
        {
            return FALSE;
        }

        for(int i = iIndex; i < GetCount(); ++i)
        {
            CDuiControl* p = GetItemAt(i);
            IListItem* pListItem = static_cast<IListItem*>(p->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem != NULL)
            {
                pListItem->SetIndex(i);
            }
        }

        if(iIndex == m_iCurSel && m_iCurSel >= 0)
        {
            int iSel = m_iCurSel;
            m_iCurSel = -1;
            SelectItem(FindSelectable(iSel, FALSE));
        }
        else if(iIndex < m_iCurSel)
        {
            m_iCurSel -= 1;
        }
        return TRUE;
    }

    BOOL CDuiCombo::RemoveAt(int iIndex)
    {
        if(!CDuiContainer::RemoveAt(iIndex))
        {
            return FALSE;
        }

        for(int i = iIndex; i < GetCount(); ++i)
        {
            CDuiControl* p = GetItemAt(i);
            IListItem* pListItem = static_cast<IListItem*>(p->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem != NULL)
            {
                pListItem->SetIndex(i);
            }
        }

        if(iIndex == m_iCurSel && m_iCurSel >= 0)
        {
            int iSel = m_iCurSel;
            m_iCurSel = -1;
            SelectItem(FindSelectable(iSel, FALSE));
        }
        else if(iIndex < m_iCurSel)
        {
            m_iCurSel -= 1;
        }
        return TRUE;
    }

    void CDuiCombo::RemoveAll()
    {
        m_iCurSel = -1;
        CDuiContainer::RemoveAll();
    }

    int CDuiCombo::GetFont() const
    {
        return m_iFont;
    }

    void CDuiCombo::SetFont(int index)
    {
        if(m_iFont == index)
        {
            return;
        }
        m_iFont = index;
        Invalidate();
    }

    DWORD CDuiCombo::GetTextColor() const
    {
        return m_dwTextColor;
    }

    void CDuiCombo::SetTextColor(DWORD dwTextColor)
    {
        if(m_dwTextColor == dwTextColor)
        {
            return;
        }
        m_dwTextColor = dwTextColor;
        Invalidate();
    }

    DWORD CDuiCombo::GetDisabledTextColor() const
    {
        return m_dwDisabledTextColor;
    }

    void CDuiCombo::SetDisabledTextColor(DWORD dwTextColor)
    {
        if(m_dwDisabledTextColor == dwTextColor)
        {
            return;
        }
        m_dwDisabledTextColor = dwTextColor;
        Invalidate();
    }

    CDuiRect CDuiCombo::GetTextPadding() const
    {
        return m_rcTextPadding;
    }

    void CDuiCombo::SetTextPadding(CDuiRect rc)
    {
        if(m_rcTextPadding == rc)
        {
            return;
        }
        m_rcTextPadding = rc;
        Invalidate();
    }

    BOOL CDuiCombo::IsShowHtml() const
    {
        return m_bShowHtml;
    }

    void CDuiCombo::SetShowHtml(BOOL bShowHtml /*= TRUE*/)
    {
        if(m_bShowHtml == bShowHtml)
        {
            return;
        }

        m_bShowHtml = bShowHtml;
        Invalidate();
    }

    LPCTSTR CDuiCombo::GetNormalImage() const
    {
        return m_sNormalImage;
    }

    void CDuiCombo::SetNormalImage(LPCTSTR pStrImage)
    {
        m_sNormalImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiCombo::GetHotImage() const
    {
        return m_sHotImage;
    }

    void CDuiCombo::SetHotImage(LPCTSTR pStrImage)
    {
        m_sHotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiCombo::GetPushedImage() const
    {
        return m_sPushedImage;
    }

    void CDuiCombo::SetPushedImage(LPCTSTR pStrImage)
    {
        m_sPushedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiCombo::GetFocusedImage() const
    {
        return m_sFocusedImage;
    }

    void CDuiCombo::SetFocusedImage(LPCTSTR pStrImage)
    {
        m_sFocusedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiCombo::GetDisabledImage() const
    {
        return m_sDisabledImage;
    }

    void CDuiCombo::SetDisabledImage(LPCTSTR pStrImage)
    {
        m_sDisabledImage = pStrImage;
        Invalidate();
    }

    BOOL CDuiCombo::GetScrollSelect() const
    {
        return m_bScrollSelect;
    }

    void CDuiCombo::SetScrollSelect(BOOL bScrollSelect)
    {
        m_bScrollSelect = bScrollSelect;
    }

    CDuiString CDuiCombo::GetDropBoxAttributeList() const
    {
        return m_sDropBoxAttributes;
    }

    void CDuiCombo::SetDropBoxAttributeList(LPCTSTR pstrList)
    {
        m_sDropBoxAttributes = pstrList;
    }

    SIZE CDuiCombo::GetDropBoxSize() const
    {
        return m_szDropBox;
    }

    void CDuiCombo::SetDropBoxSize(SIZE szDropBox)
    {
        m_szDropBox = szDropBox;
    }

    RECT CDuiCombo::GetItemTextPadding() const
    {
        return m_ListInfo.rcTextPadding;
    }

    void CDuiCombo::SetItemTextPadding(RECT rc)
    {
        if(m_ListInfo.rcTextPadding.bottom == rc.bottom &&
                m_ListInfo.rcTextPadding.top == rc.top &&
                m_ListInfo.rcTextPadding.right == rc.right &&
                m_ListInfo.rcTextPadding.left == rc.left)
        {
            return;
        }
        m_ListInfo.rcTextPadding = rc;
        Invalidate();
    }

    DWORD CDuiCombo::GetItemTextColor() const
    {
        return m_ListInfo.dwTextColor;
    }

    void CDuiCombo::SetItemTextColor(DWORD dwTextColor)
    {
        if(m_ListInfo.dwTextColor == dwTextColor)
        {
            return;
        }
        m_ListInfo.dwTextColor = dwTextColor;
        Invalidate();
    }

    DWORD CDuiCombo::GetItemBkColor() const
    {
        return m_ListInfo.dwBkColor;
    }

    void CDuiCombo::SetItemBkColor(DWORD dwBkColor)
    {
        m_ListInfo.dwBkColor = dwBkColor;
    }

    LPCTSTR CDuiCombo::GetItemBkImage() const
    {
        return m_ListInfo.sBkImage;
    }

    void CDuiCombo::SetItemBkImage(LPCTSTR pStrImage)
    {
        m_ListInfo.sBkImage = pStrImage;
    }


    BOOL CDuiCombo::IsAlternateBk() const
    {
        return m_ListInfo.bAlternateBk;
    }

    void CDuiCombo::SetAlternateBk(BOOL bAlternateBk)
    {
        m_ListInfo.bAlternateBk = bAlternateBk;
    }

    void CDuiCombo::SetSelectedItemTextColor(DWORD dwTextColor)
    {
        m_ListInfo.dwSelectedTextColor = dwTextColor;
    }

    void CDuiCombo::SetSelectedItemBkColor(DWORD dwBkColor)
    {
        m_ListInfo.dwSelectedBkColor = dwBkColor;
    }

    void CDuiCombo::SetSelectedItemImage(LPCTSTR pStrImage)
    {
        m_ListInfo.sSelectedImage = pStrImage;
    }

    DWORD CDuiCombo::GetSelectedItemTextColor() const
    {
        return m_ListInfo.dwSelectedTextColor;
    }

    DWORD CDuiCombo::GetSelectedItemBkColor() const
    {
        return m_ListInfo.dwSelectedBkColor;
    }

    LPCTSTR CDuiCombo::GetSelectedItemImage() const
    {
        return m_ListInfo.sSelectedImage;
    }

    void CDuiCombo::SetHotItemTextColor(DWORD dwTextColor)
    {
        m_ListInfo.dwHotTextColor = dwTextColor;
    }

    void CDuiCombo::SetHotItemBkColor(DWORD dwBkColor)
    {
        m_ListInfo.dwHotBkColor = dwBkColor;
    }

    void CDuiCombo::SetHotItemImage(LPCTSTR pStrImage)
    {
        m_ListInfo.sHotImage = pStrImage;
    }

    DWORD CDuiCombo::GetHotItemTextColor() const
    {
        return m_ListInfo.dwHotTextColor;
    }
    DWORD CDuiCombo::GetHotItemBkColor() const
    {
        return m_ListInfo.dwHotBkColor;
    }

    LPCTSTR CDuiCombo::GetHotItemImage() const
    {
        return m_ListInfo.sHotImage;
    }

    void CDuiCombo::SetDisabledItemTextColor(DWORD dwTextColor)
    {
        m_ListInfo.dwDisabledTextColor = dwTextColor;
    }

    void CDuiCombo::SetDisabledItemBkColor(DWORD dwBkColor)
    {
        m_ListInfo.dwDisabledBkColor = dwBkColor;
    }

    void CDuiCombo::SetDisabledItemImage(LPCTSTR pStrImage)
    {
        m_ListInfo.sDisabledImage = pStrImage;
    }

    DWORD CDuiCombo::GetDisabledItemTextColor() const
    {
        return m_ListInfo.dwDisabledTextColor;
    }

    DWORD CDuiCombo::GetDisabledItemBkColor() const
    {
        return m_ListInfo.dwDisabledBkColor;
    }

    LPCTSTR CDuiCombo::GetDisabledItemImage() const
    {
        return m_ListInfo.sDisabledImage;
    }

    DWORD CDuiCombo::GetItemLineColor() const
    {
        return m_ListInfo.dwLineColor;
    }

    void CDuiCombo::SetItemLineColor(DWORD dwLineColor)
    {
        m_ListInfo.dwLineColor = dwLineColor;
    }

    BOOL CDuiCombo::IsItemShowHtml()
    {
        return m_ListInfo.bShowHtml;
    }

    void CDuiCombo::SetItemShowHtml(BOOL bShowHtml)
    {
        if(m_ListInfo.bShowHtml == bShowHtml)
        {
            return;
        }

        m_ListInfo.bShowHtml = bShowHtml;
        Invalidate();
    }

    void CDuiCombo::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("align")) == 0)
        {
            if(_tcsstr(pstrValue, _T("left")) != NULL)
            {
                m_uTextStyle &= ~(DT_CENTER | DT_RIGHT | DT_SINGLELINE);
                m_uTextStyle |= DT_LEFT;
            }
            if(_tcsstr(pstrValue, _T("center")) != NULL)
            {
                m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
                m_uTextStyle |= DT_CENTER;
            }
            if(_tcsstr(pstrValue, _T("right")) != NULL)
            {
                m_uTextStyle &= ~(DT_LEFT | DT_CENTER | DT_SINGLELINE);
                m_uTextStyle |= DT_RIGHT;
            }
        }
        else if(_tcsicmp(pstrName, _T("valign")) == 0)
        {
            if(_tcsstr(pstrValue, _T("top")) != NULL)
            {
                m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER);
                m_uTextStyle |= (DT_TOP | DT_SINGLELINE);
            }
            if(_tcsstr(pstrValue, _T("vcenter")) != NULL)
            {
                m_uTextStyle &= ~(DT_TOP | DT_BOTTOM);
                m_uTextStyle |= (DT_VCENTER | DT_SINGLELINE);
            }
            if(_tcsstr(pstrValue, _T("bottom")) != NULL)
            {
                m_uTextStyle &= ~(DT_TOP | DT_VCENTER);
                m_uTextStyle |= (DT_BOTTOM | DT_SINGLELINE);
            }
        }
        else if(_tcsicmp(pstrName, _T("endellipsis")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("TRUE")) == 0)
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
            if(_tcsicmp(pstrValue, _T("TRUE")) == 0)
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
            CDuiRect rcTextPadding;
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
            SetShowHtml(_tcsicmp(pstrValue, _T("TRUE")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("normalimage")) == 0)
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
        else if(_tcsicmp(pstrName, _T("scrollselect")) == 0)
        {
            SetScrollSelect(_tcsicmp(pstrValue, _T("TRUE")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("dropbox")) == 0)
        {
            SetDropBoxAttributeList(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("dropboxsize")) == 0)
        {
            SIZE szDropBoxSize = { 0 };
            LPTSTR pstr = NULL;
            szDropBoxSize.cx = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            szDropBoxSize.cy = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetDropBoxSize(szDropBoxSize);
        }
        else if(_tcsicmp(pstrName, _T("itemfont")) == 0)
        {
            m_ListInfo.nFont = _ttoi(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemalign")) == 0)
        {
            if(_tcsstr(pstrValue, _T("left")) != NULL)
            {
                m_ListInfo.uTextStyle &= ~(DT_CENTER | DT_RIGHT);
                m_ListInfo.uTextStyle |= DT_LEFT;
            }
            if(_tcsstr(pstrValue, _T("center")) != NULL)
            {
                m_ListInfo.uTextStyle &= ~(DT_LEFT | DT_RIGHT);
                m_ListInfo.uTextStyle |= DT_CENTER;
            }
            if(_tcsstr(pstrValue, _T("right")) != NULL)
            {
                m_ListInfo.uTextStyle &= ~(DT_LEFT | DT_CENTER);
                m_ListInfo.uTextStyle |= DT_RIGHT;
            }
        }
        else if(_tcsicmp(pstrName, _T("itemendellipsis")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("TRUE")) == 0)
            {
                m_ListInfo.uTextStyle |= DT_END_ELLIPSIS;
            }
            else
            {
                m_ListInfo.uTextStyle &= ~DT_END_ELLIPSIS;
            }
        }
        else if(_tcsicmp(pstrName, _T("itemtextpadding")) == 0)
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
            SetItemTextPadding(rcTextPadding);
        }
        else if(_tcsicmp(pstrName, _T("itemtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itembkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itembkimage")) == 0)
        {
            SetItemBkImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemaltbk")) == 0)
        {
            SetAlternateBk(_tcsicmp(pstrValue, _T("TRUE")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("itemselectedtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedItemTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemselectedbkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedItemBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemselectedimage")) == 0)
        {
            SetSelectedItemImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemhottextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetHotItemTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemhotbkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetHotItemBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemhotimage")) == 0)
        {
            SetHotItemImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemdisabledtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetDisabledItemTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemdisabledbkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetDisabledItemBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemdisabledimage")) == 0)
        {
            SetDisabledItemImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemlinecolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemLineColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemshowhtml")) == 0)
        {
            SetItemShowHtml(_tcsicmp(pstrValue, _T("TRUE")) == 0);
        }
        else
        {
            CDuiContainer::SetAttribute(pstrName, pstrValue);
        }
    }

    SIZE CDuiCombo::EstimateSize(SIZE szAvailable)
    {
        if(GetFixedHeight() == 0)
        {
            return CDuiSize(GetFixedWidth(), GetManager()->GetDefaultFontInfo()->tm.tmHeight + 12);
        }
        return CDuiControl::EstimateSize(szAvailable);
    }

    void CDuiCombo::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        // 隐藏下拉窗口
        if(m_pComboWnd && ::IsWindow(m_pComboWnd->GetHWND()))
        {
            m_pComboWnd->Close();
        }
        // 所有元素大小置为0
        RECT rcNull = { 0 };
        for(int i = 0; i < GetItems()->GetSize(); i++)
        {
            GetItemAt(i)->SetPos(rcNull);
        }
        // 调整位置
        CDuiControl::SetPos(rc, bNeedInvalidate);
    }

    void CDuiCombo::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        CDuiControl::DoPaint(hDC, rcPaint);
    }

    void CDuiCombo::PaintText(HDC hDC)
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
        rc.left += m_rcTextPadding.left;
        rc.right -= m_rcTextPadding.right;
        rc.top += m_rcTextPadding.top;
        rc.bottom -= m_rcTextPadding.bottom;

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
                CRenderEngine::DrawHtmlText(hDC, GetManager(), rc, sText, m_dwTextColor, NULL, NULL, nLinks, m_uTextStyle);
            }
            else
            {
                CRenderEngine::DrawText(hDC, GetManager(), rc, sText, m_dwTextColor, m_iFont, m_uTextStyle);
            }
        }
        else
        {
            if(m_bShowHtml)
            {
                CRenderEngine::DrawHtmlText(hDC, GetManager(), rc, sText, m_dwDisabledTextColor, NULL, NULL, nLinks, m_uTextStyle);
            }
            else
            {
                CRenderEngine::DrawText(hDC, GetManager(), rc, sText, m_dwDisabledTextColor, m_iFont, m_uTextStyle);
            }
        }
    }

    void CDuiCombo::PaintStatusImage(HDC hDC)
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

        if((m_uButtonState & UISTATE_DISABLED) != 0)
        {
            if(!m_sDisabledImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sDisabledImage)) {}
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
                if(!DrawImage(hDC, (LPCTSTR)m_sPushedImage)) {}
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
                if(!DrawImage(hDC, (LPCTSTR)m_sHotImage)) {}
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
                if(!DrawImage(hDC, (LPCTSTR)m_sFocusedImage)) {}
                else
                {
                    return;
                }
            }
        }

        if(!m_sNormalImage.IsEmpty())
        {
            if(!DrawImage(hDC, (LPCTSTR)m_sNormalImage)) {}
            else
            {
                return;
            }
        }
    }

    BOOL CDuiCombo::Activate()
    {
        if(!CDuiContainer::Activate())
        {
            return FALSE;
        }
        if(m_pComboWnd)
        {
            return TRUE;
        }
        m_pComboWnd = new CDuiComboWnd();
        ASSERT(m_pComboWnd);
        m_pComboWnd->Init(this);
        if(GetManager() != NULL)
        {
            GetManager()->SendNotify(this, DUI_MSGTYPE_DROPDOWN);
        }
        Invalidate();
        return TRUE;
    }

    void CDuiCombo::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(GetParent() != NULL)
            {
                GetParent()->DoEvent(event);
            }
            else
            {
                CDuiContainer::DoEvent(event);
            }
            return;
        }

        if(event.Type == UIEVENT_SETFOCUS)
        {
            Invalidate();
        }
        if(event.Type == UIEVENT_KILLFOCUS)
        {
            Invalidate();
        }
        if(event.Type == UIEVENT_BUTTONDOWN)
        {
            if(IsEnabled())
            {
                Activate();
                m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
            }
            return;
        }
        if(event.Type == UIEVENT_BUTTONUP)
        {
            if((m_uButtonState & UISTATE_CAPTURED) != 0)
            {
                m_uButtonState &= ~ UISTATE_CAPTURED;
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_MOUSEMOVE)
        {
            return;
        }
        if(event.Type == UIEVENT_KEYDOWN)
        {
            switch(event.chKey)
            {
                case VK_F4:
                    Activate();
                    return;
                case VK_UP:
                    SelectItem(FindSelectable(m_iCurSel - 1, FALSE));
                    return;
                case VK_DOWN:
                    SelectItem(FindSelectable(m_iCurSel + 1, TRUE));
                    return;
                case VK_PRIOR:
                    SelectItem(FindSelectable(m_iCurSel - 1, FALSE));
                    return;
                case VK_NEXT:
                    SelectItem(FindSelectable(m_iCurSel + 1, TRUE));
                    return;
                case VK_HOME:
                    SelectItem(FindSelectable(0, FALSE));
                    return;
                case VK_END:
                    SelectItem(FindSelectable(GetCount() - 1, TRUE));
                    return;
            }
        }
        if(event.Type == UIEVENT_SCROLLWHEEL)
        {
            if(GetScrollSelect())
            {
                BOOL bDownward = LOWORD(event.wParam) == SB_LINEDOWN;
                SelectItem(FindSelectable(m_iCurSel + (bDownward ? 1 : -1), bDownward));
            }
            return;
        }
        if(event.Type == UIEVENT_CONTEXTMENU)
        {
            return;
        }
        if(event.Type == UIEVENT_MOUSEENTER)
        {
            if(::PtInRect(&GetPos(), event.ptMouse))
            {
                if((m_uButtonState & UISTATE_HOT) == 0)
                {
                    m_uButtonState |= UISTATE_HOT;
                }
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_MOUSELEAVE)
        {
            if((m_uButtonState & UISTATE_HOT) != 0)
            {
                m_uButtonState &= ~UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        CDuiContainer::DoEvent(event);
    }

}