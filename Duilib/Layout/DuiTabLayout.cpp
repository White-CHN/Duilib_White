#include "StdAfx.h"
#include "DuiTabLayout.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiTabLayout)

    CDuiTabLayout::CDuiTabLayout(void)
        : m_iCurSel(-1)
    {
    }


    CDuiTabLayout::~CDuiTabLayout(void)
    {
    }

    LPCTSTR CDuiTabLayout::GetClass() const
    {
        return DUI_CTR_TABLAYOUT;
    }

    LPVOID CDuiTabLayout::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_TABLAYOUT) == 0)
        {
            return static_cast<CDuiTabLayout*>(this);
        }
        return CDuiContainer::GetInterface(pstrName);
    }

    BOOL CDuiTabLayout::Add(CDuiControl* pControl)
    {
        BOOL ret = CDuiContainer::Add(pControl);
        if(!ret)
        {
            return ret;
        }

        if(m_iCurSel == -1 && pControl->IsVisible())
        {
            m_iCurSel = GetItemIndex(pControl);
        }
        else
        {
            pControl->SetVisible(FALSE);
        }

        return ret;
    }

    BOOL CDuiTabLayout::AddAt(CDuiControl* pControl, int iIndex)
    {
        BOOL ret = CDuiContainer::AddAt(pControl, iIndex);
        if(!ret)
        {
            return ret;
        }

        if(m_iCurSel == -1 && pControl->IsVisible())
        {
            m_iCurSel = GetItemIndex(pControl);
        }
        else if(m_iCurSel != -1 && iIndex <= m_iCurSel)
        {
            m_iCurSel += 1;
        }
        else
        {
            pControl->SetVisible(FALSE);
        }

        return ret;
    }

    BOOL CDuiTabLayout::Remove(CDuiControl* pControl)
    {
        if(pControl == NULL)
        {
            return FALSE;
        }

        int index = GetItemIndex(pControl);
        BOOL ret = CDuiContainer::Remove(pControl);
        if(!ret)
        {
            return FALSE;
        }

        if(m_iCurSel == index)
        {
            if(GetCount() > 0)
            {
                m_iCurSel = 0;
                GetItemAt(m_iCurSel)->SetVisible(TRUE);
            }
            else
            {
                m_iCurSel = -1;
            }
            NeedParentUpdate();
        }
        else if(m_iCurSel > index)
        {
            m_iCurSel -= 1;
        }

        return ret;
    }

    void CDuiTabLayout::RemoveAll()
    {
        m_iCurSel = -1;
        CDuiContainer::RemoveAll();
        NeedParentUpdate();
    }

    BOOL CDuiTabLayout::SelectItem(int iIndex)
    {
        if(iIndex < 0 || iIndex >= GetItems()->GetSize())
        {
            return FALSE;
        }
        if(iIndex == m_iCurSel)
        {
            return TRUE;
        }

        int iOldSel = m_iCurSel;
        m_iCurSel = iIndex;
        for(int it = 0; it < GetItems()->GetSize(); it++)
        {
            if(it == iIndex)
            {
                GetItemAt(it)->SetVisible(TRUE);
                GetItemAt(it)->SetFocus();
                SetPos(GetPos());
            }
            else
            {
                GetItemAt(it)->SetVisible(FALSE);
            }
        }
        NeedParentUpdate();

        if(GetManager() != NULL)
        {
            GetManager()->SetNextTabControl();
            GetManager()->SendNotify(this, DUI_MSGTYPE_TABSELECT, m_iCurSel, iOldSel);
        }
        return TRUE;
    }

    BOOL CDuiTabLayout::SelectItem(CDuiControl* pControl)
    {
        int iIndex = GetItemIndex(pControl);
        if(iIndex == -1)
        {
            return FALSE;
        }
        else
        {
            return SelectItem(iIndex);
        }
    }

    void CDuiTabLayout::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("selectedid")) == 0)
        {
            SelectItem(_ttoi(pstrValue));
        }
        return CDuiContainer::SetAttribute(pstrName, pstrValue);
    }

    void CDuiTabLayout::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::SetPos(rc, bNeedInvalidate);
        // Adjust for inset
        rc = GetPos();
        RECT rcInset = GetInset();
        rc.left += rcInset.left;
        rc.top += rcInset.top;
        rc.right -= rcInset.right;
        rc.bottom -= rcInset.bottom;

        for(int it = 0; it <  GetItems()->GetSize(); it++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(GetItemAt(it));
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                SetFloatPos(it);
                continue;
            }

            if(it != m_iCurSel)
            {
                continue;
            }

            RECT rcPadding = pControl->GetPadding();
            rc.left += rcPadding.left;
            rc.top += rcPadding.top;
            rc.right -= rcPadding.right;
            rc.bottom -= rcPadding.bottom;

            SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };

            SIZE sz = pControl->EstimateSize(szAvailable);
            if(sz.cx == 0)
            {
                sz.cx = MAX(0, szAvailable.cx);
            }
            if(sz.cx < pControl->GetMinWidth())
            {
                sz.cx = pControl->GetMinWidth();
            }
            if(sz.cx > pControl->GetMaxWidth())
            {
                sz.cx = pControl->GetMaxWidth();
            }

            if(sz.cy == 0)
            {
                sz.cy = MAX(0, szAvailable.cy);
            }
            if(sz.cy < pControl->GetMinHeight())
            {
                sz.cy = pControl->GetMinHeight();
            }
            if(sz.cy > pControl->GetMaxHeight())
            {
                sz.cy = pControl->GetMaxHeight();
            }

            RECT rcCtrl = { rc.left, rc.top, rc.left + sz.cx, rc.top + sz.cy};
            pControl->SetPos(rcCtrl);
        }
    }

}