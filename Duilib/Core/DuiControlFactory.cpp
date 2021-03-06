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
        INNER_REGISTER_DUICONTROL(CDuiTabLayout);
        INNER_REGISTER_DUICONTROL(CDuiCombo);
        INNER_REGISTER_DUICONTROL(CDuiListElement);
        INNER_REGISTER_DUICONTROL(CDuiListLabelElement);
        INNER_REGISTER_DUICONTROL(CDuiListTextElement);
        INNER_REGISTER_DUICONTROL(CDuiGroupBox);
        INNER_REGISTER_DUICONTROL(CDuiDateTime);
        INNER_REGISTER_DUICONTROL(CDuiIPAddress);
        INNER_REGISTER_DUICONTROL(CDuiAnimationTabLayout);
        INNER_REGISTER_DUICONTROL(CDuiRing);
        INNER_REGISTER_DUICONTROL(CDuiGif);
        INNER_REGISTER_DUICONTROL(CDuiColorPalette);
        INNER_REGISTER_DUICONTROL(CDuiChart);
        INNER_REGISTER_DUICONTROL(CDuiList);
        INNER_REGISTER_DUICONTROL(CDuiListHeader);
        INNER_REGISTER_DUICONTROL(CDuiListHeaderItem);
        INNER_REGISTER_DUICONTROL(CDuiListContainerElement);
        INNER_REGISTER_DUICONTROL(CDuiMenu);
        INNER_REGISTER_DUICONTROL(CDuiMenuElement);
        INNER_REGISTER_DUICONTROL(CDuiFadeButton);
        INNER_REGISTER_DUICONTROL(CDuiCheckBox);
        INNER_REGISTER_DUICONTROL(CDuiTileLayout);
        INNER_REGISTER_DUICONTROL(CDuiWater);
        INNER_REGISTER_DUICONTROL(CDuiChildLayout);
        INNER_REGISTER_DUICONTROL(CDuiRichEdit);
        INNER_REGISTER_DUICONTROL(CDuiActiveX);
        INNER_REGISTER_DUICONTROL(CDuiRollText);
        INNER_REGISTER_DUICONTROL(CDuiWebBrowser);
        INNER_REGISTER_DUICONTROL(CDuiTreeNode);
        INNER_REGISTER_DUICONTROL(CDuiTreeView);
    }

    CDuiControlFactory::~CDuiControlFactory()
    {

    }

    CDuiControl* CDuiControlFactory::CreateControl(CDuiString strClassName)
    {
        strClassName.MakeLower();
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
        strClassName.MakeLower();
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