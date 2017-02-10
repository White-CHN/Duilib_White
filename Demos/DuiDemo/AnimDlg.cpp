#include "stdafx.h"
#include "AnimDlg.h"
#include "DuiAnimLayout.h"

DUI_BEGIN_MESSAGE_MAP(CAnimDlg, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_WINDOWINIT()
DUI_END_MESSAGE_MAP()

CAnimDlg::CAnimDlg(void)
    : m_pAnimLayout(NULL)
{
}


CAnimDlg::~CAnimDlg(void)
{
}

CDuiString CAnimDlg::GetSkinFile()
{
    return _T("AnimDlg.xml");
}

LPCTSTR CAnimDlg::GetWindowClassName() const
{
    return GET_CLASS_NAME(CAnimDlg);
}

CDuiControl* CAnimDlg::CreateControl(LPCTSTR pstrClassName)
{
    if(_tcsicmp(pstrClassName, DUI_CTR_ANIMLAYOUT) == 0)
    {
        return new CDuiAnimLayout;
    }
    return NULL;
}

void CAnimDlg::InitWindow()
{
    m_pAnimLayout = static_cast<CDuiAnimLayout*>(GetPaintManager()->FindControl(_T("vlayout_background")));
    ASSERT(m_pAnimLayout != NULL);

}

void CAnimDlg::OnWindowInit(TNotifyUI& msg)
{
    m_pAnimLayout->StartEffect();
}

void CAnimDlg::OnClick(TNotifyUI& msg)
{
    if(msg.pSender->GetName() == _T("closebtn"))
    {
        Close(IDCANCEL);
        return;
    }
}
