#include "StdAfx.h"
#include "DemoFrame.h"


CDemoFrame::CDemoFrame(void)
{
}


CDemoFrame::~CDemoFrame(void)
{
}

void CDemoFrame::InitWindow()
{
    SetIcon(IDR_MAINFRAME);
    m_Icon.CreateIcon(GetHWND(), IDR_MAINFRAME, _T("Duilib开源项目\nDuilib开源项目"));
    m_pCloseBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("closebtn")));
    m_pMaxBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("maxbtn")));
    m_pRestoreBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("restorebtn")));
    m_pMinBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("minbtn")));
    m_pSkinBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("skinbtn")));


}

CDuiString CDemoFrame::GetSkinFile()
{
    return _T("XML_Demo");
}

LPCTSTR CDemoFrame::GetWindowClassName() const
{
    return _T("CDemoFrame");
}

LRESULT CDemoFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

void CDemoFrame::OnFinalMessage(HWND hWnd)
{
    __super::OnFinalMessage(hWnd);
}

LRESULT CDemoFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

    return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

CDuiControl* CDemoFrame::CreateControl(LPCTSTR pstrClass)
{
    return NULL;
}

void CDemoFrame::Notify(TNotifyUI& msg)
{
    if(msg.sType == DUI_MSGTYPE_WINDOWINIT)
    {
    }
    else if(msg.sType == DUI_MSGTYPE_CLICK)
    {
        if(msg.pSender->GetName() == _T("home"))
        {
            ShellExecute(NULL, _T("open"), _T("https://github.com/White-CHN/Duilib_White"), NULL, NULL, SW_SHOW);
        }
        if(msg.pSender == m_pMinBtn)
        {
            SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
            return;
        }
        else if(msg.pSender == m_pMaxBtn)
        {
            SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
            return;
        }
        else if(msg.pSender == m_pRestoreBtn)
        {
            SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
            return;
        }
        else if(msg.pSender == m_pCloseBtn)
        {
            Close(0);
            return;
        }
    }
}

