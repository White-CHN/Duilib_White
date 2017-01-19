#include "StdAfx.h"
#include "SkinDlg.h"
#include "SkinPictureBtn.h"
#include "SkinSliderDlg.h"


DUI_BEGIN_MESSAGE_MAP(CSkinDlg, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()

CSkinDlg::CSkinDlg(CDemoFrame* pDemoFrame)
    : m_pDemoFrame(pDemoFrame)
    , m_pTabSwitch(NULL)
{
}


CSkinDlg::~CSkinDlg(void)
{
}

void CSkinDlg::InitWindow()
{
    m_pTabSwitch = static_cast<CDuiAnimationTabLayout*>(GetPaintManager()->FindControl(_T("SkinTab")));

    CDuiString strBkImage = m_pDemoFrame->GetPaintManager()->GetRoot()->GetBkImage();
    DWORD dwBkColor = m_pDemoFrame->GetPaintManager()->GetRoot()->GetBkColor();
    if(strBkImage.IsEmpty())
    {
        GetPaintManager()->GetRoot()->SetBkColor(dwBkColor);
        GetPaintManager()->GetRoot()->SetBkImage(_T(""));
    }
    else
    {
        GetPaintManager()->GetRoot()->SetBkColor(0);
        GetPaintManager()->GetRoot()->SetBkImage(strBkImage);
    }

}

CDuiString CSkinDlg::GetSkinFile()
{
    return _T("SkinDlg.xml");
}

LPCTSTR CSkinDlg::GetWindowClassName() const
{
    return GET_CLASS_NAME(CSkinDlg);
}

void CSkinDlg::OnClick(TNotifyUI& msg)
{
    if(msg.pSender->GetName() == _T("closebtn"))
    {
        Close(IDCANCEL);
        return;
    }
    else if(msg.pSender->GetName() == _T("AlphaBtn"))
    {
        CDuiButton* pBtn = static_cast<CDuiButton*>(msg.pSender);
        CSkinSliderDlg* pSkinSliderDlg  = new CSkinSliderDlg(m_pDemoFrame->GetPaintManager(), pBtn);
        pSkinSliderDlg->Create(GetHWND(), _T("皮肤进度条窗口"), UI_WNDSTYLE_DIALOG, WS_EX_TOOLWINDOW);
        CDuiPoint ptPos(msg.pSender->GetPos().left, msg.pSender->GetPos().top);
        ::ClientToScreen(GetHWND(), &ptPos);
        ::SetWindowPos(pSkinSliderDlg->GetHWND(), NULL, ptPos.x, ptPos.y - pSkinSliderDlg->GetPaintManager()->GetClientSize().cy , 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    else if(msg.pSender->GetClass() == DUI_CTR_PICTUREBTN)
    {
        CDuiSkinPictureBtn* pBtn = static_cast<CDuiSkinPictureBtn*>(msg.pSender);
        if(pBtn)
        {
            CDuiString strBkImage = pBtn->GetBkImage();
            DWORD dwBkColor = pBtn->GetBkColor();
            if(!strBkImage.IsEmpty())
            {
                strBkImage.Replace(_T("small"), _T("bkimage"));
                m_pDemoFrame->GetPaintManager()->GetRoot()->SetBkColor(0);
                m_pDemoFrame->GetPaintManager()->GetRoot()->SetBkImage(strBkImage);
                GetPaintManager()->GetRoot()->SetBkColor(0);
                GetPaintManager()->GetRoot()->SetBkImage(strBkImage);
            }
            else
            {
                m_pDemoFrame->GetPaintManager()->GetRoot()->SetBkColor(dwBkColor);
                m_pDemoFrame->GetPaintManager()->GetRoot()->SetBkImage(_T(""));
                GetPaintManager()->GetRoot()->SetBkColor(dwBkColor);
                GetPaintManager()->GetRoot()->SetBkImage(_T(""));
            }
        }
    }
}

void CSkinDlg::OnSelectChanged(TNotifyUI& msg)
{
    if(msg.pSender->GetName() == _T("OptionPic"))
    {
        if(m_pTabSwitch)
        {
            m_pTabSwitch->SelectItem(0);
        }

    }
    else if(msg.pSender->GetName() == _T("OptionColor"))
    {
        if(m_pTabSwitch)
        {
            m_pTabSwitch->SelectItem(1);
        }
    }
}
