#include "StdAfx.h"
#include "DuiRadio.h"


namespace DuiLib
{

    IMPLEMENT_DUICONTROL(CDuiRadio)
    CDuiRadio::CDuiRadio(void)
    {
    }


    CDuiRadio::~CDuiRadio(void)
    {
    }

    CDuiString CDuiRadio::GetClass() const
    {
        return DUI_CTR_RADIO;
    }

    LPVOID CDuiRadio::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_RADIO) == 0)
        {
            return static_cast<CDuiRadio*>(this);
        }
        return CDuiOption::GetInterface(pstrName);
    }

    void CDuiRadio::DoEvent(CDuiEvent& event)
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
        if(event.Type == DUIEVENT_BUTTONDOWN || event.Type == DUIEVENT_DBLCLICK)
        {
            Selected(TRUE);
            return;
        }
        CDuiOption::DoEvent(event);
    }

}
