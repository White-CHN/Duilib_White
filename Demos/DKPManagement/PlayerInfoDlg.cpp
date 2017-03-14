#include "StdAfx.h"
#include "PlayerInfoDlg.h"
#include "DKPManageFrame.h"

DUI_BEGIN_MESSAGE_MAP(CPlayerInfoDlg, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CPlayerInfoDlg::CPlayerInfoDlg(CDuiString* pName, CDuiString* pProfession)
    : m_pNameEdit(NULL)
    , m_pCombo(NULL)
    , m_pName(pName)
    , m_pProfession(pProfession)
{
}


CPlayerInfoDlg::~CPlayerInfoDlg(void)
{
}

CDuiString CPlayerInfoDlg::GetSkinFile()
{
    return _T("PlayerInfoDlg.xml");
}

LPCTSTR CPlayerInfoDlg::GetWindowClassName() const
{
    return GET_CLASS_NAME(CPlayerInfoDlg);
}

void CPlayerInfoDlg::InitWindow()
{
    m_pNameEdit = static_cast<CDuiEdit*>(GetPaintManager()->FindControl(_T("NameEdit")));
    m_pCombo = static_cast<CDuiCombo*>(GetPaintManager()->FindControl(_T("Combo")));
}

void CPlayerInfoDlg::OnClick(CDuiNotify& msg)
{
    if(msg.pSender->GetName() == _T("closebtn"))
    {
        Close(IDCANCEL);
        return;
    }
    else if(msg.pSender->GetName() == _T("OkBtn"))
    {
        if(!m_pNameEdit->GetText().IsEmpty())
        {
            *m_pName = m_pNameEdit->GetText();
            *m_pProfession = g_Profession[m_pCombo->GetCurSel()];
            Close(IDOK);
        }
        return;
    }
    else if(msg.pSender->GetName() == _T("CancelBtn"))
    {
        Close(IDCANCEL);
        return;
    }
}
