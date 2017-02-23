#pragma once

namespace DuiLib
{
    class DUILIB_API CDuiScrollBar
        : public CDuiControl
    {
        DECLARE_DUICONTROL(CDuiScrollBar);
    public:
        CDuiScrollBar(void);
        virtual ~CDuiScrollBar(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString strName) OVERRIDE;

        void SetOwner(CDuiContainer* pOwner);

        void SetHorizontal(BOOL bHorizontal = TRUE);

        int GetScrollPos() const;
        void SetScrollPos(int nPos);

        void SetScrollRange(int nRange);
        int GetScrollRange() const;

        int GetLineSize() const;
        void SetLineSize(int nSize);

        void SetShowButton1(BOOL bShow);
        void SetButton1NormalImage(LPCTSTR pStrImage);
        void SetButton1HotImage(LPCTSTR pStrImage);
        void SetButton1PushedImage(LPCTSTR pStrImage);
        void SetButton1DisabledImage(LPCTSTR pStrImage);

        void SetShowButton2(BOOL bShow);
        void SetButton2NormalImage(LPCTSTR pStrImage);
        void SetButton2HotImage(LPCTSTR pStrImage);
        void SetButton2PushedImage(LPCTSTR pStrImage);
        void SetButton2DisabledImage(LPCTSTR pStrImage);

        void SetThumbNormalImage(LPCTSTR pStrImage);
        void SetThumbHotImage(LPCTSTR pStrImage);
        void SetThumbPushedImage(LPCTSTR pStrImage);
        void SetThumbDisabledImage(LPCTSTR pStrImage);

        void SetRailNormalImage(LPCTSTR pStrImage);
        void SetRailHotImage(LPCTSTR pStrImage);
        void SetRailPushedImage(LPCTSTR pStrImage);
        void SetRailDisabledImage(LPCTSTR pStrImage);

        LPCTSTR GetBkNormalImage();
        void SetBkNormalImage(LPCTSTR pStrImage);
        LPCTSTR GetBkHotImage();
        void SetBkHotImage(LPCTSTR pStrImage);
        LPCTSTR GetBkPushedImage();
        void SetBkPushedImage(LPCTSTR pStrImage);
        LPCTSTR GetBkDisabledImage();
        void SetBkDisabledImage(LPCTSTR pStrImage);

        void PaintBk(HDC hDC);
        void PaintButton1(HDC hDC);
        void PaintButton2(HDC hDC);
        void PaintThumb(HDC hDC);
        void PaintRail(HDC hDC);

        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        void DoEvent(CDuiEvent& event) OVERRIDE;
    private:
        enum
        {
            DEFAULT_SCROLLBAR_SIZE = 16,
        };
    private:
        BOOL m_bHorizontal;
        BOOL m_bShowButton1;
        BOOL m_bShowButton2;

        int m_nScrollPos;
        int m_nRange;
        int m_nLineSize;

        int m_nLastScrollOffset;
        int m_nScrollRepeatDelay;
        int m_nLastScrollPos;

        UINT m_uThumbState;
        UINT m_uButton1State;
        UINT m_uButton2State;

        CDuiContainer* m_pOwner;

        CDuiPoint m_ptLastMouse;

        CDuiRect m_rcThumb;
        CDuiRect m_rcButton1;
        CDuiRect m_rcButton2;

        CDuiString m_sBkDisabledImage;
        CDuiString m_sBkPushedImage;
        CDuiString m_sBkHotImage;
        CDuiString m_sBkNormalImage;

        CDuiString m_sButton1NormalImage;
        CDuiString m_sButton1HotImage;
        CDuiString m_sButton1PushedImage;
        CDuiString m_sButton1DisabledImage;

        CDuiString m_sButton2NormalImage;
        CDuiString m_sButton2HotImage;
        CDuiString m_sButton2PushedImage;
        CDuiString m_sButton2DisabledImage;

        CDuiString m_sThumbNormalImage;
        CDuiString m_sThumbHotImage;
        CDuiString m_sThumbPushedImage;
        CDuiString m_sThumbDisabledImage;

        CDuiString m_sRailNormalImage;
        CDuiString m_sRailHotImage;
        CDuiString m_sRailPushedImage;
        CDuiString m_sRailDisabledImage;

        CDuiString m_sImageModify;
    };

}