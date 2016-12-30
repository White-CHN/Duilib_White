#include "StdAfx.h"
#include "DuiVerticalLayout.h"

namespace DuiLib
{

    IMPLEMENT_DUICONTROL(CDuiVerticalLayout)

    CDuiVerticalLayout::CDuiVerticalLayout(void)
        : m_bImmMode(FALSE)
        , m_iSepHeight(0)
        , m_uButtonState(0)
    {
    }


    CDuiVerticalLayout::~CDuiVerticalLayout(void)
    {
    }

    CDuiString CDuiVerticalLayout::GetClass() const
    {
        return DUI_CTR_VERTICALLAYOUT;
    }

    LPVOID CDuiVerticalLayout::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_VERTICALLAYOUT) == 0)
        {
            return static_cast<CDuiVerticalLayout*>(this);
        }
        return CDuiContainer::GetInterface(pstrName);
    }

    UINT CDuiVerticalLayout::GetControlFlags() const
    {
        if(IsEnabled() && m_iSepHeight != 0)
        {
            return UIFLAG_SETCURSOR;
        }
        else
        {
            return 0;
        }
    }

    void CDuiVerticalLayout::SetSepHeight(int iHeight)
    {
        m_iSepHeight = iHeight;
    }

    void CDuiVerticalLayout::SetSepImmMode(BOOL bImmediately)
    {
        if(m_bImmMode == bImmediately)
        {
            return;
        }
        if((m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode && GetManager() != NULL)
        {
            GetManager()->RemovePostPaint(this);
        }

        m_bImmMode = bImmediately;
    }

    void CDuiVerticalLayout::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("sepheight")) == 0)
        {
            SetSepHeight(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("sepimm")) == 0)
        {
            SetSepImmMode(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else
        {
            CDuiContainer::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiVerticalLayout::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::SetPos(rc, bNeedInvalidate);
        rc = GetPos();
        // Adjust for inset
        RECT rcInset = GetInset();
        rc.left += rcInset.left;
        rc.top += rcInset.top;
        rc.right -= rcInset.right;
        rc.bottom -= rcInset.bottom;
        CDuiScrollBar* pVerticalScrollBar =  CDuiContainer::GetVerticalScrollBar();
        CDuiScrollBar* pHorizontalScrollBar = CDuiContainer::GetHorizontalScrollBar();

        if(pVerticalScrollBar && pVerticalScrollBar->IsVisible())
        {
            rc.right -= pVerticalScrollBar->GetFixedWidth();
        }
        if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
        {
            rc.bottom -= pHorizontalScrollBar->GetFixedHeight();
        }
        if(CDuiContainer::GetCount() == 0)
        {
            ProcessScrollBar(rc, 0, 0);
            return;
        }
        // Determine the minimum size
        SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
        if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
        {
            szAvailable.cx += pHorizontalScrollBar->GetScrollRange();
        }
        if(pVerticalScrollBar && pVerticalScrollBar->IsVisible())
        {
            szAvailable.cy += pVerticalScrollBar->GetScrollRange();
        }
        int cxNeeded = 0;
        int nAdjustables = 0;
        int cyFixed = 0;
        int nEstimateNum = 0;
        SIZE szControlAvailable;
        int iControlMaxWidth = 0;
        int iControlMaxHeight = 0;
        for(int it1 = 0; it1 < CDuiContainer::GetCount(); it1++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(CDuiContainer::GetItemAt(it1));
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                continue;
            }
            szControlAvailable = szAvailable;
            RECT rcPadding = pControl->GetPadding();
            szControlAvailable.cx -= rcPadding.left + rcPadding.right;
            iControlMaxWidth = pControl->GetFixedWidth();
            iControlMaxHeight = pControl->GetFixedHeight();
            if(iControlMaxWidth <= 0)
            {
                iControlMaxWidth = pControl->GetMaxWidth();
            }
            if(iControlMaxHeight <= 0)
            {
                iControlMaxHeight = pControl->GetMaxHeight();
            }
            if(szControlAvailable.cx > iControlMaxWidth)
            {
                szControlAvailable.cx = iControlMaxWidth;
            }
            if(szControlAvailable.cy > iControlMaxHeight)
            {
                szControlAvailable.cy = iControlMaxHeight;
            }
            SIZE sz = pControl->EstimateSize(szControlAvailable);
            if(sz.cy == 0)
            {
                nAdjustables++;
            }
            else
            {
                if(sz.cy < pControl->GetMinHeight())
                {
                    sz.cy = pControl->GetMinHeight();
                }
                if(sz.cy > pControl->GetMaxHeight())
                {
                    sz.cy = pControl->GetMaxHeight();
                }
            }
            cyFixed += sz.cy + pControl->GetPadding().top + pControl->GetPadding().bottom;
            sz.cx = MAX(sz.cx, 0);
            if(sz.cx < pControl->GetMinWidth())
            {
                sz.cx = pControl->GetMinWidth();
            }
            if(sz.cx > pControl->GetMaxWidth())
            {
                sz.cx = pControl->GetMaxWidth();
            }
            cxNeeded = MAX(cxNeeded, sz.cx + rcPadding.left + rcPadding.right);
            nEstimateNum++;
        }
        cyFixed += (nEstimateNum - 1) * GetChildPadding();
        // Place elements
        int cyNeeded = 0;
        int cyExpand = 0;
        if(nAdjustables > 0)
        {
            cyExpand = MAX(0, (szAvailable.cy - cyFixed) / nAdjustables);
        }
        // Position the elements
        SIZE szRemaining = szAvailable;
        int iPosY = rc.top;
        if(pVerticalScrollBar && pVerticalScrollBar->IsVisible())
        {
            iPosY -= pVerticalScrollBar->GetScrollPos();
        }
        int iEstimate = 0;
        int iAdjustable = 0;
        int cyFixedRemaining = cyFixed;
        for(int it2 = 0; it2 < CDuiContainer::GetCount(); it2++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(CDuiContainer::GetItemAt(it2));
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                SetFloatPos(it2);
                continue;
            }
            iEstimate += 1;
            RECT rcPadding = pControl->GetPadding();
            szRemaining.cy -= rcPadding.top;
            szControlAvailable = szRemaining;
            szControlAvailable.cx -= rcPadding.left + rcPadding.right;
            iControlMaxWidth = pControl->GetFixedWidth();
            iControlMaxHeight = pControl->GetFixedHeight();
            if(iControlMaxWidth <= 0)
            {
                iControlMaxWidth = pControl->GetMaxWidth();
            }
            if(iControlMaxHeight <= 0)
            {
                iControlMaxHeight = pControl->GetMaxHeight();
            }
            if(szControlAvailable.cx > iControlMaxWidth)
            {
                szControlAvailable.cx = iControlMaxWidth;
            }
            if(szControlAvailable.cy > iControlMaxHeight)
            {
                szControlAvailable.cy = iControlMaxHeight;
            }
            cyFixedRemaining = cyFixedRemaining - (rcPadding.top + rcPadding.bottom);
            if(iEstimate > 1)
            {
                cyFixedRemaining = cyFixedRemaining - GetChildPadding();
            }
            SIZE sz = pControl->EstimateSize(szControlAvailable);
            if(sz.cy == 0)
            {
                iAdjustable++;
                sz.cy = cyExpand;
                // Distribute remaining to last element (usually round-off left-overs)
                if(iAdjustable == nAdjustables)
                {
                    sz.cy = MAX(0, szRemaining.cy - rcPadding.bottom - cyFixedRemaining);
                }
                if(sz.cy < pControl->GetMinHeight())
                {
                    sz.cy = pControl->GetMinHeight();
                }
                if(sz.cy > pControl->GetMaxHeight())
                {
                    sz.cy = pControl->GetMaxHeight();
                }
            }
            else
            {
                if(sz.cy < pControl->GetMinHeight())
                {
                    sz.cy = pControl->GetMinHeight();
                }
                if(sz.cy > pControl->GetMaxHeight())
                {
                    sz.cy = pControl->GetMaxHeight();
                }
                cyFixedRemaining -= sz.cy;
            }
            sz.cx = pControl->GetMaxWidth();
            if(sz.cx == 0)
            {
                sz.cx = szAvailable.cx - rcPadding.left - rcPadding.right;
            }
            if(sz.cx < 0)
            {
                sz.cx = 0;
            }
            if(sz.cx > szControlAvailable.cx)
            {
                sz.cx = szControlAvailable.cx;
            }
            if(sz.cx < pControl->GetMinWidth())
            {
                sz.cx = pControl->GetMinWidth();
            }

            UINT iChildAlign = GetChildAlign();
            if(iChildAlign == DT_CENTER)
            {
                int iPosX = (rc.right + rc.left) / 2;
                if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
                {
                    iPosX += pHorizontalScrollBar->GetScrollRange() / 2;
                    iPosX -= pHorizontalScrollBar->GetScrollPos();
                }
                RECT rcCtrl = { iPosX - sz.cx / 2, iPosY + rcPadding.top, iPosX + sz.cx - sz.cx / 2, iPosY + sz.cy + rcPadding.top };
                pControl->SetPos(rcCtrl, FALSE);
            }
            else if(iChildAlign == DT_RIGHT)
            {
                int iPosX = rc.right;
                if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
                {
                    iPosX += pHorizontalScrollBar->GetScrollRange();
                    iPosX -= pHorizontalScrollBar->GetScrollPos();
                }
                RECT rcCtrl = { iPosX - rcPadding.right - sz.cx, iPosY + rcPadding.top, iPosX - rcPadding.right, iPosY + sz.cy + rcPadding.top };
                pControl->SetPos(rcCtrl, FALSE);
            }
            else
            {
                int iPosX = rc.left;
                if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
                {
                    iPosX -= pHorizontalScrollBar->GetScrollPos();
                }
                RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, iPosY + sz.cy + rcPadding.top };
                pControl->SetPos(rcCtrl, FALSE);
            }
            iPosY += sz.cy + GetChildPadding() + rcPadding.top + rcPadding.bottom;
            cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
            szRemaining.cy -= sz.cy + GetChildPadding() + rcPadding.bottom;
        }
        cyNeeded += (nEstimateNum - 1) * GetChildPadding();
        // Process the scrollbar
        ProcessScrollBar(rc, cxNeeded, cyNeeded);
    }

    RECT CDuiVerticalLayout::GetThumbRect(BOOL bUseNew /*= FALSE*/) const
    {
        if((m_uButtonState & UISTATE_CAPTURED) != 0 && bUseNew)
        {
            if(m_iSepHeight >= 0)
                return CDuiRect(m_rcNewPos.left, MAX(m_rcNewPos.bottom - m_iSepHeight, m_rcNewPos.top),
                                m_rcNewPos.right, m_rcNewPos.bottom);
            else
                return CDuiRect(m_rcNewPos.left, m_rcNewPos.top, m_rcNewPos.right,
                                MIN(m_rcNewPos.top - m_iSepHeight, m_rcNewPos.bottom));
        }
        else
        {
            if(m_iSepHeight >= 0)
                return CDuiRect(GetPos().left, MAX(GetPos().bottom - m_iSepHeight, GetPos().top), GetPos().right,
                                GetPos().bottom);
            else
                return CDuiRect(GetPos().left, GetPos().top, GetPos().right,
                                MIN(GetPos().top - m_iSepHeight, GetPos().bottom));

        }
    }

    void CDuiVerticalLayout::DoEvent(TEventUI& event)
    {
        if(m_iSepHeight != 0)
        {
            if(event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
            {
                RECT rcSeparator = GetThumbRect(FALSE);
                if(::PtInRect(&rcSeparator, event.ptMouse))
                {
                    m_uButtonState |= UISTATE_CAPTURED;
                    ptLastMouse = event.ptMouse;
                    m_rcNewPos = GetPos();
                    if(!m_bImmMode && GetManager())
                    {
                        GetManager()->AddPostPaint(this);
                    }
                    return;
                }
            }
            if(event.Type == UIEVENT_BUTTONUP)
            {
                if((m_uButtonState & UISTATE_CAPTURED) != 0)
                {
                    m_uButtonState &= ~UISTATE_CAPTURED;
                    SetPos(m_rcNewPos, FALSE);
                    if(!m_bImmMode && GetManager())
                    {
                        GetManager()->RemovePostPaint(this);
                    }
                    NeedParentUpdate();
                    return;
                }
            }
            if(event.Type == UIEVENT_MOUSEMOVE)
            {
                if((m_uButtonState & UISTATE_CAPTURED) != 0)
                {
                    LONG cy = event.ptMouse.y - ptLastMouse.y;
                    ptLastMouse = event.ptMouse;
                    RECT rc = m_rcNewPos;
                    if(m_iSepHeight >= 0)
                    {
                        if(cy > 0 && event.ptMouse.y < m_rcNewPos.bottom + m_iSepHeight)
                        {
                            return;
                        }
                        if(cy < 0 && event.ptMouse.y > m_rcNewPos.bottom)
                        {
                            return;
                        }
                        rc.bottom += cy;
                        if(rc.bottom - rc.top <= GetMinHeight())
                        {
                            if(m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight())
                            {
                                return;
                            }
                            rc.bottom = rc.top + GetMinHeight();
                        }
                        if(rc.bottom - rc.top >= GetMaxHeight())
                        {
                            if(m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight())
                            {
                                return;
                            }
                            rc.bottom = rc.top + GetMaxHeight();
                        }
                    }
                    else
                    {
                        if(cy > 0 && event.ptMouse.y < m_rcNewPos.top)
                        {
                            return;
                        }
                        if(cy < 0 && event.ptMouse.y > m_rcNewPos.top + m_iSepHeight)
                        {
                            return;
                        }
                        rc.top += cy;
                        if(rc.bottom - rc.top <= GetMinHeight())
                        {
                            if(m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight())
                            {
                                return;
                            }
                            rc.top = rc.bottom - GetMinHeight();
                        }
                        if(rc.bottom - rc.top >= GetMaxHeight())
                        {
                            if(m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight())
                            {
                                return;
                            }
                            rc.top = rc.bottom - GetMaxHeight();
                        }
                    }

                    CDuiRect rcInvalidate = GetThumbRect(TRUE);
                    m_rcNewPos = rc;
                    SetFixedHeight(m_rcNewPos.bottom - m_rcNewPos.top);

                    if(m_bImmMode)
                    {
                        SetPos(m_rcNewPos, FALSE);
                        NeedParentUpdate();
                    }
                    else
                    {
                        rcInvalidate.Join(GetThumbRect(TRUE));
                        rcInvalidate.Join(GetThumbRect(FALSE));
                        if(GetManager())
                        {
                            GetManager()->Invalidate(rcInvalidate);
                        }
                    }
                    return;
                }
            }
            if(event.Type == UIEVENT_SETCURSOR)
            {
                RECT rcSeparator = GetThumbRect(FALSE);
                if(IsEnabled() && ::PtInRect(&rcSeparator, event.ptMouse))
                {
                    ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
                    return;
                }
            }
        }
        CDuiContainer::DoEvent(event);
    }

    void CDuiVerticalLayout::DoPostPaint(HDC hDC, const RECT& rcPaint)
    {
        if((m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode)
        {
            RECT rcSeparator = GetThumbRect(TRUE);
            CRenderEngine::DrawColor(hDC, rcSeparator, 0xAA000000);
        }
    }

}
