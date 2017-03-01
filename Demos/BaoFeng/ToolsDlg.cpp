#include "StdAfx.h"
#include "ToolsDlg.h"

DUI_BEGIN_MESSAGE_MAP(CToolsDlg, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CToolsDlg::CToolsDlg(void)
    : m_pTabSwitch(NULL)
{
}


CToolsDlg::~CToolsDlg(void)
{
}

void CToolsDlg::InitWindow()
{
    m_pTabSwitch = static_cast<CDuiAnimationTabLayout*>(GetPaintManager()->FindControl(_T("tab_switch")));
}

CDuiString CToolsDlg::GetSkinFile()
{
    return _T("ToolsDlg.xml");
}

LPCTSTR CToolsDlg::GetWindowClassName() const
{
    return GET_CLASS_NAME(CToolsDlg);
}

void CToolsDlg::OnClick(CDuiNotify& msg)
{
    if(msg.pSender->GetName() == _T("closebtn"))
    {
        ShowWindow(FALSE);
        return;
    }
    else if(msg.pSender->GetName() == _T("PrevBtn"))
    {
        m_pTabSwitch->SelectItem(0);
        return;
    }
    else if(msg.pSender->GetName() == _T("NextBtn"))
    {
        m_pTabSwitch->SelectItem(1);
        return;
    }
}
