#include "StdAfx.h"
#include "DuiPaintManager.h"

namespace DuiLib
{
    CShortCut::CShortCut()
        : m_ch(0)
        , m_bPickNext(FALSE)
    {

    }
    /////////////////////////////////////////////////////////
    CDuiEvent::CDuiEvent()
        : chKey(0)
        , wKeyState(0)
        , dwTimestamp(::GetTickCount())
        , Type(0)
        , wParam(0)
        , lParam(0)
        , pSender(NULL)
    {

    }


    ///////////////////////////////////////////////////////
    CDuiNotify::CDuiNotify()
        : dwTimestamp(::GetTickCount())
        , wParam(0)
        , lParam(0)
        , pSender(NULL)
    {

    }

    ////////////////////////////////////////////////////////
    CDuiFontInfo::CDuiFontInfo()
        : bBold(FALSE)
        , bUnderline(FALSE)
        , bItalic(FALSE)
        , iSize(0)
        , hFont(NULL)
    {
        ZeroMemory(&tm, sizeof(tm));
    }
    ///////////////////////////////////////////////////////////
    CDuiResInfo::CDuiResInfo()
        : m_dwDefaultDisabledColor(0)
        , m_dwDefaultFontColor(0)
        , m_dwDefaultLinkFontColor(0)
        , m_dwDefaultLinkHoverFontColor(0)
        , m_dwDefaultSelectedBkColor(0)
    {

    }
    /////////////////////////////////////////////////////////////
    CDuiImageInfo::CDuiImageInfo()
        : bAlpha(FALSE)
        , bUseHSL(FALSE)
        , dwMask(0)
        , nX(0)
        , nY(0)
        , hBitmap(NULL)
        , pBits(NULL)
        , pSrcBits(NULL)
    {

    }

    ///////////////////////////////////////////////////////
    CDuiTabInfo::CDuiTabInfo()
        : bForward(FALSE)
        , bNextIsIt(FALSE)
        , pFocus(NULL)
        , pLast(NULL)
    {

    }

    ////////////////////////////////////////////////////////
    CDuiTimerInfo::CDuiTimerInfo()
        : bKilled(FALSE)
        , nLocalID(0)
        , uWinTimer(0)
        , hWnd(NULL)
        , pSender(NULL)
    {

    }




    ///////////////////////////////////////////////////////
    CDrawInfo::CDrawInfo()
        : uFade(255)
        , bHole(FALSE)
        , bTiledX(FALSE)
        , bTiledY(FALSE)
        , bHSL(FALSE)
        , dwMask(0)
    {

    }

    void CDrawInfo::Parse(LPCTSTR lpImage, LPCTSTR lpModify, CDuiPaintManager* pPaintManager)
    {
        // 1、aaa.jpg
        // 2、file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0'
        // mask='#FF0000' fade='255' hole='FALSE' xtiled='FALSE' ytiled='FALSE'
        sDrawString = lpImage;
        sDrawModify = lpModify;
        sImageName = lpImage;
        CDuiString sItem;
        CDuiString sValue;
        LPTSTR pstr = NULL;
        for(int i = 0; i < 2; ++i)
        {
            if(i == 1)
            {
                lpImage = lpModify;
            }
            if(!lpImage)
            {
                continue;
            }
            while(*lpImage != _T('\0'))
            {
                sItem.Empty();
                sValue.Empty();
                while(*lpImage > _T('\0') && *lpImage <= _T(' '))
                {
                    lpImage = ::CharNext(lpImage);
                }
                while(*lpImage != _T('\0') && *lpImage != _T('=') && *lpImage > _T(' '))
                {
                    LPTSTR pstrTemp = ::CharNext(lpImage);
                    while(lpImage < pstrTemp)
                    {
                        sItem += *lpImage++;
                    }
                }
                while(*lpImage > _T('\0') && *lpImage <= _T(' '))
                {
                    lpImage = ::CharNext(lpImage);
                }
                if(*lpImage++ != _T('='))
                {
                    break;
                }
                while(*lpImage > _T('\0') && *lpImage <= _T(' '))
                {
                    lpImage = ::CharNext(lpImage);
                }
                if(*lpImage++ != _T('\''))
                {
                    break;
                }
                while(*lpImage != _T('\0') && *lpImage != _T('\''))
                {
                    LPTSTR pstrTemp = ::CharNext(lpImage);
                    while(lpImage < pstrTemp)
                    {
                        sValue += *lpImage++;
                    }
                }
                if(*lpImage++ != _T('\''))
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
                        pPaintManager->GetDPIObj()->Scale(&rcDest);
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
                        pPaintManager->GetDPIObj()->Scale(&rcSource);
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
                        pPaintManager->GetDPIObj()->Scale(&rcCorner);
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
                if(*lpImage++ != _T(' '))
                {
                    break;
                }
            }
        }
        // 调整DPI资源
        CDuiString strImage =  CDuiResourceManager::GetInstance()->GetImagePath(sImageName);
        if(!strImage.IsEmpty())
        {
            sImageName = strImage;
        }
        if(pPaintManager->GetDPIObj()->GetScale() != 100)
        {
            CDuiString sScale;
            sScale.Format(_T("@%d."), pPaintManager->GetDPIObj()->GetScale());
            sImageName.Replace(_T("."), sScale);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////

    HINSTANCE CDuiPaintManager::m_hInstance = NULL;
    DWORD CDuiPaintManager::m_dwMainThreadID = 0;
    HINSTANCE CDuiPaintManager::m_hResourceInstance = NULL;
    HMODULE CDuiPaintManager::m_hMsimg32Module = NULL;

    int CDuiPaintManager::m_iResourceType = DUILIB_FILE;
    BOOL CDuiPaintManager::m_bCachedResourceZip = TRUE;
    HANDLE CDuiPaintManager::m_hResourceZip = NULL;
    CDuiString CDuiPaintManager::m_strResourcePath;
    CDuiString CDuiPaintManager::m_strResourceZip;
    CDuiString CDuiPaintManager::m_pStrResourceZipPwd;  //Garfield 20160325 带密码zip包解密
    CStdPtrArray CDuiPaintManager::m_aPreMessages;
    CStdPtrArray CDuiPaintManager::m_aPlugins;

    short CDuiPaintManager::m_Hue = 180;
    short CDuiPaintManager::m_Saturation = 100;
    short CDuiPaintManager::m_Lightness = 100;
    BOOL CDuiPaintManager::m_bUseHSL = FALSE;

    CDuiResInfo CDuiPaintManager::m_SharedResInfo;

    CDuiPaintManager::CDuiPaintManager(void)
        : m_hWndPaint(NULL)
        , m_hwndTooltip(NULL)
        , m_hDcPaint(NULL)
        , m_hDcOffscreen(NULL)
        , m_hDragBitmap(NULL)
        , m_hbmpOffscreen(NULL)
        , m_nOpacity(0xFF)
        , m_bUpdateNeeded(TRUE)
        , m_bLayered(FALSE)
        , m_bForceUseSharedRes(FALSE)
        , m_bUsedVirtualWnd(FALSE)
        , m_bIsPainting(FALSE)
        , m_bOffscreenPaint(TRUE)
        , m_bShowUpdateRect(FALSE)
        , m_bFirstLayout(TRUE)
        , m_bMouseCapture(FALSE)
        , m_bDragMode(FALSE)
        , m_bUseGdiplusText(FALSE)
        , m_bCaretActive(FALSE)
        , m_bMouseTracking(FALSE)
        , m_bCaretShowing(FALSE)
        , m_trh(0)
        , m_uTimerID(0x1000)
        , m_gdiplusToken(0)
        , m_pDPI(new CDPI)
        , m_pOffscreenBits(NULL)
        , m_pRoot(NULL)
        , m_pFocus(NULL)
        , m_pEventClick(NULL)
        , m_pEventHover(NULL)
        , m_pEventKey(NULL)
        , m_pGdiplusStartupInput(new GdiplusStartupInput)
        , m_currentCaretObject(NULL)
    {
        ZeroMemory(&m_ToolTip, sizeof(m_ToolTip));

        m_ResInfo.m_dwDefaultDisabledColor		= m_SharedResInfo.m_dwDefaultDisabledColor;
        m_ResInfo.m_dwDefaultFontColor			= m_SharedResInfo.m_dwDefaultFontColor;
        m_ResInfo.m_dwDefaultLinkFontColor		= m_SharedResInfo.m_dwDefaultLinkFontColor;
        m_ResInfo.m_dwDefaultLinkHoverFontColor = m_SharedResInfo.m_dwDefaultLinkHoverFontColor;
        m_ResInfo.m_dwDefaultSelectedBkColor	= m_SharedResInfo.m_dwDefaultSelectedBkColor;

        GdiplusStartup(&m_gdiplusToken, m_pGdiplusStartupInput, NULL);  // 加载GDI接口
    }


    CDuiPaintManager::~CDuiPaintManager(void)
    {
        m_aPreMessages.Remove(m_aPreMessages.Find(this));
        for(int i = 0; i < m_aDelayedCleanup.GetSize(); i++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(m_aDelayedCleanup[i]);
            DUI_FREE_POINT(pControl);
            m_aDelayedCleanup.Remove(i);
        }

        for(int i = 0; i < m_aAsyncNotify.GetSize(); i++)
        {
            CDuiNotify* pNotify = static_cast<CDuiNotify*>(m_aAsyncNotify[i]);
            DUI_FREE_POINT(pNotify);
            m_aAsyncNotify.Remove(i);
        }
        for(int i = 0; i < m_aFonts.GetSize(); ++i)
        {
            HANDLE handle = static_cast<HANDLE>(m_aFonts.GetAt(i));
            ::RemoveFontMemResourceEx(handle);
            m_aFonts.Remove(i);
        }
        RemoveAllTimers();
        RemoveAllFonts();
        RemoveAllImages();
        RemoveAllDrawInfos();
        RemoveAllDefaultAttributeList();
        RemoveAllStyle();
        RemoveAllOptionGroups();
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
        if(m_hDragBitmap != NULL)
        {
            ::DeleteObject(m_hDragBitmap);
            m_hDragBitmap = NULL;
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
        DUI_FREE_POINT(m_pRoot);
        DUI_FREE_POINT(m_pDPI);
        DUI_FREE_POINT(m_pGdiplusStartupInput);
        if(m_gdiplusToken != 0)
        {
            //卸载GDIPlus,GdiplusShutdown必须要在delete 之后调用，如果在之前调用，delete 就会出错。
            GdiplusShutdown(m_gdiplusToken);
            m_gdiplusToken = 0;
        }
    }

    void CDuiPaintManager::Init(HWND hWnd)
    {
        ASSERT(::IsWindow(hWnd));
        m_mNameHash.Resize();
        RemoveAllFonts();
        RemoveAllImages();
        RemoveAllStyle();
        RemoveAllDefaultAttributeList();
        RemoveAllTimers();
        RemoveAllOptionGroups();
        if(m_hWndPaint != hWnd)
        {
            m_hWndPaint = hWnd;
            m_hDcPaint = ::GetDC(hWnd);
            m_aPreMessages.Add(this);
        }
        SetTargetWnd(hWnd);
        InitDragDrop();
    }

    BOOL CDuiPaintManager::IsUpdateNeeded() const
    {
        return m_bUpdateNeeded;
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

    HWND CDuiPaintManager::GetPaintWindow() const
    {
        return m_hWndPaint;
    }

    HWND CDuiPaintManager::GetTooltipWindow() const
    {
        return m_hwndTooltip;
    }

    void CDuiPaintManager::SetPainting(BOOL bIsPainting)
    {
        m_bIsPainting = bIsPainting;
    }

    POINT CDuiPaintManager::GetMousePos() const
    {
        return m_ptLastMousePos;
    }

    CDuiSize CDuiPaintManager::GetClientSize() const
    {
        CDuiRect rcClient;
        ::GetClientRect(m_hWndPaint, &rcClient);
        return CDuiSize(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
    }

    CDuiSize CDuiPaintManager::GetInitSize() const
    {
        return m_szInitWindowSize;
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

    RECT CDuiPaintManager::GetSizeBox()
    {
        return m_rcSizeBox;
    }

    void CDuiPaintManager::SetSizeBox(RECT& rcSizeBox)
    {
        m_rcSizeBox = rcSizeBox;
    }

    RECT CDuiPaintManager::GetCaptionRect()
    {
        return GetDPIObj()->Scale(m_rcCaption);
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

    BOOL CDuiPaintManager::IsShowUpdateRect() const
    {
        return m_bShowUpdateRect;
    }

    void CDuiPaintManager::SetShowUpdateRect(BOOL bShow)
    {
        m_bShowUpdateRect = bShow;
    }

    BYTE CDuiPaintManager::GetOpacity() const
    {
        return m_nOpacity;
    }

    void CDuiPaintManager::SetOpacity(BYTE nOpacity)
    {
        m_nOpacity = nOpacity;
        if(m_hWndPaint != NULL)
        {
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
            SetLayeredWindowAttributes(m_hWndPaint, 0, nOpacity, LWA_ALPHA);
        }
    }

    BOOL CDuiPaintManager::IsLayered() const
    {
        return m_bLayered;
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
            m_bLayered = bLayered;
            if(m_pRoot != NULL)
            {
                m_pRoot->NeedUpdate();
            }
            Invalidate();
        }
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

    DWORD CDuiPaintManager::GetDefaultLinkFontColor() const
    {
        return m_ResInfo.m_dwDefaultLinkFontColor;
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

    DWORD CDuiPaintManager::GetDefaultLinkHoverFontColor() const
    {
        return m_ResInfo.m_dwDefaultLinkHoverFontColor;
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

    DWORD CDuiPaintManager::GetDefaultSelectedBkColor() const
    {
        return m_ResInfo.m_dwDefaultSelectedBkColor;

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
            CDuiEvent event;
            event.Type = DUIEVENT_KILLFOCUS;
            event.pSender = pControl;
            m_pFocus->Event(event);
            SendNotify(m_pFocus, DUI_MSGTYPE_KILLFOCUS);
            m_pFocus = NULL;
        }
        if(pControl == NULL)
        {
            return;
        }
        if(pControl != NULL
                && pControl->GetManager() == this
                && pControl->IsVisible()
                && pControl->IsEnabled())
        {
            m_pFocus = pControl;
            CDuiEvent event;
            event.Type = DUIEVENT_SETFOCUS;
            event.pSender = pControl;
            m_pFocus->Event(event);
            SendNotify(m_pFocus, DUI_MSGTYPE_SETFOCUS);
        }
    }

    void CDuiPaintManager::SetFocusNeeded(CDuiControl* pControl)
    {
        ::SetFocus(m_hWndPaint);
        if(pControl == NULL)
        {
            return;
        }
        if(m_pFocus != NULL)
        {
            CDuiEvent event;
            event.Type = DUIEVENT_KILLFOCUS;
            event.pSender = pControl;
            m_pFocus->Event(event);
            SendNotify(m_pFocus, DUI_MSGTYPE_KILLFOCUS);
            m_pFocus = NULL;
        }
        CDuiTabInfo info;
        info.pFocus = pControl;
        info.bForward = FALSE;
        m_pFocus = m_pRoot->FindControl(__FindControlFromTab, &info, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
        if(m_pRoot != NULL)
        {
            m_pRoot->NeedUpdate();
        }
    }

    BOOL CDuiPaintManager::InitDragDrop()
    {
        AddRef();
        if(FAILED(RegisterDragDrop(m_hWndPaint, this))) //calls addref
        {
            DUI_ERROR("this[0x%p] RegisterDragDrop[FALSE]", this);
            return FALSE;
        }
        else
        {
            Release();
        }
        FORMATETC ftetc = {0};
        ftetc.cfFormat = CF_BITMAP;
        ftetc.dwAspect = DVASPECT_CONTENT;
        ftetc.lindex = -1;
        ftetc.tymed = TYMED_GDI;
        AddSuportedFormat(ftetc);
        ftetc.cfFormat = CF_DIB;
        ftetc.tymed = TYMED_HGLOBAL;
        AddSuportedFormat(ftetc);
        ftetc.cfFormat = CF_HDROP;
        ftetc.tymed = TYMED_HGLOBAL;
        AddSuportedFormat(ftetc);
        ftetc.cfFormat = CF_ENHMETAFILE;
        ftetc.tymed = TYMED_ENHMF;
        AddSuportedFormat(ftetc);
        return TRUE;
    }

    WORD CDuiPaintManager::DIBNumColors(void* pv)
    {
        int bits;
        LPBITMAPINFOHEADER  lpbi;
        LPBITMAPCOREHEADER  lpbc;
        lpbi = ((LPBITMAPINFOHEADER)pv);
        lpbc = ((LPBITMAPCOREHEADER)pv);
        /*  With the BITMAPINFO format headers, the size of the palette
        *  is in biClrUsed, whereas in the BITMAPCORE - style headers, it
        *  is dependent on the bits per pixel ( = 2 raised to the power of
        *  bits/pixel).
        */
        if(lpbi->biSize != sizeof(BITMAPCOREHEADER))
        {
            if(lpbi->biClrUsed != 0)
            {
                return (WORD)lpbi->biClrUsed;
            }
            bits = lpbi->biBitCount;
        }
        else
        {
            bits = lpbc->bcBitCount;
        }
        switch(bits)
        {
            case 1:
                return 2;
            case 4:
                return 16;
            case 8:
                return 256;
            default:
                /* A 24 bitcount DIB has no color table */
                return 0;
        }
    }

    WORD CDuiPaintManager::ColorTableSize(LPVOID lpv)
    {
        LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)lpv;

        if(lpbih->biSize != sizeof(BITMAPCOREHEADER))
        {
            if(((LPBITMAPINFOHEADER)(lpbih))->biCompression == BI_BITFIELDS)
                /* Remember that 16/32bpp dibs can still have a color table */
            {
                return (sizeof(DWORD) * 3) + (DIBNumColors(lpbih) * sizeof(RGBQUAD));
            }
            else
            {
                return (WORD)(DIBNumColors(lpbih) * sizeof(RGBQUAD));
            }
        }
        else
        {
            return (WORD)(DIBNumColors(lpbih) * sizeof(RGBTRIPLE));
        }
    }

    bool CDuiPaintManager::OnDrop(FORMATETC* pFmtEtc, STGMEDIUM& medium, DWORD* pdwEffect)
    {
        POINT ptMouse = {0};
        GetCursorPos(&ptMouse);
        ::SendMessage(m_hTargetWnd, WM_LBUTTONUP, NULL, MAKELPARAM(ptMouse.x, ptMouse.y));

        if(pFmtEtc->cfFormat == CF_DIB && medium.tymed == TYMED_HGLOBAL)
        {
            if(medium.hGlobal != NULL)
            {
                LPBITMAPINFOHEADER  lpbi = (BITMAPINFOHEADER*)GlobalLock(medium.hGlobal);
                if(lpbi != NULL)
                {
                    HBITMAP hbm;
                    HDC hdc = GetDC(NULL);
                    if(hdc != NULL)
                    {
                        int i = ((BITMAPFILEHEADER*)lpbi)->bfOffBits;
                        hbm = CreateDIBitmap(hdc, (LPBITMAPINFOHEADER)lpbi,
                                             (LONG)CBM_INIT,
                                             (LPSTR)lpbi + lpbi->biSize + ColorTableSize(lpbi),
                                             (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

                        ::ReleaseDC(NULL, hdc);
                    }
                    GlobalUnlock(medium.hGlobal);
                    if(hbm != NULL)
                    {
                        hbm = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm);
                    }
                    if(hbm != NULL)
                    {
                        DeleteObject(hbm);
                    }
                    return true; //release the medium
                }
            }
        }
        if(pFmtEtc->cfFormat == CF_BITMAP && medium.tymed == TYMED_GDI)
        {
            if(medium.hBitmap != NULL)
            {
                HBITMAP hBmp = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)medium.hBitmap);
                if(hBmp != NULL)
                {
                    DeleteObject(hBmp);
                }
                return false; //don't free the bitmap
            }
        }
        if(pFmtEtc->cfFormat == CF_ENHMETAFILE && medium.tymed == TYMED_ENHMF)
        {
            ENHMETAHEADER emh;
            GetEnhMetaFileHeader(medium.hEnhMetaFile, sizeof(ENHMETAHEADER), &emh);
            RECT rc;//={0,0,EnhMetaHdr.rclBounds.right-EnhMetaHdr.rclBounds.left, EnhMetaHdr.rclBounds.bottom-EnhMetaHdr.rclBounds.top};
            HDC hDC = GetDC(m_hTargetWnd);
            //start code: taken from ENHMETA.EXE MSDN Sample
            //*ALSO NEED to GET the pallete (select and RealizePalette it, but i was too lazy*
            // Get the characteristics of the output device
            float PixelsX = (float)GetDeviceCaps(hDC, HORZRES);
            float PixelsY = (float)GetDeviceCaps(hDC, VERTRES);
            float MMX = (float)GetDeviceCaps(hDC, HORZSIZE);
            float MMY = (float)GetDeviceCaps(hDC, VERTSIZE);
            // Calculate the rect in which to draw the metafile based on the
            // intended size and the current output device resolution
            // Remember that the intended size is given in 0.01mm units, so
            // convert those to device units on the target device
            rc.top = (int)((float)(emh.rclFrame.top) * PixelsY / (MMY * 100.0f));
            rc.left = (int)((float)(emh.rclFrame.left) * PixelsX / (MMX * 100.0f));
            rc.right = (int)((float)(emh.rclFrame.right) * PixelsX / (MMX * 100.0f));
            rc.bottom = (int)((float)(emh.rclFrame.bottom) * PixelsY / (MMY * 100.0f));
            //end code: taken from ENHMETA.EXE MSDN Sample

            HDC hdcMem = CreateCompatibleDC(hDC);
            HGDIOBJ hBmpMem = CreateCompatibleBitmap(hDC, emh.rclBounds.right, emh.rclBounds.bottom);
            HGDIOBJ hOldBmp = ::SelectObject(hdcMem, hBmpMem);
            PlayEnhMetaFile(hdcMem, medium.hEnhMetaFile, &rc);
            HBITMAP hBmp = (HBITMAP)::SelectObject(hdcMem, hOldBmp);
            DeleteDC(hdcMem);
            ReleaseDC(m_hTargetWnd, hDC);
            hBmp = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp);
            if(hBmp != NULL)
            {
                DeleteObject(hBmp);
            }
            return true;
        }
        if(pFmtEtc->cfFormat == CF_HDROP && medium.tymed == TYMED_HGLOBAL)
        {
            HDROP hDrop = (HDROP)GlobalLock(medium.hGlobal);
            if(hDrop != NULL)
            {
                TCHAR szFileName[MAX_PATH];
                UINT cFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
                if(cFiles > 0)
                {
                    DragQueryFile(hDrop, 0, szFileName, sizeof(szFileName));
                    HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
                    if(hBitmap)
                    {
                        HBITMAP hBmp = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
                        if(hBmp != NULL)
                        {
                            DeleteObject(hBmp);
                        }
                    }
                }
                //DragFinish(hDrop); // base class calls ReleaseStgMedium
            }
            GlobalUnlock(medium.hGlobal);
        }
        return true; //let base free the medium
    }

    CDuiRichEdit* CDuiPaintManager::GetCurrentCaretObject()
    {
        return m_currentCaretObject;
    }

    BOOL CDuiPaintManager::CreateCaret(HBITMAP hBmp, int nWidth, int nHeight)
    {
        ::CreateCaret(m_hWndPaint, hBmp, nWidth, nHeight);
        //TODO hBmp处理位图光标
        m_rtCaret.right = m_rtCaret.left + nWidth;
        m_rtCaret.bottom = m_rtCaret.top + nHeight;
        return TRUE;
    }

    BOOL CDuiPaintManager::ShowCaret(BOOL bShow)
    {
        if(m_bCaretShowing == bShow)
        {
            return TRUE;
        }

        m_bCaretShowing = bShow;
        if(!bShow)
        {
            ::KillTimer(m_hWndPaint, CARET_TIMERID);
            if(m_bCaretActive)
            {
                Invalidate(m_rtCaret);
            }
            m_bCaretActive = FALSE;
        }
        else
        {
            ::SetTimer(m_hWndPaint, CARET_TIMERID, ::GetCaretBlinkTime(), NULL);
            if(!m_bCaretActive)
            {
                Invalidate(m_rtCaret);
                m_bCaretActive = TRUE;
            }
        }

        return TRUE;
    }

    BOOL CDuiPaintManager::SetCaretPos(CDuiRichEdit* pRichEdit, int x, int y)
    {
        if(!::SetCaretPos(x, y))
        {
            return FALSE;
        }

        m_currentCaretObject = pRichEdit;
        RECT tempRt = m_rtCaret;
        int w = m_rtCaret.right - m_rtCaret.left;
        int h = m_rtCaret.bottom - m_rtCaret.top;
        m_rtCaret.left = x;
        m_rtCaret.top = y;
        m_rtCaret.right = x + w;
        m_rtCaret.bottom = y + h;
        Invalidate(tempRt);
        Invalidate(m_rtCaret);

        return TRUE;
    }

    void CDuiPaintManager::DrawCaret(HDC hDC, const RECT& rcPaint)
    {
        if(m_currentCaretObject && (!m_currentCaretObject->IsFocused() || m_hWndPaint != ::GetFocus()))
        {
            ::KillTimer(m_hWndPaint, CARET_TIMERID);
            if(m_bCaretActive)
            {
                Invalidate(m_rtCaret);
            }
            m_bCaretActive = FALSE;
            return;
        }

        if(m_bCaretActive && m_bCaretShowing && m_currentCaretObject)
        {
            RECT temp = {};
            if(::IntersectRect(&temp, &rcPaint, &m_rtCaret))
            {
                DWORD dwColor = m_currentCaretObject->GetTextColor();
                if(dwColor == 0)
                {
                    dwColor = m_ResInfo.m_dwDefaultFontColor;
                }
                CRenderEngine::DrawColor(hDC, temp, dwColor);
            }
        }
    }

    CDuiFontInfo* CDuiPaintManager::GetDefaultFontInfo()
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

    void CDuiPaintManager::SetDefaultFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic, BOOL bShared /*= FALSE*/)
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
        CDuiFontInfo* pFontInfo = new CDuiFontInfo;
        if(!pFontInfo)
        {
            return FALSE;
        }
        ::ZeroMemory(pFontInfo, sizeof(CDuiFontInfo));
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
            CDuiFontInfo* pOldFontInfo = static_cast<CDuiFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
            if(pOldFontInfo)
            {
                ::DeleteObject(pOldFontInfo->hFont);
                DUI_FREE_POINT(pOldFontInfo);
                m_SharedResInfo.m_CustomFonts.Remove(idBuffer);
            }
            if(!m_SharedResInfo.m_CustomFonts.Insert(idBuffer, pFontInfo))
            {
                ::DeleteObject(hFont);
                DUI_FREE_POINT(pFontInfo);
                return NULL;
            }
        }
        else
        {
            CDuiFontInfo* pOldFontInfo = static_cast<CDuiFontInfo*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
            if(pOldFontInfo)
            {
                ::DeleteObject(pOldFontInfo->hFont);
                DUI_FREE_POINT(pOldFontInfo);
                m_ResInfo.m_CustomFonts.Remove(idBuffer);
            }
            if(!m_ResInfo.m_CustomFonts.Insert(idBuffer, pFontInfo))
            {
                ::DeleteObject(hFont);
                DUI_FREE_POINT(pFontInfo);
                return NULL;
            }
        }
        return hFont;
    }

    void CDuiPaintManager::AddFontArray(LPCTSTR pstrPath)
    {
        LPBYTE pData = NULL;
        DWORD dwSize = 0;
        do
        {
            CDuiString sFile = CDuiPaintManager::GetResourcePath();
            if(CDuiPaintManager::GetResourceZip().IsEmpty())
            {
                sFile += pstrPath;
                HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
                                            FILE_ATTRIBUTE_NORMAL, NULL);
                if(hFile == INVALID_HANDLE_VALUE)
                {
                    break;
                }
                dwSize = ::GetFileSize(hFile, NULL);
                if(dwSize == 0)
                {
                    break;
                }

                DWORD dwRead = 0;
                pData = new BYTE[dwSize];
                ::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
                ::CloseHandle(hFile);

                if(dwRead != dwSize)
                {
                    DUI_FREE_ARRAY(pData);
                    break;
                }
            }
            else
            {
                sFile += CDuiPaintManager::GetResourceZip();
                HZIP hz = NULL;
                if(CDuiPaintManager::IsCachedResourceZip())
                {
                    hz = (HZIP)CDuiPaintManager::GetResourceZipHandle();
                }
                else
                {
                    CDuiString sFilePwd = CDuiPaintManager::GetResourceZipPwd();
#ifdef UNICODE
                    char* pwd = w2a((wchar_t*)sFilePwd.GetData());
                    hz = OpenZip(sFile.GetData(), pwd);
                    DUI_FREE_ARRAY(pwd);
#else
                    hz = OpenZip(sFile.GetData(), sFilePwd.GetData());
#endif
                }
                if(hz == NULL)
                {
                    break;
                }
                ZIPENTRY ze = {0};
                int i = 0;
                CDuiString key = pstrPath;
                key.Replace(_T("\\"), _T("/"));
                if(FindZipItem(hz, key, true, &i, &ze) != 0)
                {
                    break;
                }
                dwSize = ze.unc_size;
                if(dwSize == 0)
                {
                    break;
                }
                pData = new BYTE[dwSize];
                int res = UnzipItem(hz, i, pData, dwSize);
                if(res != 0x00000000 && res != 0x00000600)
                {
                    DUI_FREE_ARRAY(pData);
                    if(!CDuiPaintManager::IsCachedResourceZip())
                    {
                        CloseZip(hz);
                    }
                    break;
                }
                if(!CDuiPaintManager::IsCachedResourceZip())
                {
                    CloseZip(hz);
                }
            }

        }
        while(0);

        while(!pData)
        {
            //读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
            HANDLE hFile = ::CreateFile(pstrPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if(hFile == INVALID_HANDLE_VALUE)
            {
                break;
            }
            dwSize = ::GetFileSize(hFile, NULL);
            if(dwSize == 0)
            {
                break;
            }

            DWORD dwRead = 0;
            pData = new BYTE[dwSize];
            ::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
            ::CloseHandle(hFile);

            if(dwRead != dwSize)
            {
                DUI_FREE_ARRAY(pData);
            }
            break;
        }
        DWORD nFonts;
        HANDLE hFont = ::AddFontMemResourceEx(pData, dwSize, NULL, &nFonts);
        m_aFonts.Add(hFont);
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
        CDuiFontInfo* pFontInfo = static_cast<CDuiFontInfo*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
        if(!pFontInfo)
        {
            pFontInfo = static_cast<CDuiFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
        }
        if(!pFontInfo)
        {
            return GetDefaultFontInfo()->hFont;
        }
        return pFontInfo->hFont;
    }

    HFONT CDuiPaintManager::GetFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic)
    {
        CDuiFontInfo* pFontInfo = NULL;
        for(int i = 0; i < m_ResInfo.m_CustomFonts.GetSize(); i++)
        {
            if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i))
            {
                pFontInfo = static_cast<CDuiFontInfo*>(m_ResInfo.m_CustomFonts.Find(key));
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
                pFontInfo = static_cast<CDuiFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(key));
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
        CDuiFontInfo* pFontInfo = NULL;
        for(int i = 0; i < m_ResInfo.m_CustomFonts.GetSize(); i++)
        {
            if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i))
            {
                pFontInfo = static_cast<CDuiFontInfo*>(m_ResInfo.m_CustomFonts.Find(key, FALSE));
                if(pFontInfo)
                {
                    ::DeleteObject(pFontInfo->hFont);
                    DUI_FREE_POINT(pFontInfo);
                }
            }
        }
        m_ResInfo.m_CustomFonts.RemoveAll();
    }

    CDuiFontInfo* CDuiPaintManager::GetFontInfo(int id)
    {
        if(id < 0)
        {
            return GetDefaultFontInfo();
        }
        TCHAR idBuffer[16];
        ::ZeroMemory(idBuffer, sizeof(idBuffer));
        _itot(id, idBuffer, 10);
        CDuiFontInfo* pFontInfo = static_cast<CDuiFontInfo*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
        if(!pFontInfo)
        {
            pFontInfo = static_cast<CDuiFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
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

    CDuiFontInfo* CDuiPaintManager::GetFontInfo(HFONT hFont)
    {
        CDuiFontInfo* pFontInfo = NULL;
        for(int i = 0; i < m_ResInfo.m_CustomFonts.GetSize(); i++)
        {
            if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i))
            {
                pFontInfo = static_cast<CDuiFontInfo*>(m_ResInfo.m_CustomFonts.Find(key));
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
                    pFontInfo = static_cast<CDuiFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(key));
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

    BOOL CDuiPaintManager::IsCaptured()
    {
        return m_bMouseCapture;
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
            ::InvalidateRect(m_hWndPaint, NULL, FALSE);
            return TRUE;
        }
        // Find next/previous tabbable control
        CDuiTabInfo info1;
        info1.pFocus = m_pFocus;
        info1.bForward = bForward;
        CDuiControl* pControl = m_pRoot->FindControl(__FindControlFromTab, &info1, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
        if(pControl == NULL)
        {
            if(bForward)
            {
                // Wrap around
                CDuiTabInfo info2;
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
        return TRUE;
    }

    CDPI* CDuiPaintManager::GetDPIObj()
    {
        if(m_pDPI == NULL)
        {
            m_pDPI = new CDPI;
        }
        return m_pDPI;
    }

    void CDuiPaintManager::RebuildFont(CDuiFontInfo* pFontInfo)
    {
        ::DeleteObject(pFontInfo->hFont);
        LOGFONT lf = { 0 };
        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        _tcsncpy(lf.lfFaceName, pFontInfo->sFontName, LF_FACESIZE);
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfHeight = -GetDPIObj()->Scale(pFontInfo->iSize);
        lf.lfQuality = CLEARTYPE_QUALITY;
        if(pFontInfo->bBold)
        {
            lf.lfWeight += FW_BOLD;
        }
        if(pFontInfo->bUnderline)
        {
            lf.lfUnderline = TRUE;
        }
        if(pFontInfo->bItalic)
        {
            lf.lfItalic = TRUE;
        }
        HFONT hFont = ::CreateFontIndirect(&lf);
        pFontInfo->hFont = hFont;
        ::ZeroMemory(&(pFontInfo->tm), sizeof(pFontInfo->tm));
        if(m_hDcPaint)
        {
            HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, hFont);
            ::GetTextMetrics(m_hDcPaint, &pFontInfo->tm);
            ::SelectObject(m_hDcPaint, hOldFont);
        }
    }

    void CDuiPaintManager::ResetDPIAssets()
    {
        RemoveAllDrawInfos();
        RemoveAllImages();;

        for(int it = 0; it < m_ResInfo.m_CustomFonts.GetSize(); it++)
        {
            CDuiFontInfo* pFontInfo = static_cast<CDuiFontInfo*>(m_ResInfo.m_CustomFonts.Find(m_ResInfo.m_CustomFonts[it]));
            RebuildFont(pFontInfo);
        }
        RebuildFont(&m_ResInfo.m_DefaultFontInfo);

        for(int it = 0; it < m_SharedResInfo.m_CustomFonts.GetSize(); it++)
        {
            CDuiFontInfo* pFontInfo = static_cast<CDuiFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(m_SharedResInfo.m_CustomFonts[it]));
            RebuildFont(pFontInfo);
        }
        RebuildFont(&m_SharedResInfo.m_DefaultFontInfo);

        //CStdPtrArray* richEditList = FindSubControlsByClass(GetRoot(), L"RichEditUI");
        //for(int i = 0; i < richEditList->GetSize(); i++)
        //{
        //    CRichEditUI* pT = static_cast<CRichEditUI*>((*richEditList)[i]);
        //    pT->SetFont(pT->GetFont());

        //}
    }

    void CDuiPaintManager::SetDPI(int iDPI)
    {
        int scale1 = GetDPIObj()->GetScale();
        GetDPIObj()->SetScale(iDPI);
        int scale2 = GetDPIObj()->GetScale();
        ResetDPIAssets();
        RECT rcWnd = {0};
        ::GetWindowRect(GetPaintWindow(), &rcWnd);
        RECT*  prcNewWindow = &rcWnd;
        if(!::IsZoomed(GetPaintWindow()))
        {
            RECT rc = rcWnd;
            rc.right = rcWnd.left + (rcWnd.right - rcWnd.left) * scale2 / scale1;
            rc.bottom = rcWnd.top + (rcWnd.bottom - rcWnd.top) * scale2 / scale1;
            prcNewWindow = &rc;
        }
        SetWindowPos(GetPaintWindow(), NULL, prcNewWindow->left, prcNewWindow->top, prcNewWindow->right - prcNewWindow->left, prcNewWindow->bottom - prcNewWindow->top, SWP_NOZORDER | SWP_NOACTIVATE);
        if(GetRoot() != NULL)
        {
            GetRoot()->NeedUpdate();
        }
        ::PostMessage(GetPaintWindow(), DUIMSG_SET_DPI, 0, 0);
    }

    CDuiShadow* CDuiPaintManager::GetShadow()
    {
        return &m_shadow;
    }

    BOOL CDuiPaintManager::AddNotifier(INotify* pNotifier)
    {
        ASSERT(m_aNotifiers.Find(pNotifier) < 0);
        return m_aNotifiers.Add(pNotifier);
    }

    void CDuiPaintManager::SendNotify(CDuiNotify& Msg, BOOL bAsync /*= FALSE*/)
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
                static_cast<INotify*>(m_aNotifiers[i])->Notify(Msg);
            }
        }
        else
        {
            CDuiNotify* pMsg = new CDuiNotify;
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
        CDuiNotify Msg;
        Msg.pSender = pControl;
        Msg.sType = pstrMessage;
        Msg.wParam = wParam;
        Msg.lParam = lParam;
        SendNotify(Msg, bAsync);
    }

    BOOL CDuiPaintManager::RemoveNotifier(INotify* pNotifier)
    {
        for(int i = 0; i < m_aNotifiers.GetSize(); i++)
        {
            if(static_cast<INotify*>(m_aNotifiers[i]) == pNotifier)
            {
                return m_aNotifiers.Remove(i);
            }
        }
        return FALSE;
    }

    void CDuiPaintManager::AddDelayedCleanup(CDuiControl* pControl)
    {
        pControl->SetManager(this, NULL, FALSE);
        m_aDelayedCleanup.Add(pControl);
        ::PostMessage(m_hWndPaint, WM_APP + 1, 0, 0L);
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
        m_pEventKey = NULL;
        m_pEventHover = NULL;
        m_pEventClick = NULL;
        if(m_pRoot != NULL)
        {
            m_aPostPaintControls.Empty();
            AddDelayedCleanup(m_pRoot);
        }
        m_pRoot = pControl;
        m_bUpdateNeeded = TRUE;
        m_bFirstLayout = TRUE;
        return InitControls(pControl);
    }

    void CDuiPaintManager::ReapObjects(CDuiControl* pControl)
    {
        if(pControl == m_pEventKey)
        {
            m_pEventKey = NULL;
        }
        if(pControl == m_pEventHover)
        {
            m_pEventHover = NULL;
        }
        if(pControl == m_pEventClick)
        {
            m_pEventClick = NULL;
        }
        if(pControl == m_pFocus)
        {
            m_pFocus = NULL;
        }
        KillTimer(pControl);
        const CDuiString& sName = pControl->GetName();
        if(!sName.IsEmpty())
        {
            if(pControl == FindControl(sName))
            {
                m_mNameHash.Remove(sName);
            }
        }
        for(int i = 0; i < m_aAsyncNotify.GetSize(); i++)
        {
            CDuiNotify* pMsg = static_cast<CDuiNotify*>(m_aAsyncNotify[i]);
            if(pMsg && pMsg->pSender == pControl)
            {
                pMsg->pSender = NULL;
            }
        }
    }

    BOOL CDuiPaintManager::AddPaintChildWnd(HWND hChildWnd)
    {
        RECT rcChildWnd;
        GetChildWndRect(m_hWndPaint, hChildWnd, rcChildWnd);
        Invalidate(rcChildWnd);

        if(m_aChildWnds.Find(hChildWnd) >= 0)
        {
            return FALSE;
        }
        return m_aChildWnds.Add(hChildWnd);
    }

    BOOL CDuiPaintManager::RemovePaintChildWnd(HWND hChildWnd)
    {
        for(int i = 0; i < m_aChildWnds.GetSize(); i++)
        {
            if(static_cast<HWND>(m_aChildWnds[i]) == hChildWnd)
            {
                return m_aChildWnds.Remove(i);
            }
        }
        return FALSE;
    }

    BOOL CDuiPaintManager::AddPostPaint(CDuiControl* pControl)
    {
        ASSERT(m_aPostPaintControls.Find(pControl) < 0);
        return m_aPostPaintControls.Add(pControl);
    }

    BOOL CDuiPaintManager::RemovePostPaint(CDuiControl* pControl)
    {
        for(int i = 0; i < m_aPostPaintControls.GetSize(); i++)
        {
            if(static_cast<CDuiControl*>(m_aPostPaintControls[i]) == pControl)
            {
                return m_aPostPaintControls.Remove(i);
            }
        }
        return FALSE;
    }

    CStdPtrArray* CDuiPaintManager::GetOptionGroup(LPCTSTR pStrGroupName)
    {
        LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
        if(lp)
        {
            return static_cast<CStdPtrArray*>(lp);
        }
        return NULL;
    }

    BOOL CDuiPaintManager::AddOptionGroup(LPCTSTR pStrGroupName, CDuiControl* pControl)
    {
        LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
        if(lp)
        {
            CStdPtrArray* aOptionGroup = static_cast<CStdPtrArray*>(lp);
            for(int i = 0; i < aOptionGroup->GetSize(); i++)
            {
                if(static_cast<CDuiControl*>(aOptionGroup->GetAt(i)) == pControl)
                {
                    return FALSE;
                }
            }
            aOptionGroup->Add(pControl);
        }
        else
        {
            CStdPtrArray* aOptionGroup = new CStdPtrArray(6);
            aOptionGroup->Add(pControl);
            m_mOptionGroup.Insert(pStrGroupName, aOptionGroup);
        }
        return TRUE;
    }

    void CDuiPaintManager::RemoveOptionGroup(LPCTSTR pStrGroupName, CDuiControl* pControl)
    {
        LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
        if(lp)
        {
            CStdPtrArray* aOptionGroup = static_cast<CStdPtrArray*>(lp);
            if(aOptionGroup == NULL)
            {
                return;
            }
            for(int i = 0; i < aOptionGroup->GetSize(); i++)
            {
                if(static_cast<CDuiControl*>(aOptionGroup->GetAt(i)) == pControl)
                {
                    aOptionGroup->Remove(i);
                    break;
                }
            }
            if(aOptionGroup->IsEmpty())
            {
                DUI_FREE_POINT(aOptionGroup);
                m_mOptionGroup.Remove(pStrGroupName);
            }
        }
    }

    void CDuiPaintManager::RemoveAllOptionGroups()
    {
        CStdPtrArray* aOptionGroup;
        for(int i = 0; i < m_mOptionGroup.GetSize(); i++)
        {
            if(LPCTSTR key = m_mOptionGroup.GetAt(i))
            {
                aOptionGroup = static_cast<CStdPtrArray*>(m_mOptionGroup.Find(key));
                DUI_FREE_POINT(aOptionGroup);
            }
        }
        m_mOptionGroup.RemoveAll();
    }

    CDuiControl* CDuiPaintManager::GetRoot() const
    {
        ASSERT(m_pRoot);
        return m_pRoot;
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

    CDuiControl* CDuiPaintManager::FindSubControlByName(CDuiControl* pParent, LPCTSTR pstrName) const
    {
        if(pParent == NULL)
        {
            pParent = GetRoot();
        }
        ASSERT(pParent);
        return pParent->FindControl(__FindControlFromName, (LPVOID)pstrName, UIFIND_ALL);
    }

    const CDuiImageInfo* CDuiPaintManager::GetImage(LPCTSTR bitmap)
    {
        CDuiImageInfo* data = static_cast<CDuiImageInfo*>(m_ResInfo.m_ImageHash.Find(bitmap));
        if(!data)
        {
            data = static_cast<CDuiImageInfo*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
        }
        return data;
    }

    const CDuiImageInfo* CDuiPaintManager::GetImageEx(LPCTSTR bitmap, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/, BOOL bUseHSL /*= FALSE*/, HINSTANCE instance /*= NULL*/)
    {
        const CDuiImageInfo* data = GetImage(bitmap);
        if(!data)
        {
            if(AddImage(bitmap, type, mask, bUseHSL, FALSE, instance))
            {
                if(m_bForceUseSharedRes)
                {
                    data = static_cast<CDuiImageInfo*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
                }
                else
                {
                    data = static_cast<CDuiImageInfo*>(m_ResInfo.m_ImageHash.Find(bitmap));
                }
            }
        }
        return data;
    }

    const CDuiImageInfo* CDuiPaintManager::AddImage(LPCTSTR bitmap, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/, BOOL bUseHSL /*= FALSE*/, BOOL bShared /*= FALSE*/, HINSTANCE instance /*= NULL*/)
    {
        if(bitmap == NULL || bitmap[0] == _T('\0'))
        {
            return NULL;
        }
        CDuiImageInfo* data = NULL;
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
            CRenderEngine::AdjustImage(TRUE, data, m_Hue, m_Saturation, m_Lightness);
        }
        if(data)
        {
            if(bShared || m_bForceUseSharedRes)
            {
                CDuiImageInfo* pOldImageInfo = static_cast<CDuiImageInfo*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
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
                CDuiImageInfo* pOldImageInfo = static_cast<CDuiImageInfo*>(m_ResInfo.m_ImageHash.Find(bitmap));
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

    const CDuiImageInfo* CDuiPaintManager::AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, BOOL bAlpha, BOOL bShared /*= FALSE*/)
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
        CDuiImageInfo* data = new CDuiImageInfo;
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

    void CDuiPaintManager::RemoveImage(LPCTSTR bitmap, BOOL bShared /*= FALSE*/)
    {
        CDuiImageInfo* data = NULL;
        if(bShared)
        {
            data = static_cast<CDuiImageInfo*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
            if(data)
            {
                CRenderEngine::FreeImage(data) ;
                m_SharedResInfo.m_ImageHash.Remove(bitmap);
            }
        }
        else
        {
            data = static_cast<CDuiImageInfo*>(m_ResInfo.m_ImageHash.Find(bitmap));
            if(data)
            {
                CRenderEngine::FreeImage(data) ;
                m_ResInfo.m_ImageHash.Remove(bitmap);
            }
        }
    }

    void CDuiPaintManager::RemoveAllImages()
    {
        CDuiImageInfo* data = NULL;
        for(int i = 0; i < m_ResInfo.m_ImageHash.GetSize(); i++)
        {
            if(LPCTSTR key = m_ResInfo.m_ImageHash.GetAt(i))
            {
                data = static_cast<CDuiImageInfo*>(m_ResInfo.m_ImageHash.Find(key, FALSE));
                if(data)
                {
                    CRenderEngine::FreeImage(data);
                }
            }
        }
        m_ResInfo.m_ImageHash.RemoveAll();
    }

    const CDrawInfo* CDuiPaintManager::GetDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify)
    {
        CDuiString sStrImage = pStrImage;
        CDuiString sStrModify = pStrModify;
        CDuiString sKey = sStrImage + sStrModify;
        CDrawInfo* pDrawInfo = static_cast<CDrawInfo*>(m_ResInfo.m_DrawInfoHash.Find(sKey));
        if(pDrawInfo == NULL && !sKey.IsEmpty())
        {
            pDrawInfo = new CDrawInfo();
            pDrawInfo->Parse(pStrImage, pStrModify, this);
            m_ResInfo.m_DrawInfoHash.Insert(sKey, pDrawInfo);
        }
        return pDrawInfo;
    }

    void CDuiPaintManager::RemoveAllDrawInfos()
    {
        CDrawInfo* pDrawInfo = NULL;
        for(int i = 0; i < m_ResInfo.m_DrawInfoHash.GetSize(); i++)
        {
            LPCTSTR key = m_ResInfo.m_DrawInfoHash.GetAt(i);
            if(key != NULL)
            {
                pDrawInfo = static_cast<CDrawInfo*>(m_ResInfo.m_DrawInfoHash.Find(key, FALSE));
                DUI_FREE_POINT(pDrawInfo);
            }
        }
        m_ResInfo.m_DrawInfoHash.RemoveAll();
    }

    BOOL CDuiPaintManager::AddPreMessageFilter(IMessageFilter* pFilter)
    {
        ASSERT(m_aPreMessageFilters.Find(pFilter) < 0);
        return m_aPreMessageFilters.Add(pFilter);
    }

    BOOL CDuiPaintManager::RemovePreMessageFilter(IMessageFilter* pFilter)
    {
        for(int i = 0; i < m_aPreMessageFilters.GetSize(); i++)
        {
            if(static_cast<IMessageFilter*>(m_aPreMessageFilters[i]) == pFilter)
            {
                return m_aPreMessageFilters.Remove(i);
            }
        }
        return FALSE;
    }

    BOOL CDuiPaintManager::PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes)
    {
        for(int i = 0; i < m_aPreMessageFilters.GetSize(); i++)
        {
            BOOL bHandled = FALSE;
            LRESULT lResult = static_cast<IMessageFilter*>(m_aPreMessageFilters[i])->MessageHandler(uMsg, wParam, lParam, bHandled);
            if(bHandled)
            {
                return TRUE;
            }
        }
        switch(uMsg)
        {
            case WM_KEYDOWN:
            {
                // Tabbing between controls
                if(wParam == VK_TAB)
                {
                    /*if(m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && _tcsstr(m_pFocus->GetClass(), _T("RichEditUI")) != NULL)
                    {
                    if(static_cast<CRichEditUI*>(m_pFocus)->IsWantTab())
                    {
                    return FALSE;
                    }
                    }*/
                    /*if(m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && _tcsstr(m_pFocus->GetClass(), _T("WkeWebkitUI")) != NULL)
                    {
                    return FALSE;
                    }*/
                    SetNextTabControl(::GetKeyState(VK_SHIFT) >= 0);
                    return TRUE;
                }
            }
            break;
            case WM_SYSCHAR:
            {
                // Handle ALT-shortcut key-combinations
                /*CShortCut fs = { 0 };
                fs.ch = toupper((int)wParam);
                CDuiControl* pControl = m_pRoot->FindControl(__FindControlFromShortcut, &fs, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
                if(pControl != NULL)
                {
                pControl->SetFocus();
                pControl->Activate();
                return TRUE;
                }*/
            }
            break;
            case WM_SYSKEYDOWN:
            {
                CShortCut fs;
                fs.m_ch = toupper((int)wParam);
                CDuiControl* pControl = m_pRoot->FindControl(__FindControlFromShortcut, &fs, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
                if(pControl != NULL)
                {
                    pControl->SetFocus();
                }
                if(m_pFocus != NULL)
                {
                    CDuiEvent event;
                    event.Type = DUIEVENT_SYSKEYDOWN;
                    event.chKey = (TCHAR)wParam;
                    event.ptMouse = m_ptLastMousePos;
                    event.wKeyState = MapKeyState();
                    m_pFocus->Event(event);
                }
            }
            break;
            case WM_SYSKEYUP:
            {
                CShortCut fs;
                fs.m_ch = toupper((int)wParam);
                CDuiControl* pControl = m_pRoot->FindControl(__FindControlFromShortcut, &fs, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
                if(pControl != NULL)
                {
                    pControl->SetFocus();
                }
                if(m_pFocus != NULL)
                {
                    CDuiEvent event;
                    event.Type = DUIEVENT_SYSKEYUP;
                    event.chKey = (TCHAR)wParam;
                    event.ptMouse = m_ptLastMousePos;
                    event.wKeyState = MapKeyState();
                    m_pFocus->Event(event);
                }
            }
            break;
            default:
                break;
        }
        return FALSE;
    }

    BOOL CDuiPaintManager::AddMessageFilter(IMessageFilter* pFilter)
    {
        ASSERT(m_aMessageFilters.Find(pFilter) < 0);
        return m_aMessageFilters.Add(pFilter);
    }

    BOOL CDuiPaintManager::RemoveMessageFilter(IMessageFilter* pFilter)
    {
        for(int i = 0; i < m_aMessageFilters.GetSize(); i++)
        {
            if(static_cast<IMessageFilter*>(m_aMessageFilters[i]) == pFilter)
            {
                return m_aMessageFilters.Remove(i);
            }
        }
        return FALSE;
    }

    BOOL CDuiPaintManager::AddTranslateAccelerator(ITranslateAccelerator* pTranslateAccelerator)
    {
        ASSERT(m_aTranslateAccelerator.Find(pTranslateAccelerator) < 0);
        return m_aTranslateAccelerator.Add(pTranslateAccelerator);
    }

    BOOL CDuiPaintManager::RemoveTranslateAccelerator(ITranslateAccelerator* pTranslateAccelerator)
    {
        for(int i = 0; i < m_aTranslateAccelerator.GetSize(); i++)
        {
            if(static_cast<ITranslateAccelerator*>(m_aTranslateAccelerator[i]) == pTranslateAccelerator)
            {
                return m_aTranslateAccelerator.Remove(i);
            }
        }
        return FALSE;
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

    BOOL CDuiPaintManager::SetTimer(CDuiControl* pControl, UINT nTimerID, UINT uElapse)
    {
        ASSERT(pControl != NULL);
        for(int i = 0; i < m_aTimers.GetSize(); i++)
        {
            CDuiTimerInfo* pTimer = static_cast<CDuiTimerInfo*>(m_aTimers[i]);
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
        CDuiTimerInfo* pTimer = new CDuiTimerInfo;
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
            CDuiTimerInfo* pTimer = static_cast<CDuiTimerInfo*>(m_aTimers[i]);
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

    void CDuiPaintManager::KillTimer(CDuiControl* pControl)
    {
        ASSERT(pControl != NULL);
        for(int i = 0, j = 0; i < m_aTimers.GetSize(); i++)
        {
            CDuiTimerInfo* pTimer = static_cast<CDuiTimerInfo*>(m_aTimers[i - j]);
            if(pTimer->pSender == pControl && pTimer->hWnd == m_hWndPaint)
            {
                if(pTimer->bKilled == FALSE)
                {
                    ::KillTimer(pTimer->hWnd, pTimer->uWinTimer);
                }
                DUI_FREE_POINT(pTimer);
                m_aTimers.Remove(i - j);
                j++;
            }
        }
    }

    void CDuiPaintManager::RemoveAllTimers()
    {
        for(int i = 0; i < m_aTimers.GetSize(); i++)
        {
            CDuiTimerInfo* pTimer = static_cast<CDuiTimerInfo*>(m_aTimers[i]);
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

    BOOL CDuiPaintManager::IsForceUseSharedRes() const
    {
        return m_bForceUseSharedRes;
    }

    void CDuiPaintManager::SetForceUseSharedRes(BOOL bForce)
    {
        m_bForceUseSharedRes = bForce;
    }


    CStdPtrArray* CDuiPaintManager::GetFoundControls()
    {
        return &m_aFoundControls;
    }


    LRESULT CDuiPaintManager::OnDuiNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        for(int i = 0; i < m_aDelayedCleanup.GetSize(); i++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(m_aDelayedCleanup[i]);
            DUI_FREE_POINT(pControl);
        }
        m_aDelayedCleanup.Empty();
        CDuiNotify* pMsg = NULL;
        while(pMsg = static_cast<CDuiNotify*>(m_aAsyncNotify.GetAt(0)))
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
                static_cast<INotify*>(m_aNotifiers[i])->Notify(*pMsg);
            }
            DUI_FREE_POINT(pMsg);
        }
        return 0;
    }

    LRESULT CDuiPaintManager::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        // Make sure all matching "closing" events are sent
        CDuiEvent event;
        event.ptMouse = m_ptLastMousePos;
        event.wKeyState = MapKeyState();
        if(m_pEventHover != NULL)
        {
            event.Type = DUIEVENT_MOUSELEAVE;
            event.pSender = m_pEventHover;
            m_pEventHover->Event(event);
        }
        if(m_pEventClick != NULL)
        {
            event.Type = DUIEVENT_BUTTONUP;
            event.pSender = m_pEventClick;
            m_pEventClick->Event(event);
        }

        SetFocus(NULL);

        if(::GetActiveWindow() == m_hWndPaint)
        {
            HWND hwndParent = GetWindowOwner(m_hWndPaint);
            if(hwndParent != NULL)
            {
                ::SetFocus(hwndParent);
            }
        }

        if(m_hwndTooltip != NULL)
        {
            ::DestroyWindow(m_hwndTooltip);
            m_hwndTooltip = NULL;
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
            m_rcLayeredUpdate.Empty();
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
                if(UpdateLayeredWindow(m_hWndPaint, m_hDcPaint, &ptPos, &sizeWnd, m_hDcOffscreen, &ptSrc, 0, &bf, ULW_ALPHA) == FALSE)
                {
                    DUI_ERROR("UpdateLayeredWindow FALSE");
                }
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
            CDuiEvent event;
            event.Type = DUIEVENT_WINDOWSIZE;
            event.pSender = m_pFocus;
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
            DUI_TRACE("this[0x%p] CDuiControl::FindControl[NULL]", this);
            return 0;
        }
        if(pControl->GetManager() != this)
        {
            return 0;
        }
        // 准备拖拽
        if(pControl->IsDragEnabled())
        {
            m_bDragMode = TRUE;
            if(m_hDragBitmap != NULL)
            {
                ::DeleteObject(m_hDragBitmap);
                m_hDragBitmap = NULL;
            }
            m_hDragBitmap = CRenderEngine::GenerateBitmap(this, pControl, pControl->GetPos());
        }
        // 开启捕获
        SetCapture();
        // 事件处理
        m_pEventClick = pControl;
        pControl->SetFocus();
        CDuiEvent event;
        event.Type = DUIEVENT_BUTTONDOWN;
        event.pSender = pControl;
        event.wParam = wParam;
        event.lParam = lParam;
        event.ptMouse = pt;
        event.wKeyState = (WORD)wParam;
        pControl->Event(event);
        return 0;
    }

    LRESULT CDuiPaintManager::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        ::SetFocus(m_hWndPaint);
        SetCapture();
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
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

        CDuiEvent event;
        event.Type = DUIEVENT_DBLCLICK;
        event.pSender = pControl;
        event.ptMouse = pt;
        event.wParam = wParam;
        event.lParam = lParam;
        event.wKeyState = (WORD)wParam;
        pControl->Event(event);
        m_pEventClick = pControl;
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

        CDuiEvent event;
        event.Type = DUIEVENT_BUTTONUP;
        event.pSender = m_pEventClick;
        event.wParam = wParam;
        event.lParam = lParam;
        event.ptMouse = pt;
        event.wKeyState = (WORD)wParam;
        CDuiControl* pClick = m_pEventClick;
        m_pEventClick = NULL;
        pClick->Event(event);
        return 0;
    }

    LRESULT CDuiPaintManager::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        ::SetFocus(m_hWndPaint);
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
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
        pControl->SetFocus();
        SetCapture();
        CDuiEvent event;
        event.Type = DUIEVENT_RBUTTONDOWN;
        event.pSender = pControl;
        event.wParam = wParam;
        event.lParam = lParam;
        event.ptMouse = pt;
        event.wKeyState = (WORD)wParam;
        pControl->Event(event);
        m_pEventClick = pControl;
        return 0;
    }

    LRESULT CDuiPaintManager::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        ReleaseCapture();

        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        m_ptLastMousePos = pt;
        m_pEventClick = FindControl(pt);
        if(m_pEventClick == NULL)
        {
            return 0;
        }
        CDuiEvent event;
        event.Type = DUIEVENT_RBUTTONUP;
        event.pSender = m_pEventClick;
        event.wParam = wParam;
        event.lParam = lParam;
        event.ptMouse = pt;
        event.wKeyState = (WORD)wParam;
        m_pEventClick->Event(event);
        return 0;
    }

    LRESULT CDuiPaintManager::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        if(CARET_TIMERID == LOWORD(wParam))
        {
            Invalidate(m_rtCaret);
            m_bCaretActive = !m_bCaretActive;
        }
        else
        {
            for(int i = 0; i < m_aTimers.GetSize(); i++)
            {
                const CDuiTimerInfo* pTimer = static_cast<CDuiTimerInfo*>(m_aTimers[i]);
                if(pTimer->hWnd == m_hWndPaint &&
                        pTimer->uWinTimer == LOWORD(wParam) &&
                        pTimer->bKilled == FALSE)
                {
                    CDuiEvent event;
                    event.Type = DUIEVENT_TIMER;
                    event.pSender = pTimer->pSender;
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
            CDuiEvent event;
            event.Type = DUIEVENT_MOUSEHOVER;
            event.pSender = m_pEventHover;
            event.wParam = wParam;
            event.lParam = lParam;
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
            if(!_TrackMouseEvent(&tme))
            {
                DUI_TRACE("this[0x%p] _TrackMouseEvent[FALSE]", this);
            }
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

        // 拖拽事件
        if(bNeedDrag && m_bDragMode && wParam == MK_LBUTTON)
        {
            ::ReleaseCapture();
            CIDropSource* pdsrc = new CIDropSource;
            if(pdsrc == NULL)
            {
                return 0;
            }
            pdsrc->AddRef();

            CIDataObject* pdobj = new CIDataObject(pdsrc);
            if(pdobj == NULL)
            {
                return 0;
            }
            pdobj->AddRef();

            FORMATETC fmtetc = {0};
            STGMEDIUM medium = {0};
            fmtetc.dwAspect = DVASPECT_CONTENT;
            fmtetc.lindex = -1;
            //////////////////////////////////////
            fmtetc.cfFormat = CF_BITMAP;
            fmtetc.tymed = TYMED_GDI;
            medium.tymed = TYMED_GDI;
            HBITMAP hBitmap = (HBITMAP)OleDuplicateData(m_hDragBitmap, fmtetc.cfFormat, NULL);
            medium.hBitmap = hBitmap;
            pdobj->SetData(&fmtetc, &medium, FALSE);
            //////////////////////////////////////
            BITMAP bmap;
            GetObject(hBitmap, sizeof(BITMAP), &bmap);
            RECT rc = {0, 0, bmap.bmWidth, bmap.bmHeight};
            fmtetc.cfFormat = CF_ENHMETAFILE;
            fmtetc.tymed = TYMED_ENHMF;
            HDC hMetaDC = CreateEnhMetaFile(m_hDcPaint, NULL, NULL, NULL);
            HDC hdcMem = CreateCompatibleDC(m_hDcPaint);
            HGDIOBJ hOldBmp = ::SelectObject(hdcMem, hBitmap);
            ::BitBlt(hMetaDC, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);
            ::SelectObject(hdcMem, hOldBmp);
            medium.hEnhMetaFile = CloseEnhMetaFile(hMetaDC);
            DeleteDC(hdcMem);
            medium.tymed = TYMED_ENHMF;
            pdobj->SetData(&fmtetc, &medium, TRUE);
            //////////////////////////////////////
            CDragSourceHelper dragSrcHelper;
            POINT ptDrag = {0};
            ptDrag.x = bmap.bmWidth / 2;
            ptDrag.y = bmap.bmHeight / 2;
            dragSrcHelper.InitializeFromBitmap(hBitmap, ptDrag, rc, pdobj); //will own the bmp
            DWORD dwEffect;
            HRESULT hr = ::DoDragDrop(pdobj, pdsrc, DROPEFFECT_COPY | DROPEFFECT_MOVE, &dwEffect);
            pdsrc->Release();
            pdobj->Release();
            m_bDragMode = FALSE;
            return 0;
        }
        CDuiEvent event;
        event.ptMouse = pt;
        event.wParam = wParam;
        event.lParam = lParam;
        if(pControl != m_pEventHover && m_pEventHover != NULL)
        {
            event.Type = DUIEVENT_MOUSELEAVE;
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
            event.Type = DUIEVENT_MOUSEENTER;
            event.pSender = pControl;
            pControl->Event(event);
            m_pEventHover = pControl;
        }
        if(m_pEventClick != NULL)
        {
            event.Type = DUIEVENT_MOUSEMOVE;
            event.pSender = m_pEventClick;
            m_pEventClick->Event(event);
        }
        else if(pControl != NULL)
        {
            event.Type = DUIEVENT_MOUSEMOVE;
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
        CDuiEvent event;
        event.Type = DUIEVENT_KEYDOWN;
        event.pSender = m_pFocus;
        event.wParam = wParam;
        event.lParam = lParam;
        event.chKey = (TCHAR)wParam;
        event.ptMouse = m_ptLastMousePos;
        event.wKeyState = MapKeyState();
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
        CDuiEvent event;
        event.Type = DUIEVENT_KEYUP;
        event.pSender = m_pEventKey;
        event.wParam = wParam;
        event.lParam = lParam;
        event.chKey = (TCHAR)wParam;
        event.ptMouse = m_ptLastMousePos;
        event.wKeyState = MapKeyState();
        m_pEventKey->Event(event);
        m_pEventKey = NULL;
        return 0;
    }

    LRESULT CDuiPaintManager::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
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
        CDuiEvent event;
        event.Type = DUIEVENT_SCROLLWHEEL;
        event.pSender = pControl;
        event.wParam = MAKELPARAM(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0);
        event.lParam = lParam;
        event.ptMouse = pt;
        event.wKeyState = MapKeyState();
        pControl->Event(event);

        // Let's make sure that the scroll item below the cursor is the same as before...
        ::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, (LPARAM) MAKELPARAM(m_ptLastMousePos.x, m_ptLastMousePos.y));
        bHandled = FALSE;
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
        CDuiEvent event;
        event.Type = DUIEVENT_SETCURSOR;
        event.pSender = pControl;
        event.wParam = wParam;
        event.lParam = lParam;
        event.ptMouse = pt;
        event.wKeyState = MapKeyState();
        pControl->Event(event);
        return 0;
    }

    LRESULT CDuiPaintManager::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(m_pFocus != NULL)
        {
            CDuiEvent event;
            event.Type = DUIEVENT_SETFOCUS;
            event.wParam = wParam;
            event.lParam = lParam;
            event.pSender = m_pFocus;
            m_pFocus->Event(event);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiPaintManager::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LRESULT lRes = 0;
        if(lParam == 0)
        {
            bHandled = FALSE;
            return lRes;
        }
        LPNMHDR lpNMHDR = (LPNMHDR) lParam;
        if(lpNMHDR != NULL)
        {
            lRes = ::SendMessage(lpNMHDR->hwndFrom, OCM__BASE + uMsg, wParam, lParam);
        }
        return lRes;
    }

    LRESULT CDuiPaintManager::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LRESULT lRes = 0;
        if(lParam == 0)
        {
            bHandled = FALSE;
            return lRes;
        }
        HWND hWndChild = (HWND) lParam;
        lRes = ::SendMessage(hWndChild, OCM__BASE + uMsg, wParam, lParam);
        if(lRes != 0)
        {
            return lRes;
        }
        bHandled = FALSE;
        return lRes;
    }

    LRESULT CDuiPaintManager::OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LRESULT lRes = 0;
        if(lParam == 0)
        {
            bHandled = FALSE;
            return lRes;
        }
        HWND hWndChild = (HWND) lParam;
        lRes = ::SendMessage(hWndChild, OCM__BASE + uMsg, wParam, lParam);
        if(lRes != 0)
        {
            return lRes;
        }
        bHandled = FALSE;
        return lRes;
    }

    BOOL CDuiPaintManager::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes)
    {
        if(m_hWndPaint == NULL)
        {
            return FALSE;
        }
        BOOL bHandled = FALSE;
        LRESULT lResult = 0;
        for(int i = 0; i < m_aMessageFilters.GetSize(); i++)
        {
            bHandled = FALSE;
            lResult = static_cast<IMessageFilter*>(m_aMessageFilters[i])->MessageHandler(uMsg, wParam, lParam, bHandled);
            if(bHandled)
            {
                lRes = lResult;
                return TRUE;
            }
        }

        bHandled = TRUE;
        lResult = 0;
        switch(uMsg)
        {
            case WM_APP + 1:
                lResult = OnDuiNotify(uMsg, wParam, lParam, bHandled);
                break;
            case WM_CLOSE:
                lResult = OnClose(uMsg, wParam, lParam, bHandled);
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
            case WM_LBUTTONDBLCLK:
                lResult = OnLButtonDblClk(uMsg, wParam, lParam, bHandled);
                break;
            case WM_LBUTTONUP:
                lResult = OnLButtonUp(uMsg, wParam, lParam, bHandled);
                break;
            case WM_RBUTTONDOWN:
                lResult = OnRButtonDown(uMsg, wParam, lParam, bHandled);
                break;
            case WM_RBUTTONUP:
                lResult = OnRButtonUp(uMsg, wParam, lParam, bHandled);
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
            case WM_SETFOCUS:
                lResult = OnSetFocus(uMsg, wParam, lParam, bHandled);
                break;
            case WM_NOTIFY:
                lResult = OnNotify(uMsg, wParam, lParam, bHandled);
                break;
            case WM_COMMAND:
                lResult = OnCommand(uMsg, wParam, lParam, bHandled);
                break;
            case WM_CTLCOLOREDIT:
            case WM_CTLCOLORSTATIC:
                lResult = OnCtlColor(uMsg, wParam, lParam, bHandled);
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

    BOOL CDuiPaintManager::Initialize(HINSTANCE hInstance)
    {
        ASSERT(hInstance);
        ::CoInitialize(NULL);
        ::OleInitialize(NULL);

        InitCommonControls();

        if(hInstance == NULL)
        {
            return FALSE;
        }
        m_hInstance = hInstance;
        m_dwMainThreadID = GetCurrentThreadId();
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

        CDuiImageInfo* data = NULL;
        for(int i = 0; i < m_SharedResInfo.m_ImageHash.GetSize(); i++)
        {
            if(LPCTSTR key = m_SharedResInfo.m_ImageHash.GetAt(i))
            {
                data = static_cast<CDuiImageInfo*>(m_SharedResInfo.m_ImageHash.Find(key, FALSE));
                if(data)
                {
                    CRenderEngine::FreeImage(data);
                }
            }
        }
        m_SharedResInfo.m_ImageHash.RemoveAll();

        CDuiFontInfo* pFontInfo = NULL;
        for(int i = 0; i < m_SharedResInfo.m_CustomFonts.GetSize(); i++)
        {
            if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i))
            {
                pFontInfo = static_cast<CDuiFontInfo*>(m_SharedResInfo.m_CustomFonts.Find(key, FALSE));
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
        CDuiResourceManager::GetInstance()->Release();
        if(m_hMsimg32Module != NULL)
        {
            FreeModule(m_hMsimg32Module);
            m_hMsimg32Module = NULL;
        }
        if(m_bCachedResourceZip && m_hResourceZip != NULL)
        {
            CloseZip((HZIP)m_hResourceZip);
            m_hResourceZip = NULL;
        }
        ::OleUninitialize();
        ::CoUninitialize();
    }

    HINSTANCE CDuiPaintManager::GetInstance()
    {
        return m_hInstance;
    }

    DWORD CDuiPaintManager::GetMainThreadID()
    {
        return m_dwMainThreadID;
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

    void CDuiPaintManager::SetResourceType(int iResourceType)
    {
        m_iResourceType = iResourceType;
    }

    const CDuiString& CDuiPaintManager::GetResourcePath()
    {
        return m_strResourcePath;
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

    const CDuiString& CDuiPaintManager::GetResourceZipPwd()
    {
        return m_pStrResourceZipPwd;
    }

    const CDuiString& CDuiPaintManager::GetResourceZip()
    {
        return m_strResourceZip;
    }

    void CDuiPaintManager::SetResourceZip(LPVOID pVoid, unsigned int len, LPCTSTR password)
    {
        if(m_strResourceZip == _T("membuffer"))
        {
            return;
        }
        if(m_bCachedResourceZip && m_hResourceZip != NULL)
        {
            CloseZip((HZIP)m_hResourceZip);
            m_hResourceZip = NULL;
        }
        m_strResourceZip = _T("membuffer");
        m_bCachedResourceZip = TRUE;
        m_pStrResourceZipPwd = password;  //Garfield 20160325 带密码zip包解密
        if(m_bCachedResourceZip)
        {
#ifdef UNICODE
            char* pwd = w2a((wchar_t*)password);
            m_hResourceZip = (HANDLE)OpenZip(pVoid, len, pwd);
            DUI_FREE_ARRAY(pwd);
#else
            m_hResourceZip = (HANDLE)OpenZip(pVoid, len, password);
#endif
            if(m_hResourceZip == NULL)
            {
                DUI_ERROR("OpenZip Failed");
            }
        }
    }

    void CDuiPaintManager::SetResourceZip(LPCTSTR pstrZip, BOOL bCachedResourceZip /*= FALSE*/, LPCTSTR password /*= NULL*/)
    {
        if(m_strResourceZip == pstrZip && m_bCachedResourceZip == bCachedResourceZip)
        {
            return;
        }
        if(m_bCachedResourceZip && m_hResourceZip != NULL)
        {
            CloseZip((HZIP)m_hResourceZip);
            m_hResourceZip = NULL;
        }
        m_strResourceZip = pstrZip;
        m_bCachedResourceZip = bCachedResourceZip;
        m_pStrResourceZipPwd = password;
        if(m_bCachedResourceZip)
        {
            CDuiString sFile = CDuiPaintManager::GetResourcePath();
            sFile += CDuiPaintManager::GetResourceZip();
#ifdef UNICODE
            char* pwd = w2a((wchar_t*)password);
            m_hResourceZip = (HANDLE)OpenZip(sFile.GetData(), pwd);
            DUI_FREE_ARRAY(pwd);
#else
            m_hResourceZip = (HANDLE)OpenZip(sFile.GetData(), password);
#endif
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
        *H = m_Hue;
        *S = m_Saturation;
        *L = m_Lightness;
        return m_bUseHSL;
    }

    UINT CDuiPaintManager::MapKeyState()
    {
        UINT uKeyState = 0;
        if(::GetKeyState(VK_CONTROL) < 0)
        {
            uKeyState |= MK_CONTROL;
        }
        if(::GetKeyState(VK_RBUTTON) < 0)
        {
            uKeyState |= MK_LBUTTON;
        }
        if(::GetKeyState(VK_LBUTTON) < 0)
        {
            uKeyState |= MK_RBUTTON;
        }
        if(::GetKeyState(VK_SHIFT) < 0)
        {
            uKeyState |= MK_SHIFT;
        }
        if(::GetKeyState(VK_MENU) < 0)
        {
            uKeyState |= MK_ALT;
        }
        return uKeyState;
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

    BOOL CDuiPaintManager::IsIdleMessage(const LPMSG pMsg)
    {
        // These messages should NOT cause idle processing
        switch(pMsg->message)
        {
            case WM_MOUSEMOVE:
#ifndef _WIN32_WCE
            case WM_NCMOUSEMOVE:
#endif // !_WIN32_WCE
            case WM_PAINT:
            case 0x0118:	// WM_SYSTIMER (caret blink)
                return FALSE;
        }

        return TRUE;
    }

    int CDuiPaintManager::MessageLoop()
    {
        MSG msg = { 0 };
        BOOL bDoIdle = TRUE;
        BOOL bRet = FALSE;
        for(;;)
        {
            while(!::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
            {
                bDoIdle = FALSE;
            }
            bRet = ::GetMessage(&msg, NULL, 0, 0);

            if(bRet == -1)
            {
                DUI_ERROR("::GetMessage returned -1 (error)");
                continue;   // error, don't process
            }
            else if(!bRet)
            {
                DUI_ERROR("::GetMessage - exiting");
                break;   // WM_QUIT, exit message loop
            }
            if(!CDuiPaintManager::TranslateMessage(&msg))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            if(IsIdleMessage(&msg))
            {
                bDoIdle = TRUE;
            }
        }
        return (int)msg.wParam;
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
                    return FALSE;
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
        CDuiTabInfo* pInfo = static_cast<CDuiTabInfo*>(pData);
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

    CDuiControl* CALLBACK CDuiPaintManager::__FindControlFromShortcut(CDuiControl* pThis, LPVOID pData)
    {
        if(!pThis->IsVisible())
        {
            return NULL;
        }
        CShortCut* pFS = static_cast<CShortCut*>(pData);
        if(pFS->m_ch == toupper(pThis->GetShortcut()))
        {
            pFS->m_bPickNext = TRUE;
        }
        if(pThis->GetClass() == DUI_CTR_LABEL)
        {
            return NULL;    // Labels never get focus!
        }
        return pFS->m_bPickNext ? pThis : NULL;
    }

    CDuiControl* CALLBACK CDuiPaintManager::__FindControlFromName(CDuiControl* pThis, LPVOID pData)
    {
        LPCTSTR pstrName = static_cast<LPCTSTR>(pData);
        const CDuiString& sName = pThis->GetName();
        if(sName.IsEmpty())
        {
            return NULL;
        }
        return (_tcsicmp(sName, pstrName) == 0) ? pThis : NULL;
    }

}