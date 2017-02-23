#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiFadeButton
        : public CDuiButton
        , public IAnimation
    {
        DECLARE_DUICONTROL(CDuiFadeButton)
    public:
        CDuiFadeButton(void);
        virtual ~CDuiFadeButton(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        void SetNormalImage(LPCTSTR pStrImage) OVERRIDE;

        void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) OVERRIDE;

        void OnTimer(int nTimerID);
        void DoEvent(CDuiEvent& event) OVERRIDE;

        void PaintStatusImage(HDC hDC) OVERRIDE;
    private:
        enum
        {
            FADE_ELLAPSE		= 10,
            FADE_FRAME_COUNT	= 30,
        };
    private:
        BYTE       m_bFadeAlpha;
        BOOL       m_bMouseHove;
        BOOL       m_bMouseLeave;

        CDuiString m_sOldImage;
        CDuiString m_sNewImage;
        CDuiString m_sLastImage;
    };

}