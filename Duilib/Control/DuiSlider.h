#pragma once

namespace DuiLib
{
    class DUILIB_API CDuiSlider :
        public CDuiProgress
    {
        DECLARE_DUICONTROL(CDuiSlider);
    public:
        CDuiSlider(void);
        virtual ~CDuiSlider(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString strName) OVERRIDE;

        LPCTSTR GetThumbImage() const;
        void SetThumbImage(LPCTSTR pStrImage);
        LPCTSTR GetThumbHotImage() const;
        void SetThumbHotImage(LPCTSTR pStrImage);
        LPCTSTR GetThumbPushedImage() const;
        void SetThumbPushedImage(LPCTSTR pStrImage);

        void SetThumbSize(SIZE szXY);

        int GetChangeStep();
        void SetChangeStep(int step);

        void SetCanSendMove(BOOL bCanSend);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        RECT GetThumbRect() const;
        void PaintForeImage(HDC hDC) OVERRIDE;

        void DoEvent(CDuiEvent& event) OVERRIDE;
    private:
        BOOL m_bSendMove;
        int m_nStep;
        UINT m_uButtonState;
        CDuiSize m_szThumb;
        CDuiString m_sThumbImage;
        CDuiString m_sThumbHotImage;
        CDuiString m_sThumbPushedImage;

        CDuiString m_sImageModify;
    };

}