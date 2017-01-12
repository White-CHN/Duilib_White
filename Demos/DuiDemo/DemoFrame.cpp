#include "StdAfx.h"
#include "DemoFrame.h"
#include "PopDlg.h"

CDemoFrame::CDemoFrame(void)
    : bEnglish(FALSE)
    , m_pCloseBtn(NULL)
    , m_pMaxBtn(NULL)
    , m_pRestoreBtn(NULL)
    , m_pMinBtn(NULL)
    , m_pSkinBtn(NULL)
    , m_pMenuBtn(NULL)
    , m_pSlider(NULL)
    , m_pProgress(NULL)
    , m_pMenu(NULL)
{
}


CDemoFrame::~CDemoFrame(void)
{
    DUI_FREE_POINT(m_pMenu);
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
    m_pMenuBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("menubtn")));

    m_pSlider = static_cast<CDuiSlider*>(GetPaintManager()->FindControl(_T("Slider")));
    m_pProgress = static_cast<CDuiProgress*>(GetPaintManager()->FindControl(_T("Progress")));
    m_pProgress->SetShowText(TRUE);

    CDuiChart* pPieView = static_cast<CDuiChart*>(GetPaintManager()->FindControl(_T("ChartPie")));
    if(NULL != pPieView)
    {
        pPieView->Add(_T("北京{c #FE5900}35%{/c}"), 35);
        pPieView->Add(_T("上海{c #FE5900}38%{/c}"), 38);
        pPieView->Add(_T("广州{c #FE5900}35%{/c}"), 35);
        pPieView->Add(_T("香港{c #FE5900}15%{/c}"), 15);
    }
    CDuiChart* pHistpgramView = static_cast<CDuiChart*>(GetPaintManager()->FindControl(_T("ChartHistpgram")));
    if(NULL != pHistpgramView)
    {
        pHistpgramView->Add(_T("1月{c #FE5900}13%{/c}"), 13);
        pHistpgramView->Add(_T("2月{c #FE5900}11%{/c}"), 11);
        pHistpgramView->Add(_T("3月{c #FE5900}32%{/c}"), 32);
        pHistpgramView->Add(_T("4月{c #FE5900}17%{/c}"), 17);
        pHistpgramView->Add(_T("5月{c #FE5900}8%{/c}"), 8);
        pHistpgramView->Add(_T("6月{c #FE5900}12%{/c}"), 12);
    }
    CDuiList* pList = static_cast<CDuiList*>(GetPaintManager()->FindControl(_T("list")));
    for(int i = 0; i < 100; i++)
    {
        CDuiString str;
        str.Format(_T("%d"), i);
        CDuiListTextElement* pItem  = new CDuiListTextElement();
        pList->Add(pItem);
        pItem->SetAttribute(_T("bkcolor"), _T("#FFFFFFFF"));
        pItem->SetFixedHeight(30);
        pItem->SetText(0, str);
        pItem->SetText(1, _T("小明"));
        pItem->SetText(2, str);
        pItem->SetText(3, str);

    }
}

CDuiString CDemoFrame::GetSkinFile()
{
    return _T("XML_Demo");
}

LPCTSTR CDemoFrame::GetWindowClassName() const
{
    return GET_CLASS_NAME(CDemoFrame);
}

LRESULT CDemoFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
    PostQuitMessage(0);
    return 0;
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
    else if(msg.sType == DUI_MSGTYPE_COLORCHANGED)
    {
        CDuiColorPalette* pColorPalette = static_cast<CDuiColorPalette*>(GetPaintManager()->FindControl(_T("Pallet")));
        if(msg.pSender == pColorPalette)
        {
            GetPaintManager()->GetRoot()->SetBkImage(_T(""));
            GetPaintManager()->GetRoot()->SetBkColor(pColorPalette->GetSelectColor());
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
        else if(msg.pSender == m_pMenuBtn)
        {
            DUI_FREE_POINT(m_pMenu);
            m_pMenu = new CDuiMenuWnd();
            CDuiPoint point;
            ::GetCursorPos(&point);
            m_pMenu->Init(NULL, _T("menu.xml"), point, GetPaintManager());
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
        else if(msg.pSender->GetName().CompareNoCase(_T("popwnd_btn")) == 0)
        {
            CPopDlg* pPopDlg = new CPopDlg();
            pPopDlg->Create(GetHWND(), _T("普通窗口演示"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 800, 572);
            pPopDlg->CenterWindow();
        }
        else if(msg.pSender->GetName().CompareNoCase(_T("modal_popwnd_btn")) == 0)
        {
            CPopDlg* pPopDlg = new CPopDlg();
            pPopDlg->Create(GetHWND(), _T("模式窗口演示"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 800, 572);
            pPopDlg->CenterWindow();
            pPopDlg->ShowModal();
        }
    }
}

