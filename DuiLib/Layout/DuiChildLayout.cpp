#include "StdAfx.h"
#include "DuiChildLayout.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiChildLayout)

    CDuiChildLayout::CDuiChildLayout(void)
    {
    }


    CDuiChildLayout::~CDuiChildLayout(void)
    {
    }


    LPVOID CDuiChildLayout::GetInterface(CDuiString strName)
    {
        if(strName == DUI_CTR_CHILDLAYOUT)
        {
            return static_cast<CDuiChildLayout*>(this);
        }
        return CDuiContainer::GetInterface(strName);
    }

    CDuiString CDuiChildLayout::GetClass() const
    {
        return DUI_CTR_CHILDLAYOUT;
    }

    void CDuiChildLayout::Init()
    {
        if(!m_pstrXMLFile.IsEmpty())
        {
            CDuiDlgBuilder builder;
            CDuiContainer* pChildWindow = static_cast<CDuiContainer*>(builder.Create(m_pstrXMLFile.GetData(), (UINT)0, NULL, GetManager()));
            if(pChildWindow)
            {
                Add(pChildWindow);
            }
            else
            {
                RemoveAll();
            }
        }
    }

    CDuiString CDuiChildLayout::GetChildLayoutXML() const
    {
        return m_pstrXMLFile;
    }

    void CDuiChildLayout::SetChildLayoutXML(CDuiString pXML)
    {
        m_pstrXMLFile = pXML;
    }

    void CDuiChildLayout::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("xmlfile")) == 0)
        {
            SetChildLayoutXML(pstrValue);
        }
        else
        {
            CDuiContainer::SetAttribute(pstrName, pstrValue);
        }
    }

}