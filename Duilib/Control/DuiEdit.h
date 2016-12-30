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
        LPCTSTR GetWindowClassName() const OVERRIDE;
        LPCTSTR GetSuperClassName() const OVERRIDE;

        RECT CalPos();
        BOOL Init(CDuiEdit* pOwner);

        void OnFinalMessage(HWND hWnd) OVERRIDE;

        virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnPrint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) OVERRIDE;
    private:
        enum
        {
            TIMERID_CARET			=	0x1999
        };
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
        CDuiString GetClass() const  OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName)  OVERRIDE;
        UINT GetControlFlags() const  OVERRIDE;

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

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void PaintStatusImage(HDC hDC) OVERRIDE;
        void PaintText(HDC hDC) OVERRIDE;
        void DoEvent(TEventUI& event) OVERRIDE;
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
