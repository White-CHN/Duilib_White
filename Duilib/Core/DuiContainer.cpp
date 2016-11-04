#include "StdAfx.h"
#include "DuiContainer.h"

namespace DuiLib {

	IMPLEMENT_DUICONTROL(CDuiContainer)

	CDuiContainer::CDuiContainer(void)
		: m_bAutoDestroy(true)
		, m_bDelayedDestroy(true)
		, m_iChildPadding(0)
		, m_iChildAlign(DT_LEFT)
		, m_iChildVAlign(DT_TOP)
	{
		ZeroMemory(&m_rcInset,sizeof(m_rcInset));
	}


	CDuiContainer::~CDuiContainer(void)
	{
	}


	LPVOID CDuiContainer::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("IContainer")) == 0 ) 
		{
			return static_cast<IContainer*>(this);
		}
		else if( _tcsicmp(pstrName, DUI_CTR_CONTAINER) == 0 ) 
		{
			return static_cast<CDuiContainer*>(this);
		}
		return __super::GetInterface(pstrName);
	}

	LPCTSTR CDuiContainer::GetClass() const
	{
		return _T("Container");
	}

	RECT CDuiContainer::GetInset() const
	{
		return GetManager()->GetDPIObj()->Scale(m_rcInset);
	}

	UINT CDuiContainer::GetChildAlign() const
	{
		return m_iChildAlign;
	}

	UINT CDuiContainer::GetChildVAlign() const
	{
		return m_iChildVAlign;
	}

	CDuiControl* CDuiContainer::GetItemAt(int iIndex) const
	{
		if( iIndex < 0 || iIndex >= m_items.GetSize() ) 
		{
			return NULL;
		}
		return static_cast<CDuiControl*>(m_items[iIndex]);
	}

	int CDuiContainer::GetItemIndex(CDuiControl* pControl) const
	{
		for( int i = 0; i < m_items.GetSize(); i++ ) 
		{
			if( static_cast<CDuiControl*>(m_items[i]) == pControl ) 
			{
				return i;
			}
		}

		return -1;
	}

	BOOL CDuiContainer::SetItemIndex(CDuiControl* pControl, int iIndex)
	{
		for( int it = 0; it < m_items.GetSize(); it++ ) 
		{
			if( static_cast<CDuiControl*>(m_items[it]) == pControl ) 
			{
				NeedUpdate();            
				m_items.Remove(it);
				return m_items.InsertAt(iIndex, pControl);
			}
		}

		return false;
	}

	int CDuiContainer::GetCount() const
	{
		return m_items.GetSize();
	}

	BOOL CDuiContainer::Add(CDuiControl* pControl)
	{
		if( pControl == NULL) 
		{
			return false;
		}

		if( m_pManager != NULL ) 
		{
			m_pManager->InitControls(pControl, this);
		}
		if( IsVisible() ) 
		{
			NeedUpdate();
		}
		else 
		{
			pControl->SetInternVisible(false);
		}
		return m_items.Add(pControl);   
	}

	BOOL CDuiContainer::AddAt(CDuiControl* pControl, int iIndex)
	{
		if( pControl == NULL) return false;

		if( m_pManager != NULL ) 
		{
			m_pManager->InitControls(pControl, this);
		}
		if( IsVisible() ) 
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
		if( pControl == NULL) return false;

		for( int it = 0; it < m_items.GetSize(); it++ ) 
		{
			if( static_cast<CDuiControl*>(m_items[it]) == pControl ) 
			{
				NeedUpdate();
				if( m_bAutoDestroy ) 
				{
					if( m_bDelayedDestroy && m_pManager ) 
					{
						m_pManager->AddDelayedCleanup(pControl);
					}
					else 
					{
						delete pControl;
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
		if (pControl != NULL) 
		{
			return Remove(pControl);
		}

		return false;
	}

	void CDuiContainer::RemoveAll()
	{
		for( int it = 0; m_bAutoDestroy && it < m_items.GetSize(); it++ ) 
		{
			CDuiControl* pItem = static_cast<CDuiControl*>(m_items[it]);
			if( m_bDelayedDestroy && m_pManager ) 
			{
				m_pManager->AddDelayedCleanup(pItem);             
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
		if( m_items.IsEmpty() ) 
		{
			return;
		}
		rc = m_rcItem;
		rc.left += m_rcInset.left;
		rc.top += m_rcInset.top;
		rc.right -= m_rcInset.right;
		rc.bottom -= m_rcInset.bottom;
		for( int i = 0; i < m_items.GetSize(); i++ ) 
		{
			CDuiControl* pControl = static_cast<CDuiControl*>(m_items[i]);
			if( !pControl->IsVisible() ) 
			{
				continue;
			}
			if( pControl->IsFloat() ) 
			{
				SetFloatPos(i);
			}
			else 
			{ 
				SIZE sz = { rc.right - rc.left, rc.bottom - rc.top };
				if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
				if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
				RECT rcCtrl = { rc.left, rc.top, rc.left + sz.cx, rc.top + sz.cy };
				pControl->SetPos(rcCtrl, FALSE);
			}
		}
	}

	void CDuiContainer::DoPaint(HDC hDC, const RECT& rcPaint)
	{
		RECT rcTemp = { 0 };
		if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) 
		{
			return;
		}
		CRenderClip clip;
		CRenderClip::GenerateClip(hDC, rcTemp, clip);
		__super::DoPaint(hDC, rcPaint);

		if( m_items.GetSize() > 0 ) 
		{
			//RECT rcInset = GetInset();
			RECT rc = m_rcItem;
			//rc.left += rcInset.left;
			//rc.top += rcInset.top;
			//rc.right -= rcInset.right;
			//rc.bottom -= rcInset.bottom;
			//if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
			//if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

			if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) 
			{
				for( int it = 0; it < m_items.GetSize(); it++ ) 
				{
					CDuiControl* pControl = static_cast<CDuiControl*>(m_items[it]);
					if( !pControl->IsVisible() ) 
					{
						continue;
					}
					if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) 
					{
						continue;
					}
					if( pControl ->IsFloat() ) 
					{
						if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) 
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
				for( int it = 0; it < m_items.GetSize(); it++ ) 
				{
					CDuiControl* pControl = static_cast<CDuiControl*>(m_items[it]);
					if( !pControl->IsVisible() ) 
					{
						continue;
					}
					if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) 
					{
						continue;
					}
					if( pControl ->IsFloat() ) 
					{
						if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) 
						{
							continue;
						}
						CRenderClip::UseOldClipBegin(hDC, childClip);
						pControl->DoPaint(hDC, rcPaint);
						CRenderClip::UseOldClipEnd(hDC, childClip);
					}
					else 
					{
						if( !::IntersectRect(&rcTemp, &rc, &pControl->GetPos()) ) 
						{
							continue;
						}
						pControl->DoPaint(hDC, rcPaint);
					}
				}
			}
		}
	}

	void CDuiContainer::SetFloatPos(int iIndex)
	{
		// 因为CControlUI::SetPos对float的操作影响，这里不能对float组件添加滚动条的影响
		if( iIndex < 0 || iIndex >= m_items.GetSize() ) 
		{
			return;
		}
		CDuiControl* pControl = static_cast<CDuiControl*>(m_items[iIndex]);

		if( !pControl->IsVisible() || !pControl->IsFloat()) 
		{
			return;
		}

		SIZE szXY = pControl->GetFixedXY();
		SIZE sz = {pControl->GetFixedWidth(), pControl->GetFixedHeight()};
		TPercentInfo rcPercent = pControl->GetFloatPercent();
		LONG width = m_rcItem.right - m_rcItem.left;
		LONG height = m_rcItem.bottom - m_rcItem.top;
		RECT rcCtrl = { 0 };
		rcCtrl.left = (LONG)(width*rcPercent.left) + szXY.cx+ m_rcItem.left;
		rcCtrl.top = (LONG)(height*rcPercent.top) + szXY.cy+ m_rcItem.top;
		rcCtrl.right = (LONG)(width*rcPercent.right) + szXY.cx + sz.cx+ m_rcItem.left;
		rcCtrl.bottom = (LONG)(height*rcPercent.bottom) + szXY.cy + sz.cy+ m_rcItem.top;
		pControl->SetPos(rcCtrl, false);
	}

}