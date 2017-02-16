#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiWater
        : public CDuiLabel
    {
        DECLARE_DUICONTROL(CDuiWater)
    public:
        CDuiWater(void);
        virtual ~CDuiWater(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;
        void DoInit() OVERRIDE;
        void SetVisible(BOOL bVisible = TRUE) OVERRIDE;
        void SetInternVisible(BOOL bVisible = TRUE) OVERRIDE;
        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;
        void DoEvent(TEventUI& event) OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
    private:
        BOOL m_bUpdate;
        CDib m_renderSrc;
        CDib m_renderDest;
        CWaterEffect m_waterEffect;
    };

}