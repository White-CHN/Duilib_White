#include "StdAfx.h"
#include "BaoFengFrame.h"

DUI_BEGIN_MESSAGE_MAP(CBaoFengFrame, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()

CBaoFengFrame::CBaoFengFrame(void)
    : m_pOptionPlayList(NULL)
    , m_pViewRight(NULL)
    , m_pTabSwitch(NULL)
    , m_pBoxDlg(NULL)
    , m_pOptionBox(NULL)
    , m_pMainMenu(NULL)
    , m_pToolsDlg(NULL)
{
}


CBaoFengFrame::~CBaoFengFrame(void)
{
    CDuiMenuWnd::DestroyMenu();
    DUI_FREE_POINT(m_pMainMenu);
}

void CBaoFengFrame::InitWindow()
{
    SetIcon(IDI_ICON1);
    m_pOptionPlayList = static_cast<CDuiOption*>(GetPaintManager()->FindControl(_T("OptionPlayList")));
    m_pViewRight = static_cast<CDuiVerticalLayout*>(GetPaintManager()->FindControl(_T("ViewRight")));
    m_pTabSwitch = static_cast<CDuiAnimationTabLayout*>(GetPaintManager()->FindControl(_T("tab_switch")));
    m_pOptionBox = static_cast<CDuiOption*>(GetPaintManager()->FindControl(_T("OptionBox")));

    m_pBoxDlg = new CBoxDlg;
    m_pBoxDlg->Create(GetHWND(), _T("暴风盒子"), WS_CLIPCHILDREN | WS_TABSTOP | WS_OVERLAPPED , 0);
    // 禁用最大化和最小化
    LONG styleValue = ::GetWindowLong(m_pBoxDlg->GetHWND(), GWL_STYLE);
    styleValue &= (~WS_MAXIMIZEBOX);
    ::SetWindowLong(m_pBoxDlg->GetHWND(), GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

    m_pBoxDlg->m_pOptionBox = m_pOptionBox;
    m_pBoxDlg->ShowWindow(FALSE);


    m_pToolsDlg = new CToolsDlg;
    m_pToolsDlg->Create(GetHWND(), _T("Tools"), WS_CLIPCHILDREN | WS_TABSTOP | WS_OVERLAPPED , 0);
    m_pToolsDlg->ShowWindow(FALSE);
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

LRESULT CBaoFengFrame::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    if(IsWindowVisible(m_pBoxDlg->GetHWND()))
    {
        OnMoveBoxDlg();
    }
    if(IsWindowVisible(m_pToolsDlg->GetHWND()))
    {
        OnMoveToolsDlg();
    }
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
    else if(msg.pSender->GetName() == _T("MainMenuBtn"))
    {
        DUI_FREE_POINT(m_pMainMenu);
        m_pMainMenu = new CDuiMenuWnd();
        CDuiPoint point;
        point.x = 0;
        point.y = 27;
        ClientToScreen(GetHWND(), &point);
        m_pMainMenu->Init(NULL, _T("MainMenu.xml"), point, GetPaintManager());
    }
    else if(msg.pSender->GetName() == _T("ToolsBtn"))
    {
        if(IsWindowVisible(m_pToolsDlg->GetHWND()))
        {
            m_pToolsDlg->ShowWindow(FALSE);
        }
        else
        {
            OnMoveToolsDlg();
            m_pToolsDlg->ShowWindow(TRUE);
        }
    }
}

void CBaoFengFrame::OnSelectChanged(CDuiNotify& msg)
{
    if(msg.pSender == m_pOptionPlayList)
    {
        if(m_pOptionPlayList->IsSelected())
        {
            m_pViewRight->SetVisible(TRUE);
        }
        else
        {
            m_pViewRight->SetVisible(FALSE);
        }
    }
    else if(msg.pSender->GetName() == _T("OptionVideo"))
    {
        m_pTabSwitch->SelectItem(0);
    }
    else if(msg.pSender->GetName() == _T("OptionPlaying"))
    {
        m_pTabSwitch->SelectItem(1);
    }
    else if(msg.pSender  == m_pOptionBox)
    {
        if(m_pOptionBox->IsSelected())
        {
            OnMoveBoxDlg();
        }
        else
        {
            m_pBoxDlg->ShowWindow(FALSE);
        }
    }

}

void CBaoFengFrame::OnMoveBoxDlg()
{
    CDuiRect rc;
    HWND hWnd = GetHWND();
    ::GetClientRect(hWnd, &rc);

    CDuiPoint point(rc.right, rc.top);
    ::ClientToScreen(hWnd, &point);

    CDuiRect rcBox;
    HWND hWndBox = m_pBoxDlg->GetHWND();
    ::GetClientRect(hWndBox, &rcBox);
    ::SetWindowPos(hWndBox, NULL, point.x - 3, point.y - 3, rcBox.GetWidth(), rcBox.GetHeight(), NULL);
    m_pBoxDlg->ShowWindow(TRUE);
}

void CBaoFengFrame::OnMoveToolsDlg()
{
    CDuiRect rc;
    ::GetClientRect(m_pToolsDlg->GetHWND(), &rc);

    CDuiRect rcClient;
    GetClientRect(&rcClient);
    CDuiPoint point(rcClient.left + 1, rcClient.bottom - 43 - rc.GetHeight());
    ::ClientToScreen(GetHWND(), &point);

    ::SetWindowPos(m_pToolsDlg->GetHWND(), NULL, point.x, point.y, rc.GetWidth(), rc.GetHeight(), NULL);

}
