#pragma once
namespace DuiLib
{
    class CDuiEdit;
    class CDuiEditWnd : public CDuiWnd
    {
    public:
        CDuiEditWnd(void);
        virtual ~CDuiEditWnd(void);
    public:
        virtual LPCTSTR GetWindowClassName() const;
        virtual LPCTSTR GetSuperClassName() const;

        RECT CalPos();
        BOOL Init(CDuiEdit* pOwner);

        virtual void OnFinalMessage(HWND hWnd);

        virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnPrint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    private:
        BOOL m_bInit;
        BOOL m_bDrawCaret;
        HBRUSH m_hBkBrush;
        CDuiEdit* m_pOwner;
    };

    class CDuiEdit : public CDuiLabel
    {
        DECLARE_DUICONTROL(CDuiEdit)
    public:
        CDuiEdit(void);
        virtual ~CDuiEdit(void);
    public:
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);
        virtual UINT GetControlFlags() const;

        void EmptyEditWnd();

        BOOL IsReadOnly() const;
        void SetReadOnly(BOOL bReadOnly);

        void SetNumberOnly(BOOL bNumberOnly);

        BOOL IsPasswordMode() const;
        void SetPasswordMode(BOOL bPasswordMode);

        TCHAR GetPasswordChar() const;
        void SetPasswordChar(TCHAR cPasswordChar);

        UINT GetMaxChar() const;
        void SetMaxChar(UINT uMax);

        int GetWindowStyls() const;

        LPCTSTR GetNormalImage() const;
        void SetNormalImage(LPCTSTR pStrImage);
        LPCTSTR GetHotImage() const;
        void SetHotImage(LPCTSTR pStrImage);
        LPCTSTR GetFocusedImage() const;
        void SetFocusedImage(LPCTSTR pStrImage);
        LPCTSTR GetDisabledImage() const;
        void SetDisabledImage(LPCTSTR pStrImage);
        DWORD GetNativeEditBkColor() const;
        void SetNativeEditBkColor(DWORD dwBkColor);
        DWORD GetNativeEditTextColor() const;
        void SetNativeEditTextColor(LPCTSTR pStrColor);

        LPCTSTR GetTipValue() const;
        void SetTipValue(LPCTSTR pStrTipValue);
        DWORD GetTipValueColor() const;
        void SetTipValueColor(LPCTSTR pStrColor);

        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
        virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);
        virtual void PaintStatusImage(HDC hDC);
        virtual void PaintText(HDC hDC);
        virtual void DoEvent(TEventUI& event);
    private:
        TCHAR m_cPasswordChar;
        DWORD m_dwEditbkColor;
        DWORD m_dwEditTextColor;
        DWORD m_dwTipValueColor;
        BOOL m_bReadOnly;
        BOOL m_bPasswordMode;
        INT m_iWindowStyls;
        UINT m_uMaxChar;
        UINT m_uButtonState;
        CDuiEditWnd* m_pEditWnd;

        CDuiString m_sNormalImage;
        CDuiString m_sHotImage;
        CDuiString m_sFocusedImage;
        CDuiString m_sDisabledImage;
        CDuiString m_sTipValue;
    };

}
