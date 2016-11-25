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
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);

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

        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        virtual void PaintBkColor(HDC hDC);
        virtual void PaintStatusImage(HDC hDC);
        virtual void PaintForeImage(HDC hDC);
        virtual void PaintText(HDC hDC);
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