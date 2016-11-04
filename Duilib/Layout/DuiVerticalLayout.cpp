#include "StdAfx.h"
#include "DuiVerticalLayout.h"

namespace DuiLib {

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
		rc = m_rcItem;

		SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };

		int cxNeeded = 0;
		int nAdjustables = 0;
		int cyFixed = 0;
		int nEstimateNum = 0;
		SIZE szControlAvailable;
		int iControlMaxWidth = 0;
		int iControlMaxHeight = 0;
		for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) 
		{
			CDuiControl* pControl = static_cast<CDuiControl*>(m_items[it1]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) continue;
			szControlAvailable = szAvailable;
			RECT rcPadding = pControl->GetPadding();
			szControlAvailable.cx -= rcPadding.left + rcPadding.right;
			iControlMaxWidth = pControl->GetFixedWidth();
			iControlMaxHeight = pControl->GetFixedHeight();
			if (iControlMaxWidth <= 0) iControlMaxWidth = pControl->GetMaxWidth(); 
			if (iControlMaxHeight <= 0) iControlMaxHeight = pControl->GetMaxHeight();
			if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
			if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
			SIZE sz = pControl->EstimateSize(szControlAvailable);
			if( sz.cy == 0 ) 
			{
				nAdjustables++;
			}
			else 
			{
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			}
			cyFixed += sz.cy + pControl->GetPadding().top + pControl->GetPadding().bottom;

			sz.cx = MAX(sz.cx, 0);
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
			cxNeeded = MAX(cxNeeded, sz.cx + rcPadding.left + rcPadding.right);
			nEstimateNum++;
		}
		cyFixed += (nEstimateNum - 1) * m_iChildPadding;

		// Place elements
		int cyNeeded = 0;
		int cyExpand = 0;
		if( nAdjustables > 0 ) cyExpand = MAX(0, (szAvailable.cy - cyFixed) / nAdjustables);
		// Position the elements
		SIZE szRemaining = szAvailable;
		int iPosY = rc.top;
		/*if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
		iPosY -= m_pVerticalScrollBar->GetScrollPos();
		}*/

		int iEstimate = 0;
		int iAdjustable = 0;
		int cyFixedRemaining = cyFixed;
		for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) 
		{
			CDuiControl* pControl = static_cast<CDuiControl*>(m_items[it2]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) 
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
			if (iControlMaxWidth <= 0) iControlMaxWidth = pControl->GetMaxWidth(); 
			if (iControlMaxHeight <= 0) iControlMaxHeight = pControl->GetMaxHeight();
			if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
			if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
			cyFixedRemaining = cyFixedRemaining - (rcPadding.top + rcPadding.bottom);
			if (iEstimate > 1) cyFixedRemaining = cyFixedRemaining - m_iChildPadding;
			SIZE sz = pControl->EstimateSize(szControlAvailable);
			if( sz.cy == 0 ) 
			{
				iAdjustable++;
				sz.cy = cyExpand;
				// Distribute remaining to last element (usually round-off left-overs)
				if( iAdjustable == nAdjustables ) 
				{
					sz.cy = MAX(0, szRemaining.cy - rcPadding.bottom - cyFixedRemaining);
				} 
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			}
			else 
			{
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
				cyFixedRemaining -= sz.cy;
			}

			sz.cx = pControl->GetMaxWidth();
			if( sz.cx == 0 ) sz.cx = szAvailable.cx - rcPadding.left - rcPadding.right;
			if( sz.cx < 0 ) sz.cx = 0;
			if( sz.cx > szControlAvailable.cx ) sz.cx = szControlAvailable.cx;
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();

			UINT iChildAlign = GetChildAlign(); 
			if (iChildAlign == DT_CENTER) {
				int iPosX = (rc.right + rc.left) / 2;
				/*if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
				iPosX += m_pHorizontalScrollBar->GetScrollRange() / 2;
				iPosX -= m_pHorizontalScrollBar->GetScrollPos();
				}*/
				RECT rcCtrl = { iPosX - sz.cx/2, iPosY + rcPadding.top, iPosX + sz.cx - sz.cx/2, iPosY + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);
			}
			else if (iChildAlign == DT_RIGHT) {
				int iPosX = rc.right;
				/*if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
				iPosX += m_pHorizontalScrollBar->GetScrollRange();
				iPosX -= m_pHorizontalScrollBar->GetScrollPos();
				}*/
				RECT rcCtrl = { iPosX - rcPadding.right - sz.cx, iPosY + rcPadding.top, iPosX - rcPadding.right, iPosY + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);
			}
			else {
				int iPosX = rc.left;
				/*if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
				iPosX -= m_pHorizontalScrollBar->GetScrollPos();
				}*/
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, iPosY + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);
			}

			iPosY += sz.cy + m_iChildPadding + rcPadding.top + rcPadding.bottom;
			cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
			szRemaining.cy -= sz.cy + m_iChildPadding + rcPadding.bottom;
		}
		cyNeeded += (nEstimateNum - 1) * m_iChildPadding;
	}

}
