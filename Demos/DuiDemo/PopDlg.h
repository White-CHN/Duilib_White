#pragma once
class CPopDlg
    : public CDuiDlgImplBase
{
public:
    CPopDlg(void);
    virtual ~CPopDlg(void);
public:
    virtual CDuiString GetSkinFile();
    virtual LPCTSTR GetWindowClassName() const;

    virtual void InitWindow();

    virtual CDuiControl* CreateControl(LPCTSTR pstrClass);
    virtual void Notify(TNotifyUI& msg) ;
private:
    CDuiButton* m_pCloseBtn;
    CDuiButton* m_pMaxBtn;
    CDuiButton* m_pRestoreBtn;
    CDuiButton* m_pMinBtn;
};

