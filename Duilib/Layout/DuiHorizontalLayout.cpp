#include "StdAfx.h"
#include "DuiHorizontalLayout.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiHorizontalLayout)
    CDuiHorizontalLayout::CDuiHorizontalLayout(void)
        : m_bImmMode(FALSE)
        , m_iSepWidth(0)
        , m_uButtonState(0)
    {

    }


    CDuiHorizontalLayout::~CDuiHorizontalLayout(void)
    {
    }

    LPCTSTR CDuiHorizontalLayout::GetClass() const
    {
        return DUI_CTR_HORIZONTALLAYOUT;
    }

    LPVOID CDuiHorizontalLayout::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_HORIZONTALLAYOUT) == 0)
        {
            return static_cast<CDuiHorizontalLayout*>(this);
        }
        return CDuiContainer::GetInterface(pstrName);
    }

    UINT CDuiHorizontalLayout::GetControlFlags() const
    {
        if(IsEnabled() && m_iSepWidth != 0)
        {
            return UIFLAG_SETCURSOR;
        }
        else
        {
            return 0;
        }
    }

    void CDuiHorizontalLayout::SetSepWidth(int iWidth)
    {
        m_iSepWidth = iWidth;
    }

    void CDuiHorizontalLayout::SetSepImmMode(BOOL bImmediately)
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

    void CDuiHorizontalLayout::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("sepwidth")) == 0)
        {
            SetSepWidth(_ttoi(pstrValue));
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

    void CDuiHorizontalLayout::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::SetPos(rc, bNeedInvalidate);
        rc = GetPos();

        // Adjust for inset
        RECT rcInset = GetInset();
        rc.left += rcInset.left;
        rc.top += rcInset.top;
        rc.right -= rcInset.right;
        rc.bottom -= rcInset.bottom;
        CDuiScrollBar* pVerticalScrollBar = GetVerticalScrollBar();
        CDuiScrollBar* pHorizontalScrollBar = GetHorizontalScrollBar();
        if(pVerticalScrollBar && pVerticalScrollBar->IsVisible())
        {
            rc.right -= pVerticalScrollBar->GetFixedWidth();
        }
        if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
        {
            rc.bottom -= pHorizontalScrollBar->GetFixedHeight();
        }

        if(GetCount() == 0)
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

        int cyNeeded = 0;
        int nAdjustables = 0;
        int cxFixed = 0;
        int nEstimateNum = 0;
        SIZE szControlAvailable;
        int iControlMaxWidth = 0;
        int iControlMaxHeight = 0;
        for(int it1 = 0; it1 < GetCount(); it1++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(GetItemAt(it1));
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
            szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
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
            if(sz.cx == 0)
            {
                nAdjustables++;
            }
            else
            {
                if(sz.cx < pControl->GetMinWidth())
                {
                    sz.cx = pControl->GetMinWidth();
                }
                if(sz.cx > pControl->GetMaxWidth())
                {
                    sz.cx = pControl->GetMaxWidth();
                }
            }
            cxFixed += sz.cx + pControl->GetPadding().left + pControl->GetPadding().right;

            sz.cy = MAX(sz.cy, 0);
            if(sz.cy < pControl->GetMinHeight())
            {
                sz.cy = pControl->GetMinHeight();
            }
            if(sz.cy > pControl->GetMaxHeight())
            {
                sz.cy = pControl->GetMaxHeight();
            }
            cyNeeded = MAX(cyNeeded, sz.cy + rcPadding.top + rcPadding.bottom);
            nEstimateNum++;
        }
        cxFixed += (nEstimateNum - 1) * GetChildPadding();

        // Place elements
        int cxNeeded = 0;
        int cxExpand = 0;
        if(nAdjustables > 0)
        {
            cxExpand = MAX(0, (szAvailable.cx - cxFixed) / nAdjustables);
        }
        // Position the elements
        SIZE szRemaining = szAvailable;
        int iPosX = rc.left;
        if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
        {
            iPosX -= pHorizontalScrollBar->GetScrollPos();
        }
        int iEstimate = 0;
        int iAdjustable = 0;
        int cxFixedRemaining = cxFixed;
        for(int it2 = 0; it2 < GetCount(); it2++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(GetItemAt(it2));
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
            szRemaining.cx -= rcPadding.left;

            szControlAvailable = szRemaining;
            szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
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
            cxFixedRemaining = cxFixedRemaining - (rcPadding.left + rcPadding.right);
            if(iEstimate > 1)
            {
                cxFixedRemaining = cxFixedRemaining - GetChildPadding();
            }
            SIZE sz = pControl->EstimateSize(szControlAvailable);
            if(sz.cx == 0)
            {
                iAdjustable++;
                sz.cx = cxExpand;
                // Distribute remaining to last element (usually round-off left-overs)
                if(iAdjustable == nAdjustables)
                {
                    sz.cx = MAX(0, szRemaining.cx - rcPadding.right - cxFixedRemaining);
                }
                if(sz.cx < pControl->GetMinWidth())
                {
                    sz.cx = pControl->GetMinWidth();
                }
                if(sz.cx > pControl->GetMaxWidth())
                {
                    sz.cx = pControl->GetMaxWidth();
                }
            }
            else
            {
                if(sz.cx < pControl->GetMinWidth())
                {
                    sz.cx = pControl->GetMinWidth();
                }
                if(sz.cx > pControl->GetMaxWidth())
                {
                    sz.cx = pControl->GetMaxWidth();
                }
                cxFixedRemaining -= sz.cx;
            }

            sz.cy = pControl->GetMaxHeight();
            if(sz.cy == 0)
            {
                sz.cy = szAvailable.cy - rcPadding.top - rcPadding.bottom;
            }
            if(sz.cy < 0)
            {
                sz.cy = 0;
            }
            if(sz.cy > szControlAvailable.cy)
            {
                sz.cy = szControlAvailable.cy;
            }
            if(sz.cy < pControl->GetMinHeight())
            {
                sz.cy = pControl->GetMinHeight();
            }

            UINT iChildAlign = GetChildVAlign();
            if(iChildAlign == DT_VCENTER)
            {
                int iPosY = (rc.bottom + rc.top) / 2;
                if(pVerticalScrollBar && pVerticalScrollBar->IsVisible())
                {
                    iPosY += pVerticalScrollBar->GetScrollRange() / 2;
                    iPosY -= pVerticalScrollBar->GetScrollPos();
                }
                RECT rcCtrl = { iPosX + rcPadding.left, iPosY - sz.cy / 2, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy - sz.cy / 2 };
                pControl->SetPos(rcCtrl, FALSE);
            }
            else if(iChildAlign == DT_BOTTOM)
            {
                int iPosY = rc.bottom;
                if(pVerticalScrollBar && pVerticalScrollBar->IsVisible())
                {
                    iPosY += pVerticalScrollBar->GetScrollRange();
                    iPosY -= pVerticalScrollBar->GetScrollPos();
                }
                RECT rcCtrl = { iPosX + rcPadding.left, iPosY - rcPadding.bottom - sz.cy, iPosX + sz.cx + rcPadding.left, iPosY - rcPadding.bottom };
                pControl->SetPos(rcCtrl, FALSE);
            }
            else
            {
                int iPosY = rc.top;
                if(pVerticalScrollBar && pVerticalScrollBar->IsVisible())
                {
                    iPosY -= pVerticalScrollBar->GetScrollPos();
                }
                RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy + rcPadding.top };
                pControl->SetPos(rcCtrl, FALSE);
            }

            iPosX += sz.cx + GetChildPadding() + rcPadding.left + rcPadding.right;
            cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
            szRemaining.cx -= sz.cx + GetChildPadding() + rcPadding.right;
        }
        cxNeeded += (nEstimateNum - 1) * GetChildPadding();

        // Process the scrollbar
        ProcessScrollBar(rc, cxNeeded, cyNeeded);
    }

    RECT CDuiHorizontalLayout::GetThumbRect(BOOL bUseNew /*= FALSE*/) const
    {
        if((m_uButtonState & UISTATE_CAPTURED) != 0 && bUseNew)
        {
            if(m_iSepWidth >= 0)
            {
                return CDuiRect(m_rcNewPos.right - m_iSepWidth, m_rcNewPos.top, m_rcNewPos.right, m_rcNewPos.bottom);
            }
            else
            {
                return CDuiRect(m_rcNewPos.left, m_rcNewPos.top, m_rcNewPos.left - m_iSepWidth, m_rcNewPos.bottom);
            }
        }
        else
        {
            if(m_iSepWidth >= 0)
            {
                return CDuiRect(GetPos().right - m_iSepWidth, GetPos().top, GetPos().right, GetPos().bottom);
            }
            else
            {
                return CDuiRect(GetPos().left, GetPos().top, GetPos().left - m_iSepWidth, GetPos().bottom);
            }
        }
    }

    void CDuiHorizontalLayout::DoEvent(TEventUI& event)
    {
        if(m_iSepWidth != 0)
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
                    LONG cx = event.ptMouse.x - ptLastMouse.x;
                    ptLastMouse = event.ptMouse;
                    RECT rc = m_rcNewPos;
                    if(m_iSepWidth >= 0)
                    {
                        if(cx > 0 && event.ptMouse.x < m_rcNewPos.right - m_iSepWidth)
                        {
                            return;
                        }
                        if(cx < 0 && event.ptMouse.x > m_rcNewPos.right)
                        {
                            return;
                        }
                        rc.right += cx;
                        if(rc.right - rc.left <= GetMinWidth())
                        {
                            if(m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth())
                            {
                                return;
                            }
                            rc.right = rc.left + GetMinWidth();
                        }
                        if(rc.right - rc.left >= GetMaxWidth())
                        {
                            if(m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth())
                            {
                                return;
                            }
                            rc.right = rc.left + GetMaxWidth();
                        }
                    }
                    else
                    {
                        if(cx > 0 && event.ptMouse.x < m_rcNewPos.left)
                        {
                            return;
                        }
                        if(cx < 0 && event.ptMouse.x > m_rcNewPos.left - m_iSepWidth)
                        {
                            return;
                        }
                        rc.left += cx;
                        if(rc.right - rc.left <= GetMinWidth())
                        {
                            if(m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth())
                            {
                                return;
                            }
                            rc.left = rc.right - GetMinWidth();
                        }
                        if(rc.right - rc.left >= GetMaxWidth())
                        {
                            if(m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth())
                            {
                                return;
                            }
                            rc.left = rc.right - GetMaxWidth();
                        }
                    }

                    CDuiRect rcInvalidate = GetThumbRect(TRUE);
                    m_rcNewPos = rc;
                    SetFixedWidth(m_rcNewPos.right - m_rcNewPos.left);

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
                    ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
                    return;
                }
            }
        }
        CDuiContainer::DoEvent(event);
    }

    void CDuiHorizontalLayout::DoPostPaint(HDC hDC, const RECT& rcPaint)
    {
        if((m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode)
        {
            RECT rcSeparator = GetThumbRect(TRUE);
            CRenderEngine::DrawColor(hDC, rcSeparator, 0xAA000000);
        }
    }

}
