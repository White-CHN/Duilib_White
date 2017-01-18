#include "StdAfx.h"
#include "MessageBox.h"


DUI_BEGIN_MESSAGE_MAP(CMessageBox, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CMessageBox::CMessageBox(void)
{
}


CMessageBox::~CMessageBox(void)
{
}

void CMessageBox::SetMsg(LPCTSTR lpstrMsg)
{
    if(lstrlen(lpstrMsg) <= 0)
    {
        return;
    }

    CDuiControl* pControl = static_cast<CDuiControl*>(GetPaintManager()->FindControl(_T("MessageText")));
    if(pControl)
    {
        pControl->SetText(lpstrMsg);
    }
}

void CMessageBox::SetTitle(LPCTSTR lpstrTitle)
{
    if(lstrlen(lpstrTitle) <= 0)
    {
        return;
    }

    CDuiControl* pControl = static_cast<CDuiControl*>(GetPaintManager()->FindControl(_T("MessageTitle")));
    if(pControl)
    {
        pControl->SetText(lpstrTitle);
    }
}

CDuiString CMessageBox::GetSkinFile()
{
    return _T("MessageBox.xml");
}

LPCTSTR CMessageBox::GetWindowClassName() const
{
    return GET_CLASS_NAME(CMessageBox);
}

void CMessageBox::OnClick(TNotifyUI& msg)
{
    if(msg.pSender->GetName() == _T("closebtn"))
    {
        Close(IDCANCEL);
        return;
    }
    else if(msg.pSender->GetName() == _T("BtnCancel"))
    {
        Close(IDCANCEL);
        return;
    }
    else if(msg.pSender->GetName() == _T("BtnOK"))
    {
        Close(IDOK);
        return;
    }
}

int CMessageBox::MessageBox(HWND hParent, LPCTSTR lpstrTitle, LPCTSTR lpstrMsg)
{
    CMessageBox* pWnd = new CMessageBox();
    pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
    pWnd->CenterWindow();
    pWnd->SetTitle(lpstrTitle);
    pWnd->SetMsg(lpstrMsg);
    return pWnd->ShowModal();
}

void CMessageBox::ShowMessageBox(HWND hParent, LPCTSTR lpstrTitle, LPCTSTR lpstrMsg)
{
    CMessageBox* pWnd = new CMessageBox();
    pWnd->Create(hParent, _T("msgwnd"), UI_WNDSTYLE_FRAME, 0);
    pWnd->CenterWindow();
    pWnd->SetTitle(lpstrTitle);
    pWnd->SetMsg(lpstrMsg);
    pWnd->ShowWindow(TRUE);
}
