#pragma once
class CPlayerInfoDlg
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CPlayerInfoDlg(CDuiString* pName, CDuiString* pProfession);
    virtual ~CPlayerInfoDlg(void);
public:
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;
    void InitWindow() OVERRIDE;

    void OnClick(CDuiNotify& msg);
private:
    CDuiEdit* m_pNameEdit;
    CDuiCombo* m_pCombo;
    CDuiString* m_pName;
    CDuiString* m_pProfession;
};

