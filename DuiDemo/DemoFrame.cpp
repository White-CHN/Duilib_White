#include "StdAfx.h"
#include "DemoFrame.h"


CDemoFrame::CDemoFrame(void)
{
}


CDemoFrame::~CDemoFrame(void)
{
}

DuiLib::CDuiString CDemoFrame::GetSkinFile()
{
    return _T("main.xml");
}

LPCTSTR CDemoFrame::GetWindowClassName() const
{
    return _T("MainWnd");
}

LRESULT CDemoFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg, wParam, lParam);
}

void CDemoFrame::OnFinalMessage(HWND hWnd)
{
}

LRESULT CDemoFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

CDuiControl* CDemoFrame::CreateControl(LPCTSTR pstrClass)
{
    return NULL;
}

void CDemoFrame::Notify(TNotifyUI& msg)
{
    if(msg.sType == _T("windowinit"))
    {
    }
}

