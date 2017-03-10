#pragma once
class CPlayerInfoDlg
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CPlayerInfoDlg(void);
    virtual ~CPlayerInfoDlg(void);
public:
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;
    void InitWindow() OVERRIDE;

    void OnClick(CDuiNotify& msg);
};

