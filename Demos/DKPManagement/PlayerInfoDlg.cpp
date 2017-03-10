#include "StdAfx.h"
#include "PlayerInfoDlg.h"

DUI_BEGIN_MESSAGE_MAP(CPlayerInfoDlg, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CPlayerInfoDlg::CPlayerInfoDlg(void)
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

}

void CPlayerInfoDlg::OnClick(CDuiNotify& msg)
{
    if(msg.pSender->GetName() == _T("closebtn"))
    {
        Close();
        return;
    }
}
