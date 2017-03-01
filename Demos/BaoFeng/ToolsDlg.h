#pragma once
class CToolsDlg
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CToolsDlg(void);
    virtual ~CToolsDlg(void);
public:
    void InitWindow() OVERRIDE;
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;

    void OnClick(CDuiNotify& msg);
private:
    CDuiAnimationTabLayout* m_pTabSwitch;
};

