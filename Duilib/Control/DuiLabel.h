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
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);
        virtual UINT GetControlFlags() const;

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

        BOOL IsShowHtml();
        void SetShowHtml(BOOL bShowHtml = TRUE);

        virtual void SetText(LPCTSTR pstrText);

        BOOL GetAutoCalcWidth() const;
        virtual void SetAutoCalcWidth(BOOL bAutoCalcWidth);

        virtual SIZE EstimateSize(SIZE szAvailable);
        virtual void DoEvent(TEventUI& event);
        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        virtual void PaintText(HDC hDC);
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