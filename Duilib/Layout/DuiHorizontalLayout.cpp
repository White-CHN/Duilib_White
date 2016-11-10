#include "StdAfx.h"
#include "DuiHorizontalLayout.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiHorizontalLayout)
    CDuiHorizontalLayout::CDuiHorizontalLayout(void)
    {
    }


    CDuiHorizontalLayout::~CDuiHorizontalLayout(void)
    {
    }

    LPCTSTR CDuiHorizontalLayout::GetClass() const
    {
        return _T("HorizontalLayout");
    }

    void CDuiHorizontalLayout::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::SetPos(rc, bNeedInvalidate);
        rc = m_rcItem;
        SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
        int cyNeeded = 0;
        int nAdjustables = 0;
        int cxFixed = 0;
        int nEstimateNum = 0;
        SIZE szControlAvailable;
        int iControlMaxWidth = 0;
        int iControlMaxHeight = 0;
        for(int it1 = 0; it1 < m_items.GetSize(); it1++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(m_items[it1]);
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
        cxFixed += (nEstimateNum - 1) * m_iChildPadding;
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
        /*if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        iPosX -= m_pHorizontalScrollBar->GetScrollPos();
        }*/
        int iEstimate = 0;
        int iAdjustable = 0;
        int cxFixedRemaining = cxFixed;
        for(int it2 = 0; it2 < m_items.GetSize(); it2++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(m_items[it2]);
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
                cxFixedRemaining = cxFixedRemaining - m_iChildPadding;
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
            if(pControl->GetFixedHeight() <= 0)
            {
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
            }
            UINT iChildAlign = GetChildVAlign();
            if(iChildAlign == DT_VCENTER)
            {
                int iPosY = (rc.bottom + rc.top) / 2;
                /*if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
                iPosY += m_pVerticalScrollBar->GetScrollRange() / 2;
                iPosY -= m_pVerticalScrollBar->GetScrollPos();
                }*/
                RECT rcCtrl = { iPosX + rcPadding.left, iPosY - sz.cy / 2, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy - sz.cy / 2 };
                pControl->SetPos(rcCtrl, false);
            }
            else if(iChildAlign == DT_BOTTOM)
            {
                int iPosY = rc.bottom;
                /*if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
                iPosY += m_pVerticalScrollBar->GetScrollRange();
                iPosY -= m_pVerticalScrollBar->GetScrollPos();
                }*/
                RECT rcCtrl = { iPosX + rcPadding.left, iPosY - rcPadding.bottom - sz.cy, iPosX + sz.cx + rcPadding.left, iPosY - rcPadding.bottom };
                pControl->SetPos(rcCtrl, false);
            }
            else
            {
                int iPosY = rc.top;
                /*if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
                iPosY -= m_pVerticalScrollBar->GetScrollPos();
                }*/
                RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy + rcPadding.top };
                pControl->SetPos(rcCtrl, false);
            }
            iPosX += sz.cx + m_iChildPadding + rcPadding.left + rcPadding.right;
            cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
            szRemaining.cx -= sz.cx + m_iChildPadding + rcPadding.right;
        }
        cxNeeded += (nEstimateNum - 1) * m_iChildPadding;
    }

}
