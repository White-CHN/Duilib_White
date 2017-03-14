#include "StdAfx.h"
#include "OperateMarkDlg.h"
#include "DKPManageFrame.h"


DUI_BEGIN_MESSAGE_MAP(COperateMarkDlg, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

COperateMarkDlg::COperateMarkDlg(CDuiString* pMark, CDuiString* pRemarks)
    : m_pMark(pMark)
    , m_pRemarks(pRemarks)
    , m_pMarkEdit(NULL)
    , m_pRemarksEdit(NULL)
{
}


COperateMarkDlg::~COperateMarkDlg(void)
{
}

CDuiString COperateMarkDlg::GetSkinFile()
{
    return _T("OperateMarkDlg.xml");
}

LPCTSTR COperateMarkDlg::GetWindowClassName() const
{
    return GET_CLASS_NAME(COperateMarkDlg);
}

void COperateMarkDlg::InitWindow()
{
    m_pMarkEdit = static_cast<CDuiEdit*>(GetPaintManager()->FindControl(_T("MarkEdit")));
    m_pRemarksEdit = static_cast<CDuiEdit*>(GetPaintManager()->FindControl(_T("RemarksEdit")));

    if(m_nType == TYPE_SUBTRACT)
    {
        CDuiLabel* pLabel = static_cast<CDuiLabel*>(GetPaintManager()->FindControl(_T("TitleLabel")));
        if(pLabel)
        {
            pLabel->SetText(_T("¿Û·Ö"));
        }
    }
}

void COperateMarkDlg::OnClick(CDuiNotify& msg)
{
    if(msg.pSender->GetName() == _T("closebtn"))
    {
        Close(IDCANCEL);
        return;
    }
    else if(msg.pSender->GetName() == _T("OkBtn"))
    {
        if(!m_pMarkEdit->GetText().IsEmpty())
        {
            *m_pMark = m_pMarkEdit->GetText();
            *m_pRemarks = m_pRemarksEdit->GetText();
            Close(IDOK);
        }
        return;
    }
    else if(msg.pSender->GetName() == _T("CancelBtn"))
    {
        Close(IDCANCEL);
        return;
    }
}
