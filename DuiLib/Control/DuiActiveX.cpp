#include "StdAfx.h"
#include "DuiActiveX.h"

namespace DuiLib
{
    ///////////////////////////////////////////////////////////////////////
    CActiveXWnd::CActiveXWnd()
        : m_bDrawCaret(FALSE)
        , m_iLayeredTick(0)
        , m_pOwner(NULL)
    {

    }

    CActiveXWnd::~CActiveXWnd()
    {

    }

    HWND CActiveXWnd::Init(CActiveXCtrl* pOwner, HWND hWndParent)
    {
        m_pOwner = pOwner;
        return Create(hWndParent, _T("CDUIActiveX"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0L, 0, 0, 0, 0, NULL);
    }

    LPCTSTR CActiveXWnd::GetWindowClassName() const
    {
        return GET_CLASS_NAME(CActiveXWnd);
    }

    void CActiveXWnd::OnFinalMessage(HWND hWnd)
    {
        if(m_pOwner->GetOwner()->GetManager()->IsLayered())
        {
            m_pOwner->GetOwner()->GetManager()->RemovePaintChildWnd(hWnd);
        }
    }

    LRESULT CActiveXWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch(uMsg)
        {
            case WM_CREATE:
                lRes = OnCreate(uMsg, wParam, lParam, bHandled);
                break;
            case WM_TIMER:
                lRes = OnTimer(uMsg, wParam, lParam, bHandled);
                break;
            case WM_PAINT:
                lRes = OnPaint(uMsg, wParam, lParam, bHandled);
                break;
            case WM_PRINT:
                lRes = OnPrint(uMsg, wParam, lParam, bHandled);
                break;
            case WM_SETFOCUS:
                lRes = OnSetFocus(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KILLFOCUS:
                lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
                break;
            case WM_ERASEBKGND:
                lRes = OnEraseBkgnd(uMsg, wParam, lParam, bHandled);
                break;
            case WM_MOUSEACTIVATE:
                lRes = OnMouseActivate(uMsg, wParam, lParam, bHandled);
                break;
            case WM_MOUSEWHEEL:
                break;
            default:
                bHandled = FALSE;
        }
        if(!bHandled)
        {
            return CDuiWnd::HandleMessage(uMsg, wParam, lParam);
        }
        return lRes;
    }

    void CActiveXWnd::DoVerb(LONG iVerb)
    {
        if(m_pOwner == NULL)
        {
            return;
        }
        if(m_pOwner->GetOwner() == NULL)
        {
            return;
        }
        IOleObject* pUnk = NULL;
        m_pOwner->GetOwner()->GetControl(IID_IOleObject, (LPVOID*) &pUnk);
        if(pUnk == NULL)
        {
            return;
        }
        CSafeRelease<IOleObject> RefOleObject = pUnk;
        IOleClientSite* pOleClientSite = NULL;
        m_pOwner->QueryInterface(IID_IOleClientSite, (LPVOID*) &pOleClientSite);
        CSafeRelease<IOleClientSite> RefOleClientSite = pOleClientSite;
        pUnk->DoVerb(iVerb, NULL, pOleClientSite, 0, GetHWND(), &m_pOwner->GetOwner()->GetPos());
    }

    LRESULT CActiveXWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(m_pOwner->GetOwner()->GetManager()->IsLayered())
        {
            ::SetTimer(GetHWND(), CARET_TIMERID, ::GetCaretBlinkTime(), NULL);
        }
        return 0;
    }

    LRESULT CActiveXWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(wParam == CARET_TIMERID)
        {
            if(m_pOwner->GetOwner()->GetManager()->IsLayered())
            {
                m_pOwner->GetOwner()->GetManager()->AddPaintChildWnd(GetHWND());
                m_iLayeredTick += 1;
                if(m_iLayeredTick >= 10)
                {
                    m_iLayeredTick = 0;
                    m_bDrawCaret = !m_bDrawCaret;
                }
            }
            return 0;
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CActiveXWnd::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        IOleObject* pUnk = NULL;
        m_pOwner->GetOwner()->GetControl(IID_IOleObject, (LPVOID*) &pUnk);
        if(pUnk == NULL)
        {
            return 0;
        }
        CSafeRelease<IOleObject> RefOleObject = pUnk;
        DWORD dwMiscStatus = 0;
        pUnk->GetMiscStatus(DVASPECT_CONTENT, &dwMiscStatus);
        if((dwMiscStatus & OLEMISC_NOUIACTIVATE) != 0)
        {
            return 0;
        }
        if(!m_pOwner->IsInPlaceActive())
        {
            DoVerb(OLEIVERB_INPLACEACTIVATE);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CActiveXWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        m_pOwner->SetFocused(TRUE);
        if(!m_pOwner->IsUIActivated())
        {
            DoVerb(OLEIVERB_UIACTIVATE);
        }
        return 0;
    }

    LRESULT CActiveXWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        m_pOwner->SetFocused(FALSE);
        return 0;
    }

    LRESULT CActiveXWnd::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(m_pOwner->GetViewObject() == NULL)
        {
            bHandled = FALSE;
        }
        return 1;
    }

    LRESULT CActiveXWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        PAINTSTRUCT ps = { 0 };
        ::BeginPaint(GetHWND(), &ps);
        ::EndPaint(GetHWND(), &ps);
        return 1;
    }

    LRESULT CActiveXWnd::OnPrint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        RECT rcClient;
        ::GetClientRect(GetHWND(), &rcClient);
        m_pOwner->GetViewObject()->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, (HDC)wParam, (RECTL*) &rcClient, NULL, NULL, NULL);

        if(m_bDrawCaret)
        {
            RECT rcPos = m_pOwner->GetOwner()->GetPos();
            GUITHREADINFO guiThreadInfo;
            guiThreadInfo.cbSize = sizeof(GUITHREADINFO);
            ::GetGUIThreadInfo(NULL, &guiThreadInfo);
            if(guiThreadInfo.hwndCaret)
            {
                POINT ptCaret;
                ptCaret.x = guiThreadInfo.rcCaret.left;
                ptCaret.y = guiThreadInfo.rcCaret.top;
                ::ClientToScreen(guiThreadInfo.hwndCaret, &ptCaret);
                ::ScreenToClient(m_pOwner->GetOwner()->GetManager()->GetPaintWindow(), &ptCaret);
                if(::PtInRect(&rcPos, ptCaret))
                {
                    RECT rcCaret;
                    rcCaret = guiThreadInfo.rcCaret;
                    rcCaret.right = rcCaret.left;
                    CRenderEngine::DrawLine((HDC)wParam, rcCaret, 1, 0xFF000000);
                }
            }
        }

        return 1;
    }
    //////////////////////////////////////////////////////////////////////////////
    CActiveXEnum::CActiveXEnum(IUnknown* pUnk)
        : m_iPos(0)
        , m_dwRef(1)
        , m_pUnk(pUnk)
    {
        m_pUnk->AddRef();
    }


    CActiveXEnum::~CActiveXEnum()
    {
        m_pUnk->Release();
    }

    STDMETHODIMP_(ULONG) CActiveXEnum::AddRef()
    {
        return ++m_dwRef;
    }

    STDMETHODIMP_(ULONG) CActiveXEnum::Release()
    {
        LONG lRef = --m_dwRef;
        if(lRef == 0)
        {
            delete this;
        }
        return lRef;
    }

    STDMETHODIMP CActiveXEnum::QueryInterface(REFIID riid, LPVOID* ppvObject)
    {
        *ppvObject = NULL;
        if(riid == IID_IUnknown)
        {
            *ppvObject = static_cast<IEnumUnknown*>(this);
        }
        else if(riid == IID_IEnumUnknown)
        {
            *ppvObject = static_cast<IEnumUnknown*>(this);
        }
        if(*ppvObject != NULL)
        {
            AddRef();
        }
        return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
    }

    STDMETHODIMP CActiveXEnum::Next(ULONG celt, IUnknown** rgelt, ULONG* pceltFetched)
    {
        if(pceltFetched != NULL)
        {
            *pceltFetched = 0;
        }
        if(++m_iPos > 1)
        {
            return S_FALSE;
        }
        *rgelt = m_pUnk;
        (*rgelt)->AddRef();
        if(pceltFetched != NULL)
        {
            *pceltFetched = 1;
        }
        return S_OK;
    }

    STDMETHODIMP CActiveXEnum::Skip(ULONG celt)
    {
        m_iPos += celt;
        return S_OK;
    }

    STDMETHODIMP CActiveXEnum::Reset(void)
    {
        m_iPos = 0;
        return S_OK;
    }

    STDMETHODIMP CActiveXEnum::Clone(IEnumUnknown** ppenum)
    {
        return E_NOTIMPL;
    }

    //////////////////////////////////////////////////////////////////////////
    CActiveXCtrl::CActiveXCtrl()
        : m_bLocked(FALSE)
        , m_bFocused(FALSE)
        , m_bCaptured(FALSE)
        , m_bUIActivated(FALSE)
        , m_bInPlaceActive(FALSE)
        , m_bWindowless(TRUE)
        , m_dwRef(1)
        , m_pOwner(NULL)
        , m_pWindow(NULL)
        , m_pUnkSite(NULL)
        , m_pViewObject(NULL)
        , m_pInPlaceObject(NULL)
    {

    }

    CActiveXCtrl::~CActiveXCtrl()
    {
        if(m_pWindow != NULL)
        {
            ::DestroyWindow(*m_pWindow);
            DUI_FREE_POINT(m_pWindow);
        }
        if(m_pUnkSite != NULL)
        {
            m_pUnkSite->Release();
        }
        if(m_pViewObject != NULL)
        {
            m_pViewObject->Release();
        }
        if(m_pInPlaceObject != NULL)
        {
            m_pInPlaceObject->Release();
        }
    }

    CDuiActiveX* CActiveXCtrl::GetOwner() const
    {
        return m_pOwner;
    }

    void CActiveXCtrl::SetOwner(CDuiActiveX* pOwner)
    {
        m_pOwner = pOwner;
    }

    BOOL CActiveXCtrl::IsCaptured() const
    {
        return m_bCaptured;
    }

    BOOL CActiveXCtrl::IsWindowless() const
    {
        return m_bWindowless;
    }

    CActiveXWnd* CActiveXCtrl::GetWnd() const
    {
        return m_pWindow;
    }

    IOleInPlaceObjectWindowless* CActiveXCtrl::GetInPlaceObject() const
    {
        return m_pInPlaceObject;
    }

    BOOL CActiveXCtrl::IsInPlaceActive() const
    {
        return m_bInPlaceActive;
    }

    void CActiveXCtrl::SetInPlaceActive(BOOL bInPlaceActive)
    {
        m_bInPlaceActive = bInPlaceActive;
    }

    BOOL CActiveXCtrl::IsFocused() const
    {
        return m_bFocused;
    }

    void CActiveXCtrl::SetFocused(BOOL bFocused)
    {
        m_bFocused = bFocused;
    }
    BOOL CActiveXCtrl::IsUIActivated() const
    {
        return m_bUIActivated;
    }

    void CActiveXCtrl::SetUIActivated(BOOL bUIActivated)
    {
        m_bUIActivated = bUIActivated;
    }

    IViewObject* CActiveXCtrl::GetViewObject() const
    {
        return m_pViewObject;
    }

    void CActiveXCtrl::SetViewObject(IViewObject* pViewObject)
    {
        m_pViewObject = pViewObject;
    }

    STDMETHODIMP_(ULONG) CActiveXCtrl::AddRef()
    {
        return ++m_dwRef;
    }

    STDMETHODIMP_(ULONG) CActiveXCtrl::Release()
    {
        LONG lRef = --m_dwRef;
        if(lRef == 0)
        {
            delete this;
        }
        return lRef;
    }

    STDMETHODIMP CActiveXCtrl::QueryInterface(REFIID riid, LPVOID* ppvObject)
    {
        *ppvObject = NULL;
        if(riid == IID_IUnknown)
        {
            *ppvObject = static_cast<IOleWindow*>(this);
        }
        else if(riid == IID_IOleClientSite)
        {
            *ppvObject = static_cast<IOleClientSite*>(this);
        }
        else if(riid == IID_IOleInPlaceSiteWindowless)
        {
            *ppvObject = static_cast<IOleInPlaceSiteWindowless*>(this);
        }
        else if(riid == IID_IOleInPlaceSiteEx)
        {
            *ppvObject = static_cast<IOleInPlaceSiteEx*>(this);
        }
        else if(riid == IID_IOleInPlaceSite)
        {
            *ppvObject = static_cast<IOleInPlaceSite*>(this);
        }
        else if(riid == IID_IOleWindow)
        {
            *ppvObject = static_cast<IOleWindow*>(this);
        }
        else if(riid == IID_IOleControlSite)
        {
            *ppvObject = static_cast<IOleControlSite*>(this);
        }
        else if(riid == IID_IOleContainer)
        {
            *ppvObject = static_cast<IOleContainer*>(this);
        }
        else if(riid == IID_IObjectWithSite)
        {
            *ppvObject = static_cast<IObjectWithSite*>(this);
        }
        if(*ppvObject != NULL)
        {
            AddRef();
        }
        return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
    }

    STDMETHODIMP CActiveXCtrl::SetSite(IUnknown* pUnkSite)
    {
        DUI_TRACE("AX: CActiveXCtrl::SetSite");
        if(m_pUnkSite != NULL)
        {
            m_pUnkSite->Release();
            m_pUnkSite = NULL;
        }
        if(pUnkSite != NULL)
        {
            m_pUnkSite = pUnkSite;
            m_pUnkSite->AddRef();
        }
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::GetSite(REFIID riid, LPVOID* ppvSite)
    {
        DUI_TRACE("AX: CActiveXCtrl::GetSite");
        if(ppvSite == NULL)
        {
            return E_POINTER;
        }
        *ppvSite = NULL;
        if(m_pUnkSite == NULL)
        {
            return E_FAIL;
        }
        return m_pUnkSite->QueryInterface(riid, ppvSite);
    }

    STDMETHODIMP CActiveXCtrl::SaveObject(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::SaveObject");
        return E_NOTIMPL;
    }

    STDMETHODIMP CActiveXCtrl::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk)
    {
        DUI_TRACE("AX: CActiveXCtrl::GetMoniker");
        if(ppmk != NULL)
        {
            *ppmk = NULL;
        }
        return E_NOTIMPL;
    }

    STDMETHODIMP CActiveXCtrl::GetContainer(IOleContainer** ppContainer)
    {
        DUI_TRACE("AX: CActiveXCtrl::GetContainer");
        if(ppContainer == NULL)
        {
            return E_POINTER;
        }
        *ppContainer = NULL;
        HRESULT Hr = E_NOTIMPL;
        if(m_pUnkSite != NULL)
        {
            Hr = m_pUnkSite->QueryInterface(IID_IOleContainer, (LPVOID*) ppContainer);
        }
        if(FAILED(Hr))
        {
            Hr = QueryInterface(IID_IOleContainer, (LPVOID*) ppContainer);
        }
        return Hr;
    }

    STDMETHODIMP CActiveXCtrl::ShowObject(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::ShowObject");
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        HDC hDC = ::GetDC(m_pOwner->GetHostWindow());
        if(hDC == NULL)
        {
            return E_FAIL;
        }
        if(m_pViewObject != NULL)
        {
            m_pViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hDC, (RECTL*) &m_pOwner->GetPos(), (RECTL*) &m_pOwner->GetPos(), NULL, NULL);
        }
        ::ReleaseDC(m_pOwner->GetHostWindow(), hDC);
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::OnShowWindow(BOOL fShow)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnShowWindow");
        return E_NOTIMPL;
    }

    STDMETHODIMP CActiveXCtrl::RequestNewObjectLayout(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::RequestNewObjectLayout");
        return E_NOTIMPL;
    }

    STDMETHODIMP CActiveXCtrl::CanWindowlessActivate(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::CanWindowlessActivate");
        return S_OK;  // Yes, we can!!
    }

    STDMETHODIMP CActiveXCtrl::GetCapture(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::GetCapture");
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        return m_bCaptured ? S_OK : S_FALSE;
    }

    STDMETHODIMP CActiveXCtrl::SetCapture(BOOL fCapture)
    {
        DUI_TRACE("AX: CActiveXCtrl::SetCapture");
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        m_bCaptured = (fCapture == TRUE);
        if(fCapture)
        {
            ::SetCapture(m_pOwner->GetHostWindow());
        }
        else
        {
            ::ReleaseCapture();
        }
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::GetFocus(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::GetFocus");
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        return m_bFocused ? S_OK : S_FALSE;
    }

    STDMETHODIMP CActiveXCtrl::SetFocus(BOOL fFocus)
    {
        DUI_TRACE("AX: CActiveXCtrl::SetFocus");
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        if(fFocus)
        {
            m_pOwner->SetFocus();
        }
        m_bFocused = (fFocus == TRUE);
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::GetDC(LPCRECT pRect, DWORD grfFlags, HDC* phDC)
    {
        DUI_TRACE("AX: CActiveXCtrl::GetDC");
        if(phDC == NULL)
        {
            return E_POINTER;
        }
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        if(m_bWindowless)
        {
            return S_FALSE;
        }
        *phDC = ::GetDC(m_pOwner->GetHostWindow());
        if((grfFlags & OLEDC_PAINTBKGND) != 0)
        {
            CDuiRect rcItem = m_pOwner->GetPos();
            if(!m_bWindowless)
            {
                rcItem.ResetOffset();
            }
            ::FillRect(*phDC, &rcItem, (HBRUSH)(COLOR_WINDOW + 1));
        }
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::ReleaseDC(HDC hDC)
    {
        DUI_TRACE("AX: CActiveXCtrl::ReleaseDC");
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        ::ReleaseDC(m_pOwner->GetHostWindow(), hDC);
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::InvalidateRect(LPCRECT pRect, BOOL fErase)
    {
        DUI_TRACE("AX: CActiveXCtrl::InvalidateRect");
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        if(m_pOwner->GetHostWindow() == NULL)
        {
            return E_FAIL;
        }
        return ::InvalidateRect(m_pOwner->GetHostWindow(), pRect, fErase) ? S_OK : E_FAIL;
    }

    STDMETHODIMP CActiveXCtrl::InvalidateRgn(HRGN hRGN, BOOL fErase)
    {
        DUI_TRACE("AX: CActiveXCtrl::InvalidateRgn");
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        return ::InvalidateRgn(m_pOwner->GetHostWindow(), hRGN, fErase) ? S_OK : E_FAIL;
    }

    STDMETHODIMP CActiveXCtrl::ScrollRect(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip)
    {
        DUI_TRACE("AX: CActiveXCtrl::ScrollRect");
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::AdjustRect(LPRECT prc)
    {
        DUI_TRACE("AX: CActiveXCtrl::AdjustRect");
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::OnDefWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnDefWindowMessage");
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        *plResult = ::DefWindowProc(m_pOwner->GetHostWindow(), msg, wParam, lParam);
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::OnInPlaceActivateEx(BOOL* pfNoRedraw, DWORD dwFlags)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnInPlaceActivateEx");
        ASSERT(m_pInPlaceObject == NULL);
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        if(m_pOwner->GetOleObject() == NULL)
        {
            return E_UNEXPECTED;
        }
        ::OleLockRunning(m_pOwner->GetOleObject(), TRUE, FALSE);
        HWND hWndFrame = m_pOwner->GetManager()->GetPaintWindow();
        HRESULT Hr = E_FAIL;
        if((dwFlags & ACTIVATE_WINDOWLESS) != 0)
        {
            m_bWindowless = true;
            Hr = m_pOwner->GetOleObject()->QueryInterface(IID_IOleInPlaceObjectWindowless, (LPVOID*) &m_pInPlaceObject);
            m_pOwner->SetHostWindow(hWndFrame);
            m_pOwner->GetManager()->AddMessageFilter(m_pOwner);
        }
        if(FAILED(Hr))
        {
            m_bWindowless = false;
            Hr = CreateActiveXWnd();
            if(FAILED(Hr))
            {
                return Hr;
            }
            Hr = m_pOwner->GetOleObject()->QueryInterface(IID_IOleInPlaceObject, (LPVOID*) &m_pInPlaceObject);
        }
        if(m_pInPlaceObject != NULL && !m_pOwner->IsMFC())
        {
            CDuiRect rcItem = m_pOwner->GetPos();
            if(!m_bWindowless)
            {
                rcItem.ResetOffset();
            }
            m_pInPlaceObject->SetObjectRects(&rcItem, &rcItem);
        }
        m_bInPlaceActive = SUCCEEDED(Hr);
        return Hr;
    }

    STDMETHODIMP CActiveXCtrl::OnInPlaceDeactivateEx(BOOL fNoRedraw)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnInPlaceDeactivateEx");
        m_bInPlaceActive = false;
        if(m_pInPlaceObject != NULL)
        {
            m_pInPlaceObject->Release();
            m_pInPlaceObject = NULL;
        }
        if(m_pWindow != NULL)
        {
            ::DestroyWindow(*m_pWindow);
            DUI_FREE_POINT(m_pWindow);
        }
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::RequestUIActivate(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::RequestUIActivate");
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::CanInPlaceActivate(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::CanInPlaceActivate");
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::OnInPlaceActivate(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnInPlaceActivate");
        BOOL bDummy = FALSE;
        return OnInPlaceActivateEx(&bDummy, 0);
    }

    STDMETHODIMP CActiveXCtrl::OnUIActivate(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnUIActivate");
        m_bUIActivated = true;
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::GetWindowContext(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
    {
        DUI_TRACE("AX: CActiveXCtrl::GetWindowContext");
        if(ppDoc == NULL)
        {
            return E_POINTER;
        }
        if(ppFrame == NULL)
        {
            return E_POINTER;
        }
        if(lprcPosRect == NULL)
        {
            return E_POINTER;
        }
        if(lprcClipRect == NULL)
        {
            return E_POINTER;
        }
        if(m_pWindow)
        {
            ::GetClientRect(m_pWindow->GetHWND(), lprcPosRect);
            ::GetClientRect(m_pWindow->GetHWND(), lprcClipRect);
        }
        //*ppFrame = new CActiveXFrameWnd(m_pOwner); ????
        *ppDoc = NULL;
        ACCEL ac = { 0 };
        HACCEL hac = ::CreateAcceleratorTable(&ac, 1);
        lpFrameInfo->cb = sizeof(OLEINPLACEFRAMEINFO);
        lpFrameInfo->fMDIApp = FALSE;
        lpFrameInfo->hwndFrame = m_pOwner->GetManager()->GetPaintWindow();
        lpFrameInfo->haccel = hac;
        lpFrameInfo->cAccelEntries = 1;
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::Scroll(SIZE scrollExtant)
    {
        DUI_TRACE("AX: CActiveXCtrl::Scroll");
        return E_NOTIMPL;
    }

    STDMETHODIMP CActiveXCtrl::OnUIDeactivate(BOOL fUndoable)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnUIDeactivate");
        m_bUIActivated = false;
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::OnInPlaceDeactivate(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnInPlaceDeactivate");
        return OnInPlaceDeactivateEx(TRUE);
    }

    STDMETHODIMP CActiveXCtrl::DiscardUndoState(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::DiscardUndoState");
        return E_NOTIMPL;
    }

    STDMETHODIMP CActiveXCtrl::DeactivateAndUndo(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::DeactivateAndUndo");
        return E_NOTIMPL;
    }

    STDMETHODIMP CActiveXCtrl::OnPosRectChange(LPCRECT lprcPosRect)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnPosRectChange");
        return E_NOTIMPL;
    }

    STDMETHODIMP CActiveXCtrl::GetWindow(HWND* phwnd)
    {
        DUI_TRACE("AX: CActiveXCtrl::GetWindow");
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        if(m_pOwner->GetHostWindow() == NULL)
        {
            CreateActiveXWnd();
        }
        if(m_pOwner->GetHostWindow() == NULL)
        {
            return E_FAIL;
        }
        *phwnd = m_pOwner->GetHostWindow();
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::ContextSensitiveHelp(BOOL fEnterMode)
    {
        DUI_TRACE("AX: CActiveXCtrl::ContextSensitiveHelp");
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::OnControlInfoChanged(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnControlInfoChanged");
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::LockInPlaceActive(BOOL fLock)
    {
        DUI_TRACE("AX: CActiveXCtrl::LockInPlaceActive");
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::GetExtendedControl(IDispatch** ppDisp)
    {
        DUI_TRACE("AX: CActiveXCtrl::GetExtendedControl");
        if(ppDisp == NULL)
        {
            return E_POINTER;
        }
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        if(m_pOwner->GetOleObject() == NULL)
        {
            return E_UNEXPECTED;
        }
        return m_pOwner->GetOleObject()->QueryInterface(IID_IDispatch, (LPVOID*) ppDisp);
    }

    STDMETHODIMP CActiveXCtrl::TransformCoords(POINTL* pPtlHimetric, POINTF* pPtfContainer, DWORD dwFlags)
    {
        DUI_TRACE("AX: CActiveXCtrl::TransformCoords");
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::TranslateAccelerator(MSG* pMsg, DWORD grfModifiers)
    {
        DUI_TRACE("AX: CActiveXCtrl::TranslateAccelerator");
        return S_FALSE;
    }

    STDMETHODIMP CActiveXCtrl::OnFocus(BOOL fGotFocus)
    {
        DUI_TRACE("AX: CActiveXCtrl::OnFocus");
        m_bFocused = (fGotFocus == TRUE);
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::ShowPropertyFrame(void)
    {
        DUI_TRACE("AX: CActiveXCtrl::ShowPropertyFrame");
        return E_NOTIMPL;
    }

    STDMETHODIMP CActiveXCtrl::EnumObjects(DWORD grfFlags, IEnumUnknown** ppenum)
    {
        DUI_TRACE("AX: CActiveXCtrl::EnumObjects");
        if(ppenum == NULL)
        {
            return E_POINTER;
        }
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        *ppenum = new CActiveXEnum(m_pOwner->GetOleObject());
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::LockContainer(BOOL fLock)
    {
        DUI_TRACE("AX: CActiveXCtrl::LockContainer");
        m_bLocked = fLock != FALSE;
        return S_OK;
    }

    STDMETHODIMP CActiveXCtrl::ParseDisplayName(IBindCtx* pbc, LPOLESTR pszDisplayName, ULONG* pchEaten, IMoniker** ppmkOut)
    {
        DUI_TRACE("AX: CActiveXCtrl::ParseDisplayName");
        return E_NOTIMPL;
    }

    HRESULT CActiveXCtrl::CreateActiveXWnd()
    {
        if(m_pWindow != NULL)
        {
            return S_OK;
        }
        m_pWindow = new CActiveXWnd;
        if(m_pWindow == NULL)
        {
            return E_OUTOFMEMORY;
        }
        m_pOwner->SetHostWindow(m_pWindow->Init(this, m_pOwner->GetManager()->GetPaintWindow()));
        return S_OK;
    }

    /////////////////////////////////////////////////////////////
    CActiveXFrameWnd::CActiveXFrameWnd(CDuiActiveX* pOwner)
        : m_dwRef(1)
        , m_pOwner(pOwner)
        , m_pActiveObject(NULL)
    {

    }


    CActiveXFrameWnd::~CActiveXFrameWnd()
    {
        if(m_pActiveObject != NULL)
        {
            m_pActiveObject->Release();
        }
    }

    STDMETHODIMP_(ULONG) CActiveXFrameWnd::AddRef()
    {
        return ++m_dwRef;
    }

    STDMETHODIMP_(ULONG) CActiveXFrameWnd::Release()
    {
        ULONG lRef = --m_dwRef;
        if(lRef == 0)
        {
            delete this;
        }
        return lRef;
    }

    STDMETHODIMP CActiveXFrameWnd::QueryInterface(REFIID riid, LPVOID* ppvObject)
    {
        *ppvObject = NULL;
        if(riid == IID_IUnknown)
        {
            *ppvObject = static_cast<IOleInPlaceFrame*>(this);
        }
        else if(riid == IID_IOleWindow)
        {
            *ppvObject = static_cast<IOleWindow*>(this);
        }
        else if(riid == IID_IOleInPlaceFrame)
        {
            *ppvObject = static_cast<IOleInPlaceFrame*>(this);
        }
        else if(riid == IID_IOleInPlaceUIWindow)
        {
            *ppvObject = static_cast<IOleInPlaceUIWindow*>(this);
        }
        if(*ppvObject != NULL)
        {
            AddRef();
        }
        return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::RemoveMenus(HMENU hmenuShared)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::SetStatusText(LPCOLESTR pszStatusText)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::EnableModeless(BOOL fEnable)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::TranslateAccelerator(LPMSG lpmsg, WORD wID)
    {
        return S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::GetWindow(HWND* phwnd)
    {
        if(m_pOwner == NULL)
        {
            return E_UNEXPECTED;
        }
        *phwnd = m_pOwner->GetManager()->GetPaintWindow();
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::ContextSensitiveHelp(BOOL fEnterMode)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::GetBorder(LPRECT lprectBorder)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::RequestBorderSpace(LPCBORDERWIDTHS pborderwidths)
    {
        return INPLACE_E_NOTOOLSPACE;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::SetBorderSpace(LPCBORDERWIDTHS pborderwidths)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CActiveXFrameWnd::SetActiveObject(IOleInPlaceActiveObject* pActiveObject, LPCOLESTR pszObjName)
    {
        if(pActiveObject != NULL)
        {
            pActiveObject->AddRef();
        }
        if(m_pActiveObject != NULL)
        {
            m_pActiveObject->Release();
        }
        m_pActiveObject = pActiveObject;
        return S_OK;
    }

    //////////////////////////////////////////////////////////////////////


    IMPLEMENT_DUICONTROL(CDuiActiveX)

    CDuiActiveX::CDuiActiveX(void)
        : m_clsid(IID_NULL)
        , m_bMFC(FALSE)
        , m_bCreated(FALSE)
        , m_bDelayCreate(TRUE)
        , m_pUnk(NULL)
        , m_pControl(NULL)
    {
        ::ZeroMemory(&m_hwndHost, sizeof(HWND));
    }


    CDuiActiveX::~CDuiActiveX(void)
    {
        ReleaseControl();
    }


    CDuiString CDuiActiveX::GetClass() const
    {
        return DUI_CTR_ACTIVEX;
    }

    LPVOID CDuiActiveX::GetInterface(LPCTSTR pstrName)
    {
        if(_tcscmp(pstrName, DUI_CTR_ACTIVEX) == 0)
        {
            return static_cast<CDuiActiveX*>(this);
        }
        return CDuiControl::GetInterface(pstrName);
    }

    IOleObject* CDuiActiveX::GetOleObject() const
    {
        return m_pUnk;
    }

    HWND CDuiActiveX::GetHostWindow() const
    {
        return m_hwndHost;
    }

    void CDuiActiveX::SetHostWindow(HWND hwndHost)
    {
        m_hwndHost = hwndHost;
    }

    BOOL CDuiActiveX::IsMFC() const
    {
        return m_bMFC;
    }

    void CDuiActiveX::SetMFC(BOOL bMFC /*= FALSE*/)
    {
        if(m_bMFC == bMFC)
        {
            return;
        }
        m_bMFC = bMFC;
    }

    BOOL CDuiActiveX::IsDelayCreate() const
    {
        return m_bDelayCreate;
    }

    void CDuiActiveX::SetDelayCreate(BOOL bDelayCreate /*= TRUE*/)
    {
        if(m_bDelayCreate == bDelayCreate)
        {
            return;
        }
        if(bDelayCreate == FALSE)
        {
            if(m_bCreated == FALSE && m_clsid != IID_NULL)
            {
                DoCreateControl();
            }
        }
        m_bDelayCreate = bDelayCreate;
    }

    CDuiString CDuiActiveX::GetModuleName() const
    {
        return m_sModuleName;
    }

    void CDuiActiveX::SetModuleName(LPCTSTR pstrText)
    {
        m_sModuleName = pstrText;
    }

    HRESULT CDuiActiveX::GetControl(const IID iid, LPVOID* ppRet)
    {
        ASSERT(ppRet != NULL);
        ASSERT(*ppRet == NULL);
        if(ppRet == NULL)
        {
            return E_POINTER;
        }
        if(m_pUnk == NULL)
        {
            return E_PENDING;
        }
        return m_pUnk->QueryInterface(iid, (LPVOID*) ppRet);
    }

    BOOL CDuiActiveX::CreateControl(const CLSID clsid)
    {
        ASSERT(clsid != IID_NULL);
        if(clsid == IID_NULL)
        {
            return FALSE;
        }
        m_bCreated = FALSE;
        m_clsid = clsid;
        if(!m_bDelayCreate)
        {
            DoCreateControl();
        }
        return true;
    }

    BOOL CDuiActiveX::CreateControl(LPCTSTR pstrCLSID)
    {
        CLSID clsid = { 0 };
        OLECHAR szCLSID[100] = { 0 };
#ifndef _UNICODE
        ::MultiByteToWideChar(::GetACP(), 0, pstrCLSID, -1, szCLSID, SIZEOF_ARRAY(szCLSID) - 1);
#else
        _tcsncpy(szCLSID, pstrCLSID, SIZEOF_ARRAY(szCLSID) - 1);
#endif
        if(pstrCLSID[0] == '{')
        {
            ::CLSIDFromString(szCLSID, &clsid);
        }
        else
        {
            ::CLSIDFromProgID(szCLSID, &clsid);
        }
        return CreateControl(clsid);
    }

    void CDuiActiveX::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcscmp(pstrName, _T("clsid")) == 0)
        {
            CreateControl(pstrValue);
        }
        else if(_tcscmp(pstrName, _T("modulename")) == 0)
        {
            SetModuleName(pstrValue);
        }
        else if(_tcscmp(pstrName, _T("delaycreate")) == 0)
        {
            SetDelayCreate(_tcscmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcscmp(pstrName, _T("mfc")) == 0)
        {
            SetMFC(_tcscmp(pstrValue, _T("true")) == 0);
        }
        else
        {
            CDuiControl::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiActiveX::SetVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiControl::SetVisible(bVisible);
        if(m_hwndHost != NULL && !m_pControl->IsWindowless())
        {
            ::ShowWindow(m_hwndHost, IsVisible() ? SW_SHOW : SW_HIDE);
        }
    }

    void CDuiActiveX::SetInternVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiControl::SetInternVisible(bVisible);
        if(m_hwndHost != NULL && !m_pControl->IsWindowless())
        {
            ::ShowWindow(m_hwndHost, IsVisible() ? SW_SHOW : SW_HIDE);
        }
    }

    void CDuiActiveX::PixelToHiMetric(const SIZEL* lpSizeInPix, LPSIZEL lpSizeInHiMetric)
    {
#define HIMETRIC_PER_INCH   2540
#define MAP_PIX_TO_LOGHIM(x,ppli)   MulDiv(HIMETRIC_PER_INCH, (x), (ppli))
#define MAP_LOGHIM_TO_PIX(x,ppli)   MulDiv((ppli), (x), HIMETRIC_PER_INCH)
        int nPixelsPerInchX;    // Pixels per logical inch along width
        int nPixelsPerInchY;    // Pixels per logical inch along height
        HDC hDCScreen = ::GetDC(NULL);
        nPixelsPerInchX = ::GetDeviceCaps(hDCScreen, LOGPIXELSX);
        nPixelsPerInchY = ::GetDeviceCaps(hDCScreen, LOGPIXELSY);
        ::ReleaseDC(NULL, hDCScreen);
        lpSizeInHiMetric->cx = MAP_PIX_TO_LOGHIM(lpSizeInPix->cx, nPixelsPerInchX);
        lpSizeInHiMetric->cy = MAP_PIX_TO_LOGHIM(lpSizeInPix->cy, nPixelsPerInchY);
    }

    void CDuiActiveX::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::SetPos(rc, bNeedInvalidate);

        if(!m_bCreated)
        {
            DoCreateControl();
        }

        if(m_pUnk == NULL)
        {
            return;
        }
        if(m_pControl == NULL)
        {
            return;
        }

        SIZEL hmSize = { 0 };
        SIZEL pxSize = { 0 };
        pxSize.cx = GetPos().right - GetPos().left;
        pxSize.cy = GetPos().bottom - GetPos().top;
        PixelToHiMetric(&pxSize, &hmSize);

        if(m_pUnk != NULL)
        {
            m_pUnk->SetExtent(DVASPECT_CONTENT, &hmSize);
        }
        if(m_pControl->GetInPlaceObject() != NULL)
        {
            CDuiRect rcItem = GetPos();
            if(!m_pControl->IsWindowless())
            {
                rcItem.ResetOffset();
            }
            m_pControl->GetInPlaceObject()->SetObjectRects(&rcItem, &rcItem);
        }
        if(!m_pControl->IsWindowless())
        {
            ASSERT(m_pControl->GetWnd());
            ::MoveWindow(*m_pControl->GetWnd(), GetPos().left, GetPos().top, GetPos().right - GetPos().left, GetPos().bottom - GetPos().top, TRUE);
        }
    }

    void CDuiActiveX::Move(SIZE szOffset, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::Move(szOffset, bNeedInvalidate);
        if(!m_pControl->IsWindowless())
        {
            ASSERT(m_pControl->GetWnd());
            ::MoveWindow(*m_pControl->GetWnd(), GetPos().left, GetPos().top, GetPos().right - GetPos().left, GetPos().bottom - GetPos().top, TRUE);
        }
    }

    void CDuiActiveX::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        if(!::IntersectRect(&GetPaintRect(), &rcPaint, &GetPos()))
        {
            return;
        }

        if(m_pControl != NULL && m_pControl->IsWindowless() && m_pControl->GetViewObject() != NULL)
        {
            m_pControl->GetViewObject()->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hDC, (RECTL*) &GetPos(), (RECTL*) &GetPos(), NULL, NULL);
        }
    }

    LRESULT CDuiActiveX::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(m_pControl == NULL)
        {
            return 0;
        }
        ASSERT(m_pControl->IsWindowless());
        if(!m_pControl->IsInPlaceActive())
        {
            return 0;
        }
        if(m_pControl->GetInPlaceObject() == NULL)
        {
            return 0;
        }
        if(!IsMouseEnabled() && uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST)
        {
            return 0;
        }
        BOOL bWasHandled = TRUE;
        if((uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) || uMsg == WM_SETCURSOR)
        {
            // Mouse message only go when captured or inside rect
            DWORD dwHitResult = m_pControl->IsCaptured() ? HITRESULT_HIT : HITRESULT_OUTSIDE;
            if(dwHitResult == HITRESULT_OUTSIDE && m_pControl->GetViewObject() != NULL)
            {
                IViewObjectEx* pViewEx = NULL;
                m_pControl->GetViewObject()->QueryInterface(IID_IViewObjectEx, (LPVOID*) &pViewEx);
                if(pViewEx != NULL)
                {
                    POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                    pViewEx->QueryHitPoint(DVASPECT_CONTENT, &GetPos(), ptMouse, 0, &dwHitResult);
                    pViewEx->Release();
                }
            }
            if(dwHitResult != HITRESULT_HIT)
            {
                return 0;
            }
            if(uMsg == WM_SETCURSOR)
            {
                bWasHandled = FALSE;
            }
        }
        else if(uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST)
        {
            // Keyboard messages just go when we have focus
            if(!IsFocused())
            {
                return 0;
            }
        }
        else
        {
            switch(uMsg)
            {
                case WM_HELP:
                case WM_CONTEXTMENU:
                    bWasHandled = FALSE;
                    break;
                default:
                    return 0;
            }
        }
        LRESULT lResult = 0;
        HRESULT Hr = m_pControl->GetInPlaceObject()->OnWindowMessage(uMsg, wParam, lParam, &lResult);
        if(Hr == S_OK)
        {
            bHandled = bWasHandled;
        }
        return lResult;
    }

    void CDuiActiveX::ReleaseControl()
    {
        // ÒÆ³ýÏûÏ¢Á´
        GetManager()->RemoveMessageFilter(this);

        if(m_pUnk != NULL)
        {
            IObjectWithSite* pSite = NULL;
            m_pUnk->QueryInterface(IID_IObjectWithSite, (LPVOID*) &pSite);
            if(pSite != NULL)
            {
                pSite->SetSite(NULL);
                pSite->Release();
            }
            if(!IsMFC())
            {
                m_pUnk->Close(OLECLOSE_NOSAVE);
            }
            m_pUnk->SetClientSite(NULL);
            m_pUnk->Release();
            m_pUnk = NULL;
        }
        // Ïú»ÙCActiveXCtrl
        if(m_pControl != NULL)
        {
            m_pControl->SetOwner(NULL);
            m_pControl->Release();
            m_pControl = NULL;
        }

        m_hwndHost = NULL;
    }

    BOOL CDuiActiveX::DoCreateControl()
    {
        ReleaseControl();
        // At this point we'll create the ActiveX control
        m_bCreated = TRUE;
        IOleControl* pOleControl = NULL;

        HRESULT Hr = -1;
        if(!m_sModuleName.IsEmpty())
        {
            HMODULE hModule = ::LoadLibrary((LPCTSTR)m_sModuleName);
            if(hModule != NULL)
            {
                IClassFactory* aClassFactory = NULL;
                DllGetClassObjectFunc aDllGetClassObjectFunc = (DllGetClassObjectFunc)::GetProcAddress(hModule, "DllGetClassObject");
                Hr = aDllGetClassObjectFunc(m_clsid, IID_IClassFactory, (LPVOID*)&aClassFactory);
                if(SUCCEEDED(Hr))
                {
                    Hr = aClassFactory->CreateInstance(NULL, IID_IOleObject, (LPVOID*)&pOleControl);
                }
                aClassFactory->Release();
            }
        }
        if(FAILED(Hr))
        {
            Hr = ::CoCreateInstance(m_clsid, NULL, CLSCTX_ALL, IID_IOleControl, (LPVOID*)&pOleControl);
        }
        ASSERT(SUCCEEDED(Hr));
        if(FAILED(Hr))
        {
            return FALSE;
        }
        pOleControl->QueryInterface(IID_IOleObject, (LPVOID*) &m_pUnk);
        pOleControl->Release();
        if(m_pUnk == NULL)
        {
            return FALSE;
        }
        // Create the host too
        m_pControl = new CActiveXCtrl();
        m_pControl->SetOwner(this);
        // More control creation stuff
        DWORD dwMiscStatus = 0;
        m_pUnk->GetMiscStatus(DVASPECT_CONTENT, &dwMiscStatus);
        IOleClientSite* pOleClientSite = NULL;
        m_pControl->QueryInterface(IID_IOleClientSite, (LPVOID*) &pOleClientSite);
        CSafeRelease<IOleClientSite> RefOleClientSite = pOleClientSite;
        // Initialize control
        if((dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST) != 0)
        {
            m_pUnk->SetClientSite(pOleClientSite);
        }
        IPersistStreamInit* pPersistStreamInit = NULL;
        m_pUnk->QueryInterface(IID_IPersistStreamInit, (LPVOID*) &pPersistStreamInit);
        if(pPersistStreamInit != NULL)
        {
            Hr = pPersistStreamInit->InitNew();
            pPersistStreamInit->Release();
        }
        if(FAILED(Hr))
        {
            return FALSE;
        }
        if((dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST) == 0)
        {
            m_pUnk->SetClientSite(pOleClientSite);
        }
        // Grab the view...
        IViewObject* pViewObject = m_pControl->GetViewObject();
        Hr = m_pUnk->QueryInterface(IID_IViewObjectEx, (LPVOID*)&pViewObject);
        if(FAILED(Hr))
        {
            Hr = m_pUnk->QueryInterface(IID_IViewObject2, (LPVOID*) &pViewObject);
        }
        if(FAILED(Hr))
        {
            Hr = m_pUnk->QueryInterface(IID_IViewObject, (LPVOID*) &pViewObject);
        }
        m_pControl->SetViewObject(pViewObject);
        // Activate and done...
        m_pUnk->SetHostNames(OLESTR("CDuiActiveX"), NULL);
        if(GetManager() != NULL)
        {
            GetManager()->SendNotify((CDuiControl*)this, DUI_MSGTYPE_SHOWACTIVEX, 0, 0, FALSE);
        }
        if((dwMiscStatus & OLEMISC_INVISIBLEATRUNTIME) == 0)
        {
            try
            {
                Hr = m_pUnk->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, pOleClientSite, 0, GetManager()->GetPaintWindow(), &GetPos());
            }
            catch(...)
            {
            }
        }
        IObjectWithSite* pSite = NULL;
        m_pUnk->QueryInterface(IID_IObjectWithSite, (LPVOID*) &pSite);
        if(pSite != NULL)
        {
            pSite->SetSite(static_cast<IOleClientSite*>(m_pControl));
            pSite->Release();
        }
        return SUCCEEDED(Hr);
    }

}