#pragma once
class CBoxDlg
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CBoxDlg(void);
    virtual ~CBoxDlg(void);
public:
    void InitWindow() OVERRIDE;
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;

    void OnClick(CDuiNotify& msg);
public:
    CDuiOption* m_pOptionBox;
    CDuiButton* m_pMaxBtn;
    CDuiButton* m_pRestoreBtn;
    CDuiRect m_rcClient;
};

