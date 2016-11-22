#include "StdAfx.h"
#include "DuiPaintManager.h"

namespace DuiLib
{

    tagTDrawInfo::tagTDrawInfo()
    {
        Clear();
    }

    void tagTDrawInfo::Parse(LPCTSTR pStrImage, LPCTSTR pStrModify, CDuiPaintManager* paintManager)
    {
        // 1、aaa.jpg
        // 2、file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0'
        // mask='#FF0000' fade='255' hole='FALSE' xtiled='FALSE' ytiled='FALSE'
        sDrawString = pStrImage;
        sDrawModify = pStrModify;
        sImageName = pStrImage;
        CDuiString sItem;
        CDuiString sValue;
        LPTSTR pstr = NULL;
        for(int i = 0; i < 2; ++i)
        {
            if(i == 1)
            {
                pStrImage = pStrModify;
            }
            if(!pStrImage)
            {
                continue;
            }
            while(*pStrImage != _T('\0'))
            {
                sItem.SetEmpty();
                sValue.SetEmpty();
                while(*pStrImage > _T('\0') && *pStrImage <= _T(' '))
                {
                    pStrImage = ::CharNext(pStrImage);
                }
                while(*pStrImage != _T('\0') && *pStrImage != _T('=') && *pStrImage > _T(' '))
                {
                    LPTSTR pstrTemp = ::CharNext(pStrImage);
                    while(pStrImage < pstrTemp)
                    {
                        sItem += *pStrImage++;
                    }
                }
                while(*pStrImage > _T('\0') && *pStrImage <= _T(' '))
                {
                    pStrImage = ::CharNext(pStrImage);
                }
                if(*pStrImage++ != _T('='))
                {
                    break;
                }
                while(*pStrImage > _T('\0') && *pStrImage <= _T(' '))
                {
                    pStrImage = ::CharNext(pStrImage);
                }
                if(*pStrImage++ != _T('\''))
                {
                    break;
                }
                while(*pStrImage != _T('\0') && *pStrImage != _T('\''))
                {
                    LPTSTR pstrTemp = ::CharNext(pStrImage);
                    while(pStrImage < pstrTemp)
                    {
                        sValue += *pStrImage++;
                    }
                }
                if(*pStrImage++ != _T('\''))
                {
                    break;
                }
                if(!sValue.IsEmpty())
                {
                    if(sItem == _T("file") || sItem == _T("res"))
                    {
                        sImageName = sValue;
                    }
                    else if(sItem == _T("restype"))
                    {
                        sResType = sValue;
                    }
                    else if(sItem == _T("dest"))
                    {
                        rcDest.left = _tcstol(sValue.GetData(), &pstr, 10);
                        ASSERT(pstr);
                        rcDest.top = _tcstol(pstr + 1, &pstr, 10);
                        ASSERT(pstr);
                        rcDest.right = _tcstol(pstr + 1, &pstr, 10);
                        ASSERT(pstr);
                        rcDest.bottom = _tcstol(pstr + 1, &pstr, 10);
                        ASSERT(pstr);
                        paintManager->GetDPIObj()->Scale(&rcDest);
                    }
                    else if(sItem == _T("source"))
                    {
                        rcSource.left = _tcstol(sValue.GetData(), &pstr, 10);
                        ASSERT(pstr);
                        rcSource.top = _tcstol(pstr + 1, &pstr, 10);
                        ASSERT(pstr);
                        rcSource.right = _tcstol(pstr + 1, &pstr, 10);
                        ASSERT(pstr);
                        rcSource.bottom = _tcstol(pstr + 1, &pstr, 10);
                        ASSERT(pstr);
                        paintManager->GetDPIObj()->Scale(&rcSource);
                    }
                    else if(sItem == _T("corner"))
                    {
                        rcCorner.left = _tcstol(sValue.GetData(), &pstr, 10);
                        ASSERT(pstr);
                        rcCorner.top = _tcstol(pstr + 1, &pstr, 10);
                        ASSERT(pstr);
                        rcCorner.right = _tcstol(pstr + 1, &pstr, 10);
                        ASSERT(pstr);
                        rcCorner.bottom = _tcstol(pstr + 1, &pstr, 10);
                        ASSERT(pstr);
                        paintManager->GetDPIObj()->Scale(&rcCorner);
                    }
                    else if(sItem == _T("mask"))
                    {
                        if(sValue[0] == _T('#'))
                        {
                            dwMask = _tcstoul(sValue.GetData() + 1, &pstr, 16);
                        }
                        else
                        {
                            dwMask = _tcstoul(sValue.GetData(), &pstr, 16);
                        }
                    }
                    else if(sItem == _T("fade"))
                    {
                        uFade = (BYTE)_tcstoul(sValue.GetData(), &pstr, 10);
                    }
                    else if(sItem == _T("hole"))
                    {
                        bHole = (_tcsicmp(sValue.GetData(), _T("TRUE")) == 0);
                    }
                    else if(sItem == _T("xtiled"))
                    {
                        bTiledX = (_tcsicmp(sValue.GetData(), _T("TRUE")) == 0);
                    }
                    else if(sItem == _T("ytiled"))
                    {
                        bTiledY = (_tcsicmp(sValue.GetData(), _T("TRUE")) == 0);
                    }
                    else if(sItem == _T("hsl"))
                    {
                        bHSL = (_tcsicmp(sValue.GetData(), _T("TRUE")) == 0);
                    }
                }
                if(*pStrImage++ != _T(' '))
                {
                    break;
                }
            }
        }
        // 调整DPI资源
        if(paintManager->GetDPIObj()->GetScale() != 100)
        {
            CDuiString sScale;
            sScale.Format(_T("@%d."), paintManager->GetDPIObj()->GetScale());
            sImageName.Replace(_T("."), sScale);
        }
    }
    void tagTDrawInfo::Clear()
    {
        sDrawString.SetEmpty();
        sDrawModify.SetEmpty();
        sImageName.SetEmpty();
        memset(&rcDest, 0, sizeof(RECT));
        memset(&rcSource, 0, sizeof(RECT));
        memset(&rcCorner, 0, sizeof(RECT));
        dwMask = 0;
        uFade = 255;
        bHole = FALSE;
        bTiledX = FALSE;
        bTiledY = FALSE;
        bHSL = FALSE;
    }

    typedef BOOL (__stdcall* PFUNCUPDATELAYEREDWINDOW)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);
    PFUNCUPDATELAYEREDWINDOW g_fUpdateLayeredWindow = NULL;

    HINSTANCE CDuiPaintManager::m_hInstance = NULL;
    HINSTANCE CDuiPaintManager::m_hResourceInstance = NULL;
    HMODULE CDuiPaintManager::m_hMsimg32Module = NULL;

    int CDuiPaintManager::m_iResourceType = DUILIB_FILE;
    BOOL CDuiPaintManager::m_bCachedResourceZip = TRUE;
    HANDLE CDuiPaintManager::m_hResourceZip = NULL;
    CDuiString CDuiPaintManager::m_strResourcePath = _T("");
    CDuiString CDuiPaintManager::m_strResourceZip = _T("");
    CStdPtrArray CDuiPaintManager::m_aPreMessages;
    CStdPtrArray CDuiPaintManager::m_aPlugins;

    short CDuiPaintManager::m_H = 180;
    short CDuiPaintManager::m_S = 100;
    short CDuiPaintManager::m_L = 100;
    BOOL CDuiPaintManager::m_bUseHSL = FALSE;

    TResInfo CDuiPaintManager::m_SharedResInfo = {0};

    CDuiPaintManager::CDuiPaintManager(void)
        : m_hWndPaint(NULL)
        , m_hwndTooltip(NULL)
        , m_hDcPaint(NULL)
        , m_hDcOffscreen(NULL)
        , m_nOpacity(0xFF)
        , m_bUpdateNeeded(TRUE)
        , m_bLayered(FALSE)
        , m_bForceUseSharedRes(FALSE)
        , m_bUsedVirtualWnd(FALSE)
        , m_bIsPainting(FALSE)
        , m_bFocusNeeded(TRUE)
        , m_bOffscreenPaint(TRUE)
        , m_bShowUpdateRect(FALSE)
        , m_bFirstLayout(TRUE)
        , m_bLayeredChanged(FALSE)
        , m_bMouseCapture(FALSE)
        , m_bDragMode(FALSE)
        , m_bUseGdiplusText(FALSE)
        , m_bCaretActive(FALSE)
        , m_bMouseTracking(FALSE)
        , m_trh(0)
        , m_uTimerID(0x1000)
        , m_gdiplusToken(0)
        , m_pDPI(NULL)
        , m_pOffscreenBits(NULL)
        , m_pRoot(NULL)
        , m_pFocus(NULL)
        , m_pEventClick(NULL)
        , m_pEventHover(NULL)
        , m_pEventKey(NULL)
        , m_pGdiplusStartupInput(NULL)
    {
        ZeroMemory(&m_hbmpOffscreen, sizeof(m_hbmpOffscreen));
        ZeroMemory(&m_ptLastMousePos, sizeof(m_ptLastMousePos));
        ZeroMemory(&m_szInitWindowSize, sizeof(m_szInitWindowSize));
        ZeroMemory(&m_rcSizeBox, sizeof(m_rcSizeBox));
        ZeroMemory(&m_rcCaption, sizeof(m_rcCaption));
        ZeroMemory(&m_szRoundCorner, sizeof(m_szRoundCorner));
        ZeroMemory(&m_szMinWindow, sizeof(m_szMinWindow));
        ZeroMemory(&m_szMaxWindow, sizeof(m_szMaxWindow));
        ZeroMemory(&m_rcLayeredUpdate, sizeof(m_rcLayeredUpdate));
        ZeroMemory(&m_rtCaret, sizeof(m_rtCaret));
        ZeroMemory(&m_ToolTip, sizeof(m_ToolTip));

        m_ResInfo.m_dwDefaultDisabledColor		= m_SharedResInfo.m_dwDefaultDisabledColor;
        m_ResInfo.m_dwDefaultFontColor			= m_SharedResInfo.m_dwDefaultFontColor;
        m_ResInfo.m_dwDefaultLinkFontColor		= m_SharedResInfo.m_dwDefaultLinkFontColor;
        m_ResInfo.m_dwDefaultLinkHoverFontColor = m_SharedResInfo.m_dwDefaultLinkHoverFontColor;
        m_ResInfo.m_dwDefaultSelectedBkColor	= m_SharedResInfo.m_dwDefaultSelectedBkColor;
        m_pGdiplusStartupInput = new Gdiplus::GdiplusStartupInput;
        Gdiplus::GdiplusStartup(&m_gdiplusToken, m_pGdiplusStartupInput, NULL);  // 加载GDI接口
        m_pDPI = new CDPI;
    }


    CDuiPaintManager::~CDuiPaintManager(void)
    {
        m_aPreMessages.Remove(m_aPreMessages.Find(this));
        for(int i = 0; i < m_aDelayedCleanup.GetSize(); i++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(m_aDelayedCleanup[i]);
            DUI_FREE_POINT(pControl);
        }
        for(int i = 0; i < m_aAsyncNotify.GetSize(); i++)
        {
            TNotifyUI* pNotify = static_cast<TNotifyUI*>(m_aAsyncNotify[i]);
            DUI_FREE_POINT(pNotify);
        }
        RemoveAllTimers();
        RemoveAllFonts();
        RemoveAllImages();
        RemoveAllDrawInfos();
        RemoveAllDefaultAttributeList();
        RemoveAllStyle();
        if(m_hwndTooltip != NULL)
        {
            ::DestroyWindow(m_hwndTooltip);
            m_hwndTooltip = NULL;
        }
        if(m_hDcOffscreen != NULL)
        {
            ::DeleteDC(m_hDcOffscreen);
            m_hDcOffscreen = NULL;
        }
        if(m_hbmpOffscreen != NULL)
        {
            ::DeleteObject(m_hbmpOffscreen);
            m_hbmpOffscreen = NULL;
        }
        if(m_ResInfo.m_DefaultFontInfo.hFont != NULL)
        {
            ::DeleteObject(m_ResInfo.m_DefaultFontInfo.hFont);
            m_ResInfo.m_DefaultFontInfo.hFont = NULL;
        }
        if(m_hDcPaint != NULL)
        {
            ::ReleaseDC(m_hWndPaint, m_hDcPaint);
            m_hDcPaint = NULL;
        }
        if(m_gdiplusToken != 0)
        {
            //卸载GDIPlus
            Gdiplus::GdiplusShutdown(m_gdiplusToken);
            m_gdiplusToken = 0;
        }
        DUI_FREE_POINT(m_pDPI);
        DUI_FREE_POINT(m_pGdiplusStartupInput);
        DUI_FREE_POINT(m_pRoot);
    }

    void CDuiPaintManager::Init(HWND hWnd)
    {
        ASSERT(::IsWindow(hWnd));
        if(m_hWndPaint != hWnd)
        {
            m_hWndPaint = hWnd;
            m_hDcPaint = ::GetDC(hWnd);
            m_aPreMessages.Add(this);
        }
    }

    void CDuiPaintManager::NeedUpdate()
    {
        m_bUpdateNeeded = TRUE;
    }

    BOOL CDuiPaintManager::Invalidate()
    {
        RECT rcClient = { 0 };
        ::GetClientRect(m_hWndPaint, &rcClient);
        ::UnionRect(&m_rcLayeredUpdate, &m_rcLayeredUpdate, &rcClient);
        return ::InvalidateRect(m_hWndPaint, NULL, FALSE);
    }

    BOOL CDuiPaintManager::Invalidate(RECT& rcItem)
    {
        if(rcItem.left < 0)
        {
            rcItem.left = 0;
        }
        if(rcItem .top < 0)
        {
            rcItem.top = 0;
        }
        if(rcItem.right < rcItem.left)
        {
            rcItem.right = rcItem.left;
        }
        if(rcItem.bottom < rcItem.top)
        {
            rcItem.bottom = rcItem.top;
        }
        ::UnionRect(&m_rcLayeredUpdate, &m_rcLayeredUpdate, &rcItem);
        return ::InvalidateRect(m_hWndPaint, &rcItem, FALSE);
    }

    HDC CDuiPaintManager::GetPaintDC() const
    {
        return m_hDcPaint;
    }

    POINT CDuiPaintManager::GetMousePos() const
    {
        return m_ptLastMousePos;
    }

    void CDuiPaintManager::SetPainting(BOOL bIsPainting)
    {
        m_bIsPainting = bIsPainting;
    }

    BOOL CDuiPaintManager::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes)
    {
        BOOL bHandled = FALSE;
        LRESULT lResult = 0;
        if(m_hWndPaint == NULL)
        {
            DUI_ERROR(_T("this[%x] m_hWndPaint[%x]"), this, m_hWndPaint);
            return FALSE;
        }
        // Cycle through listeners
        for(int i = 0; i < m_aMessageFilters.GetSize(); i++)
        {
            bHandled = FALSE;
            lResult = static_cast<IMessageFilterUI*>(m_aMessageFilters[i])->MessageHandler(uMsg, wParam, lParam, bHandled);
            if(bHandled)
            {
                lRes = lResult;
                return TRUE;
            }
        }
        // Custom handling of events
        bHandled = TRUE;
        lResult = 0;
        switch(uMsg)
        {
            case WM_APP + 1:
                lResult = OnDuiNotify(uMsg, wParam, lParam, bHandled);
                break;
            case WM_PAINT:
                lResult = OnPaint(uMsg, wParam, lParam, bHandled);
                break;
            case WM_SIZE:
                lResult = OnSize(uMsg, wParam, lParam, bHandled);
                break;
            case WM_LBUTTONDOWN:
                lResult = OnLButtonDown(uMsg, wParam, lParam, bHandled);
                break;
            case WM_LBUTTONUP:
                lResult = OnLButtonUp(uMsg, wParam, lParam, bHandled);
                break;
            case WM_TIMER:
                lResult = OnTimer(uMsg, wParam, lParam, bHandled);
                break;
            case WM_MOUSEHOVER:
                lResult = OnMouseHover(uMsg, wParam, lParam, bHandled);
                break;
            case WM_MOUSELEAVE:
                lResult = OnMouseLeave(uMsg, wParam, lParam, bHandled);
                break;
            case WM_MOUSEMOVE:
                lResult = OnMouseMove(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KEYDOWN:
                lResult = OnKeyDown(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KEYUP:
                lResult = OnKeyUp(uMsg, wParam, lParam, bHandled);
                break;
            case WM_MOUSEWHEEL:
                lResult = OnMouseWheel(uMsg, wParam, lParam, bHandled);
                break;
            case WM_SETCURSOR:
                lResult = OnSetCursor(uMsg, wParam, lParam, bHandled);
                break;
            default:
                bHandled = FALSE;
                break;
        }
        if(bHandled)
        {
            lRes = lResult;
            return TRUE;
        }
        return FALSE;
    }

    BOOL CDuiPaintManager::AddPreMessageFilter(IMessageFilterUI* pFilter)
    {
        ASSERT(m_aPreMessageFilters.Find(pFilter) < 0);
        return m_aPreMessageFilters.Add(pFilter);
    }

    HWND CDuiPaintManager::GetPaintWindow() const
    {
        return m_hWndPaint;
    }

    void CDuiPaintManager::RemoveImage(LPCTSTR bitmap, BOOL bShared /*= FALSE*/)
    {
        TImageInfo* data = NULL;
        if(bShared)
        {
            data = static_cast<TImageInfo*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
            if(data)
            {
                CRenderEngine::FreeImage(data) ;
                m_SharedResInfo.m_ImageHash.Remove(bitmap);
            }
        }
        else
        {
            data = static_cast<TImageInfo*>(m_ResInfo.m_ImageHash.Find(bitmap));
            if(data)
            {
                CRenderEngine::FreeImage(data) ;
                m_ResInfo.m_ImageHash.Remove(bitmap);
            }
        }
    }

    void CDuiPaintManager::SetInitSize(int cx, int cy)
    {
        m_szInitWindowSize.cx = cx;
        m_szInitWindowSize.cy = cy;
        if(m_pRoot == NULL && m_hWndPaint != NULL)
        {
            ::SetWindowPos(m_hWndPaint, NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
        }
    }

    RECT& CDuiPaintManager::GetSizeBox()
    {
        return m_rcSizeBox;
    }

    void CDuiPaintManager::SetSizeBox(RECT& rcSizeBox)
    {
        m_rcSizeBox = rcSizeBox;
    }

    RECT& CDuiPaintManager::GetCaptionRect()
    {
        return m_rcCaption;
    }

    void CDuiPaintManager::SetCaptionRect(RECT& rcCaption)
    {
        m_rcCaption = rcCaption;
    }

    SIZE CDuiPaintManager::GetRoundCorner() const
    {
        return m_szRoundCorner;
    }

    void CDuiPaintManager::SetRoundCorner(int cx, int cy)
    {
        m_szRoundCorner.cx = cx;
        m_szRoundCorner.cy = cy;
    }

    SIZE CDuiPaintManager::GetMinInfo() const
    {
        return m_szMinWindow;
    }

    void CDuiPaintManager::SetMinInfo(int cx, int cy)
    {
        ASSERT(cx >= 0 && cy >= 0);
        m_szMinWindow.cx = cx;
        m_szMinWindow.cy = cy;
    }

    SIZE CDuiPaintManager::GetMaxInfo() const
    {
        return m_szMaxWindow;
    }

    void CDuiPaintManager::SetMaxInfo(int cx, int cy)
    {
        ASSERT(cx >= 0 && cy >= 0);
        m_szMaxWindow.cx = cx;
        m_szMaxWindow.cy = cy;
    }

    void CDuiPaintManager::SetShowUpdateRect(BOOL bShow)
    {
        m_bShowUpdateRect = bShow;
    }

    void CDuiPaintManager::SetOpacity(BYTE nOpacity)
    {
        m_nOpacity = nOpacity;
        if(m_hWndPaint != NULL)
        {
            typedef BOOL (__stdcall * PFUNCSETLAYEREDWINDOWATTR)(HWND, COLORREF, BYTE, DWORD);
            PFUNCSETLAYEREDWINDOWATTR fSetLayeredWindowAttributes;
            HMODULE hUser32 = ::GetModuleHandle(_T("User32.dll"));
            if(hUser32)
            {
                fSetLayeredWindowAttributes = (PFUNCSETLAYEREDWINDOWATTR)::GetProcAddress(hUser32, "SetLayeredWindowAttributes");
                if(fSetLayeredWindowAttributes == NULL)
                {
                    return;
                }
            }
            DWORD dwStyle = ::GetWindowLong(m_hWndPaint, GWL_EXSTYLE);
            DWORD dwNewStyle = dwStyle;
            if(nOpacity >= 0 && nOpacity < 256)
            {
                dwNewStyle |= WS_EX_LAYERED;
            }
            else
            {
                dwNewStyle &= ~WS_EX_LAYERED;
            }
            if(dwStyle != dwNewStyle)
            {
                ::SetWindowLong(m_hWndPaint, GWL_EXSTYLE, dwNewStyle);
            }
            fSetLayeredWindowAttributes(m_hWndPaint, 0, nOpacity, LWA_ALPHA);
        }
    }

    void CDuiPaintManager::SetLayered(BOOL bLayered)
    {
        if(m_hWndPaint != NULL && bLayered != m_bLayered)
        {
            UINT uStyle = GetWindowStyle(m_hWndPaint);
            if((uStyle & WS_CHILD) != 0)
            {
                return;
            }
            if(g_fUpdateLayeredWindow == NULL)
            {
                HMODULE hUser32 = ::GetModuleHandle(_T("User32.dll"));
                if(hUser32)
                {
                    g_fUpdateLayeredWindow = (PFUNCUPDATELAYEREDWINDOW)::GetProcAddress(hUser32, "UpdateLayeredWindow");
                    if(g_fUpdateLayeredWindow == NULL)
                    {
                        return;
                    }
                }
            }
            m_bLayered = bLayered;
            if(m_pRoot != NULL)
            {
                m_pRoot->NeedUpdate();
            }
            Invalidate();
        }
    }

    void CDuiPaintManager::SetLayeredOpacity(BYTE nOpacity)
    {
        m_nOpacity = nOpacity;
        m_bLayeredChanged = TRUE;
        Invalidate();
    }


    DWORD CDuiPaintManager::GetDefaultDisabledColor() const
    {
        return m_ResInfo.m_dwDefaultDisabledColor;
    }

    void CDuiPaintManager::SetDefaultDisabledColor(DWORD dwColor, BOOL bShared /*= FALSE*/)
    {
        if(bShared)
        {
            if(m_ResInfo.m_dwDefaultDisabledColor == m_SharedResInfo.m_dwDefaultDisabledColor)
            {
                m_ResInfo.m_dwDefaultDisabledColor = dwColor;
            }
            m_SharedResInfo.m_dwDefaultDisabledColor = dwColor;
        }
        else
        {
            m_ResInfo.m_dwDefaultDisabledColor = dwColor;
        }
    }

    DWORD CDuiPaintManager::GetDefaultFontColor() const
    {
        return m_ResInfo.m_dwDefaultFontColor;
    }

    void CDuiPaintManager::SetDefaultFontColor(DWORD dwColor, BOOL bShared /*= FALSE*/)
    {
        if(bShared)
        {
            if(m_ResInfo.m_dwDefaultFontColor == m_SharedResInfo.m_dwDefaultFontColor)
            {
                m_ResInfo.m_dwDefaultFontColor = dwColor;
            }
            m_SharedResInfo.m_dwDefaultFontColor = dwColor;
        }
        else
        {
            m_ResInfo.m_dwDefaultFontColor = dwColor;
        }
    }

    void CDuiPaintManager::SetDefaultLinkFontColor(DWORD dwColor, BOOL bShared /*= FALSE*/)
    {
        if(bShared)
        {
            if(m_ResInfo.m_dwDefaultLinkFontColor == m_SharedResInfo.m_dwDefaultLinkFontColor)
            {
                m_ResInfo.m_dwDefaultLinkFontColor = dwColor;
            }
            m_SharedResInfo.m_dwDefaultLinkFontColor = dwColor;
        }
        else
        {
            m_ResInfo.m_dwDefaultLinkFontColor = dwColor;
        }
    }

    void CDuiPaintManager::SetDefaultLinkHoverFontColor(DWORD dwColor, BOOL bShared /*= FALSE*/)
    {
        if(bShared)
        {
            if(m_ResInfo.m_dwDefaultLinkHoverFontColor == m_SharedResInfo.m_dwDefaultLinkHoverFontColor)
            {
                m_ResInfo.m_dwDefaultLinkHoverFontColor = dwColor;
            }
            m_SharedResInfo.m_dwDefaultLinkHoverFontColor = dwColor;
        }
        else
        {
            m_ResInfo.m_dwDefaultLinkHoverFontColor = dwColor;
        }
    }

    void CDuiPaintManager::SetDefaultSelectedBkColor(DWORD dwColor, BOOL bShared /*= FALSE*/)
    {
        if(bShared)
        {
            if(m_ResInfo.m_dwDefaultSelectedBkColor == m_SharedResInfo.m_dwDefaultSelectedBkColor)
            {
                m_ResInfo.m_dwDefaultSelectedBkColor = dwColor;
            }
            m_SharedResInfo.m_dwDefaultSelectedBkColor = dwColor;
        }
        else
        {
            m_ResInfo.m_dwDefaultSelectedBkColor = dwColor;
        }
    }

    void CDuiPaintManager::AddStyle(LPCTSTR pName, LPCTSTR pstrStyle, BOOL bShared /*= FALSE*/)
    {
        CDuiString* pStyle = new CDuiString(pstrStyle);
        if(bShared || m_bForceUseSharedRes)
        {
            if(!m_SharedResInfo.m_StyleHash.Insert(pName, pStyle))
            {
                DUI_FREE_POINT(pStyle);
            }
        }
        else
        {
            if(!m_ResInfo.m_StyleHash.Insert(pName, pStyle))
            {
                DUI_FREE_POINT(pStyle);
            }
        }
    }

    LPCTSTR CDuiPaintManager::GetStyle(LPCTSTR pName) const
    {
        CDuiString* pStyle = static_cast<CDuiString*>(m_ResInfo.m_StyleHash.Find(pName));
        if(!pStyle)
        {
            pStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Find(pName));
        }
        if(pStyle)
        {
            return pStyle->GetData();
        }
        return NULL;
    }

    void CDuiPaintManager::RemoveAllStyle()
    {
        CDuiString* pStyle = NULL;
        for(int i = 0; i < m_ResInfo.m_StyleHash.GetSize(); i++)
        {
            if(LPCTSTR key = m_ResInfo.m_StyleHash.GetAt(i))
            {
                pStyle = static_cast<CDuiString*>(m_ResInfo.m_StyleHash.Find(key));
                DUI_FREE_POINT(pStyle);
            }
        }
        m_ResInfo.m_StyleHash.RemoveAll();
    }

    CDuiControl* CDuiPaintManager::GetFocus() const
    {
        return m_pFocus;
    }

    void CDuiPaintManager::SetFocus(CDuiControl* pControl)
    {
        HWND hFocusWnd = ::GetFocus();
        if(hFocusWnd != m_hWndPaint && pControl != m_pFocus)
        {
            ::SetFocus(m_hWndPaint);
        }
        if(pControl == m_pFocus)
        {
            return;
        }
        if(m_pFocus != NULL)
        {
            TEventUI event = { 0 };
            event.Type = UIEVENT_KILLFOCUS;
            event.pSender = pControl;
            event.dwTimestamp = ::GetTickCount();
            m_pFocus->Event(event);
            SendNotify(m_pFocus, DUI_MSGTYPE_KILLFOCUS);
            m_pFocus = NULL;
        }
        if(pControl != NULL
                && pControl->GetManager() == this
                && pControl->IsVisible()
                && pControl->IsEnabled())
        {
            m_pFocus = pControl;
            TEventUI event = { 0 };
            event.Type = UIEVENT_SETFOCUS;
            event.pSender = pControl;
            event.dwTimestamp = ::GetTickCount();
            m_pFocus->Event(event);
            SendNotify(m_pFocus, DUI_MSGTYPE_SETFOCUS);
        }
    }

    void CDuiPaintManager::DrawCaret(HDC hDC, const RECT& rcPaint)
    {
    }

    TFontInfo* CDuiPaintManager::GetDefaultFontInfo()
    {
        if(m_ResInfo.m_DefaultFontInfo.sFontName.IsEmpty())
        {
            if(m_SharedResInfo.m_DefaultFontInfo.tm.tmHeight == 0)
            {
                HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, m_SharedResInfo.m_DefaultFontInfo.hFont);
                ::GetTextMetrics(m_hDcPaint, &m_SharedResInfo.m_DefaultFontInfo.tm);
                ::SelectObject(m_hDcPaint, hOldFont);
            }
            return &m_SharedResInfo.m_DefaultFontInfo;
        }
        else
        {
            if(m_ResInfo.m_DefaultFontInfo.tm.tmHeight == 0)
            {
                HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, m_ResInfo.m_DefaultFontInfo.hFont);
                ::GetTextMetrics(m_hDcPaint, &m_ResInfo.m_DefaultFontInfo.tm);
                ::SelectObject(m_hDcPaint, hOldFont);
            }
            return &m_ResInfo.m_DefaultFontInfo;
        }
    }

    void CDuiPaintManager::SetDefaultFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic, BOOL bShared /*= false*/)
    {
        LOGFONT lf = { 0 };
        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        if(lstrlen(pStrFontName) > 0)
        {
            TCHAR szFaceName[32] = {0};//_T("@");
            _tcsncat(szFaceName, pStrFontName, LF_FACESIZE);
            _tcsncpy(lf.lfFaceName, szFaceName, LF_FACESIZE);
        }
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfHeight = -GetDPIObj()->Scale(nSize);
        if(bBold)
        {
            lf.lfWeight += FW_BOLD;
        }
        if(bUnderline)
        {
            lf.lfUnderline = TRUE;
        }
        if(bItalic)
        {
            lf.lfItalic = TRUE;
        }
        HFONT hFont = ::CreateFontIndirect(&lf);
        if(hFont == NULL)
        {
            return;
        }
        if(bShared)
        {
            ::DeleteObject(m_SharedResInfo.m_DefaultFontInfo.hFont);
            m_SharedResInfo.m_DefaultFontInfo.hFont = hFont;
            m_SharedResInfo.m_DefaultFontInfo.sFontName = lf.lfFaceName;
            m_SharedResInfo.m_DefaultFontInfo.iSize = nSize;
            m_SharedResInfo.m_DefaultFontInfo.bBold = bBold;
            m_SharedResInfo.m_DefaultFontInfo.bUnderline = bUnderline;
            m_SharedResInfo.m_DefaultFontInfo.bItalic = bItalic;
            ::ZeroMemory(&m_SharedResInfo.m_DefaultFontInfo.tm, sizeof(m_SharedResInfo.m_DefaultFontInfo.tm));
            if(m_hDcPaint)
            {
                HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, hFont);
                ::GetTextMetrics(m_hDcPaint, &m_SharedResInfo.m_DefaultFontInfo.tm);
                ::SelectObject(m_hDcPaint, hOldFont);
            }
        }
        else
        {
            ::DeleteObject(m_ResInfo.m_DefaultFontInfo.hFont);
            m_ResInfo.m_DefaultFontInfo.hFont = hFont;
            m_ResInfo.m_DefaultFontInfo.sFontName = lf.lfFaceName;
            m_ResInfo.m_DefaultFontInfo.iSize = nSize;
            m_ResInfo.m_DefaultFontInfo.bBold = bBold;
            m_ResInfo.m_DefaultFontInfo.bUnderline = bUnderline;
            m_ResInfo.m_DefaultFontInfo.bItalic = bItalic;
            ::ZeroMemory(&m_ResInfo.m_DefaultFontInfo.tm, sizeof(m_ResInfo.m_DefaultFontInfo.tm));
            if(m_hDcPaint)
            {
                HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, hFont);
                ::GetTextMetrics(m_hDcPaint, &m_ResInfo.m_DefaultFontInfo.tm);
                ::SelectObject(m_hDcPaint, hOldFont);
            }
        }
    }

    HFONT CDuiPaintManager::AddFont(int id, LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic, BOOL bShared /*= FALSE*/)
    {
        LOGFONT lf = { 0 };
        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        if(lstrlen(pStrFontName) > 0)
        {
            TCHAR szFaceName[32] = {0};//_T("@");
            _tcsncat(szFaceName, pStrFontName, LF_FACESIZE);
            _tcsncpy(lf.lfFaceName, szFaceName, LF_FACESIZE);
        }
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfHeight = -GetDPIObj()->Scale(nSize);
        if(bBold)
        {
            lf.lfWeight = FW_BOLD;
        }
        if(bUnderline)
        {
            lf.lfUnderline = TRUE;
        }
        if(bItalic)
        {
            lf.lfItalic = TRUE;
        }
        HFONT hFont = ::CreateFontIndirect(&lf);
        if(hFont == NULL)
        {
            return NULL;
        }
        TFontInfo* pFontInfo = new TFontInfo;
        if(!pFontInfo)
        {
            return FALSE;
        }
        ::ZeroMemory(pFontInfo, sizeof(TFontInfo));
        pFontInfo->hFont = hFont;
        pFontInfo->sFontName = lf.lfFaceName;
        pFontInfo->iSize = nSize;
        pFontInfo->bBold = bBold;
        pFontInfo->bUnderline = bUnderline;
        pFontInfo->bItalic = bItalic;
        if(m_hDcPaint)
        {
            HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, hFont);
            ::GetTextMetrics(m_hDcPaint, &pFontInfo->tm);
            ::SelectObject(m_hDcPaint, hOldFont);
        }
        TCHAR idBuffer[16];
        ::ZeroMemory(idBuffer, sizeof(idBuffer));
        _itot(id, idBuffer, 10);
        if(bShared || m_bForceUseSharedRes)
        {
            TFontInfo* pOldFontInfo = static_cast<TFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
            if(pOldFontInfo)
            {
                ::DeleteObject(pOldFontInfo->hFont);
                delete pOldFontInfo;
                m_SharedResInfo.m_CustomFonts.Remove(idBuffer);
            }
            if(!m_SharedResInfo.m_CustomFonts.Insert(idBuffer, pFontInfo))
            {
                ::DeleteObject(hFont);
                delete pFontInfo;
                return NULL;
            }
        }
        else
        {
            TFontInfo* pOldFontInfo = static_cast<TFontInfo*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
            if(pOldFontInfo)
            {
                ::DeleteObject(pOldFontInfo->hFont);
                delete pOldFontInfo;
                m_ResInfo.m_CustomFonts.Remove(idBuffer);
            }
            if(!m_ResInfo.m_CustomFonts.Insert(idBuffer, pFontInfo))
            {
                ::DeleteObject(hFont);
                delete pFontInfo;
                return NULL;
            }
        }
        return hFont;
    }

    HFONT CDuiPaintManager::GetFont(int id)
    {
        if(id < 0)
        {
            return GetDefaultFontInfo()->hFont;
        }
        TCHAR idBuffer[16];
        ::ZeroMemory(idBuffer, sizeof(idBuffer));
        _itot(id, idBuffer, 10);
        TFontInfo* pFontInfo = static_cast<TFontInfo*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
        if(!pFontInfo)
        {
            pFontInfo = static_cast<TFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
        }
        if(!pFontInfo)
        {
            return GetDefaultFontInfo()->hFont;
        }
        return pFontInfo->hFont;
    }

    HFONT CDuiPaintManager::GetFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic)
    {
        TFontInfo* pFontInfo = NULL;
        for(int i = 0; i < m_ResInfo.m_CustomFonts.GetSize(); i++)
        {
            if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i))
            {
                pFontInfo = static_cast<TFontInfo*>(m_ResInfo.m_CustomFonts.Find(key));
                if(pFontInfo && pFontInfo->sFontName == pStrFontName && pFontInfo->iSize == nSize &&
                        pFontInfo->bBold == bBold && pFontInfo->bUnderline == bUnderline && pFontInfo->bItalic == bItalic)
                {
                    return pFontInfo->hFont;
                }
            }
        }
        for(int i = 0; i < m_SharedResInfo.m_CustomFonts.GetSize(); i++)
        {
            if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i))
            {
                pFontInfo = static_cast<TFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(key));
                if(pFontInfo && pFontInfo->sFontName == pStrFontName && pFontInfo->iSize == nSize &&
                        pFontInfo->bBold == bBold && pFontInfo->bUnderline == bUnderline && pFontInfo->bItalic == bItalic)
                {
                    return pFontInfo->hFont;
                }
            }
        }
        return NULL;
    }

    void CDuiPaintManager::RemoveAllFonts()
    {
        TFontInfo* pFontInfo = NULL;
        for(int i = 0; i < m_ResInfo.m_CustomFonts.GetSize(); i++)
        {
            if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i))
            {
                pFontInfo = static_cast<TFontInfo*>(m_ResInfo.m_CustomFonts.Find(key, false));
                if(pFontInfo)
                {
                    ::DeleteObject(pFontInfo->hFont);
                    DUI_FREE_POINT(pFontInfo);
                }
            }
        }
        m_ResInfo.m_CustomFonts.RemoveAll();
    }

    TFontInfo* CDuiPaintManager::GetFontInfo(int id)
    {
        if(id < 0)
        {
            return GetDefaultFontInfo();
        }
        TCHAR idBuffer[16];
        ::ZeroMemory(idBuffer, sizeof(idBuffer));
        _itot(id, idBuffer, 10);
        TFontInfo* pFontInfo = static_cast<TFontInfo*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
        if(!pFontInfo)
        {
            pFontInfo = static_cast<TFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
        }
        if(!pFontInfo)
        {
            pFontInfo = GetDefaultFontInfo();
        }
        if(pFontInfo->tm.tmHeight == 0)
        {
            HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, pFontInfo->hFont);
            ::GetTextMetrics(m_hDcPaint, &pFontInfo->tm);
            ::SelectObject(m_hDcPaint, hOldFont);
        }
        return pFontInfo;
    }

    TFontInfo* CDuiPaintManager::GetFontInfo(HFONT hFont)
    {
        TFontInfo* pFontInfo = NULL;
        for(int i = 0; i < m_ResInfo.m_CustomFonts.GetSize(); i++)
        {
            if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i))
            {
                pFontInfo = static_cast<TFontInfo*>(m_ResInfo.m_CustomFonts.Find(key));
                if(pFontInfo && pFontInfo->hFont == hFont)
                {
                    break;
                }
            }
        }
        if(!pFontInfo)
        {
            for(int i = 0; i < m_SharedResInfo.m_CustomFonts.GetSize(); i++)
            {
                if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i))
                {
                    pFontInfo = static_cast<TFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(key));
                    if(pFontInfo && pFontInfo->hFont == hFont)
                    {
                        break;
                    }
                }
            }
        }
        if(!pFontInfo)
        {
            pFontInfo = GetDefaultFontInfo();
        }
        if(pFontInfo->tm.tmHeight == 0)
        {
            HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, pFontInfo->hFont);
            ::GetTextMetrics(m_hDcPaint, &pFontInfo->tm);
            ::SelectObject(m_hDcPaint, hOldFont);
        }
        return pFontInfo;
    }

    void CDuiPaintManager::SetCapture()
    {
        ::SetCapture(m_hWndPaint);
        m_bMouseCapture = TRUE;
    }

    void CDuiPaintManager::ReleaseCapture()
    {
        ::ReleaseCapture();
        m_bMouseCapture = FALSE;
        m_bDragMode = FALSE;
    }

    void CDuiPaintManager::UsedVirtualWnd(BOOL bUsed)
    {
        m_bUsedVirtualWnd = bUsed;
    }

    BOOL CDuiPaintManager::IsUseGdiplusText() const
    {
        return m_bUseGdiplusText;
    }

    int CDuiPaintManager::GetGdiplusTextRenderingHint() const
    {
        return m_trh;
    }

    void CDuiPaintManager::SetUseGdiplusText(BOOL bUse)
    {
        m_bUseGdiplusText = bUse;
    }

    void CDuiPaintManager::SetGdiplusTextRenderingHint(int trh)
    {
        m_trh = trh;
    }

    BOOL CDuiPaintManager::SetNextTabControl(BOOL bForward /*= TRUE*/)
    {
        // If we're in the process of restructuring the layout we can delay the
        // focus calulation until the next repaint.
        if(m_bUpdateNeeded && bForward)
        {
            m_bFocusNeeded = true;
            ::InvalidateRect(m_hWndPaint, NULL, FALSE);
            return true;
        }
        // Find next/previous tabbable control
        FINDTABINFO info1 = { 0 };
        info1.pFocus = m_pFocus;
        info1.bForward = bForward;
        CDuiControl* pControl = m_pRoot->FindControl(__FindControlFromTab, &info1, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
        if(pControl == NULL)
        {
            if(bForward)
            {
                // Wrap around
                FINDTABINFO info2 = { 0 };
                info2.pFocus = bForward ? NULL : info1.pLast;
                info2.bForward = bForward;
                pControl = m_pRoot->FindControl(__FindControlFromTab, &info2, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
            }
            else
            {
                pControl = info1.pLast;
            }
        }
        if(pControl != NULL)
        {
            SetFocus(pControl);
        }
        m_bFocusNeeded = false;
        return true;
    }

    CDPI* CDuiPaintManager::GetDPIObj()
    {
        if(m_pDPI == NULL)
        {
            m_pDPI = new CDPI;
        }
        return m_pDPI;
    }

    CDuiShadow* CDuiPaintManager::GetShadow()
    {
        return &m_shadow;
    }

    BOOL CDuiPaintManager::AddNotifier(INotifyUI* pNotifier)
    {
        ASSERT(m_aNotifiers.Find(pNotifier) < 0);
        return m_aNotifiers.Add(pNotifier);
    }

    void CDuiPaintManager::SendNotify(TNotifyUI& Msg, BOOL bAsync /*= FALSE*/)
    {
        Msg.ptMouse = m_ptLastMousePos;
        Msg.dwTimestamp = ::GetTickCount();
        if(m_bUsedVirtualWnd)
        {
            Msg.sVirtualWnd = Msg.pSender->GetVirtualWnd();
        }
        if(!bAsync)
        {
            // Send to all listeners
            if(Msg.pSender != NULL)
            {
                if(Msg.pSender->OnNotify)
                {
                    Msg.pSender->OnNotify(&Msg);
                }
            }
            for(int i = 0; i < m_aNotifiers.GetSize(); i++)
            {
                static_cast<INotifyUI*>(m_aNotifiers[i])->Notify(Msg);
            }
        }
        else
        {
            TNotifyUI* pMsg = new TNotifyUI;
            pMsg->pSender = Msg.pSender;
            pMsg->sType = Msg.sType;
            pMsg->wParam = Msg.wParam;
            pMsg->lParam = Msg.lParam;
            pMsg->ptMouse = Msg.ptMouse;
            pMsg->dwTimestamp = Msg.dwTimestamp;
            m_aAsyncNotify.Add(pMsg);
            ::PostMessage(m_hWndPaint, WM_APP + 1, 0, 0L);
        }
    }

    void CDuiPaintManager::SendNotify(CDuiControl* pControl, LPCTSTR pstrMessage, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/, BOOL bAsync /*= FALSE*/)
    {
        TNotifyUI Msg;
        Msg.pSender = pControl;
        Msg.sType = pstrMessage;
        Msg.wParam = wParam;
        Msg.lParam = lParam;
        SendNotify(Msg, bAsync);
    }

    void CDuiPaintManager::AddDelayedCleanup(CDuiControl* pControl)
    {
        pControl->SetManager(this, NULL, FALSE);
        m_aDelayedCleanup.Add(pControl);
        ::PostMessage(m_hWndPaint, WM_APP + 1, 0, 0L);
    }

    DWORD CDuiPaintManager::GetDefaultLinkFontColor() const
    {
        return m_ResInfo.m_dwDefaultLinkFontColor;
    }

    DWORD CDuiPaintManager::GetDefaultLinkHoverFontColor() const
    {
        return m_ResInfo.m_dwDefaultLinkHoverFontColor;
    }

    DWORD CDuiPaintManager::GetDefaultSelectedBkColor() const
    {
        return m_ResInfo.m_dwDefaultSelectedBkColor;
    }

    void CDuiPaintManager::AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList, BOOL bShared /*= FALSE*/)
    {
        if(bShared || m_bForceUseSharedRes)
        {
            CDuiString* pDefaultAttr = new CDuiString(pStrControlAttrList);
            if(pDefaultAttr != NULL)
            {
                CDuiString* pOldDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Set(pStrControlName, (LPVOID)pDefaultAttr));
                DUI_FREE_POINT(pOldDefaultAttr);
            }
        }
        else
        {
            CDuiString* pDefaultAttr = new CDuiString(pStrControlAttrList);
            if(pDefaultAttr != NULL)
            {
                CDuiString* pOldDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Set(pStrControlName, (LPVOID)pDefaultAttr));
                DUI_FREE_POINT(pOldDefaultAttr);
            }
        }
    }

    LPCTSTR CDuiPaintManager::GetDefaultAttributeList(LPCTSTR pStrControlName) const
    {
        CDuiString* pDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Find(pStrControlName));
        if(!pDefaultAttr)
        {
            pDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Find(pStrControlName));
        }
        if(pDefaultAttr)
        {
            return pDefaultAttr->GetData();
        }
        return NULL;
    }


    void CDuiPaintManager::RemoveAllDefaultAttributeList()
    {
        CDuiString* pDefaultAttr = NULL;
        for(int i = 0; i < m_ResInfo.m_AttrHash.GetSize(); i++)
        {
            if(LPCTSTR key = m_ResInfo.m_AttrHash.GetAt(i))
            {
                pDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Find(key));
                DUI_FREE_POINT(pDefaultAttr);
            }
        }
        m_ResInfo.m_AttrHash.RemoveAll();
    }

    BOOL CDuiPaintManager::InitControls(CDuiControl* pControl, CDuiControl* pParent /*= NULL*/)
    {
        ASSERT(pControl);
        if(pControl == NULL)
        {
            return FALSE;
        }
        pControl->SetManager(this, pParent != NULL ? pParent : pControl->GetParent(), TRUE);
        pControl->FindControl(__FindControlFromNameHash, this, UIFIND_ALL);
        return TRUE;
    }

    BOOL CDuiPaintManager::AttachDialog(CDuiControl* pControl)
    {
        ASSERT(::IsWindow(m_hWndPaint));
        // 创建阴影窗口
        m_shadow.Create(this);
        SetFocus(NULL);
        if(m_pRoot != NULL)
        {
            m_aPostPaintControls.Empty();
            AddDelayedCleanup(m_pRoot);
        }
        m_bUpdateNeeded = TRUE;
        m_bFocusNeeded = TRUE;
        m_bFirstLayout = TRUE;
        m_pRoot = pControl;
        return InitControls(pControl);
    }

    CDuiControl* CDuiPaintManager::FindControl(POINT pt) const
    {
        ASSERT(m_pRoot);
        return m_pRoot->FindControl(__FindControlFromPoint, &pt, UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
    }

    CDuiControl* CDuiPaintManager::FindControl(LPCTSTR pstrName) const
    {
        ASSERT(m_pRoot);
        return static_cast<CDuiControl*>(m_mNameHash.Find(pstrName));
    }

    BOOL CDuiPaintManager::IsLayered()
    {
        return m_bLayered;
    }

    const TImageInfo* CDuiPaintManager::GetImage(LPCTSTR bitmap)
    {
        TImageInfo* data = static_cast<TImageInfo*>(m_ResInfo.m_ImageHash.Find(bitmap));
        if(!data)
        {
            data = static_cast<TImageInfo*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
        }
        return data;
    }

    const TImageInfo* CDuiPaintManager::GetImageEx(LPCTSTR bitmap, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/, BOOL bUseHSL /*= FALSE*/, HINSTANCE instance /*= NULL*/)
    {
        const TImageInfo* data = GetImage(bitmap);
        if(!data)
        {
            if(AddImage(bitmap, type, mask, bUseHSL, FALSE, instance))
            {
                if(m_bForceUseSharedRes)
                {
                    data = static_cast<TImageInfo*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
                }
                else
                {
                    data = static_cast<TImageInfo*>(m_ResInfo.m_ImageHash.Find(bitmap));
                }
            }
        }
        return data;
    }

    const TImageInfo* CDuiPaintManager::AddImage(LPCTSTR bitmap, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/, BOOL bUseHSL /*= FALSE*/, BOOL bShared /*= FALSE*/, HINSTANCE instance /*= NULL*/)
    {
        if(bitmap == NULL || bitmap[0] == _T('\0'))
        {
            return NULL;
        }
        TImageInfo* data = NULL;
        if(type != NULL)
        {
            if(isdigit(*bitmap))
            {
                LPTSTR pstr = NULL;
                int iIndex = _tcstol(bitmap, &pstr, 10);
                data = CRenderEngine::LoadImage(iIndex, type, mask, instance);
            }
        }
        else
        {
            data = CRenderEngine::LoadImage(bitmap, NULL, mask, instance);
        }
        if(data == NULL)
        {
            return NULL;
        }
        data->bUseHSL = bUseHSL;
        if(type != NULL)
        {
            data->sResType = type;
        }
        data->dwMask = mask;
        if(data->bUseHSL)
        {
            data->pSrcBits = new BYTE[data->nX * data->nY * 4];
            ::CopyMemory(data->pSrcBits, data->pBits, data->nX * data->nY * 4);
        }
        else
        {
            data->pSrcBits = NULL;
        }
        if(m_bUseHSL)
        {
            CRenderEngine::AdjustImage(TRUE, data, m_H, m_S, m_L);
        }
        if(data)
        {
            if(bShared || m_bForceUseSharedRes)
            {
                TImageInfo* pOldImageInfo = static_cast<TImageInfo*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
                if(pOldImageInfo)
                {
                    CRenderEngine::FreeImage(pOldImageInfo);
                    m_SharedResInfo.m_ImageHash.Remove(bitmap);
                }
                if(!m_SharedResInfo.m_ImageHash.Insert(bitmap, data))
                {
                    CRenderEngine::FreeImage(data);
                    data = NULL;
                }
            }
            else
            {
                TImageInfo* pOldImageInfo = static_cast<TImageInfo*>(m_ResInfo.m_ImageHash.Find(bitmap));
                if(pOldImageInfo)
                {
                    CRenderEngine::FreeImage(pOldImageInfo);
                    m_ResInfo.m_ImageHash.Remove(bitmap);
                }
                if(!m_ResInfo.m_ImageHash.Insert(bitmap, data))
                {
                    CRenderEngine::FreeImage(data);
                    data = NULL;
                }
            }
        }
        return data;
    }

    const TImageInfo* CDuiPaintManager::AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, BOOL bAlpha, BOOL bShared /*= FALSE*/)
    {
        // 因无法确定外部HBITMAP格式，不能使用hsl调整
        if(bitmap == NULL || bitmap[0] == _T('\0'))
        {
            return NULL;
        }
        if(hBitmap == NULL || iWidth <= 0 || iHeight <= 0)
        {
            return NULL;
        }
        TImageInfo* data = new TImageInfo;
        data->pBits = NULL;
        data->pSrcBits = NULL;
        data->hBitmap = hBitmap;
        data->pBits = NULL;
        data->nX = iWidth;
        data->nY = iHeight;
        data->bAlpha = bAlpha;
        data->bUseHSL = FALSE;
        data->pSrcBits = NULL;
        data->dwMask = 0;
        if(bShared || m_bForceUseSharedRes)
        {
            if(!m_SharedResInfo.m_ImageHash.Insert(bitmap, data))
            {
                CRenderEngine::FreeImage(data);
                data = NULL;
            }
        }
        else
        {
            if(!m_ResInfo.m_ImageHash.Insert(bitmap, data))
            {
                CRenderEngine::FreeImage(data);
                data = NULL;
            }
        }
        return data;
    }

    void CDuiPaintManager::RemoveAllImages()
    {
        TImageInfo* data = NULL;
        for(int i = 0; i < m_ResInfo.m_ImageHash.GetSize(); i++)
        {
            if(LPCTSTR key = m_ResInfo.m_ImageHash.GetAt(i))
            {
                data = static_cast<TImageInfo*>(m_ResInfo.m_ImageHash.Find(key, FALSE));
                if(data)
                {
                    CRenderEngine::FreeImage(data);
                }
            }
        }
        m_ResInfo.m_ImageHash.RemoveAll();
    }

    const TDrawInfo* CDuiPaintManager::GetDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify)
    {
        CDuiString sStrImage = pStrImage;
        CDuiString sStrModify = pStrModify;
        CDuiString sKey = sStrImage + sStrModify;
        TDrawInfo* pDrawInfo = static_cast<TDrawInfo*>(m_ResInfo.m_DrawInfoHash.Find(sKey));
        if(pDrawInfo == NULL && !sKey.IsEmpty())
        {
            pDrawInfo = new TDrawInfo();
            pDrawInfo->Parse(pStrImage, pStrModify, this);
            m_ResInfo.m_DrawInfoHash.Insert(sKey, pDrawInfo);
        }
        return pDrawInfo;
    }

    void CDuiPaintManager::RemoveAllDrawInfos()
    {
        TDrawInfo* pDrawInfo = NULL;
        for(int i = 0; i < m_ResInfo.m_DrawInfoHash.GetSize(); i++)
        {
            LPCTSTR key = m_ResInfo.m_DrawInfoHash.GetAt(i);
            if(key != NULL)
            {
                pDrawInfo = static_cast<TDrawInfo*>(m_ResInfo.m_DrawInfoHash.Find(key, FALSE));
                DUI_FREE_POINT(pDrawInfo);
            }
        }
        m_ResInfo.m_DrawInfoHash.RemoveAll();
    }

    BOOL CDuiPaintManager::TranslateAccelerator(LPMSG pMsg)
    {
        for(int i = 0; i < m_aTranslateAccelerator.GetSize(); i++)
        {
            LRESULT lResult = static_cast<ITranslateAccelerator*>(m_aTranslateAccelerator[i])->TranslateAccelerator(pMsg);
            if(lResult == S_OK)
            {
                return TRUE;
            }
        }
        return FALSE;
    }

    BOOL CDuiPaintManager::PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes)
    {
        for(int i = 0; i < m_aPreMessageFilters.GetSize(); i++)
        {
            BOOL bHandled = FALSE;
            LRESULT lResult = static_cast<IMessageFilterUI*>(m_aPreMessageFilters[i])->MessageHandler(uMsg, wParam, lParam, bHandled);
            if(bHandled)
            {
                return TRUE;
            }
        }
        //switch( uMsg ) {
        //case WM_KEYDOWN:
        //	{
        //		// Tabbing between controls
        //		if( wParam == VK_TAB ) {
        //			if( m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && _tcsstr(m_pFocus->GetClass(), _T("RichEditUI")) != NULL ) {
        //				if( static_cast<CRichEditUI*>(m_pFocus)->IsWantTab() ) return FALSE;
        //			}
        //			if( m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && _tcsstr(m_pFocus->GetClass(), _T("WkeWebkitUI")) != NULL ) {
        //				return FALSE;
        //			}
        //			SetNextTabControl(::GetKeyState(VK_SHIFT) >= 0);
        //			return TRUE;
        //		}
        //	}
        //	break;
        //case WM_SYSCHAR:
        //	{
        //		// Handle ALT-shortcut key-combinations
        //		FINDSHORTCUT fs = { 0 };
        //		fs.ch = toupper((int)wParam);
        //		CControlUI* pControl = m_pRoot->FindControl(__FindControlFromShortcut, &fs, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
        //		if( pControl != NULL ) {
        //			pControl->SetFocus();
        //			pControl->Activate();
        //			return TRUE;
        //		}
        //	}
        //	break;
        //case WM_SYSKEYDOWN:
        //	{
        //		if( m_pFocus != NULL ) {
        //			TEventUI event = { 0 };
        //			event.Type = UIEVENT_SYSKEY;
        //			event.chKey = (TCHAR)wParam;
        //			event.ptMouse = m_ptLastMousePos;
        //			event.wKeyState = MapKeyState();
        //			event.dwTimestamp = ::GetTickCount();
        //			m_pFocus->Event(event);
        //		}
        //	}
        //	break;
        //}
        return FALSE;
    }

    BOOL CDuiPaintManager::SetTimer(CDuiControl* pControl, UINT nTimerID, UINT uElapse)
    {
        ASSERT(pControl != NULL);
        for(int i = 0; i < m_aTimers.GetSize(); i++)
        {
            TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
            if(pTimer->pSender == pControl
                    && pTimer->hWnd == m_hWndPaint
                    && pTimer->nLocalID == nTimerID)
            {
                if(pTimer->bKilled == TRUE)
                {
                    if(::SetTimer(m_hWndPaint, pTimer->uWinTimer, uElapse, NULL))
                    {
                        pTimer->bKilled = FALSE;
                        return TRUE;
                    }
                    return FALSE;
                }
                return FALSE;
            }
        }
        m_uTimerID = (++m_uTimerID) % 0xF0; //0xf1-0xfe特殊用途
        if(!::SetTimer(m_hWndPaint, m_uTimerID, uElapse, NULL))
        {
            return FALSE;
        }
        TIMERINFO* pTimer = new TIMERINFO;
        if(pTimer == NULL)
        {
            return FALSE;
        }
        pTimer->hWnd = m_hWndPaint;
        pTimer->pSender = pControl;
        pTimer->nLocalID = nTimerID;
        pTimer->uWinTimer = m_uTimerID;
        pTimer->bKilled = FALSE;
        return m_aTimers.Add(pTimer);
    }

    BOOL CDuiPaintManager::KillTimer(CDuiControl* pControl, UINT nTimerID)
    {
        ASSERT(pControl != NULL);
        for(int i = 0; i < m_aTimers.GetSize(); i++)
        {
            TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
            if(pTimer->pSender == pControl
                    && pTimer->hWnd == m_hWndPaint
                    && pTimer->nLocalID == nTimerID)
            {
                if(pTimer->bKilled == FALSE)
                {
                    if(::IsWindow(m_hWndPaint))
                    {
                        ::KillTimer(pTimer->hWnd, pTimer->uWinTimer);
                    }
                    pTimer->bKilled = TRUE;
                    return TRUE;
                }
            }
        }
        return FALSE;
    }

    void CDuiPaintManager::RemoveAllTimers()
    {
        for(int i = 0; i < m_aTimers.GetSize(); i++)
        {
            TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
            if(pTimer && pTimer->hWnd == m_hWndPaint)
            {
                if(pTimer->bKilled == FALSE)
                {
                    if(::IsWindow(m_hWndPaint))
                    {
                        ::KillTimer(m_hWndPaint, pTimer->uWinTimer);
                    }
                }
                DUI_FREE_POINT(pTimer);
            }
        }

        m_aTimers.Empty();
    }

    HINSTANCE CDuiPaintManager::GetInstance()
    {
        return m_hInstance;
    }

    BOOL CDuiPaintManager::Initialize(HINSTANCE hInstance)
    {
        ASSERT(hInstance);
        m_hInstance = hInstance;
        if(m_hMsimg32Module == NULL)
        {
            m_hMsimg32Module = ::LoadLibrary(_T("msimg32.dll"));
        }

        m_SharedResInfo.m_dwDefaultDisabledColor			= 0xFFA7A6AA;
        m_SharedResInfo.m_dwDefaultFontColor				= 0xFF000000;
        m_SharedResInfo.m_dwDefaultLinkFontColor			= 0xFF0000FF;
        m_SharedResInfo.m_dwDefaultLinkHoverFontColor		= 0xFFD3215F;
        m_SharedResInfo.m_dwDefaultSelectedBkColor			= 0xFFBAE4FF;
        LOGFONT lf = { 0 };
        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        lf.lfCharSet = DEFAULT_CHARSET;
        m_SharedResInfo.m_DefaultFontInfo.hFont			= ::CreateFontIndirect(&lf);
        m_SharedResInfo.m_DefaultFontInfo.sFontName		= lf.lfFaceName;
        m_SharedResInfo.m_DefaultFontInfo.iSize			= -lf.lfHeight;
        m_SharedResInfo.m_DefaultFontInfo.bBold			= (lf.lfWeight >= FW_BOLD);
        m_SharedResInfo.m_DefaultFontInfo.bUnderline	= (lf.lfUnderline == TRUE);
        m_SharedResInfo.m_DefaultFontInfo.bItalic		= (lf.lfItalic == TRUE);
        ::ZeroMemory(&m_SharedResInfo.m_DefaultFontInfo.tm, sizeof(m_SharedResInfo.m_DefaultFontInfo.tm));

        CDuiShadow::Initialize(m_hInstance);
        return TRUE;
    }

    void CDuiPaintManager::RemoveAllShared()
    {
        if(m_SharedResInfo.m_DefaultFontInfo.hFont != NULL)
        {
            ::DeleteObject(m_SharedResInfo.m_DefaultFontInfo.hFont);
            m_SharedResInfo.m_DefaultFontInfo.hFont = NULL;
        }

        CDuiString* pDefaultAttr = NULL;
        for(int i = 0; i < m_SharedResInfo.m_AttrHash.GetSize(); i++)
        {
            if(LPCTSTR key = m_SharedResInfo.m_AttrHash.GetAt(i))
            {
                pDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Find(key));
                DUI_FREE_POINT(pDefaultAttr);
            }
        }
        m_SharedResInfo.m_AttrHash.RemoveAll();

        TImageInfo* data = NULL;
        for(int i = 0; i < m_SharedResInfo.m_ImageHash.GetSize(); i++)
        {
            if(LPCTSTR key = m_SharedResInfo.m_ImageHash.GetAt(i))
            {
                data = static_cast<TImageInfo*>(m_SharedResInfo.m_ImageHash.Find(key, FALSE));
                if(data)
                {
                    CRenderEngine::FreeImage(data);
                }
            }
        }
        m_SharedResInfo.m_ImageHash.RemoveAll();

        TFontInfo* pFontInfo = NULL;
        for(int i = 0; i < m_SharedResInfo.m_CustomFonts.GetSize(); i++)
        {
            if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i))
            {
                pFontInfo = static_cast<TFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(key, false));
                if(pFontInfo)
                {
                    ::DeleteObject(pFontInfo->hFont);
                    DUI_FREE_POINT(pFontInfo);
                }
            }
        }
        m_SharedResInfo.m_CustomFonts.RemoveAll();

        CDuiString* pStyle = NULL;
        for(int i = 0; i < m_SharedResInfo.m_StyleHash.GetSize(); i++)
        {
            if(LPCTSTR key = m_SharedResInfo.m_StyleHash.GetAt(i))
            {
                pStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Find(key));
                DUI_FREE_POINT(pStyle);
            }
        }
        m_SharedResInfo.m_StyleHash.RemoveAll();
    }

    void CDuiPaintManager::Uninitialize()
    {
        RemoveAllShared();

        CDuiControlFactory::GetInstance()->Release();

        if(m_hMsimg32Module != NULL)
        {
            FreeModule(m_hMsimg32Module);
            m_hMsimg32Module = NULL;
        }
    }

    CDuiString CDuiPaintManager::GetInstancePath()
    {
        if(m_hInstance == NULL)
        {
            return _T('\0');
        }
        TCHAR tszModule[MAX_PATH + 1] = { 0 };
        ::GetModuleFileName(m_hInstance, tszModule, MAX_PATH);
        CDuiString strInstancePath = tszModule;
        int pos = strInstancePath.ReverseFind(_T('\\'));
        if(pos >= 0)
        {
            strInstancePath = strInstancePath.Left(pos + 1);
        }
        return strInstancePath;
    }

    HINSTANCE CDuiPaintManager::GetResourceDll()
    {
        if(m_hResourceInstance == NULL)
        {
            return m_hInstance;
        }
        return m_hResourceInstance;
    }

    int CDuiPaintManager::GetResourceType()
    {
        return m_iResourceType;
    }

    const CDuiString& CDuiPaintManager::GetResourcePath()
    {
        return m_strResourcePath;
    }

    const CDuiString& CDuiPaintManager::GetResourceZip()
    {
        return m_strResourceZip;
    }

    void CDuiPaintManager::SetResourceType(int iResourceType)
    {
        m_iResourceType = iResourceType;
    }

    void CDuiPaintManager::SetResourcePath(LPCTSTR pStrPath)
    {
        m_strResourcePath = pStrPath;
        if(m_strResourcePath.IsEmpty())
        {
            return;
        }
        TCHAR cEnd = m_strResourcePath.GetAt(m_strResourcePath.GetLength() - 1);
        if(cEnd != _T('\\') && cEnd != _T('/'))
        {
            m_strResourcePath += _T('\\');
        }
    }

    BOOL CDuiPaintManager::IsCachedResourceZip()
    {
        return m_bCachedResourceZip;
    }

    HANDLE CDuiPaintManager::GetResourceZipHandle()
    {
        return m_hResourceZip;
    }

    BOOL CDuiPaintManager::GetHSL(short* H, short* S, short* L)
    {
        *H = m_H;
        *S = m_S;
        *L = m_L;
        return m_bUseHSL;
    }

    UINT CDuiPaintManager::MapKeyState()
    {
        UINT uState = 0;
        if(::GetKeyState(VK_CONTROL) < 0)
        {
            uState |= MK_CONTROL;
        }
        if(::GetKeyState(VK_RBUTTON) < 0)
        {
            uState |= MK_LBUTTON;
        }
        if(::GetKeyState(VK_LBUTTON) < 0)
        {
            uState |= MK_RBUTTON;
        }
        if(::GetKeyState(VK_SHIFT) < 0)
        {
            uState |= MK_SHIFT;
        }
        if(::GetKeyState(VK_MENU) < 0)
        {
            uState |= MK_ALT;
        }
        return uState;
    }

    LRESULT CDuiPaintManager::OnDuiNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        for(int i = 0; i < m_aDelayedCleanup.GetSize(); i++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(m_aDelayedCleanup[i]);
            DUI_FREE_POINT(pControl);
        }
        m_aDelayedCleanup.Empty();
        TNotifyUI* pMsg = NULL;
        while(pMsg = static_cast<TNotifyUI*>(m_aAsyncNotify.GetAt(0)))
        {
            m_aAsyncNotify.Remove(0);
            if(pMsg->pSender != NULL)
            {
                if(pMsg->pSender->OnNotify)
                {
                    pMsg->pSender->OnNotify(pMsg);
                }
            }
            for(int i = 0; i < m_aNotifiers.GetSize(); i++)
            {
                static_cast<INotifyUI*>(m_aNotifiers[i])->Notify(*pMsg);
            }
            DUI_FREE_POINT(pMsg);
        }
        return 0;
    }

    LRESULT CDuiPaintManager::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        RECT rcPaint = { 0 };
        if(!::GetUpdateRect(m_hWndPaint, &rcPaint, FALSE))
        {
            return 0;
        }
        if(m_pRoot == NULL)
        {
            PAINTSTRUCT ps = { 0 };
            ::BeginPaint(m_hWndPaint, &ps);
            ::EndPaint(m_hWndPaint, &ps);
            return 0;
        }
        BOOL bNeedSizeMsg = FALSE;
        RECT rcClient = { 0 };
        ::GetClientRect(m_hWndPaint, &rcClient);
        DWORD dwWidth = rcClient.right - rcClient.left;
        DWORD dwHeight = rcClient.bottom - rcClient.top;
        SetPainting(TRUE);
        if(m_bUpdateNeeded)
        {
            m_bUpdateNeeded = FALSE;
            if(!::IsRectEmpty(&rcClient) && !::IsIconic(m_hWndPaint))
            {
                if(m_pRoot->IsUpdateNeeded())
                {
                    if(m_hDcOffscreen != NULL)
                    {
                        ::DeleteDC(m_hDcOffscreen);
                        m_hDcOffscreen = NULL;
                    }
                    if(m_hbmpOffscreen != NULL)
                    {
                        ::DeleteObject(m_hbmpOffscreen);
                        m_hbmpOffscreen = NULL;
                    }
                    m_pRoot->SetPos(rcClient, TRUE);
                    bNeedSizeMsg = TRUE;
                }
                else
                {
                    CDuiControl* pControl = NULL;
                    m_aFoundControls.Empty();
                    m_pRoot->FindControl(__FindControlsFromUpdate, NULL, UIFIND_VISIBLE | UIFIND_ME_FIRST | UIFIND_UPDATETEST);
                    for(int it = 0; it < m_aFoundControls.GetSize(); it++)
                    {
                        pControl = static_cast<CDuiControl*>(m_aFoundControls[it]);
                        if(!pControl->IsFloat())
                        {
                            pControl->SetPos(pControl->GetPos(), TRUE);
                        }
                        else
                        {
                            pControl->SetPos(pControl->GetRelativePos(), TRUE);
                        }
                    }
                }
                // We'll want to notify the window when it is first initialized
                // with the correct layout. The window form would take the time
                // to submit swipes/animations.
                if(m_bFirstLayout)
                {
                    m_bFirstLayout = FALSE;
                    SendNotify(m_pRoot, DUI_MSGTYPE_WINDOWINIT,  0, 0, FALSE);
                    // 更新阴影窗口显示
                    m_shadow.Update(m_hWndPaint);
                }
            }
        }
        // Set focus to first control?
        if(m_bFocusNeeded)
        {
            SetNextTabControl();
        }
        if(m_bLayered)
        {
            DWORD dwExStyle = ::GetWindowLong(m_hWndPaint, GWL_EXSTYLE);
            DWORD dwNewExStyle = dwExStyle | WS_EX_LAYERED;
            if(dwExStyle != dwNewExStyle)
            {
                ::SetWindowLong(m_hWndPaint, GWL_EXSTYLE, dwNewExStyle);
            }
            m_bOffscreenPaint = TRUE;
            UnionRect(&rcPaint, &rcPaint, &m_rcLayeredUpdate);
            if(rcPaint.right > rcClient.right)
            {
                rcPaint.right = rcClient.right;
            }
            if(rcPaint.bottom > rcClient.bottom)
            {
                rcPaint.bottom = rcClient.bottom;
            }
            ::ZeroMemory(&m_rcLayeredUpdate, sizeof(m_rcLayeredUpdate));
        }
        if(m_bOffscreenPaint && m_hbmpOffscreen == NULL)
        {
            m_hDcOffscreen = ::CreateCompatibleDC(m_hDcPaint);
            m_hbmpOffscreen = CRenderEngine::CreateARGB32Bitmap(m_hDcPaint, dwWidth, dwHeight, (LPBYTE*)&m_pOffscreenBits);
            ASSERT(m_hDcOffscreen);
            ASSERT(m_hbmpOffscreen);
        }
        // Begin Windows paint
        PAINTSTRUCT ps = { 0 };
        ::BeginPaint(m_hWndPaint, &ps);
        if(m_bOffscreenPaint)
        {
            HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(m_hDcOffscreen, m_hbmpOffscreen);
            int iSaveDC = ::SaveDC(m_hDcOffscreen);
            if(m_bLayered)
            {
                for(LONG y = rcClient.bottom - rcPaint.bottom; y < rcClient.bottom - rcPaint.top; ++y)
                {
                    for(LONG x = rcPaint.left; x < rcPaint.right; ++x)
                    {
                        int i = (y * dwWidth + x) * 4;
                        *(DWORD*)(&m_pOffscreenBits[i]) = 0;
                    }
                }
            }
            m_pRoot->DoPaint(m_hDcOffscreen, rcPaint);
            DrawCaret(m_hDcOffscreen, rcPaint);
            for(int i = 0; i < m_aPostPaintControls.GetSize(); i++)
            {
                CDuiControl* pPostPaintControl = static_cast<CDuiControl*>(m_aPostPaintControls[i]);
                pPostPaintControl->DoPostPaint(m_hDcOffscreen, rcPaint);
            }
            if(m_bLayered)
            {
                for(LONG y = rcClient.bottom - rcPaint.bottom; y < rcClient.bottom - rcPaint.top; ++y)
                {
                    for(LONG x = rcPaint.left; x < rcPaint.right; ++x)
                    {
                        int i = (y * dwWidth + x) * 4;
                        if((m_pOffscreenBits[i + 3] == 0) && (m_pOffscreenBits[i + 0] != 0 || m_pOffscreenBits[i + 1] != 0 || m_pOffscreenBits[i + 2] != 0))
                        {
                            m_pOffscreenBits[i + 3] = 255;
                        }
                    }
                }
                for(int i = 0; i < m_aChildWnds.GetSize();)
                {
                    HWND hChildWnd = static_cast<HWND>(m_aChildWnds[i]);
                    if(!::IsWindow(hChildWnd))
                    {
                        m_aChildWnds.Remove(i);
                        continue;
                    }
                    ++i;
                    if(!::IsWindowVisible(hChildWnd))
                    {
                        continue;
                    }
                    RECT rcChildWnd;
                    GetChildWndRect(m_hWndPaint, hChildWnd, rcChildWnd);
                    RECT rcTemp = { 0 };
                    if(!::IntersectRect(&rcTemp, &rcPaint, &rcChildWnd))
                    {
                        continue;
                    }
                    COLORREF* pChildBitmapBits = NULL;
                    HDC hChildMemDC = ::CreateCompatibleDC(m_hDcOffscreen);
                    HBITMAP hChildBitmap = CRenderEngine::CreateARGB32Bitmap(hChildMemDC, rcChildWnd.right - rcChildWnd.left, rcChildWnd.bottom - rcChildWnd.top, (LPBYTE*)&pChildBitmapBits);
                    ::ZeroMemory(pChildBitmapBits, (rcChildWnd.right - rcChildWnd.left) * (rcChildWnd.bottom - rcChildWnd.top) * 4);
                    HBITMAP hOldChildBitmap = (HBITMAP) ::SelectObject(hChildMemDC, hChildBitmap);
                    ::SendMessage(hChildWnd, WM_PRINT, (WPARAM)hChildMemDC, (LPARAM)(PRF_CHECKVISIBLE | PRF_CHILDREN | PRF_CLIENT | PRF_OWNED));
                    COLORREF* pChildBitmapBit;
                    for(LONG y = 0; y < rcChildWnd.bottom - rcChildWnd.top; y++)
                    {
                        for(LONG x = 0; x < rcChildWnd.right - rcChildWnd.left; x++)
                        {
                            pChildBitmapBit = pChildBitmapBits + y * (rcChildWnd.right - rcChildWnd.left) + x;
                            if(*pChildBitmapBit != 0x00000000)
                            {
                                *pChildBitmapBit |= 0xff000000;
                            }
                        }
                    }
                    ::BitBlt(m_hDcOffscreen, rcChildWnd.left, rcChildWnd.top, rcChildWnd.right - rcChildWnd.left, rcChildWnd.bottom - rcChildWnd.top, hChildMemDC, 0, 0, SRCCOPY);
                    ::SelectObject(hChildMemDC, hOldChildBitmap);
                    ::DeleteObject(hChildBitmap);
                    ::DeleteDC(hChildMemDC);
                }
            }
            ::RestoreDC(m_hDcOffscreen, iSaveDC);
            if(m_bLayered)
            {
                RECT rcWnd = { 0 };
                ::GetWindowRect(m_hWndPaint, &rcWnd);
                BLENDFUNCTION bf = { AC_SRC_OVER, 0, m_nOpacity, AC_SRC_ALPHA };
                POINT ptPos   = { rcWnd.left, rcWnd.top };
                SIZE sizeWnd  = { dwWidth, dwHeight };
                POINT ptSrc   = { 0, 0 };
                g_fUpdateLayeredWindow(m_hWndPaint, m_hDcPaint, &ptPos, &sizeWnd, m_hDcOffscreen, &ptSrc, 0, &bf, ULW_ALPHA);
            }
            else
            {
                ::BitBlt(m_hDcPaint, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top, m_hDcOffscreen, rcPaint.left, rcPaint.top, SRCCOPY);
            }
            ::SelectObject(m_hDcOffscreen, hOldBitmap);
            if(m_bShowUpdateRect)
            {
                CRenderEngine::DrawRect(m_hDcPaint, rcPaint, 1, 0xFFFF0000);
            }
        }
        else
        {
            // A standard paint job
            int iSaveDC = ::SaveDC(m_hDcPaint);
            m_pRoot->DoPaint(m_hDcPaint, rcPaint);
            for(int i = 0; i < m_aPostPaintControls.GetSize(); i++)
            {
                CDuiControl* pPostPaintControl = static_cast<CDuiControl*>(m_aPostPaintControls[i]);
                pPostPaintControl->DoPostPaint(m_hDcPaint, rcPaint);
            }
            ::RestoreDC(m_hDcPaint, iSaveDC);
        }
        // All Done!
        ::EndPaint(m_hWndPaint, &ps);
        if(m_bUpdateNeeded)
        {
            Invalidate();
        }
        SetPainting(FALSE);
        // 发送窗口大小改变消息
        if(bNeedSizeMsg)
        {
            this->SendNotify(m_pRoot, DUI_MSGTYPE_WINDOWSIZE, 0, 0, TRUE);
        }
        return 0;
    }

    LRESULT CDuiPaintManager::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(m_pFocus != NULL)
        {
            TEventUI event = { 0 };
            event.Type = UIEVENT_WINDOWSIZE;
            event.pSender = m_pFocus;
            event.dwTimestamp = ::GetTickCount();
            m_pFocus->Event(event);
        }
        if(m_pRoot != NULL)
        {
            m_pRoot->NeedUpdate();
        }
        return 0;
    }

    LRESULT CDuiPaintManager::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        ::SetFocus(m_hWndPaint);
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        m_ptLastMousePos = pt;
        CDuiControl* pControl = FindControl(pt);
        if(pControl == NULL)
        {
            DUI_TRACE(_T("this[%x] CDuiControl::FindControl[NULL]"), this);
            return 0;
        }
        if(pControl->GetManager() != this)
        {
            return 0;
        }
        // 准备拖拽
        //if(pControl->IsDragEnabled())
        //{
        //    m_bDragMode = true;
        //    if(m_hDragBitmap != NULL)
        //    {
        //        ::DeleteObject(m_hDragBitmap);
        //        m_hDragBitmap = NULL;
        //    }
        //    m_hDragBitmap = CRenderEngine::GenerateBitmap(this, pControl, pControl->GetPos());
        //}
        // 开启捕获
        SetCapture();
        // 事件处理
        m_pEventClick = pControl;
        pControl->SetFocus();
        TEventUI event = { 0 };
        event.Type = UIEVENT_BUTTONDOWN;
        event.pSender = pControl;
        event.wParam = wParam;
        event.lParam = lParam;
        event.ptMouse = pt;
        event.wKeyState = (WORD)wParam;
        event.dwTimestamp = ::GetTickCount();
        pControl->Event(event);
        return 0;
    }

    LRESULT CDuiPaintManager::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        ReleaseCapture();
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        m_ptLastMousePos = pt;
        if(m_pEventClick == NULL)
        {
            return 0;
        }

        TEventUI event = { 0 };
        event.Type = UIEVENT_BUTTONUP;
        event.pSender = m_pEventClick;
        event.wParam = wParam;
        event.lParam = lParam;
        event.ptMouse = pt;
        event.wKeyState = (WORD)wParam;
        event.dwTimestamp = ::GetTickCount();
        CDuiControl* pClick = m_pEventClick;
        m_pEventClick = NULL;
        pClick->Event(event);
        return 0;
    }

    LRESULT CDuiPaintManager::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        if(TIMERID_CARET == LOWORD(wParam))
        {
            Invalidate(m_rtCaret);
            m_bCaretActive = !m_bCaretActive;
        }
        else
        {
            for(int i = 0; i < m_aTimers.GetSize(); i++)
            {
                const TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
                if(pTimer->hWnd == m_hWndPaint &&
                        pTimer->uWinTimer == LOWORD(wParam) &&
                        pTimer->bKilled == FALSE)
                {
                    TEventUI event = { 0 };
                    event.Type = UIEVENT_TIMER;
                    event.pSender = pTimer->pSender;
                    event.dwTimestamp = ::GetTickCount();
                    event.ptMouse = m_ptLastMousePos;
                    event.wKeyState = MapKeyState();
                    event.wParam = pTimer->nLocalID;
                    event.lParam = lParam;
                    pTimer->pSender->Event(event);
                    break;
                }
            }
        }
        return 0;
    }

    LRESULT CDuiPaintManager::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        m_bMouseTracking = FALSE;
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        CDuiControl* pHover = FindControl(pt);
        if(pHover == NULL)
        {
            bHandled = FALSE;
            return 0;
        }
        // Generate mouse hover event
        if(m_pEventHover != NULL)
        {
            TEventUI event = { 0 };
            event.Type = UIEVENT_MOUSEHOVER;
            event.pSender = m_pEventHover;
            event.wParam = wParam;
            event.lParam = lParam;
            event.dwTimestamp = ::GetTickCount();
            event.ptMouse = pt;
            event.wKeyState = MapKeyState();
            m_pEventHover->Event(event);
        }
        // Create tooltip information
        CDuiString sToolTip = pHover->GetToolTip();
        if(sToolTip.IsEmpty())
        {
            return 0;
        }
        ::ZeroMemory(&m_ToolTip, sizeof(TOOLINFO));
        m_ToolTip.cbSize = sizeof(TOOLINFO);
        m_ToolTip.uFlags = TTF_IDISHWND;
        m_ToolTip.hwnd = m_hWndPaint;
        m_ToolTip.uId = (UINT_PTR) m_hWndPaint;
        m_ToolTip.hinst = m_hInstance;
        m_ToolTip.lpszText = const_cast<LPTSTR>((LPCTSTR) sToolTip);
        m_ToolTip.rect = pHover->GetPos();
        if(m_hwndTooltip == NULL)
        {
            m_hwndTooltip = ::CreateWindowEx(0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hWndPaint, NULL, m_hInstance, NULL);
            ::SendMessage(m_hwndTooltip, TTM_ADDTOOL, 0, (LPARAM) &m_ToolTip);
            ::SendMessage(m_hwndTooltip, TTM_SETMAXTIPWIDTH, 0, pHover->GetToolTipWidth());
        }
        if(!::IsWindowVisible(m_hwndTooltip))
        {
            ::SendMessage(m_hwndTooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolTip);
            ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&m_ToolTip);
        }
        return 0;
    }

    LRESULT CDuiPaintManager::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        if(m_hwndTooltip != NULL)
        {
            ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, FALSE, (LPARAM) &m_ToolTip);
        }
        if(m_bMouseTracking)
        {
            POINT pt = { 0 };
            RECT rcWnd = { 0 };
            ::GetCursorPos(&pt);
            ::GetWindowRect(m_hWndPaint, &rcWnd);
            if(!::IsIconic(m_hWndPaint) && ::GetActiveWindow() == m_hWndPaint && ::PtInRect(&rcWnd, pt))
            {
                if(::SendMessage(m_hWndPaint, WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y)) == HTCLIENT)
                {
                    ::ScreenToClient(m_hWndPaint, &pt);
                    ::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, MAKELPARAM(pt.x, pt.y));
                }
                else
                {
                    ::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, (LPARAM) - 1);
                }
            }
            else
            {
                ::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, (LPARAM) - 1);
            }
        }
        m_bMouseTracking = FALSE;
        return 0;
    }

    LRESULT CDuiPaintManager::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        if(!m_bMouseTracking)
        {
            TRACKMOUSEEVENT tme = { 0 };
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.hwndTrack = m_hWndPaint;
            tme.dwHoverTime = m_hwndTooltip == NULL ? 400UL : (DWORD) ::SendMessage(m_hwndTooltip, TTM_GETDELAYTIME, TTDT_INITIAL, 0L);
            _TrackMouseEvent(&tme);
            m_bMouseTracking = TRUE;
        }
        // Generate the appropriate mouse messages
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        // 是否移动
        BOOL bNeedDrag = TRUE;
        if(m_ptLastMousePos.x == pt.x && m_ptLastMousePos.y == pt.y)
        {
            bNeedDrag = FALSE;
        }
        // 记录鼠标位置
        m_ptLastMousePos = pt;
        CDuiControl* pControl = FindControl(pt);
        if(pControl != NULL && pControl->GetManager() != this)
        {
            return 0;
        }

        //// 拖拽事件
        //if(bNeedDrag && m_bDragMode && wParam == MK_LBUTTON)
        //{
        //    ::ReleaseCapture();
        //    CIDropSource* pdsrc = new CIDropSource;
        //    if(pdsrc == NULL)
        //    {
        //        return 0;
        //    }
        //    pdsrc->AddRef();

        //    CIDataObject* pdobj = new CIDataObject(pdsrc);
        //    if(pdobj == NULL)
        //    {
        //        return 0;
        //    }
        //    pdobj->AddRef();

        //    FORMATETC fmtetc = {0};
        //    STGMEDIUM medium = {0};
        //    fmtetc.dwAspect = DVASPECT_CONTENT;
        //    fmtetc.lindex = -1;
        //    //////////////////////////////////////
        //    fmtetc.cfFormat = CF_BITMAP;
        //    fmtetc.tymed = TYMED_GDI;
        //    medium.tymed = TYMED_GDI;
        //    HBITMAP hBitmap = (HBITMAP)OleDuplicateData(m_hDragBitmap, fmtetc.cfFormat, NULL);
        //    medium.hBitmap = hBitmap;
        //    pdobj->SetData(&fmtetc, &medium, FALSE);
        //    //////////////////////////////////////
        //    BITMAP bmap;
        //    GetObject(hBitmap, sizeof(BITMAP), &bmap);
        //    RECT rc = {0, 0, bmap.bmWidth, bmap.bmHeight};
        //    fmtetc.cfFormat = CF_ENHMETAFILE;
        //    fmtetc.tymed = TYMED_ENHMF;
        //    HDC hMetaDC = CreateEnhMetaFile(m_hDcPaint, NULL, NULL, NULL);
        //    HDC hdcMem = CreateCompatibleDC(m_hDcPaint);
        //    HGDIOBJ hOldBmp = ::SelectObject(hdcMem, hBitmap);
        //    ::BitBlt(hMetaDC, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);
        //    ::SelectObject(hdcMem, hOldBmp);
        //    medium.hEnhMetaFile = CloseEnhMetaFile(hMetaDC);
        //    DeleteDC(hdcMem);
        //    medium.tymed = TYMED_ENHMF;
        //    pdobj->SetData(&fmtetc, &medium, TRUE);
        //    //////////////////////////////////////
        //    CDragSourceHelper dragSrcHelper;
        //    POINT ptDrag = {0};
        //    ptDrag.x = bmap.bmWidth / 2;
        //    ptDrag.y = bmap.bmHeight / 2;
        //    dragSrcHelper.InitializeFromBitmap(hBitmap, ptDrag, rc, pdobj); //will own the bmp
        //    DWORD dwEffect;
        //    HRESULT hr = ::DoDragDrop(pdobj, pdsrc, DROPEFFECT_COPY | DROPEFFECT_MOVE, &dwEffect);
        //    pdsrc->Release();
        //    pdobj->Release();
        //    m_bDragMode = false;
        //    return 0;
        //}
        TEventUI event = { 0 };
        event.ptMouse = pt;
        event.wParam = wParam;
        event.lParam = lParam;
        event.dwTimestamp = ::GetTickCount();
        if(pControl != m_pEventHover && m_pEventHover != NULL)
        {
            event.Type = UIEVENT_MOUSELEAVE;
            event.pSender = m_pEventHover;
            m_pEventHover->Event(event);
            m_pEventHover = NULL;
            if(m_hwndTooltip != NULL)
            {
                ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, FALSE, (LPARAM) &m_ToolTip);
            }
        }
        if(pControl != m_pEventHover && pControl != NULL)
        {
            event.Type = UIEVENT_MOUSEENTER;
            event.pSender = pControl;
            pControl->Event(event);
            m_pEventHover = pControl;
        }
        if(m_pEventClick != NULL)
        {
            event.Type = UIEVENT_MOUSEMOVE;
            event.pSender = m_pEventClick;
            m_pEventClick->Event(event);
        }
        else if(pControl != NULL)
        {
            event.Type = UIEVENT_MOUSEMOVE;
            event.pSender = pControl;
            pControl->Event(event);
        }
        return 0;
    }

    LRESULT CDuiPaintManager::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        if(m_pFocus == NULL)
        {
            return 0;
        }
        TEventUI event = { 0 };
        event.Type = UIEVENT_KEYDOWN;
        event.pSender = m_pFocus;
        event.wParam = wParam;
        event.lParam = lParam;
        event.chKey = (TCHAR)wParam;
        event.ptMouse = m_ptLastMousePos;
        event.wKeyState = MapKeyState();
        event.dwTimestamp = ::GetTickCount();
        m_pFocus->Event(event);
        m_pEventKey = m_pFocus;
        return 0;
    }

    LRESULT CDuiPaintManager::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        if(m_pEventKey == NULL)
        {
            return 0;
        }
        TEventUI event = { 0 };
        event.Type = UIEVENT_KEYUP;
        event.pSender = m_pEventKey;
        event.wParam = wParam;
        event.lParam = lParam;
        event.chKey = (TCHAR)wParam;
        event.ptMouse = m_ptLastMousePos;
        event.wKeyState = MapKeyState();
        event.dwTimestamp = ::GetTickCount();
        m_pEventKey->Event(event);
        m_pEventKey = NULL;
        return 0;
    }

    LRESULT CDuiPaintManager::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ::ScreenToClient(m_hWndPaint, &pt);
        m_ptLastMousePos = pt;
        CDuiControl* pControl = FindControl(pt);
        if(pControl == NULL)
        {
            return 0;
        }
        if(pControl->GetManager() != this)
        {
            return 0;
        }
        int zDelta = (int)(short) HIWORD(wParam);
        TEventUI event = { 0 };
        event.Type = UIEVENT_SCROLLWHEEL;
        event.pSender = pControl;
        event.wParam = MAKELPARAM(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0);
        event.lParam = lParam;
        event.ptMouse = pt;
        event.wKeyState = MapKeyState();
        event.dwTimestamp = ::GetTickCount();
        pControl->Event(event);

        // Let's make sure that the scroll item below the cursor is the same as before...
        ::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, (LPARAM) MAKELPARAM(m_ptLastMousePos.x, m_ptLastMousePos.y));
        return 0;
    }

    LRESULT CDuiPaintManager::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(LOWORD(lParam) != HTCLIENT)
        {
            bHandled = FALSE;
            return 0;
        }
        if(m_bMouseCapture)
        {
            return 0;
        }

        POINT pt = { 0 };
        ::GetCursorPos(&pt);
        ::ScreenToClient(m_hWndPaint, &pt);
        CDuiControl* pControl = FindControl(pt);
        if(pControl == NULL)
        {
            bHandled = FALSE;
            return 0;
        }
        if((pControl->GetControlFlags() & UIFLAG_SETCURSOR) == 0)
        {
            bHandled = FALSE;
            return 0;
        }
        TEventUI event = { 0 };
        event.Type = UIEVENT_SETCURSOR;
        event.pSender = pControl;
        event.wParam = wParam;
        event.lParam = lParam;
        event.ptMouse = pt;
        event.wKeyState = MapKeyState();
        event.dwTimestamp = ::GetTickCount();
        pControl->Event(event);
        return 0;
    }

    CStdPtrArray* CDuiPaintManager::GetFoundControls()
    {
        return &m_aFoundControls;
    }

    CDuiControl* CALLBACK CDuiPaintManager::__FindControlFromNameHash(CDuiControl* pThis, LPVOID pData)
    {
        CDuiPaintManager* pManager = static_cast<CDuiPaintManager*>(pData);
        const CDuiString& sName = pThis->GetName();
        if(sName.IsEmpty())
        {
            return NULL;
        }
        // Add this control to the hash list
        pManager->m_mNameHash.Set(sName, pThis);
        return NULL; // Attempt to add all controls
    }

    CDuiControl* CALLBACK CDuiPaintManager::__FindControlsFromUpdate(CDuiControl* pThis, LPVOID pData)
    {
        if(pThis->IsUpdateNeeded())
        {
            pThis->GetManager()->GetFoundControls()->Add((LPVOID)pThis);
            return pThis;
        }
        return NULL;
    }

    CDuiControl* CALLBACK CDuiPaintManager::__FindControlFromTab(CDuiControl* pThis, LPVOID pData)
    {
        FINDTABINFO* pInfo = static_cast<FINDTABINFO*>(pData);
        if(pInfo->pFocus == pThis)
        {
            if(pInfo->bForward)
            {
                pInfo->bNextIsIt = TRUE;
            }
            return pInfo->bForward ? NULL : pInfo->pLast;
        }
        if((pThis->GetControlFlags() & UIFLAG_TABSTOP) == 0)
        {
            return NULL;
        }
        pInfo->pLast = pThis;
        if(pInfo->bNextIsIt || pInfo->pFocus == NULL)
        {
            return pThis;
        }
        return NULL;  // Examine all controls
    }

    CDuiControl* CALLBACK CDuiPaintManager::__FindControlFromPoint(CDuiControl* pThis, LPVOID pData)
    {
        LPPOINT pPoint = static_cast<LPPOINT>(pData);
        return ::PtInRect(&pThis->GetPos(), *pPoint) ? pThis : NULL;
    }

    BOOL CDuiPaintManager::TranslateMessage(const LPMSG pMsg)
    {
        // Pretranslate Message takes care of system-wide messages, such as
        // tabbing and shortcut key-combos. We'll look for all messages for
        // each window and any child control attached.
        UINT uStyle = GetWindowStyle(pMsg->hwnd);
        UINT uChildRes = uStyle & WS_CHILD;
        LRESULT lRes = 0;
        if(uChildRes != 0)
        {
            HWND hWndParent = ::GetParent(pMsg->hwnd);
            for(int i = 0; i < m_aPreMessages.GetSize(); i++)
            {
                CDuiPaintManager* pPaintManager = static_cast<CDuiPaintManager*>(m_aPreMessages[i]);
                HWND hTempParent = hWndParent;
                while(hTempParent)
                {
                    if(pMsg->hwnd == pPaintManager->GetPaintWindow() || hTempParent == pPaintManager->GetPaintWindow())
                    {
                        if(pPaintManager->TranslateAccelerator(pMsg))
                        {
                            return TRUE;
                        }
                        pPaintManager->PreMessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, lRes);
                    }
                    hTempParent = GetParent(hTempParent);
                }
            }
        }
        else
        {
            for(int i = 0; i < m_aPreMessages.GetSize(); i++)
            {
                CDuiPaintManager* pPaintManager = static_cast<CDuiPaintManager*>(m_aPreMessages[i]);
                if(pMsg->hwnd == pPaintManager->GetPaintWindow())
                {
                    if(pPaintManager->TranslateAccelerator(pMsg))
                    {
                        return TRUE;
                    }
                    if(pPaintManager->PreMessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, lRes))
                    {
                        return TRUE;
                    }
                    return FALSE;
                }
            }
        }
        return FALSE;
    }

    void CDuiPaintManager::MessageLoop()
    {
        MSG msg = { 0 };
        while(::GetMessage(&msg, NULL, 0, 0))
        {
            if(!CDuiPaintManager::TranslateMessage(&msg))
            {
                ::TranslateMessage(&msg);
                try
                {
                    ::DispatchMessage(&msg);
                }
                catch(...)
                {
                    DUI_TRACE(_T("EXCEPTION!"));
#ifdef _DEBUG
                    throw "CPaintManagerUI::MessageLoop";
#endif
                }
            }
        }
    }

    void CDuiPaintManager::GetChildWndRect(HWND hWnd, HWND hChildWnd, RECT& rcChildWnd)
    {
        ::GetWindowRect(hChildWnd, &rcChildWnd);
        POINT pt;
        pt.x = rcChildWnd.left;
        pt.y = rcChildWnd.top;
        ::ScreenToClient(hWnd, &pt);
        rcChildWnd.left = pt.x;
        rcChildWnd.top = pt.y;
        pt.x = rcChildWnd.right;
        pt.y = rcChildWnd.bottom;
        ::ScreenToClient(hWnd, &pt);
        rcChildWnd.right = pt.x;
        rcChildWnd.bottom = pt.y;
    }

    BOOL CDuiPaintManager::LoadPlugin(LPCTSTR pstrModuleName)
    {
        ASSERT(!::IsBadStringPtr(pstrModuleName, -1) || pstrModuleName == NULL);
        if(pstrModuleName == NULL)
        {
            return FALSE;
        }
        HMODULE hModule = ::LoadLibrary(pstrModuleName);
        if(hModule != NULL)
        {
            LPCREATECONTROL lpCreateControl = (LPCREATECONTROL)::GetProcAddress(hModule, "CreateControl");
            if(lpCreateControl != NULL)
            {
                if(m_aPlugins.Find(lpCreateControl) >= 0)
                {
                    return TRUE;
                }
                m_aPlugins.Add(lpCreateControl);
                return TRUE;
            }
        }
        return FALSE;
    }

    CStdPtrArray* CDuiPaintManager::GetPlugins()
    {
        return &m_aPlugins;
    }

}