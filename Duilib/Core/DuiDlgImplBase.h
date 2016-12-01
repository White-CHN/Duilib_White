#pragma once

namespace DuiLib
{

    class DUILIB_API CDuiDlgImplBase
        : public CDuiWnd
        , public IMessageFilterUI
        , public IDialogBuilderCallback
        , public INotifyUI
        , public IQueryControlText
    {
    public:
        CDuiDlgImplBase(void);
        ~CDuiDlgImplBase(void);
    public:
        CDuiPaintManager* GetPaintManager();
    protected:
        virtual void InitWindow();

        virtual CDuiString GetSkinType();		//FindResource��������������Դ���ͣ��Զ�����Դ���͵�����
        virtual CDuiString GetSkinFile() = 0;

        virtual void Notify(TNotifyUI& msg);

        virtual LPCTSTR QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType);

        virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
        virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        virtual void OnFinalMessage(HWND hWnd);

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

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    private:
        BOOL IsInStaticControl(CDuiControl* pControl);
    private:
        CDuiPaintManager m_PaintManager;
    };


}
