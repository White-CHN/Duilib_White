#include "StdAfx.h"
#include "DuiControlFactory.h"

namespace DuiLib
{
    CDuiControlFactory::CDuiControlFactory()
    {
        INNER_REGISTER_DUICONTROL(CDuiControl);
        INNER_REGISTER_DUICONTROL(CDuiContainer);
        INNER_REGISTER_DUICONTROL(CDuiVerticalLayout);
        INNER_REGISTER_DUICONTROL(CDuiHorizontalLayout);
        INNER_REGISTER_DUICONTROL(CDuiScrollBar);
        INNER_REGISTER_DUICONTROL(CDuiLabel);
        INNER_REGISTER_DUICONTROL(CDuiButton);
        INNER_REGISTER_DUICONTROL(CDuiEdit);
        INNER_REGISTER_DUICONTROL(CDuiOption);
        INNER_REGISTER_DUICONTROL(CDuiRadio);
        INNER_REGISTER_DUICONTROL(CDuiProgress);
        INNER_REGISTER_DUICONTROL(CDuiSlider);
    }

    CDuiControlFactory::~CDuiControlFactory()
    {
    }

    CDuiControl* CDuiControlFactory::CreateControl(CDuiString strClassName)
    {
        MAP_DUI_CTRATECLASS::iterator iter = m_mapControl.find(strClassName);
        if(iter == m_mapControl.end())
        {
            return NULL;
        }
        else
        {
            return (CDuiControl*)(iter->second());
        }
    }

    void CDuiControlFactory::RegistControl(CDuiString strClassName, CreateClass pFunc)
    {
        m_mapControl.insert(MAP_DUI_CTRATECLASS::value_type(strClassName, pFunc));
    }

    CDuiControlFactory* CDuiControlFactory::GetInstance()
    {
        static CDuiControlFactory* pInstance = new CDuiControlFactory;
        return pInstance;
    }

    void CDuiControlFactory::Release()
    {
        delete this;
    }
}