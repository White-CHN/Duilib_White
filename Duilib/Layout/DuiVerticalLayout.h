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
        virtual CDuiString GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);
        virtual UINT GetControlFlags() const;

        void SetSepHeight(int iHeight);
        void SetSepImmMode(BOOL bImmediately);
        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);

        RECT GetThumbRect(BOOL bUseNew = FALSE) const;
        virtual void DoEvent(TEventUI& event);

        virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);
    private:
        BOOL m_bImmMode;
        INT m_iSepHeight;
        UINT m_uButtonState;
        CDuiPoint ptLastMouse;
        CDuiRect m_rcNewPos;
    };


}
