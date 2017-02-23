#pragma once
class CMessageBox
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CMessageBox(void);
    virtual ~CMessageBox(void);
public:
    void SetMsg(LPCTSTR lpstrMsg);
    void SetTitle(LPCTSTR lpstrTitle);

    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;
    void OnClick(CDuiNotify& msg);

    static int MessageBox(HWND hParent, LPCTSTR lpstrTitle, LPCTSTR lpstrMsg);
    static void ShowMessageBox(HWND hParent, LPCTSTR lpstrTitle, LPCTSTR lpstrMsg);
};

