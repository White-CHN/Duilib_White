#include "StdAfx.h"
#include "DemoFrame.h"


CDemoFrame::CDemoFrame(void)
    : bEnglish(FALSE)
{
}


CDemoFrame::~CDemoFrame(void)
{
}

void CDemoFrame::InitWindow()
{
    CDuiResourceManager::GetInstance()->SetTextQueryInterface(this);
    CDuiResourceManager::GetInstance()->SetLanguage(_T("cn_zh"));
    CDuiResourceManager::GetInstance()->LoadLanguage(_T("lan_cn.xml"));
    SetIcon(IDR_MAINFRAME);
    m_Icon.CreateIcon(GetHWND(), IDR_MAINFRAME, _T("Duilib开源项目\nDuilib开源项目"));
    m_pCloseBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("closebtn")));
    m_pMaxBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("maxbtn")));
    m_pRestoreBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("restorebtn")));
    m_pMinBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("minbtn")));
    m_pSkinBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("skinbtn")));

    m_pSlider = static_cast<CDuiSlider*>(GetPaintManager()->FindControl(_T("Slider")));
    m_pProgress = static_cast<CDuiProgress*>(GetPaintManager()->FindControl(_T("Progress")));
    m_pProgress->SetShowText(TRUE);
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
    return CDuiDlgImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CDemoFrame::OnFinalMessage(HWND hWnd)
{
    CDuiDlgImplBase::OnFinalMessage(hWnd);
}

LRESULT CDemoFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

    return CDuiDlgImplBase::MessageHandler(uMsg, wParam, lParam, bHandled);
}

CDuiControl* CDemoFrame::CreateControl(LPCTSTR pstrClass)
{
    return NULL;
}

LPCTSTR CDemoFrame::QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType)
{
    CDuiString sLanguage = CDuiResourceManager::GetInstance()->GetLanguage();
    if(sLanguage == _T("en"))
    {
        if(lstrcmpi(lpstrId, _T("Join")) == 0)
        {
            return _T("Add White QQ:450756957");
        }
    }
    else
    {
        if(lstrcmpi(lpstrId, _T("Join")) == 0)
        {
            return _T("骚扰White QQ:450756957");
        }
    }

    return CDuiDlgImplBase::QueryControlText(lpstrId, lpstrType);
}

void CDemoFrame::Notify(TNotifyUI& msg)
{
    CDuiString name = msg.pSender->GetName();
    if(msg.sType == DUI_MSGTYPE_WINDOWINIT)
    {
    }
    else if(msg.sType == DUI_MSGTYPE_VALUECHANGED)
    {
        if(msg.pSender == m_pSlider && m_pProgress != NULL)
        {
            m_pProgress->SetValue(m_pSlider->GetValue());
        }
    }
    else if(msg.sType == DUI_MSGTYPE_SELECTCHANGED)
    {
        CDuiTabLayout* pTabSwitch = static_cast<CDuiTabLayout*>(GetPaintManager()->FindControl(_T("tab_switch")));
        if(name.CompareNoCase(_T("basic_tab")) == 0)
        {
            pTabSwitch->SelectItem(0);
        }
        if(name.CompareNoCase(_T("rich_tab")) == 0)
        {
            pTabSwitch->SelectItem(1);
        }
        if(name.CompareNoCase(_T("ex_tab")) == 0)
        {
            pTabSwitch->SelectItem(2);
        }
        if(name.CompareNoCase(_T("ani_tab")) == 0)
        {
            pTabSwitch->SelectItem(3);
        }
        if(name.CompareNoCase(_T("split_tab")) == 0)
        {
            pTabSwitch->SelectItem(4);
        }
    }
    else if(msg.sType == DUI_MSGTYPE_CLICK)
    {
        if(msg.pSender->GetName() == _T("home"))
        {
            ShellExecute(NULL, _T("open"), _T("https://github.com/White-CHN/Duilib_White"), NULL, NULL, SW_SHOW);
        }
        else if(msg.pSender->GetName() == _T("join"))
        {
            ShellExecute(NULL, _T("open"), _T("tencent://Message/?Uin=450756957&Menu=yes"), NULL, NULL, SW_SHOW);
        }
        else if(msg.pSender->GetName() == _T("joingroup"))
        {
            ShellExecute(NULL, _T("open"), _T("http://qm.qq.com/cgi-bin/qm/qr?k=tGGUVkSEUP7-aAHtd8KSMulikqLyUcBZ#"), NULL, NULL, SW_SHOW);
        }
        else if(msg.pSender == m_pMinBtn)
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
        else if(msg.pSender == m_pSkinBtn)
        {
            if(!bEnglish)
            {
                CDuiResourceManager::GetInstance()->SetLanguage(_T("en"));
                CDuiResourceManager::GetInstance()->LoadLanguage(_T("lan_en.xml"));
            }
            else
            {
                CDuiResourceManager::GetInstance()->SetLanguage(_T("cn_zh"));
                CDuiResourceManager::GetInstance()->LoadLanguage(_T("lan_cn.xml"));
            }
            bEnglish = !bEnglish;
            CDuiResourceManager::GetInstance()->ReloadText();
            GetPaintManager()->GetRoot()->NeedUpdate();
            return;
        }
        else if(msg.pSender->GetName().CompareNoCase(_T("dpi_btn")) == 0)
        {
            int nDPI = _ttoi(msg.pSender->GetUserData());
            GetPaintManager()->SetDPI(nDPI);
        }
    }
}

