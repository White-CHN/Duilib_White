#pragma once
namespace DuiLib
{
#define  GROUPBOX_TEXT_OFFSET		20		//定义GroupBox中的Text相对于左边的偏移

    class DUILIB_API CDuiGroupBox
        : public CDuiVerticalLayout
    {
        DECLARE_DUICONTROL(CDuiGroupBox)
    public:
        CDuiGroupBox(void);
        virtual ~CDuiGroupBox(void);
    public:
        LPCTSTR GetClass() const override;
        LPVOID GetInterface(LPCTSTR pstrName) override;

        DWORD GetTextColor() const;
        void SetTextColor(DWORD dwTextColor);

        DWORD GetDisabledTextColor() const;
        void SetDisabledTextColor(DWORD dwTextColor);

        int GetFont() const;
        void SetFont(int index);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

        void PaintText(HDC hDC) override;
        void PaintBorder(HDC hDC) override;
    private:
        SIZE CalcrectSize(SIZE szAvailable);
    private:
        int m_iFont;
        DWORD m_dwTextColor;
        DWORD m_dwDisabledTextColor;
        UINT m_uTextStyle;
    };

}