#include "StdAfx.h"
#include "DuiActiveX.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiActiveX)

    CDuiActiveX::CDuiActiveX(void)
    {
    }


    CDuiActiveX::~CDuiActiveX(void)
    {
    }


    CDuiString CDuiActiveX::GetClass() const
    {
        return DUI_CTR_ACTIVEX;
    }

    LPVOID CDuiActiveX::GetInterface(LPCTSTR pstrName)
    {
        if(_tcscmp(pstrName, DUI_CTR_ACTIVEX) == 0)
        {
            return static_cast<CDuiActiveX*>(this);
        }
        return CDuiControl::GetInterface(pstrName);
    }

    LRESULT CDuiActiveX::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

}