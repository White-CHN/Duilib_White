#pragma once

namespace DuiLib
{
    typedef HRESULT(__stdcall* DllGetClassObjectFunc)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

    class CActiveXCtrl;
    class CDuiActiveX;

    class CActiveXWnd : public CDuiWnd
    {
    public:
        CActiveXWnd();
        virtual ~CActiveXWnd();
    public:
        HWND Init(CActiveXCtrl* pOwner, HWND hWndParent);

        LPCTSTR GetWindowClassName() const OVERRIDE;
        void OnFinalMessage(HWND hWnd) OVERRIDE;
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) OVERRIDE;
    protected:
        void DoVerb(LONG iVerb);

        virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnPrint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    private:
        BOOL m_bDrawCaret;
        int m_iLayeredTick;
        CActiveXCtrl* m_pOwner;
    };

    class CActiveXEnum : public IEnumUnknown
    {
    public:
        CActiveXEnum(IUnknown* pUnk);
        virtual ~CActiveXEnum();
    public:
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();
        STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObject);
        STDMETHOD(Next)(ULONG celt, IUnknown** rgelt, ULONG* pceltFetched);
        STDMETHOD(Skip)(ULONG celt);
        STDMETHOD(Reset)(void);
        STDMETHOD(Clone)(IEnumUnknown** ppenum);
    private:
        LONG m_iPos;
        ULONG m_dwRef;
        IUnknown* m_pUnk;
    };

    class CActiveXCtrl :
        public IOleClientSite,
        public IOleInPlaceSiteWindowless,
        public IOleControlSite,
        public IObjectWithSite,
        public IOleContainer
    {
    public:
        CActiveXCtrl();
        virtual ~CActiveXCtrl();
    public:
        CDuiActiveX* GetOwner() const;
        void SetOwner(CDuiActiveX* pOwner);

        BOOL IsCaptured() const;
        BOOL IsWindowless() const;
        CActiveXWnd* GetWnd() const;
        IOleInPlaceObjectWindowless* GetInPlaceObject() const;

        BOOL IsInPlaceActive() const;
        void SetInPlaceActive(BOOL bInPlaceActive);

        BOOL IsFocused() const;
        void SetFocused(BOOL bFocused);

        BOOL IsUIActivated() const;
        void SetUIActivated(BOOL bUIActivated);

        IViewObject* GetViewObject() const;
        void SetViewObject(IViewObject* pViewObject);
        // IUnknown
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();
        STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObject);

        // IObjectWithSite
        STDMETHOD(SetSite)(IUnknown* pUnkSite);
        STDMETHOD(GetSite)(REFIID riid, LPVOID* ppvSite);

        // IOleClientSite
        STDMETHOD(SaveObject)(void);
        STDMETHOD(GetMoniker)(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk);
        STDMETHOD(GetContainer)(IOleContainer** ppContainer);
        STDMETHOD(ShowObject)(void);
        STDMETHOD(OnShowWindow)(BOOL fShow);
        STDMETHOD(RequestNewObjectLayout)(void);

        // IOleInPlaceSiteWindowless
        STDMETHOD(CanWindowlessActivate)(void);
        STDMETHOD(GetCapture)(void);
        STDMETHOD(SetCapture)(BOOL fCapture);
        STDMETHOD(GetFocus)(void);
        STDMETHOD(SetFocus)(BOOL fFocus);
        STDMETHOD(GetDC)(LPCRECT pRect, DWORD grfFlags, HDC* phDC);
        STDMETHOD(ReleaseDC)(HDC hDC);
        STDMETHOD(InvalidateRect)(LPCRECT pRect, BOOL fErase);
        STDMETHOD(InvalidateRgn)(HRGN hRGN, BOOL fErase);
        STDMETHOD(ScrollRect)(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip);
        STDMETHOD(AdjustRect)(LPRECT prc);
        STDMETHOD(OnDefWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);

        // IOleInPlaceSiteEx
        STDMETHOD(OnInPlaceActivateEx)(BOOL* pfNoRedraw, DWORD dwFlags);
        STDMETHOD(OnInPlaceDeactivateEx)(BOOL fNoRedraw);
        STDMETHOD(RequestUIActivate)(void);

        // IOleInPlaceSite
        STDMETHOD(CanInPlaceActivate)(void);
        STDMETHOD(OnInPlaceActivate)(void);
        STDMETHOD(OnUIActivate)(void);
        STDMETHOD(GetWindowContext)(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo);
        STDMETHOD(Scroll)(SIZE scrollExtant);
        STDMETHOD(OnUIDeactivate)(BOOL fUndoable);
        STDMETHOD(OnInPlaceDeactivate)(void);
        STDMETHOD(DiscardUndoState)(void);
        STDMETHOD(DeactivateAndUndo)(void);
        STDMETHOD(OnPosRectChange)(LPCRECT lprcPosRect);

        // IOleWindow
        STDMETHOD(GetWindow)(HWND* phwnd);
        STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);

        // IOleControlSite
        STDMETHOD(OnControlInfoChanged)(void);
        STDMETHOD(LockInPlaceActive)(BOOL fLock);
        STDMETHOD(GetExtendedControl)(IDispatch** ppDisp);
        STDMETHOD(TransformCoords)(POINTL* pPtlHimetric, POINTF* pPtfContainer, DWORD dwFlags);
        STDMETHOD(TranslateAccelerator)(MSG* pMsg, DWORD grfModifiers);
        STDMETHOD(OnFocus)(BOOL fGotFocus);
        STDMETHOD(ShowPropertyFrame)(void);

        // IOleContainer
        STDMETHOD(EnumObjects)(DWORD grfFlags, IEnumUnknown** ppenum);
        STDMETHOD(LockContainer)(BOOL fLock);

        // IParseDisplayName
        STDMETHOD(ParseDisplayName)(IBindCtx* pbc, LPOLESTR pszDisplayName, ULONG* pchEaten, IMoniker** ppmkOut);
    protected:
        HRESULT CreateActiveXWnd();
    private:
        BOOL m_bLocked;
        BOOL m_bFocused;
        BOOL m_bCaptured;
        BOOL m_bUIActivated;
        BOOL m_bInPlaceActive;
        BOOL m_bWindowless;

        LONG m_dwRef;

        CDuiActiveX* m_pOwner;
        CActiveXWnd* m_pWindow;
        IUnknown* m_pUnkSite;
        IViewObject* m_pViewObject;
        IOleInPlaceObjectWindowless* m_pInPlaceObject;
    };

    class CActiveXFrameWnd : public IOleInPlaceFrame
    {
    public:
        CActiveXFrameWnd(CDuiActiveX* pOwner);
        virtual ~CActiveXFrameWnd();
    public:
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();
        STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObject);

        // IOleInPlaceFrame
        HRESULT STDMETHODCALLTYPE InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths);
        HRESULT STDMETHODCALLTYPE SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject);
        HRESULT STDMETHODCALLTYPE RemoveMenus(HMENU hmenuShared);
        HRESULT STDMETHODCALLTYPE SetStatusText(LPCOLESTR pszStatusText);
        HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable);
        HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpmsg, WORD wID);

        // IOleWindow
        HRESULT STDMETHODCALLTYPE GetWindow(HWND* phwnd);
        HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
        // IOleInPlaceUIWindow
        HRESULT STDMETHODCALLTYPE GetBorder(LPRECT lprectBorder) ;
        HRESULT STDMETHODCALLTYPE RequestBorderSpace(LPCBORDERWIDTHS pborderwidths) ;
        HRESULT STDMETHODCALLTYPE SetBorderSpace(LPCBORDERWIDTHS pborderwidths);
        HRESULT STDMETHODCALLTYPE SetActiveObject(IOleInPlaceActiveObject* pActiveObject, LPCOLESTR pszObjName);
    private:
        ULONG m_dwRef;
        CDuiActiveX* m_pOwner;
        IOleInPlaceActiveObject* m_pActiveObject;
    };


    class DUILIB_API CDuiActiveX
        : public CDuiControl
        , public IMessageFilterUI
    {
        DECLARE_DUICONTROL(CDuiActiveX)
    public:
        CDuiActiveX(void);
        virtual ~CDuiActiveX(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        IOleObject* GetOleObject() const;

        HWND GetHostWindow() const;
        void SetHostWindow(HWND hwndHost);

        void SetCLSID(CLSID clsid);

        void SetCreated(BOOL bCreated);

        virtual BOOL IsMFC() const;
        virtual void SetMFC(BOOL bMFC = FALSE);

        virtual BOOL IsDelayCreate() const;
        virtual void SetDelayCreate(BOOL bDelayCreate = TRUE);

        CDuiString GetModuleName() const;
        void SetModuleName(LPCTSTR pstrText);

        HRESULT GetControl(const IID iid, LPVOID* ppRet);
        BOOL CreateControl(const CLSID clsid);
        BOOL CreateControl(LPCTSTR pstrCLSID);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void SetVisible(BOOL bVisible = TRUE) OVERRIDE;
        void SetInternVisible(BOOL bVisible = TRUE) OVERRIDE;
        void PixelToHiMetric(const SIZEL* lpSizeInPix, LPSIZEL lpSizeInHiMetric);
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void Move(SIZE szOffset, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;

        LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
    protected:
        virtual void ReleaseControl();
        virtual BOOL DoCreateControl();
    private:
        CLSID m_clsid;
        BOOL m_bMFC;
        BOOL m_bCreated;
        BOOL m_bDelayCreate;
        IOleObject* m_pUnk;
        CActiveXCtrl* m_pControl;
        HWND m_hwndHost;
        CDuiString m_sModuleName;
    };

}