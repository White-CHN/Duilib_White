#include "StdAfx.h"
#include "DemoFrame.h"
#include "PopDlg.h"
#include "MessageBox.h"
#include "SkinDlg.h"
#include "AnimDlg.h"

DUI_BEGIN_MESSAGE_MAP(CDemoFrame, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_WINDOWINIT()
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED_MOVE, OnValueChangedMove)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_COLORCHANGED, OnColorChanged)
DUI_END_MESSAGE_MAP()

CDemoFrame::CDemoFrame(void)
    : bEnglish(FALSE)
    , m_bIsSplit(FALSE)
    , m_pTabSwitch(NULL)
    , m_pSlider(NULL)
    , m_pProgress(NULL)
    , m_pMenu(NULL)
    , m_pComboControlNames(NULL)
    , m_pListControl(NULL)
    , m_pEditXML(NULL)
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

    m_pTabSwitch = static_cast<CDuiAnimationTabLayout*>(GetPaintManager()->FindControl(_T("tab_switch")));
    m_pListControl = static_cast<CDuiList*>(GetPaintManager()->FindControl(_T("list_xml")));
    m_pSlider = static_cast<CDuiSlider*>(GetPaintManager()->FindControl(_T("Slider")));
    m_pProgress = static_cast<CDuiProgress*>(GetPaintManager()->FindControl(_T("Progress")));
    m_pEditXML = static_cast<CDuiEdit*>(GetPaintManager()->FindControl(_T("EditXML")));

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
    if(pList)
    {
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

    m_pComboControlNames = static_cast<CDuiCombo*>(GetPaintManager()->FindControl(_T("ComboControlNames")));
    if(m_pComboControlNames != NULL)
    {
        m_pComboControlNames->SetItemTextPadding(CDuiRect(5, 0, 0, 0));
        for(map<CDuiString, IControl*>::iterator it = m_ControlsName.m_mapControlsName.begin(); it != m_ControlsName.m_mapControlsName.end(); ++it)
        {

            CDuiListLabelElement* pItem = new CDuiListLabelElement;
            m_pComboControlNames->Add(pItem);
            CDuiString str = it->first;
            pItem->SetText(str);
            pItem->SetAttribute(_T("height"), _T("25"));
        }
        if(m_pComboControlNames->GetCount() > 0)
        {
            m_pComboControlNames->SelectItem(0);
        }
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

void CDemoFrame::OnClick(TNotifyUI& msg)
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
    else if(msg.pSender->GetName() == _T("minbtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
        return;
    }
    else if(msg.pSender->GetName() == _T("maxbtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        return;
    }
    else if(msg.pSender->GetName() == _T("restorebtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
        return;
    }
    else if(msg.pSender->GetName() == _T("closebtn"))
    {
        if(CMessageBox::MessageBox(GetHWND(), _T("提示"), _T("是否关闭")) == IDOK)
        {
            Close();
        }
        return;
    }
    else if(msg.pSender->GetName() == _T("menubtn"))
    {
        DUI_FREE_POINT(m_pMenu);
        m_pMenu = new CDuiMenuWnd();
        CDuiPoint point;
        ::GetCursorPos(&point);
        m_pMenu->Init(NULL, _T("menu.xml"), point, GetPaintManager());
    }
    else if(msg.pSender->GetName() == _T("skinbtn"))
    {
        CSkinDlg* pSkinDlg = new CSkinDlg(this);
        pSkinDlg->Create(GetHWND(), _T("皮肤窗口"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
        pSkinDlg->CenterWindow();
        pSkinDlg->ShowModal();
    }
    else if(msg.pSender->GetName() == _T("dpi_btn"))
    {
        int nDPI = _ttoi(msg.pSender->GetUserData());
        GetPaintManager()->SetDPI(nDPI);
    }
    else if(msg.pSender->GetName() == _T("popwnd_btn"))
    {
        CPopDlg* pPopDlg = new CPopDlg();
        pPopDlg->Create(GetHWND(), _T("普通窗口演示"), UI_WNDSTYLE_DIALOG, WS_EX_TOOLWINDOW);
        pPopDlg->CenterWindow();
    }
    else if(msg.pSender->GetName() == _T("modal_popwnd_btn"))
    {
        CPopDlg* pPopDlg = new CPopDlg();
        pPopDlg->Create(GetHWND(), _T("模式窗口演示"), UI_WNDSTYLE_DIALOG, WS_EX_TOOLWINDOW);
        pPopDlg->CenterWindow();
        pPopDlg->ShowModal();
    }
    else if(msg.pSender->GetName() == _T("anim_popwnd_btn"))
    {
        CAnimDlg* pDlg = new CAnimDlg();
        pDlg->Create(GetHWND(), _T("动画窗口演示"), UI_WNDSTYLE_DIALOG, WS_EX_TOOLWINDOW);
        pDlg->CenterWindow();
    }
    else if(msg.pSender->GetName() == _T("BtnCreateXML"))
    {
        CDuiString strText;
        int nComboIndex = m_pComboControlNames->GetCurSel();
        int nListCount = m_pListControl->GetCount();
        if(nComboIndex >= 0 && nListCount > 0)
        {
            CDuiString strControlName = m_pComboControlNames->GetText();
            if(strControlName.IsEmpty())
            {
                return;
            }
            CDuiString strAttribute;
            for(int i = 0; i < nListCount; i++)
            {
                CDuiString str, strName, strValue;
                CDuiListContainerElement* pListItem = static_cast<CDuiListContainerElement*>(m_pListControl->GetItemAt(i));
                strName = pListItem->GetItemAt(0)->GetText();
                strValue = pListItem->GetItemAt(1)->GetText();
                if(strValue.IsEmpty() || strName.IsEmpty())
                {
                    continue;
                }
                strName += _T("=\"");
                strValue += _T("\" ");
                strAttribute += strName + strValue;
            }
            if(strAttribute.IsEmpty())
            {
                strText.Format(_T("<%s />"), strControlName);
            }
            else
            {
                strText.Format(_T("<%s "), strControlName);
                if(m_bIsSplit)
                {
                    strText = strText + strAttribute + _T("></");
                    strText = strText + strControlName + _T(">");
                }
                else
                {
                    strText = strText + strAttribute + _T("/>");
                }

            }
        }
        m_pEditXML->SetText(strText);
    }
}

void CDemoFrame::OnWindowInit(TNotifyUI& msg)
{

}

void CDemoFrame::OnValueChanged(TNotifyUI& msg)
{
    if(msg.pSender == m_pSlider && m_pProgress != NULL)
    {
        m_pProgress->SetValue(m_pSlider->GetValue());
    }
}

void CDemoFrame::OnValueChangedMove(TNotifyUI& msg)
{
    if(msg.pSender == m_pSlider && m_pProgress != NULL)
    {
        m_pProgress->SetValue(m_pSlider->GetValue());
    }
}

void CDemoFrame::OnItemSelect(TNotifyUI& msg)
{
    if(msg.pSender == m_pComboControlNames)
    {
        CDuiString strName = m_pComboControlNames->GetText();
        m_pListControl->RemoveAll();
        for(map<CDuiString, IControl*>::iterator it = m_ControlsName.m_mapControlsName.begin(); it != m_ControlsName.m_mapControlsName.end(); ++it)
        {
            if(strName == it->first)
            {
                IControl* pControl = it->second;
                for(size_t i = 0; i < pControl->m_vtAttributes.size(); i++)
                {
                    CDuiListContainerElement* pListItem  = new CDuiListContainerElement;
                    pListItem->SetChildVAlign(DT_VCENTER);
                    pListItem->SetChildAlign(DT_CENTER);
                    pListItem->SetFixedHeight(25);
                    pListItem->SetManager(GetPaintManager(), NULL, FALSE);
                    m_pListControl->Add(pListItem);

                    CDuiLabel* pLabel1 = new CDuiLabel;
                    pLabel1->SetManager(GetPaintManager(), NULL, FALSE);
                    pLabel1->SetAttribute(_T("align"), _T("center"));
                    pLabel1->SetText(pControl->m_vtAttributes[i].m_strName);
                    pListItem->Add(pLabel1);

                    if(pControl->m_vtAttributes[i].m_strType == DATATYPE_BOOL
                            || pControl->m_vtAttributes[i].m_strType == DATATYPE_FLOAT_ALIGN
                            || pControl->m_vtAttributes[i].m_strType == DATATYPE_CURSOR
                            || pControl->m_vtAttributes[i].m_strType == DATATYPE_ALIGN
                            || pControl->m_vtAttributes[i].m_strType == DATATYPE_VALIGN
                            || pControl->m_vtAttributes[i].m_strType == DATATYPE_ANIMATION)
                    {
                        CDuiCombo* pCombo = new CDuiCombo;
                        if(pCombo)
                        {
                            pListItem->Add(pCombo);
                            pCombo->SetAttribute(_T("style"), _T("combo_style"));
                            pCombo->SetAttribute(_T("align"), _T("center"));
                            pCombo->SetAttribute(_T("itemalign"), _T("center"));
                            vector<CDuiString> vtValues;
                            if(pControl->m_vtAttributes[i].m_strType == DATATYPE_BOOL)
                            {
                                vtValues.push_back(_T(""));
                                vtValues.push_back(_T("true"));
                                vtValues.push_back(_T("false"));
                            }
                            else if(pControl->m_vtAttributes[i].m_strType == DATATYPE_ANIMATION)
                            {
                                vtValues.push_back(_T(""));
                                vtValues.push_back(_T("vertical"));
                                vtValues.push_back(_T("horizontal"));
                            }
                            else if(pControl->m_vtAttributes[i].m_strType == DATATYPE_ALIGN)
                            {
                                vtValues.push_back(_T(""));
                                vtValues.push_back(_T("left"));
                                vtValues.push_back(_T("center"));
                                vtValues.push_back(_T("right"));
                            }
                            else if(pControl->m_vtAttributes[i].m_strType == DATATYPE_VALIGN)
                            {
                                vtValues.push_back(_T(""));
                                vtValues.push_back(_T("top"));
                                vtValues.push_back(_T("vcenter"));
                                vtValues.push_back(_T("bottom"));
                            }
                            else if(pControl->m_vtAttributes[i].m_strType == DATATYPE_FLOAT_ALIGN)
                            {
                                vtValues.push_back(_T(""));
                                vtValues.push_back(_T("null"));
                                vtValues.push_back(_T("left"));
                                vtValues.push_back(_T("center"));
                                vtValues.push_back(_T("right"));
                                vtValues.push_back(_T("top"));
                                vtValues.push_back(_T("vcenter"));
                                vtValues.push_back(_T("bottom"));
                            }

                            else if(pControl->m_vtAttributes[i].m_strType == DATATYPE_CURSOR)
                            {
                                vtValues.push_back(_T(""));
                                vtValues.push_back(_T("arrow"));
                                vtValues.push_back(_T("ibeam"));
                                vtValues.push_back(_T("wait"));
                                vtValues.push_back(_T("cross"));
                                vtValues.push_back(_T("uparrow"));
                                vtValues.push_back(_T("size"));
                                vtValues.push_back(_T("icon"));
                                vtValues.push_back(_T("sizenwse"));
                                vtValues.push_back(_T("sizenesw"));
                                vtValues.push_back(_T("sizewe"));
                                vtValues.push_back(_T("sizens"));
                                vtValues.push_back(_T("sizeall"));
                                vtValues.push_back(_T("no"));
                                vtValues.push_back(_T("hand"));
                            }
                            for(size_t i = 0; i < vtValues.size(); i++)
                            {
                                CDuiListLabelElement* pElement = new CDuiListLabelElement;
                                pElement->SetText(vtValues[i]);
                                pCombo->Add(pElement);
                            }
                        }
                    }
                    else
                    {
                        CDuiEdit* pEdit = new CDuiEdit;
                        pEdit->SetManager(GetPaintManager(), NULL, FALSE);
                        pEdit->SetAttribute(_T("style"), _T("edit_style"));
                        pEdit->SetText(pControl->m_vtAttributes[i].m_strValue);
                        pListItem->Add(pEdit);
                    }


                    CDuiLabel* pLabel2 = new CDuiLabel;
                    pLabel2->SetManager(GetPaintManager(), NULL, FALSE);
                    pLabel2->SetAttribute(_T("align"), _T("center"));
                    pLabel2->SetText(pControl->m_vtAttributes[i].m_strType);
                    pListItem->Add(pLabel2);

                    CDuiLabel* pLabel3 = new CDuiLabel;
                    pLabel3->SetManager(GetPaintManager(), NULL, FALSE);
                    pLabel3->SetAttribute(_T("align"), _T("center"));
                    pLabel3->SetText(pControl->m_vtAttributes[i].m_strDefault);
                    pListItem->Add(pLabel3);

                    CDuiLabel* pLabel4 = new CDuiLabel;
                    pLabel4->SetManager(GetPaintManager(), NULL, FALSE);
                    pLabel4->SetText(pControl->m_vtAttributes[i].m_strRemarks);
                    pListItem->Add(pLabel4);
                }
                break;
            }
        }
    }
}

void CDemoFrame::OnSelectChanged(TNotifyUI& msg)
{
    if(msg.pSender->GetName() == _T("basic_tab"))
    {
        m_pTabSwitch->SelectItem(0);
    }
    else if(msg.pSender->GetName() == _T("rich_tab"))
    {
        m_pTabSwitch->SelectItem(1);
    }
    else if(msg.pSender->GetName() == _T("ex_tab"))
    {
        m_pTabSwitch->SelectItem(2);
    }
    else if(msg.pSender->GetName() == _T("ani_tab"))
    {
        m_pTabSwitch->SelectItem(3);
    }
    else if(msg.pSender->GetName() == _T("split_tab"))
    {
        m_pTabSwitch->SelectItem(4);
    }
    else if(msg.pSender->GetName() == _T("xml_tab"))
    {
        m_pTabSwitch->SelectItem(5);
    }
    else if(msg.pSender->GetName() == _T("OptionSplit"))
    {
        CDuiOption* pOption = static_cast<CDuiOption*>(msg.pSender);
        if(pOption)
        {
            m_bIsSplit = pOption->IsSelected();
        }
    }
}

void CDemoFrame::OnColorChanged(TNotifyUI& msg)
{
    if(msg.pSender->GetName() == _T("Pallet"))
    {
        CDuiColorPalette* pColorPalette = static_cast<CDuiColorPalette*>(msg.pSender);
        GetPaintManager()->GetRoot()->SetBkImage(_T(""));
        GetPaintManager()->GetRoot()->SetBkColor(pColorPalette->GetSelectColor());
    }
}

LRESULT CDemoFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    if(uMsg == DUIMSG_MENU_CLICK)
    {
        bHandled = TRUE;
        MenuCmd* pMenuCmd = (MenuCmd*)wParam;
        if(pMenuCmd != NULL)
        {
            BOOL bChecked = pMenuCmd->bChecked;
            CDuiString sMenuName = pMenuCmd->strName;
            CDuiString sUserData = pMenuCmd->strUserData;
            CDuiString sText = pMenuCmd->strText;
            if(sMenuName.CompareNoCase(_T("lan")) == 0)
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
            }
            else if(sMenuName == _T("exit"))
            {
                Close(0);
            }
            else if(sMenuName == _T("sdi"))
            {
                ::PostThreadMessage(CDuiPaintManager::GetMainThreadID(), WM_USER, 0, 0L);
            }
        }
    }
    else if(uMsg == DUIMSG_ICON)
    {
        UINT uIconMsg = (UINT)lParam;
        if(uIconMsg == WM_LBUTTONUP)
        {
            BOOL bVisible = IsWindowVisible(GetHWND());
            ::ShowWindow(GetHWND(), !bVisible ?  SW_SHOW : SW_HIDE);
        }
        else if(uIconMsg == WM_RBUTTONUP)
        {
            DUI_FREE_POINT(m_pMenu);
            m_pMenu = new CDuiMenuWnd();
            CDuiPoint point;
            ::GetCursorPos(&point);
            point.x += 5;
            point.y -= 5;
            m_pMenu->Init(NULL, _T("menu.xml"), point, GetPaintManager(), NULL, eMenuAlignment_Left | eMenuAlignment_Bottom);
            // 动态添加后重新设置菜单的大小
            m_pMenu->ResizeMenu();
        }
    }

    return 0;
}

