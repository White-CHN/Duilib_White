#pragma once

namespace DuiLib
{

    class DUILIB_API CDuiHorizontalLayout
        : public CDuiContainer
    {
        DECLARE_DUICONTROL(CDuiHorizontalLayout)
    public:
        CDuiHorizontalLayout(void);
        virtual ~CDuiHorizontalLayout(void);

    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString strName) OVERRIDE;
        UINT GetControlFlags() const OVERRIDE;

        void SetSepWidth(int iWidth);
        void SetSepImmMode(BOOL bImmediately);
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;

        RECT GetThumbRect(BOOL bUseNew = FALSE) const;
        void DoEvent(CDuiEvent& event) OVERRIDE;

        void DoPostPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;
    private:
        BOOL m_bImmMode;
        int m_iSepWidth;
        UINT m_uButtonState;
        CDuiPoint ptLastMouse;
        CDuiRect m_rcNewPos;
    };

}