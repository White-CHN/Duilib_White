#include "StdAfx.h"
#include "DKPManageFrame.h"
#include "PlayerInfoDlg.h"


DUI_BEGIN_MESSAGE_MAP(CDKPManageFrame, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_CLICK_CTRNAME(_T("AddBtn"), OnAddBtn)
DUI_ON_CLICK_CTRNAME(_T("DelBtn"), OnDelBtn)
DUI_ON_CLICK_CTRNAME(_T("AddMarkBtn"), OnAddMarkBtn)
DUI_ON_CLICK_CTRNAME(_T("DelMarkBtn"), OnDelMarkBtn)
DUI_ON_CLICK_CTRNAME(_T("UnselectedBtn"), OnUnselectedBtn)
DUI_ON_CLICK_CTRNAME(_T("ExcelBtn"), OnExcelBtn)
DUI_END_MESSAGE_MAP()

CDKPManageFrame::CDKPManageFrame(void)
    : m_pList(NULL)
{
}


CDKPManageFrame::~CDKPManageFrame(void)
{
}

CDuiString CDKPManageFrame::GetSkinFile()
{
    return _T("DKPManagement.xml");
}

LPCTSTR CDKPManageFrame::GetWindowClassName() const
{
    return GET_CLASS_NAME(CDKPManageFrame);
}

LRESULT CDKPManageFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
    PostQuitMessage(0);
    return 0;
}

void CDKPManageFrame::InitWindow()
{
    SetIcon(IDI_ICON2);
    m_pList = static_cast<CDuiList*>(GetPaintManager()->FindControl(_T("list")));

    try
    {
        CDuiString strConn, strDBPath;
        strDBPath.Format(_T("%s\\dat.accdb"), CDuiPaintManager::GetResourcePath());
        strConn.Format(_T("Provider=Microsoft.ACE.OLEDB.12.0;Data source=%s"), strDBPath);
        m_DateDB.SetConnectionTimeout();
        if(m_DateDB.Open(strConn) == FALSE)
        {
            AfxMessageBox(_T("数据库连接失败"));
            return;
        }
    }
    catch(_com_error e)
    {
        AfxMessageBox(_T("InitWindow::Open DB Falied[%s]"));
        return ;
    }
}

void CDKPManageFrame::OnClick(CDuiNotify& msg)
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
}

void CDKPManageFrame::OnAddBtn(CDuiNotify& msg)
{
    CPlayerInfoDlg* dlg = new CPlayerInfoDlg();
    dlg->Create(GetHWND(), _T("Player Info Dialog"), UI_WNDSTYLE_DIALOG, WS_EX_TOOLWINDOW);
    dlg->CenterWindow();
    dlg->ShowModal();


    CDuiListContainerElement* p = new CDuiListContainerElement;
    m_pList->Add(p);
    p->SetFixedHeight(24);


    CDuiOption* pOption = new CDuiOption;
    p->Add(pOption);
    pOption->SetAttribute(_T("selectedimage"), _T("file='CheckBox/checked.png' dest='4,4,20,20'"));
    pOption->SetAttribute(_T("normalimage"), _T("file='CheckBox/unchecked.png' dest='4,4,20,20'"));


    for(int i = 0; i < 7; i++)
    {
        CDuiLabel* pLabel = new CDuiLabel;
        p->Add(pLabel);
        pLabel->SetAttribute(_T("align"), _T("center"));
        pLabel->SetText(_T("1"));
    }

    CDuiEdit* pEdit = new CDuiEdit;
    p->Add(pEdit);
    pEdit->SetAttribute(_T("style"), _T("edit_style"));
}

void CDKPManageFrame::OnDelBtn(CDuiNotify& msg)
{
    while(1)
    {
        BOOL bExist = FALSE;
        for(int i = 0; i < m_pList->GetCount(); i++)
        {
            CDuiListContainerElement* p = static_cast<CDuiListContainerElement*>(m_pList->GetItemAt(i));
            if(p)
            {
                CDuiOption* pOption = static_cast<CDuiOption*>(p->GetItemAt(0));
                if(pOption && pOption->IsSelected())
                {
                    m_pList->RemoveAt(i);
                    bExist = TRUE;
                    break;
                }
            }
        }
        if(bExist == FALSE)
        {
            break;
        }
    }
}

void CDKPManageFrame::OnAddMarkBtn(CDuiNotify& msg)
{

}

void CDKPManageFrame::OnDelMarkBtn(CDuiNotify& msg)
{

}

void CDKPManageFrame::OnUnselectedBtn(CDuiNotify& msg)
{
    for(int i = 0; i < m_pList->GetCount(); i++)
    {
        CDuiListContainerElement* p = static_cast<CDuiListContainerElement*>(m_pList->GetItemAt(i));
        if(p)
        {
            CDuiOption* pOption = static_cast<CDuiOption*>(p->GetItemAt(0));
            if(pOption)
            {
                pOption->Selected(FALSE);
            }
        }
    }
}

void CDKPManageFrame::OnExcelBtn(CDuiNotify& msg)
{

}
