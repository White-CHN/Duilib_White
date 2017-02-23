#include "StdAfx.h"
#include "DuiCheckBox.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiCheckBox)

    CDuiCheckBox::CDuiCheckBox(void)
        : m_bAutoCheck(FALSE)
    {
    }


    CDuiCheckBox::~CDuiCheckBox(void)
    {
    }

    CDuiString CDuiCheckBox::GetClass() const
    {
        return DUI_CTR_CHECKBOX;
    }

    LPVOID CDuiCheckBox::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_CHECKBOX) == 0)
        {
            return static_cast<CDuiCheckBox*>(this);
        }
        return CDuiOption::GetInterface(pstrName);
    }


    BOOL CDuiCheckBox::IsAutoCheck() const
    {
        return m_bAutoCheck;
    }

    void CDuiCheckBox::SetAutoCheck(BOOL bEnable)
    {
        m_bAutoCheck = bEnable;
    }

    void CDuiCheckBox::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("EnableAutoCheck")) == 0)
        {
            SetAutoCheck(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else
        {
            CDuiOption::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiCheckBox::DoEvent(CDuiEvent& event)
    {
        if(!IsMouseEnabled() && event.Type > DUIEVENT__MOUSEBEGIN && event.Type < DUIEVENT__MOUSEEND)
        {
            if(GetParent() != NULL)
            {
                GetParent()->DoEvent(event);
            }
            else
            {
                CDuiOption::DoEvent(event);
            }
            return;
        }
        if(m_bAutoCheck && (event.Type == DUIEVENT_BUTTONDOWN || event.Type == DUIEVENT_DBLCLICK))
        {
            if(::PtInRect(&GetPos(), event.ptMouse) && IsEnabled())
            {
                Selected(!IsSelected());
                GetManager()->SendNotify(this, DUI_MSGTYPE_CHECKCLICK, 0, 0);
                Invalidate();
            }
            return;
        }
        CDuiOption::DoEvent(event);
    }

}