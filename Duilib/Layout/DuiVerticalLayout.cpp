#include "StdAfx.h"
#include "DuiVerticalLayout.h"

namespace DuiLib
{

    IMPLEMENT_DUICONTROL(CDuiVerticalLayout)

    CDuiVerticalLayout::CDuiVerticalLayout(void)
    {
    }


    CDuiVerticalLayout::~CDuiVerticalLayout(void)
    {
    }

    LPCTSTR CDuiVerticalLayout::GetClass() const
    {
        return _T("VerticalLayout");
    }

    void CDuiVerticalLayout::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::SetPos(rc, bNeedInvalidate);
        rc = GetPos();
        // Adjust for inset
        RECT rcInset = GetInset();
        if(GetManager())
        {
            GetManager()->GetDPIObj()->Scale(&rcInset);
        }
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
        int cxNeeded = 0;
        int nAdjustables = 0;
        int cyFixed = 0;
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
            if(pControl->GetFixedWidth() <= 0)
            {
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

}
