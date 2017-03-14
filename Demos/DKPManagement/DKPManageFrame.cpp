#include "StdAfx.h"
#include "DKPManageFrame.h"
#include "PlayerInfoDlg.h"
#include "OperateMarkDlg.h"



DUI_BEGIN_MESSAGE_MAP(CDKPManageFrame, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_CLICK_CTRNAME(_T("AddBtn"), OnAddBtn)
DUI_ON_CLICK_CTRNAME(_T("DelBtn"), OnDelBtn)
DUI_ON_CLICK_CTRNAME(_T("AddMarkBtn"), OnAddMarkBtn)
DUI_ON_CLICK_CTRNAME(_T("DelMarkBtn"), OnDelMarkBtn)
DUI_ON_CLICK_CTRNAME(_T("UnselectedBtn"), OnUnselectedBtn)
DUI_ON_CLICK_CTRNAME(_T("ExcelBtn"), OnExcelBtn)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_TEXTCHANGED, OnTextChanged)
DUI_END_MESSAGE_MAP()

CDKPManageFrame::CDKPManageFrame(void)
    : m_pInfoList(NULL)
    , m_pHisList(NULL)
    , m_pTab(NULL)
    , m_pTypeCombo(NULL)
    , m_pNameEdit(NULL)
    , m_pHisCombo(NULL)
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
    SetIcon(IDR_MAINFRAME);
    m_pInfoList = static_cast<CDuiList*>(GetPaintManager()->FindControl(_T("InfoList")));
    m_pHisList = static_cast<CDuiList*>(GetPaintManager()->FindControl(_T("HisList")));
    m_pTab = static_cast<CDuiAnimationTabLayout*>(GetPaintManager()->FindControl(_T("Tab")));
    m_pTypeCombo = static_cast<CDuiCombo*>(GetPaintManager()->FindControl(_T("TypeCombo")));
    m_pNameEdit = static_cast<CDuiEdit*>(GetPaintManager()->FindControl(_T("NameEdit")));
    m_pHisCombo = static_cast<CDuiCombo*>(GetPaintManager()->FindControl(_T("HisCombo")));
    try
    {
        CDuiString strConn, strDBPath;
        strDBPath.Format(_T("%sDKPManagement\\dat.accdb"), CCommon::GetAppPath());
        strConn.Format(_T("Provider=Microsoft.ACE.OLEDB.12.0;Data source=%s"), strDBPath);
        m_DateDB.SetConnectionTimeout();
        if(m_DateDB.Open(strConn) == FALSE)
        {
            AfxMessageBox(_T("数据库连接失败"));
            return;
        }
        else
        {
            miniAdo ado;
            if(ado.Open(m_DateDB.GetActiveConnection()))
            {
                // 加载人员信息
                CString strSQL;
                strSQL.Format(_T("select ID,Name,Profession,AddMark,DelMark,Mark,CreateTime,Remarks from PlayerInfo order by ID"));
                ado.OpenRecordset(strSQL);
                while(!ado.get_EOF())
                {
                    UINT uID = ado.GetFieldValueAsInt(_T("ID"));
                    CString strName = ado.GetFieldValueAsStr(_T("Name"));
                    CString strProfession = ado.GetFieldValueAsStr(_T("Profession"));
                    int nAddMark = ado.GetFieldValueAsInt(_T("AddMark"));
                    int nDelMark = ado.GetFieldValueAsInt(_T("DelMark"));
                    int nMark = ado.GetFieldValueAsInt(_T("Mark"));
                    CString strTime = ado.GetFieldValueAsStr(_T("CreateTime"));
                    CString strRemarks = ado.GetFieldValueAsStr(_T("Remarks"));
                    OnAddListItem(uID, strName.GetBuffer(), strProfession.GetBuffer(), nAddMark, nDelMark, nMark, strTime.GetBuffer(), strRemarks.GetBuffer());
                    ado.Next();
                }
                ado.CloseRecordset();
            }
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
    CDuiString strName;
    CDuiString strProfession;
    CPlayerInfoDlg* dlg = new CPlayerInfoDlg(&strName, &strProfession);
    dlg->Create(GetHWND(), _T("Player Information Dialog"), UI_WNDSTYLE_DIALOG, WS_EX_TOOLWINDOW);
    dlg->CenterWindow();
    if(dlg->ShowModal() == IDOK)
    {
        miniAdo ado;
        if(ado.Open(m_DateDB.GetActiveConnection()))
        {
            CString strSQL;
            strSQL.Format(_T("insert into PlayerInfo (Name,Profession,CreateTime) VALUES ('%s','%s',now())"), strName.GetData(), strProfession.GetData());
            if(ado.Execute(strSQL))
            {
                strSQL.Format(_T("select ID,CreateTime from PlayerInfo where Name='%s'"), strName.GetData());
                ado.OpenRecordset(strSQL);
                UINT uID = ado.GetFieldValueAsInt(_T("ID"));
                CString strTime = ado.GetFieldValueAsStr(_T("CreateTime"));
                ado.CloseRecordset();
                OnAddListItem(uID, strName, strProfession, 0, 0, 0, strTime.GetBuffer(), _T(""));

                CString strRecord;
                strRecord.Format(_T("添加成员：姓名[%s] 职业[%s]"), strName.GetData(), strProfession.GetData());
                strSQL.Format(_T("insert into HistoryRecord (Owner,CreateTime,Record) VALUES (%d,now(),'%s')"), uID, strRecord);
                ado.Execute(strSQL);
            }
        }
    }
}

void CDKPManageFrame::OnDelBtn(CDuiNotify& msg)
{
    miniAdo ado;
    CString strSQL;
    if(ado.Open(m_DateDB.GetActiveConnection()) == FALSE)
    {
        return ;
    }
    while(1)
    {
        BOOL bExist = FALSE;
        for(int i = 0; i < m_pInfoList->GetCount(); i++)
        {
            CDuiListContainerElement* p = static_cast<CDuiListContainerElement*>(m_pInfoList->GetItemAt(i));
            if(p)
            {
                CDuiOption* pOption = static_cast<CDuiOption*>(p->GetItemAt(0));
                if(pOption && pOption->IsSelected() && p->IsVisible())
                {
                    CDuiLabel* pLabel = static_cast<CDuiOption*>(p->GetItemAt(1));
                    int nID = _ttoi(pLabel->GetText().GetData());
                    strSQL.Format(_T("delete from PlayerInfo where ID=%d"), nID);
                    ado.Execute(strSQL);

                    CDuiLabel* pLabelName = static_cast<CDuiOption*>(p->GetItemAt(2));
                    CDuiLabel* pLabelProfession = static_cast<CDuiOption*>(p->GetItemAt(3));
                    CString strRecord;
                    strRecord.Format(_T("删除成员：姓名[%s] 职业[%s]"), pLabelName->GetText().GetData(), pLabelProfession->GetText().GetData());
                    strSQL.Format(_T("insert into HistoryRecord (Owner,CreateTime,Record) VALUES (%d,now(),'%s')"), nID, strRecord);
                    ado.Execute(strSQL);

                    m_pInfoList->RemoveAt(i);
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
    CDuiString strMark;
    CDuiString strRemarks;
    COperateMarkDlg* dlg = new COperateMarkDlg(&strMark, &strRemarks);
    dlg->m_nType = TYPE_ADD;
    dlg->Create(GetHWND(), _T("Operate Mark Dialog"), UI_WNDSTYLE_DIALOG, WS_EX_TOOLWINDOW);
    dlg->CenterWindow();
    if(dlg->ShowModal() == IDOK)
    {
        int nMark = _ttoi(strMark);
        if(nMark == 0)
        {
            return;
        }
        miniAdo ado;
        CString strSQL;
        if(ado.Open(m_DateDB.GetActiveConnection()))
        {
            for(int i = 0; i < m_pInfoList->GetCount(); i++)
            {
                CDuiListContainerElement* pContainer = static_cast<CDuiListContainerElement*>(m_pInfoList->GetItemAt(i));
                if(pContainer)
                {
                    CDuiOption* pOption = static_cast<CDuiOption*>(pContainer->GetItemAt(0));
                    if(pOption && pOption->IsSelected() && pContainer->IsVisible())
                    {
                        int nID, nAddMark, nDelMark = 0;
                        CDuiLabel* pLabelID = static_cast<CDuiOption*>(pContainer->GetItemAt(1));
                        if(pLabelID)
                        {
                            nID =  _ttoi(pLabelID->GetText());
                        }

                        CDuiLabel* pLabelAddMark = static_cast<CDuiOption*>(pContainer->GetItemAt(4));
                        if(pLabelAddMark)
                        {
                            nAddMark =  _ttoi(pLabelAddMark->GetText());
                        }

                        CDuiLabel* pLabelDelMark = static_cast<CDuiOption*>(pContainer->GetItemAt(5));
                        if(pLabelDelMark)
                        {
                            nDelMark =  _ttoi(pLabelDelMark->GetText());
                        }
                        int nResAdd = nAddMark + nMark;
                        int nRes = nAddMark + nMark - nDelMark;

                        CDuiString strResAdd;
                        strResAdd.Format(_T("%d"), nResAdd);

                        CDuiString strRes;
                        strRes.Format(_T("%d"), nRes);
                        // 更新累计分数
                        pLabelAddMark->SetText(strResAdd);
                        // 更新当前分数
                        CDuiLabel* pLabelMark = static_cast<CDuiOption*>(pContainer->GetItemAt(6));
                        pLabelMark->SetText(strRes);

                        // 更新时间
                        CDuiLabel* pLabelTime = static_cast<CDuiOption*>(pContainer->GetItemAt(7));
                        CTime tm = CTime::GetCurrentTime();
                        CDuiString strTime;
                        strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetMinute());
                        pLabelTime->SetText(strTime);

                        strSQL.Format(_T("update PlayerInfo set AddMark=%d,Mark=%d,CreateTime=now() where ID=%d"), nResAdd, nRes, nID);
                        ado.Execute(strSQL);

                        CString strRecord;
                        strRecord.Format(_T("加分操作：加[%d]分 当前分数[%d] 备注[%s]"), nMark, nRes, strRemarks.GetData());
                        strSQL.Format(_T("insert into HistoryRecord (Owner,CreateTime,Record) VALUES (%d,now(),'%s')"), nID, strRecord);
                        ado.Execute(strSQL);
                    }
                }
            }
        }
    }
}

void CDKPManageFrame::OnDelMarkBtn(CDuiNotify& msg)
{
    CDuiString strMark;
    CDuiString strRemarks;
    COperateMarkDlg* dlg = new COperateMarkDlg(&strMark, &strRemarks);
    dlg->m_nType = TYPE_SUBTRACT;
    dlg->Create(GetHWND(), _T("Operate Mark Dialog"), UI_WNDSTYLE_DIALOG, WS_EX_TOOLWINDOW);
    dlg->CenterWindow();
    if(dlg->ShowModal() == IDOK)
    {
        int nMark = _ttoi(strMark);
        if(nMark == 0)
        {
            return;
        }
        miniAdo ado;
        CString strSQL;
        if(ado.Open(m_DateDB.GetActiveConnection()))
        {
            for(int i = 0; i < m_pInfoList->GetCount(); i++)
            {
                CDuiListContainerElement* pContainer = static_cast<CDuiListContainerElement*>(m_pInfoList->GetItemAt(i));
                if(pContainer)
                {
                    CDuiOption* pOption = static_cast<CDuiOption*>(pContainer->GetItemAt(0));
                    if(pOption && pOption->IsSelected() && pContainer->IsVisible())
                    {
                        int nID, nAddMark, nDelMark = 0;
                        CDuiLabel* pLabelID = static_cast<CDuiOption*>(pContainer->GetItemAt(1));
                        if(pLabelID)
                        {
                            nID =  _ttoi(pLabelID->GetText());
                        }

                        CDuiLabel* pLabelAddMark = static_cast<CDuiOption*>(pContainer->GetItemAt(4));
                        if(pLabelAddMark)
                        {
                            nAddMark =  _ttoi(pLabelAddMark->GetText());
                        }

                        CDuiLabel* pLabelDelMark = static_cast<CDuiOption*>(pContainer->GetItemAt(5));
                        if(pLabelDelMark)
                        {
                            nDelMark =  _ttoi(pLabelDelMark->GetText());
                        }
                        int nResDel = nDelMark + nMark;
                        int nRes = nAddMark - nMark - nDelMark;

                        CDuiString strResDel;
                        strResDel.Format(_T("%d"), nResDel);

                        CDuiString strRes;
                        strRes.Format(_T("%d"), nRes);
                        // 更新累计分数
                        pLabelDelMark->SetText(strResDel);
                        // 更新当前分数
                        CDuiLabel* pLabelMark = static_cast<CDuiOption*>(pContainer->GetItemAt(6));
                        pLabelMark->SetText(strRes);

                        // 更新时间
                        CDuiLabel* pLabelTime = static_cast<CDuiOption*>(pContainer->GetItemAt(7));
                        CTime tm = CTime::GetCurrentTime();
                        CDuiString strTime;
                        strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetMinute());
                        pLabelTime->SetText(strTime);

                        strSQL.Format(_T("update PlayerInfo set DelMark=%d,Mark=%d,CreateTime=now() where ID=%d"), nResDel, nRes, nID);
                        ado.Execute(strSQL);

                        CString strRecord;
                        strRecord.Format(_T("扣分操作：扣[%d]分 当前分数[%d] 备注[%s]"), nMark, nRes, strRemarks.GetData());
                        strSQL.Format(_T("insert into HistoryRecord (Owner,CreateTime,Record) VALUES (%d,now(),'%s')"), nID, strRecord);
                        ado.Execute(strSQL);
                    }
                }
            }
        }
    }
}

void CDKPManageFrame::OnUnselectedBtn(CDuiNotify& msg)
{
    for(int i = 0; i < m_pInfoList->GetCount(); i++)
    {
        CDuiListContainerElement* p = static_cast<CDuiListContainerElement*>(m_pInfoList->GetItemAt(i));
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
    CTime dt = CTime::GetCurrentTime();
    CString strFileName;
    strFileName.Format(_T("%04d年%02d月%02d日 %02d时%02d分%02d秒 DKP分数统计.xls"), dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond());

    CString sTempPath = CCommon::GetAppPath() + "DKPManagement\\Player Information.xls";

    CFileDialog FileDlg(FALSE, NULL, strFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("文件 (*.xls)|*.xls||"), NULL);
    if(FileDlg.DoModal() == IDOK)
    {
        strFileName = FileDlg.GetPathName();

        if(m_pInfoList->GetCount() == 0)
        {
            return;
        }

        if(CopyFile(sTempPath, strFileName, FALSE) == FALSE)
        {
            return ;
        }

        Excel::_ApplicationPtr ptrExcelApp;
        Excel::_WorkbookPtr ptrBook;
        Excel::_WorksheetPtr ptrSheet;
        Excel::RangePtr ptrRange;
        Excel::RangePtr ptrResizeRange;

        ptrExcelApp.CreateInstance(__uuidof(Excel::Application));
        if(ptrExcelApp == NULL)
        {
            return ;
        }

        int nRows = m_pInfoList->GetCount();
        int nCols = 6;

        DWORD nIndex[2];
        nIndex[0] = nRows;
        nIndex[1] = nCols;

        CComSafeArrayBound	bound[2];
        bound[0].SetCount(nRows);
        bound[0].SetLowerBound(0);
        bound[1].SetCount(nCols);
        bound[1].SetLowerBound(0);

        CComSafeArray<VARIANT> saData(bound, 2);

        CString sItemValue;

        for(nIndex[0] = 0; nIndex[0] < nRows; nIndex[0]++)
        {
            CDuiListContainerElement* pContainer = static_cast<CDuiListContainerElement*>(m_pInfoList->GetItemAt(nIndex[0]));
            for(nIndex[1] = 0; nIndex[1] < nCols; nIndex[1]++)
            {
                CDuiLabel* pLabel = static_cast<CDuiLabel*>(pContainer->GetItemAt(nIndex[1] + 1));
                sItemValue = pLabel->GetText().GetData();
                saData.MultiDimSetAt((long*)nIndex, CComVariant(sItemValue));
            }
        }

        ptrBook =  ptrExcelApp->Workbooks->Open(_bstr_t(strFileName));
        ptrSheet =  ptrBook->Sheets->GetItem(1);

        ptrRange = ptrSheet->GetRange(_variant_t(_T("A2")), _variant_t(_T("A2")));
        ptrResizeRange = ptrRange->GetResize(_variant_t(long(nRows)), _variant_t((long)nCols));

        _variant_t vArray;
        vArray.vt = VT_ARRAY | VT_VARIANT;
        vArray.parray  = saData.Detach();
        ptrResizeRange->Value = vArray;

        ptrBook->Save();
        ptrExcelApp->Quit();
    }
}

void CDKPManageFrame::OnSelectChanged(CDuiNotify& msg)
{
    if(msg.pSender->GetName() == _T("InfoOpt"))
    {
        if(m_pTab)
        {
            m_pTab->SelectItem(0);
        }
    }
    else if(msg.pSender->GetName() == _T("HisOpt"))
    {
        if(m_pTab)
        {
            // 历史操作记录
            miniAdo ado;
            if(ado.Open(m_DateDB.GetActiveConnection()))
            {
                CString strSQL;
                m_pHisList->RemoveAll();
                strSQL.Format(_T("select Owner,CreateTime,Record from HistoryRecord order by CreateTime"));
                ado.OpenRecordset(strSQL);
                while(!ado.get_EOF())
                {
                    CString strID = ado.GetFieldValueAsStr(_T("Owner"));
                    CString strTime = ado.GetFieldValueAsStr(_T("CreateTime"));
                    CString strRecord = ado.GetFieldValueAsStr(_T("Record"));
                    OnAddHisListItem(strID.GetBuffer(), strTime.GetBuffer(), strRecord.GetBuffer());
                    ado.Next();
                }
                ado.CloseRecordset();

                m_pHisCombo->RemoveAll();
                CDuiListLabelElement* pLabelAll = new CDuiListLabelElement;
                m_pHisCombo->Add(pLabelAll);
                pLabelAll->SetFixedHeight(25);
                pLabelAll->Select(TRUE);
                pLabelAll->SetText(_T("全部"));

                strSQL.Format(_T("select distinct Owner from HistoryRecord order by Owner"));
                ado.OpenRecordset(strSQL);
                while(!ado.get_EOF())
                {
                    CString strID = ado.GetFieldValueAsStr(_T("Owner"));
                    CDuiListLabelElement* pLabel = new CDuiListLabelElement;
                    m_pHisCombo->Add(pLabel);
                    pLabel->SetFixedHeight(25);
                    pLabel->SetText(strID.GetBuffer());
                    ado.Next();
                }
                ado.CloseRecordset();
            }
            m_pTab->SelectItem(1);
        }
    }

}

void CDKPManageFrame::OnItemSelect(CDuiNotify& msg)
{
    if(msg.pSender->GetName() == _T("TypeCombo"))
    {
        OnTypeComboItemSelect();
        OnNameEditTextChanged();
    }
    else if(msg.pSender->GetName() == _T("HisCombo"))
    {
        for(int i = 0; i < m_pHisList->GetCount(); i++)
        {
            CDuiListContainerElement* pContainer = static_cast<CDuiListContainerElement*>(m_pHisList->GetItemAt(i));
            if(pContainer)
            {
                pContainer->SetVisible(TRUE);
            }
        }
        if(m_pHisCombo && m_pHisCombo->GetCurSel() > 0)
        {
            for(int i = 0; i < m_pHisList->GetCount(); i++)
            {
                CDuiListContainerElement* pContainer = static_cast<CDuiListContainerElement*>(m_pHisList->GetItemAt(i));
                if(pContainer)
                {
                    CDuiLabel* pLabel = static_cast<CDuiLabel*>(pContainer->GetItemAt(0));
                    if(pLabel && pLabel->GetText() != m_pHisCombo->GetText())
                    {
                        pContainer->SetVisible(FALSE);
                    }
                }
            }
        }
    }
}

void CDKPManageFrame::OnTextChanged(CDuiNotify& msg)
{
    CDuiString strName = msg.pSender->GetName();
    if(strName == _T("NameEdit"))
    {
        OnTypeComboItemSelect();
        OnNameEditTextChanged();
    }
    if(!strName.IsEmpty() && _ttoi(strName) > 0)
    {
        CDuiEdit* pEdit = static_cast<CDuiEdit*>(msg.pSender);
        int nID = _ttoi(strName);
        miniAdo ado;
        if(ado.Open(m_DateDB.GetActiveConnection()))
        {
            CString strSQL;
            strSQL.Format(_T("update PlayerInfo set Remarks='%s' where ID=%d"), pEdit->GetText().GetData(), nID);
            ado.Execute(strSQL);
        }
    }
}

void CDKPManageFrame::OnTypeComboItemSelect()
{
    for(int i = 0; i < m_pInfoList->GetCount(); i++)
    {
        CDuiListContainerElement* pContainer = static_cast<CDuiListContainerElement*>(m_pInfoList->GetItemAt(i));
        if(pContainer)
        {
            pContainer->SetVisible(TRUE);
        }
    }
    if(m_pTypeCombo && m_pTypeCombo->GetCurSel() > 0 && m_pTypeCombo->GetText() == g_Profession[m_pTypeCombo->GetCurSel() - 1])
    {
        for(int i = 0; i < m_pInfoList->GetCount(); i++)
        {
            CDuiListContainerElement* pContainer = static_cast<CDuiListContainerElement*>(m_pInfoList->GetItemAt(i));
            if(pContainer)
            {
                CDuiLabel* pLabel = static_cast<CDuiLabel*>(pContainer->GetItemAt(3));
                if(pLabel && pLabel->GetText() != m_pTypeCombo->GetText())
                {
                    pContainer->SetVisible(FALSE);
                }
            }
        }
    }
}

void CDKPManageFrame::OnNameEditTextChanged()
{
    CDuiString strName = m_pNameEdit->GetText();
    CString strSQL;
    miniAdo ado;
    if(!strName.IsEmpty() && ado.Open(m_DateDB.GetActiveConnection()))
    {
        CString strType;
        vector<int> vtIDs;
        if(m_pTypeCombo && m_pTypeCombo->GetCurSel() > 0 && m_pTypeCombo->GetText() == g_Profession[m_pTypeCombo->GetCurSel() - 1])
        {
            strType.Format(_T("and Profession='%s'"), m_pTypeCombo->GetText().GetData());
        }
        strSQL.Format(_T("select ID from PlayerInfo where Name like '%%%s%%' %s"), strName.GetData(), strType);
        ado.OpenRecordset(strSQL);
        while(!ado.get_EOF())
        {
            vtIDs.push_back(ado.GetFieldValueAsInt(_T("ID")));
            ado.Next();
        }
        ado.CloseRecordset();

        for(int i = 0; i < m_pInfoList->GetCount(); i++)
        {
            CDuiListContainerElement* pContainer = static_cast<CDuiListContainerElement*>(m_pInfoList->GetItemAt(i));
            if(pContainer)
            {
                CDuiLabel* pLabel = static_cast<CDuiLabel*>(pContainer->GetItemAt(1));
                BOOL bExist = FALSE;
                for(int j = 0; j < (int)vtIDs.size() && pLabel; j++)
                {
                    if(vtIDs[j] == _ttoi(pLabel->GetText()))
                    {
                        bExist = TRUE;
                        break;
                    }
                }
                if(bExist == FALSE)
                {
                    pContainer->SetVisible(FALSE);
                }
            }
        }
    }
}

BOOL CDKPManageFrame::OnAddListItem(UINT uID, CDuiString strName, CDuiString strProfession, int nAddMark, int nDelMark, int nMark, CDuiString strTime, CDuiString strRemarks)
{
    if(strName.IsEmpty() || strProfession.IsEmpty() || strTime.IsEmpty())
    {
        return FALSE;
    }
    CDuiListContainerElement* pContainer = new CDuiListContainerElement;
    m_pInfoList->Add(pContainer);
    pContainer->SetFixedHeight(24);

    CDuiOption* pOption = new CDuiOption;
    pContainer->Add(pOption);
    pOption->SetAttribute(_T("selectedimage"), _T("file='CheckBox/checked.png' dest='4,4,20,20'"));
    pOption->SetAttribute(_T("normalimage"), _T("file='CheckBox/unchecked.png' dest='4,4,20,20'"));

    // 编号
    CDuiLabel* pLabelID = new CDuiLabel;
    CDuiString strID;
    strID.Format(_T("%u"), uID);
    pContainer->Add(pLabelID);
    pLabelID->SetAttribute(_T("align"), _T("center"));
    pLabelID->SetText(strID);
    // 姓名
    CDuiLabel* pLabelName = new CDuiLabel;
    pContainer->Add(pLabelName);
    pLabelName->SetAttribute(_T("align"), _T("center"));
    pLabelName->SetText(strName);
    // 职业
    CDuiLabel* pLabelProfession = new CDuiLabel;
    pContainer->Add(pLabelProfession);
    pLabelProfession->SetAttribute(_T("align"), _T("center"));
    pLabelProfession->SetText(strProfession);

    // 累计分数

    CDuiLabel* pLabelAdd = new CDuiLabel;
    CDuiString strAdd;
    strAdd.Format(_T("%d"), nAddMark);
    pContainer->Add(pLabelAdd);
    pLabelAdd->SetAttribute(_T("align"), _T("center"));
    pLabelAdd->SetText(strAdd);

    CDuiLabel* pLabelDel = new CDuiLabel;
    CDuiString strDel;
    strDel.Format(_T("%d"), nDelMark);
    pContainer->Add(pLabelDel);
    pLabelDel->SetAttribute(_T("align"), _T("center"));
    pLabelDel->SetText(strDel);

    CDuiLabel* pLabelMark = new CDuiLabel;
    CDuiString strMark;
    strMark.Format(_T("%d"), nMark);
    pContainer->Add(pLabelMark);
    pLabelMark->SetAttribute(_T("align"), _T("center"));
    pLabelMark->SetText(strMark);
    // 时间
    CDuiLabel* pLabelTime = new CDuiLabel;
    pContainer->Add(pLabelTime);
    pLabelTime->SetAttribute(_T("align"), _T("center"));
    pLabelTime->SetText(strTime);

    CDuiEdit* pEdit = new CDuiEdit;
    pContainer->Add(pEdit);
    pEdit->SetAttribute(_T("style"), _T("edit_style"));
    pEdit->SetText(strRemarks);
    pEdit->SetName(strID);
    return TRUE;
}

BOOL CDKPManageFrame::OnAddHisListItem(CDuiString strID, CDuiString strTime, CDuiString strRecord)
{
    if(strID.IsEmpty() || strTime.IsEmpty())
    {
        return FALSE;
    }
    CDuiListContainerElement* pContainer = new CDuiListContainerElement;
    m_pHisList->Add(pContainer);
    pContainer->SetFixedHeight(24);

    CDuiLabel* pLabelID = new CDuiLabel;
    pContainer->Add(pLabelID);
    pLabelID->SetAttribute(_T("align"), _T("center"));
    pLabelID->SetText(strID);

    CDuiLabel* pLabelTime = new CDuiLabel;
    pContainer->Add(pLabelTime);
    pLabelTime->SetAttribute(_T("align"), _T("center"));
    pLabelTime->SetText(strTime);

    CDuiLabel* pLabelRecord = new CDuiLabel;
    pContainer->Add(pLabelRecord);
    pLabelRecord->SetText(strRecord);
    return TRUE;
}
