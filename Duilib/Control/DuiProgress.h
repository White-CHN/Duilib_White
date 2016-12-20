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
        LPCTSTR GetClass() const override;
        LPVOID GetInterface(LPCTSTR pstrName) override;

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

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

        void PaintForeColor(HDC hDC) override;
        void PaintForeImage(HDC hDC) override;
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
