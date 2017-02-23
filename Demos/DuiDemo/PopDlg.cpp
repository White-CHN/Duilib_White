#include "StdAfx.h"
#include "PopDlg.h"



DUI_BEGIN_MESSAGE_MAP(CPopDlg, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CPopDlg::CPopDlg(void)

{
}


CPopDlg::~CPopDlg(void)
{
}

CDuiString CPopDlg::GetSkinFile()
{
    return _T("XML_Pop");
}

LPCTSTR CPopDlg::GetWindowClassName() const
{
    return GET_CLASS_NAME(CPopDlg);
}

void CPopDlg::OnClick(CDuiNotify& msg)
{
    if(msg.pSender->GetName() == _T("minbtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
        return;
    }
    else if(msg.pSender->GetName() == _T("maxbtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        return;
    }
    else if(msg.pSender->GetName() == _T("restorebtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
        return;
    }
    else if(msg.pSender->GetName() == _T("closebtn"))
    {
        Close(IDCANCEL);
        return;
    }
}