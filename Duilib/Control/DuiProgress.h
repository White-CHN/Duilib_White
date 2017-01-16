#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiProgress :
        public CDuiLabel
    {
        DECLARE_DUICONTROL(CDuiProgress)
    public:
        CDuiProgress(void);
        virtual ~CDuiProgress(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        BOOL IsShowText();
        void SetShowText(BOOL bShowText = TRUE);

        BOOL IsHorizontal() const;
        void SetHorizontal(BOOL bHorizontal = TRUE);

        int GetMinValue() const;
        void SetMinValue(int nMin);

        int GetMaxValue() const;
        void SetMaxValue(int nMax);

        int GetValue() const;
        virtual void UpdateText();
        void SetValue(int nValue);

        void SetStretchForeImage(BOOL bStretchForeImage = TRUE);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void PaintForeColor(HDC hDC) OVERRIDE;
        void PaintForeImage(HDC hDC) OVERRIDE;
        void PaintText(HDC hDC) OVERRIDE;
    private:
        BOOL m_bShowText;
        BOOL m_bHorizontal;
        BOOL m_bStretchForeImage;
        int m_nMin;
        int m_nMax;
        int m_nValue;
        CDuiString m_sForeImageModify;
    };


}
