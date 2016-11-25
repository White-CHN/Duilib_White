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
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);

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

        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        RECT GetThumbRect() const;
        virtual void PaintForeImage(HDC hDC);

        virtual void DoEvent(TEventUI& event);
    private:
        BOOL m_bSendMove;
        int m_nStep;
        UINT m_uButtonState;
        SIZE m_szThumb;
        CDuiString m_sThumbImage;
        CDuiString m_sThumbHotImage;
        CDuiString m_sThumbPushedImage;

        CDuiString m_sImageModify;
    };

}