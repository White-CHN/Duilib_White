#pragma once
class CDemoFrame : public CDuiDlgImplBase
{
public:
    CDemoFrame(void);
    virtual ~CDemoFrame(void);
public:
    void InitWindow() OVERRIDE;
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;

    CDuiControl* CreateControl(LPCTSTR pstrClass) OVERRIDE;

    LPCTSTR QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType) OVERRIDE;

    void Notify(TNotifyUI& msg)  OVERRIDE;

    LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
private:
    BOOL bEnglish;
    CDuiButton* m_pCloseBtn;
    CDuiButton* m_pMaxBtn;
    CDuiButton* m_pRestoreBtn;
    CDuiButton* m_pMinBtn;
    CDuiButton* m_pSkinBtn;
    CDuiButton* m_pMenuBtn;
    CDuiSlider* m_pSlider;
    CDuiProgress* m_pProgress;
    CDuiMenuWnd* m_pMenu;
    CDuiIcon m_Icon;
};

