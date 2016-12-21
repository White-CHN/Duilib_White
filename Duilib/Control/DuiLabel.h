#pragma once

namespace DuiLib
{

    class DUILIB_API CDuiLabel
        : public CDuiControl
    {
        DECLARE_DUICONTROL(CDuiLabel)
    public:
        CDuiLabel(void);
        virtual ~CDuiLabel(void);
    public:
        LPCTSTR GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;
        UINT GetControlFlags() const OVERRIDE;

        int GetFont() const;
        void SetFont(int index);

        UINT GetTextStyle() const;
        void SetTextStyle(UINT uStyle);

        DWORD GetTextColor() const;
        void SetTextColor(DWORD dwTextColor);

        DWORD GetDisabledTextColor() const;
        void SetDisabledTextColor(DWORD dwTextColor);

        CDuiRect GetTextPadding() const;
        void SetTextPadding(CDuiRect rc);

        BOOL IsShowHtml() ;
        void SetShowHtml(BOOL bShowHtml = TRUE) ;

        void SetText(LPCTSTR pstrText) OVERRIDE;

        BOOL GetAutoCalcWidth() const;
        virtual void SetAutoCalcWidth(BOOL bAutoCalcWidth);

        SIZE EstimateSize(SIZE szAvailable) OVERRIDE;
        void DoEvent(TEventUI& event) OVERRIDE;
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void PaintText(HDC hDC) OVERRIDE;
    private:
        BOOL m_bShowHtml;
        BOOL m_bAutoCalcWidth;
        int	m_iFont;
        DWORD m_dwTextColor;
        DWORD m_dwDisabledTextColor;
        UINT m_uTextStyle;
        CDuiRect	m_rcTextPadding;
    };


}