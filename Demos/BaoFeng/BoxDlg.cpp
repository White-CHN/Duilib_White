#include "StdAfx.h"
#include "BoxDlg.h"


DUI_BEGIN_MESSAGE_MAP(CBoxDlg, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CBoxDlg::CBoxDlg(void)
    : m_pOptionBox(NULL)
    , m_pMaxBtn(NULL)
    , m_pRestoreBtn(NULL)
{
}


CBoxDlg::~CBoxDlg(void)
{
}

void CBoxDlg::InitWindow()
{
    m_pMaxBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("maxbtn")));
    m_pRestoreBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("restorebtn")));
}

CDuiString CBoxDlg::GetSkinFile()
{
    return _T("BoxDlg.xml");
}

LPCTSTR CBoxDlg::GetWindowClassName() const
{
    return GET_CLASS_NAME(CBoxDlg);
}

void CBoxDlg::OnClick(CDuiNotify& msg)
{
    if(msg.pSender->GetName() == _T("closebtn"))
    {
        ShowWindow(FALSE);
        m_pOptionBox->Selected(FALSE);
        return;
    }
    else if(msg.pSender->GetName() == _T("maxbtn"))
    {
        CDuiRect rt;
        GetWindowRect(&rt);
        GetWindowRect(&m_rcClient);
        rt.top = 0;
        rt.bottom = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYCAPTION);
        MoveWindow(&rt);

        m_pMaxBtn->SetVisible(FALSE);
        m_pRestoreBtn->SetVisible(TRUE);
    }
    else if(msg.pSender->GetName() == _T("restorebtn"))
    {
        MoveWindow(&m_rcClient);
        m_pMaxBtn->SetVisible(TRUE);
        m_pRestoreBtn->SetVisible(FALSE);
    }
}
