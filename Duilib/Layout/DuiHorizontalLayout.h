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
        virtual CDuiString GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);
        virtual UINT GetControlFlags() const;

        void SetSepWidth(int iWidth);
        void SetSepImmMode(BOOL bImmediately);
        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
        virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);

        RECT GetThumbRect(BOOL bUseNew = FALSE) const;
        virtual void DoEvent(TEventUI& event);

        virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);
    private:
        BOOL m_bImmMode;
        int m_iSepWidth;
        UINT m_uButtonState;
        CDuiPoint ptLastMouse;
        CDuiRect m_rcNewPos;
    };

}