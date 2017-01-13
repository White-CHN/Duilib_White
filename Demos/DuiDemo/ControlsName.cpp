#include "stdafx.h"
#include "ControlsName.h"


CAttribute::CAttribute(void)
{

}

/////////////////////////////////////

CControl::CControl(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strRemarks = _T("�ؼ����ƣ�ͬһ�����ڱ���Ψһ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("height");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�ؼ�Ԥ��ĸ߶�");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("width");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�ؼ�Ԥ��Ŀ��");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("float");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�Ƿ�ʹ�þ��Զ�λ");
    m_vtAttributes.push_back(Attribute);
}
/////////////////////////////////////////

CControlsName::CControlsName(void)
{
    m_mapControlsName[DUI_CTR_CONTROL] = new CControl;
    //m_vtControlsName.push_back(DUI_CTR_LABEL);
    //m_vtControlsName.push_back(DUI_CTR_SCROLLBAR);
    //m_vtControlsName.push_back(DUI_CTR_BUTTON);
    //m_vtControlsName.push_back(DUI_CTR_OPTION);
    //m_vtControlsName.push_back(DUI_CTR_RADIO);
    //m_vtControlsName.push_back(DUI_CTR_EDIT);
    //m_vtControlsName.push_back(DUI_CTR_PROGRESS);
    //m_vtControlsName.push_back(DUI_CTR_SLIDER);
    //m_vtControlsName.push_back(DUI_CTR_LISTELEMENT);
    //m_vtControlsName.push_back(DUI_CTR_LISTLABELELEMENT);
    //m_vtControlsName.push_back(DUI_CTR_LISTTEXTELEMENT);
    //m_vtControlsName.push_back(DUI_CTR_DATETIME);
    //m_vtControlsName.push_back(DUI_CTR_IPADDRESS);
    //m_vtControlsName.push_back(DUI_CTR_RING);
    //m_vtControlsName.push_back(DUI_CTR_GIF);
    //m_vtControlsName.push_back(DUI_CTR_COLORPALETTE);

    //m_vtControlsName.push_back(DUI_CTR_CONTAINER);
    //m_vtControlsName.push_back(DUI_CTR_VERTICALLAYOUT);
    //m_vtControlsName.push_back(DUI_CTR_HORIZONTALLAYOUT);
    //m_vtControlsName.push_back(DUI_CTR_TABLAYOUT);
    //m_vtControlsName.push_back(DUI_CTR_ANIMATION_TABLAYOUT);
    //m_vtControlsName.push_back(DUI_CTR_COMBO);
    //m_vtControlsName.push_back(DUI_CTR_GROUPBOX);
    //m_vtControlsName.push_back(DUI_CTR_LISTHEADERITEM);
    //m_vtControlsName.push_back(DUI_CTR_LISTHEADER);
    //m_vtControlsName.push_back(DUI_CTR_LIST);
    //m_vtControlsName.push_back(DUI_CTR_LISTCONTAINERELEMENT);
    //m_vtControlsName.push_back(DUI_CTR_MENU);
    //m_vtControlsName.push_back(DUI_CTR_MENUELEMENT);
}


CControlsName::~CControlsName(void)
{
    for(map<CDuiString, CControl*>::iterator it = m_mapControlsName.begin(); it != m_mapControlsName.end();)
    {
        CControl* pControl = it->second;
        it = m_mapControlsName.erase(it);
        DUI_FREE_POINT(pControl);
    }
}


