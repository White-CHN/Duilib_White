#include "StdAfx.h"
#include "BaoFengFrame.h"

DUI_BEGIN_MESSAGE_MAP(CBaoFengFrame, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CBaoFengFrame::CBaoFengFrame(void)
{
}


CBaoFengFrame::~CBaoFengFrame(void)
{
}

void CBaoFengFrame::InitWindow()
{

}

CDuiString CBaoFengFrame::GetSkinFile()
{
    return _T("BaoFeng.xml");
}

LPCTSTR CBaoFengFrame::GetWindowClassName() const
{
    return GET_CLASS_NAME(CBaoFengFrame);
}

LRESULT CBaoFengFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
    PostQuitMessage(0);
    return 0;
}

void CBaoFengFrame::OnClick(CDuiNotify& msg)
{
    if(msg.pSender->GetName() == _T("minbtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
        return;
    }
    else if(msg.pSender->GetName() == _T("maxbtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
        return;
    }
    else if(msg.pSender->GetName() == _T("restorebtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_RESTORE);
        return;
    }
    else if(msg.pSender->GetName() == _T("closebtn"))
    {
        Close();
        return;
    }
}
