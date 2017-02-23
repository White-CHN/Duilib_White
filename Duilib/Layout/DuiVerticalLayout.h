#pragma once

namespace DuiLib
{

    class DUILIB_API CDuiVerticalLayout
        : public CDuiContainer
    {
        DECLARE_DUICONTROL(CDuiVerticalLayout)
    public:
        CDuiVerticalLayout(void);
        virtual ~CDuiVerticalLayout(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;
        UINT GetControlFlags() const OVERRIDE;

        void SetSepHeight(int iHeight);
        void SetSepImmMode(BOOL bImmediately);
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;

        RECT GetThumbRect(BOOL bUseNew = FALSE) const;
        void DoEvent(CDuiEvent& event) OVERRIDE;

        void DoPostPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;
    private:
        BOOL m_bImmMode;
        int m_iSepHeight;
        UINT m_uButtonState;
        CDuiPoint ptLastMouse;
        CDuiRect m_rcNewPos;
    };


}
