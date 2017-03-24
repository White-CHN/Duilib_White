#pragma once
#include "Utility.h"
#include "MySqlite.h"

const CDuiString g_Profession[8] = {_T("ÊØ»¤"), _T("½£ÐÇ"), _T("ÖÎÓú"), _T("»¤·¨"),
                                    _T("Ä§µÀ"), _T("¾«Áé"), _T("É±ÐÇ"), _T("¹­ÐÇ")
                                   };

enum
{
    TYPE_ADD,
    TYPE_SUBTRACT
};

class CDKPManageFrame
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CDKPManageFrame(void);
    virtual ~CDKPManageFrame(void);
public:
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
    void InitWindow() OVERRIDE;

    void OnClick(CDuiNotify& msg);
    void OnAddBtn(CDuiNotify& msg);
    void OnDelBtn(CDuiNotify& msg);
    void OnAddMarkBtn(CDuiNotify& msg);
    void OnDelMarkBtn(CDuiNotify& msg);
    void OnClearMarkBtn(CDuiNotify& msg);
    void OnUnselectedBtn(CDuiNotify& msg);
    void OnExcelBtn(CDuiNotify& msg);
    void OnSelectChanged(CDuiNotify& msg);
    void OnItemSelect(CDuiNotify& msg);
    void OnTextChanged(CDuiNotify& msg);

    void OnTypeComboItemSelect();
    void OnNameEditTextChanged();
private:
    void OnInitInfoOrderBy(CString strOrder);
    BOOL OnAddListItem(UINT uID, CDuiString strName, CDuiString strProfession, int nAddMark, int nDelMark, int nMark, CDuiString strTime, CDuiString strRemarks);
    BOOL OnAddHisListItem(CDuiString strID, CDuiString strTime, CDuiString strRecord);
private:
    CDuiList* m_pInfoList;
    CDuiList* m_pHisList;
    CDuiAnimationTabLayout* m_pTab;
    CDuiCombo* m_pTypeCombo;
    CDuiEdit* m_pNameEdit;
    CDuiCombo* m_pHisCombo;
    CDuiLabel* m_pTipLabel;
    CMySqlite m_MySqlite;
};

