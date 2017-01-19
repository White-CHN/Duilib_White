#pragma once
class CSkinSliderDlg
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CSkinSliderDlg(CDuiPaintManager* pPaintManager, CDuiButton* pBtn);
    virtual ~CSkinSliderDlg(void);
public:
    void InitWindow() OVERRIDE;
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;
    LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
    LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
    void OnValueChanged(TNotifyUI& msg);
    void OnValueChangedMove(TNotifyUI& msg);
private:
    CDuiPaintManager* m_pPaintManager;
    CDuiButton* m_pBtn;
    CDuiSlider* m_pSlider;
};

