#pragma once
#include "DuiAnimLayout.h"
class CAnimDlg
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CAnimDlg(void);
    virtual ~CAnimDlg(void);
public:
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;

    CDuiControl* CreateControl(LPCTSTR pstrClassName) OVERRIDE;

    void InitWindow() OVERRIDE;
    void OnWindowInit(TNotifyUI& msg);

    void OnClick(TNotifyUI& msg);
private:
    CDuiAnimLayout*		m_pAnimLayout;
};

