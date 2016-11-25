#pragma once
class CDemoFrame : public CDuiDlgImplBase
{
public:
    CDemoFrame(void);
    virtual ~CDemoFrame(void);
    virtual void InitWindow();
    virtual CDuiString GetSkinFile();
    virtual LPCTSTR GetWindowClassName() const;

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnFinalMessage(HWND hWnd);

    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;

    virtual CDuiControl* CreateControl(LPCTSTR pstrClass);

    virtual void Notify(TNotifyUI& msg) ;
private:
    CDuiButton* m_pCloseBtn;
    CDuiButton* m_pMaxBtn;
    CDuiButton* m_pRestoreBtn;
    CDuiButton* m_pMinBtn;
    CDuiButton* m_pSkinBtn;
    CDuiSlider* m_pSlider;
    CDuiProgress* m_pProgress;
    CDuiIcon m_Icon;
};

