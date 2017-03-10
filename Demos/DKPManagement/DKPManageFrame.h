#pragma once
#include "MiniAdo.h"
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
    void OnUnselectedBtn(CDuiNotify& msg);
    void OnExcelBtn(CDuiNotify& msg);
private:
    CDuiList* m_pList;
    CADODatabase m_DateDB;
};

