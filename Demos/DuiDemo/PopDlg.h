#pragma once
class CPopDlg
    : public CDuiDlgImplBase
{
public:
    CPopDlg(void);
    virtual ~CPopDlg(void);
public:
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;

    void InitWindow() OVERRIDE;

    CDuiControl* CreateControl(LPCTSTR pstrClass) OVERRIDE;
    void Notify(TNotifyUI& msg) OVERRIDE;
private:
    CDuiButton* m_pCloseBtn;
    CDuiButton* m_pMaxBtn;
    CDuiButton* m_pRestoreBtn;
    CDuiButton* m_pMinBtn;
};

