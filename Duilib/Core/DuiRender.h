#pragma once

#ifdef USE_XIMAGE_EFFECT
    class CxImage;
#endif
namespace DuiLib
{
    /////////////////////////////////////////////////////////////////////////////////////
    //
#define MAX_FONT_ID		30000

    class DUILIB_API CRenderClip
    {
    public:
        CRenderClip();
        ~CRenderClip();

    public:
        static void GenerateClip(HDC hDC, RECT rc, CRenderClip& clip);
        static void GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRenderClip& clip);
        static void UseOldClipBegin(HDC hDC, CRenderClip& clip);
        static void UseOldClipEnd(HDC hDC, CRenderClip& clip);
    private:
        RECT rcItem;
        HDC hDC;
        HRGN hRgn;
        HRGN hOldRgn;
    };

    /////////////////////////////////////////////////////////////////////////////////////
    //

    class DUILIB_API CRenderEngine
    {
    public:
        static DWORD AdjustColor(DWORD dwColor, short H, short S, short L);
        static HBITMAP CreateARGB32Bitmap(HDC hDC, int cx, int cy, BYTE** pBits);
        static void AdjustImage(BOOL bUseHSL, TImageInfo* imageInfo, short H, short S, short L);
        static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL);
#ifdef USE_XIMAGE_EFFECT
        static CxImage* LoadGifImageX(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0);
#endif
        static void FreeImage(TImageInfo* bitmap, BOOL bDelete = TRUE);
        static TImageInfo* LoadImage(LPCTSTR pStrImage, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL);
        static TImageInfo* LoadImage(UINT nID, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL);

        static Gdiplus::Image*	GdiplusLoadImage(LPCTSTR pstrPath);
        static Gdiplus::Image* GdiplusLoadImage(LPVOID pBuf, size_t dwSize);

        static BOOL DrawIconImageString(HDC hDC, CDuiPaintManager* pManager, const RECT& rcItem, const RECT& rcPaint, \
                                        LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
        static BOOL MakeFitIconDest(const RECT& rcControl, const CDuiSize& szIcon, const CDuiString& sAlign, RECT& rcDest);

        static void DrawText(HDC hDC, CDuiPaintManager* pManager, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, \
                             int iFont, UINT uStyle, DWORD dwTextBKColor);

        static void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
                              const RECT& rcBmpPart, const RECT& rcCorners, BOOL bAlpha, BYTE uFade = 255,
                              BOOL hole = FALSE, BOOL xtiled = FALSE, BOOL ytiled = FALSE);

        static BOOL DrawImageInfo(HDC hDC, CDuiPaintManager* pManager, const RECT& rcItem, const RECT& rcPaint, const TDrawInfo* pDrawInfo, HINSTANCE instance = NULL);
        static BOOL DrawImageString(HDC hDC, CDuiPaintManager* pManager, const RECT& rcItem, const RECT& rcPaint, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL, HINSTANCE instance = NULL);

        static void DrawColor(HDC hDC, const RECT& rc, DWORD color);
        static void DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, BOOL bVertical, int nSteps);

        // 以下函数中的颜色参数alpha值无效
        static void DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID);
        static void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID);
        static void DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID);
        static void DrawText(HDC hDC, CDuiPaintManager* pManager, RECT& rc, LPCTSTR pstrText, \
                             DWORD dwTextColor, int iFont, UINT uStyle);
        static void DrawHtmlText(HDC hDC, CDuiPaintManager* pManager, RECT& rc, LPCTSTR pstrText,
                                 DWORD dwTextColor, RECT* pLinks, CDuiString* sLinks, int& nLinkRects, UINT uStyle);
        static HBITMAP GenerateBitmap(CDuiPaintManager* pManager, CDuiControl* pControl, RECT rc);
        static SIZE GetTextSize(HDC hDC, CDuiPaintManager* pManager , LPCTSTR pstrText, int iFont, UINT uStyle);

        //alpha utilities
        static void CheckAlphaColor(DWORD& dwColor);
    };

} // namespace DuiLib
