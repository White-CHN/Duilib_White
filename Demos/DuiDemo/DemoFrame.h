#pragma once
class CDemoFrame : public CDuiDlgImplBase
{
public:
    CDemoFrame(void);
    virtual ~CDemoFrame(void);
public:
    void InitWindow() override;
    CDuiString GetSkinFile() override;
    LPCTSTR GetWindowClassName() const override;

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

    CDuiControl* CreateControl(LPCTSTR pstrClass) override;

    LPCTSTR QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType) override;

    void Notify(TNotifyUI& msg)  override;
private:
    BOOL bEnglish;
    CDuiButton* m_pCloseBtn;
    CDuiButton* m_pMaxBtn;
    CDuiButton* m_pRestoreBtn;
    CDuiButton* m_pMinBtn;
    CDuiButton* m_pSkinBtn;
    CDuiSlider* m_pSlider;
    CDuiProgress* m_pProgress;
    CDuiIcon m_Icon;
};

