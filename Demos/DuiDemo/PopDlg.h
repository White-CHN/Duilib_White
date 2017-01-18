#pragma once
class CPopDlg
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CPopDlg(void);
    virtual ~CPopDlg(void);
public:
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;
    void OnClick(TNotifyUI& msg);
};

