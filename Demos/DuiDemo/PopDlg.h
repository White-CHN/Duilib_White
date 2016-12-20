#pragma once
class CPopDlg
    : public CDuiDlgImplBase
{
public:
    CPopDlg(void);
    virtual ~CPopDlg(void);
public:
    CDuiString GetSkinFile() override;
    LPCTSTR GetWindowClassName() const override;

    void InitWindow() override;

    CDuiControl* CreateControl(LPCTSTR pstrClass) override;
    void Notify(TNotifyUI& msg) override;
private:
    CDuiButton* m_pCloseBtn;
    CDuiButton* m_pMaxBtn;
    CDuiButton* m_pRestoreBtn;
    CDuiButton* m_pMinBtn;
};

