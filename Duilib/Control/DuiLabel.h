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
        void SetFont(int index);
        int GetFont() const;

        void SetTextColor(DWORD dwTextColor);

        void SetDisabledTextColor(DWORD dwTextColor);

        RECT GetTextPadding() const;
        void SetTextPadding(RECT rc);

        void SetShowHtml(BOOL bShowHtml = TRUE);

        virtual void SetAutoCalcWidth(BOOL bAutoCalcWidth);

        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);
        virtual UINT GetControlFlags() const;
        virtual SIZE EstimateSize(SIZE szAvailable);
        virtual void DoEvent(TEventUI& event);
        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        virtual void PaintText(HDC hDC);
    protected:
        BOOL	m_bShowHtml;
        BOOL	m_bAutoCalcWidth;
        int		m_iFont;
        DWORD	m_dwTextColor;
        DWORD	m_dwDisabledTextColor;
        UINT	m_uTextStyle;
        RECT	m_rcTextPadding;
    };


}