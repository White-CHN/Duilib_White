#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiOption :
        public CDuiButton
    {
        DECLARE_DUICONTROL(CDuiOption)
    public:
        CDuiOption(void);
        virtual ~CDuiOption(void);
    public:
        LPCTSTR GetClass() const override;
        LPVOID GetInterface(LPCTSTR pstrName) override;

        BOOL Activate() override;

        void SetGroup(LPCTSTR pStrGroupName = NULL);

        BOOL IsSelected() const;
        void Selected(BOOL bSelected);

        LPCTSTR GetSelectedImage();
        void SetSelectedImage(LPCTSTR pStrImage);

        LPCTSTR GetSelectedHotImage();
        void SetSelectedHotImage(LPCTSTR pStrImage);

        LPCTSTR GetSelectedPushedImage();
        void SetSelectedPushedImage(LPCTSTR pStrImage);

        void SetSelectedTextColor(DWORD dwTextColor);
        DWORD GetSelectedTextColor();

        void SetSelectedBkColor(DWORD dwBkColor);
        DWORD GetSelectBkColor();

        LPCTSTR GetSelectedForedImage();
        void SetSelectedForedImage(LPCTSTR pStrImage);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

        void PaintBkColor(HDC hDC) override;
        void PaintStatusImage(HDC hDC) override;
        void PaintForeImage(HDC hDC) override;
        void PaintText(HDC hDC) override;
    private:
        BOOL m_bSelected;

        DWORD m_dwSelectedBkColor;
        DWORD m_dwSelectedTextColor;

        CDuiString m_sGroupName;

        CDuiString m_sSelectedImage;
        CDuiString m_sSelectedHotImage;
        CDuiString m_sSelectedPushedImage;
        CDuiString m_sSelectedForeImage;
    };

}