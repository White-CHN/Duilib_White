#pragma once

namespace DuiLib
{
    class DUILIB_API CDuiWnd
    {
    public:
        CDuiWnd(void);
        virtual ~CDuiWnd(void);
    public:
        HWND GetHWND() const;
        operator HWND() const;

        HWND Create(HWND hWndParent, LPCTSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle, const CDuiRect rc, HMENU hMenu = NULL);
        HWND Create(HWND hWndParent, LPCTSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT, HMENU hMenu = NULL);

        LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
        LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

        HWND SubClass(HWND hWnd);
        void UnSubClass();

        BOOL ShowWindow(BOOL bShow = TRUE, BOOL bTakeFocus = TRUE);
        BOOL CenterWindow();	// 居中，支持扩展屏幕

        UINT_PTR ShowModal();

        virtual void Close(UINT nRet = IDOK);

        void SetIcon(UINT nRes);

        BOOL ResizeClient(int cx = -1, int cy = -1);
    protected:
        virtual UINT GetClassStyle() const;
        virtual LPCTSTR GetSuperClassName() const;
        virtual LPCTSTR GetWindowClassName() const = 0;

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void OnFinalMessage(HWND hWnd);
    private:
        BOOL RegisterWindowClass();
        BOOL RegisterSuperClass();

        static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    private:
        HWND m_hWnd;
        BOOL m_bSubclassed;
        WNDPROC m_OldWndProc;
    };


}
