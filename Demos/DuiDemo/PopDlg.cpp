#include "StdAfx.h"
#include "PopDlg.h"


CPopDlg::CPopDlg(void)
{
}


CPopDlg::~CPopDlg(void)
{
}

CDuiString CPopDlg::GetSkinFile()
{
    return _T("XML_Pop");
}

LPCTSTR CPopDlg::GetWindowClassName() const
{
    return GET_CLASS_NAME(CPopDlg);
}

void CPopDlg::InitWindow()
{
    m_pCloseBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("closebtn")));
    m_pMaxBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("maxbtn")));
    m_pRestoreBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("restorebtn")));
    m_pMinBtn = static_cast<CDuiButton*>(GetPaintManager()->FindControl(_T("minbtn")));
}

CDuiControl* CPopDlg::CreateControl(LPCTSTR pstrClass)
{
    return NULL;
}

void CPopDlg::Notify(TNotifyUI& msg)
{
    if(msg.sType == DUI_MSGTYPE_CLICK)
    {
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
