#pragma once

namespace DuiLib
{

    class DUILIB_API CDuiChart :
        public CDuiControl
    {
        DECLARE_DUICONTROL(CDuiChart)
    public:
        CDuiChart(void);
        virtual ~CDuiChart(void);
    public:
        enum eChartType
        {
            CHARTVIEW_PIE,
            CHARTVIEW_HISTOGRAM
        };
        typedef struct tagCHARTITEM
        {
            CDuiString name;
            double value;
        } CHARTITEM, *LPCHARTITEM;
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString strName) OVERRIDE;

        BOOL Add(LPCTSTR name, double value);
        BOOL AddAt(LPCTSTR name, double value, int iIndex);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        virtual void DoPaintPie(HDC hDC, const RECT& rcPaint);
        virtual void DoPaintHistogram(HDC hDC, const RECT& rcPaint);
        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;

    private:
        BOOL m_bShowHtml;
        BOOL m_bShowText;

        int m_iFont;
        int m_ViewStyle;
        int m_PillarImageWidth;
        int m_ShadowImageHeight;

        DWORD m_dwTextColor;
        DWORD m_dwDisabledTextColor;

        CDuiString m_sShadowImage;
        CDuiString m_sPillarImage;

        vector<CHARTITEM> m_items;
    };

}