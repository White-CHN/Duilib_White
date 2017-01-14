#pragma once

namespace DuiLib
{

    class DUILIB_API CDuiDlgImplBase
        : public CDuiWnd
        , public IMessageFilterUI
        , public IDialogBuilderCallback
        , public INotifyUI
        , public IQueryControlText
        , public CDuiNotifyPump
    {
        DUI_DECLARE_MESSAGE_MAP()
    public:
        CDuiDlgImplBase(void);
        virtual ~CDuiDlgImplBase(void);
    public:
        CDuiPaintManager* GetPaintManager();
    protected:
        virtual void InitWindow();

        virtual CDuiString GetSkinType();		//FindResource第三个参数，资源类型，自定义资源类型的名称
        virtual CDuiString GetSkinFile() = 0;

        void Notify(TNotifyUI& msg) OVERRIDE;
        CDuiControl* CreateControl(LPCTSTR pstrClass) OVERRIDE;
        LPCTSTR QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType) OVERRIDE;

        virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
        LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;

        virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        void OnFinalMessage(HWND hWnd) OVERRIDE;

        virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#if defined(WIN32) && !defined(UNDER_CE)
        virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif
        virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) OVERRIDE;
    private:
        BOOL IsInStaticControl(CDuiControl* pControl);
    private:
        CDuiPaintManager m_PaintManager;
        vector<CDuiString> m_vtStaticName;
    };


}
