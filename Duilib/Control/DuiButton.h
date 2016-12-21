#pragma once
namespace DuiLib
{

    class DUILIB_API CDuiButton
        : public CDuiLabel
    {
        DECLARE_DUICONTROL(CDuiButton)
    public:
        CDuiButton(void);
        virtual ~CDuiButton(void);
    public:
        LPCTSTR GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;
        UINT GetControlFlags() const OVERRIDE;

        virtual LPCTSTR GetNormalImage();
        virtual void SetNormalImage(LPCTSTR pStrImage);
        virtual LPCTSTR GetHotImage();
        virtual void SetHotImage(LPCTSTR pStrImage);
        virtual LPCTSTR GetPushedImage();
        virtual void SetPushedImage(LPCTSTR pStrImage);
        virtual LPCTSTR GetFocusedImage();
        virtual void SetFocusedImage(LPCTSTR pStrImage);
        virtual LPCTSTR GetDisabledImage();
        virtual void SetDisabledImage(LPCTSTR pStrImage);
        virtual LPCTSTR GetHotForeImage();
        virtual void SetHotForeImage(LPCTSTR pStrImage);

        int	 GetBindTabLayoutIndex();
        void BindTabIndex(int iBindTabIndex);
        LPCTSTR GetBindTabLayoutName();
        void BindTabLayoutName(LPCTSTR iTabLayoutName);
        void BindTriggerTabSel(int iSetSelectIndex = -1);
        void RemoveBindTabIndex();

        UINT GetButtonState();
        void SetButtonStste(UINT uButtonState);

        DWORD GetHotBkColor() const;
        void SetHotBkColor(DWORD dwColor);
        DWORD GetPushedBkColor() const;
        void SetPushedBkColor(DWORD dwColor);
        DWORD GetHotTextColor() const;
        void SetHotTextColor(DWORD dwColor);
        DWORD GetPushedTextColor() const;
        void SetPushedTextColor(DWORD dwColor);
        DWORD GetFocusedTextColor() const;
        void SetFocusedTextColor(DWORD dwColor);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void PaintText(HDC hDC) OVERRIDE;
        void PaintBkColor(HDC hDC) OVERRIDE;
        void PaintStatusImage(HDC hDC) OVERRIDE;
        void PaintForeImage(HDC hDC) OVERRIDE;

        BOOL Activate() OVERRIDE;
        void DoEvent(TEventUI& event) OVERRIDE;
    private:
        int	m_iBindTabIndex;
        UINT m_uButtonState;
        DWORD m_dwHotBkColor;
        DWORD m_dwPushedBkColor;
        DWORD m_dwHotTextColor;
        DWORD m_dwPushedTextColor;
        DWORD m_dwFocusedTextColor;

        CDuiString m_sNormalImage;
        CDuiString m_sHotImage;
        CDuiString m_sPushedImage;
        CDuiString m_sFocusedImage;
        CDuiString m_sDisabledImage;
        CDuiString m_sHotForeImage;
        CDuiString m_sPushedForeImage;//‘›Œ¥ µœ÷

        CDuiString m_sBindTabLayoutName;
    };

}
