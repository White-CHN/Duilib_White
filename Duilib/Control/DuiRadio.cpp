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

    void CDuiRadio::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
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
        if(event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
        {
            Selected(TRUE);
            return;
        }
        CDuiOption::DoEvent(event);
    }

}
