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
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);

        DWORD GetTextColor() const;
        void SetTextColor(DWORD dwTextColor);

        DWORD GetDisabledTextColor() const;
        void SetDisabledTextColor(DWORD dwTextColor);

        int GetFont() const;
        void SetFont(int index);

        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        virtual void PaintText(HDC hDC);
        virtual void PaintBorder(HDC hDC);
    private:
        SIZE CalcrectSize(SIZE szAvailable);
    private:
        int m_iFont;
        DWORD m_dwTextColor;
        DWORD m_dwDisabledTextColor;
        UINT m_uTextStyle;
        RECT m_rcTextPadding;
    };

}