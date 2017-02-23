#pragma once

#include <mshtmhst.h>

namespace DuiLib
{
    class CDuiWebBrowser;

    class DUILIB_API CWebBrowserEventHandler
    {
    public:
        virtual void BeforeNavigate2(CDuiWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url, VARIANT*& Flags, VARIANT*& TargetFrameName, VARIANT*& PostData, VARIANT*& Headers, VARIANT_BOOL*& Cancel) {}
        virtual void NavigateError(CDuiWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url, VARIANT*& TargetFrameName, VARIANT*& StatusCode, VARIANT_BOOL*& Cancel) {}
        virtual void NavigateComplete2(CDuiWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url) {}
        virtual void ProgressChange(CDuiWebBrowser* pWeb, LONG nProgress, LONG nProgressMax) {}
        virtual void NewWindow3(CDuiWebBrowser* pWeb, IDispatch** pDisp, VARIANT_BOOL*& Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl) {}
        virtual void CommandStateChange(CDuiWebBrowser* pWeb, long Command, VARIANT_BOOL Enable) {};
        virtual void TitleChange(CDuiWebBrowser* pWeb, BSTR bstrTitle) {};
        virtual void DocumentComplete(CDuiWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url) {}

        // interface IDocHostUIHandler
        virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(CDuiWebBrowser* pWeb,
                /* [in] */ DWORD dwID,
                /* [in] */ POINT __RPC_FAR* ppt,
                /* [in] */ IUnknown __RPC_FAR* pcmdtReserved,
                /* [in] */ IDispatch __RPC_FAR* pdispReserved)
        {
            //return E_NOTIMPL;
            //返回 E_NOTIMPL 正常弹出系统右键菜单
            return S_OK;
            //返回S_OK 则可屏蔽系统右键菜单
        }

        virtual HRESULT STDMETHODCALLTYPE GetHostInfo(CDuiWebBrowser* pWeb,
                /* [out][in] */ DOCHOSTUIINFO __RPC_FAR* pInfo)
        {
            return E_NOTIMPL;
        }

        virtual HRESULT STDMETHODCALLTYPE ShowUI(CDuiWebBrowser* pWeb,
                /* [in] */ DWORD dwID,
                /* [in] */ IOleInPlaceActiveObject __RPC_FAR* pActiveObject,
                /* [in] */ IOleCommandTarget __RPC_FAR* pCommandTarget,
                /* [in] */ IOleInPlaceFrame __RPC_FAR* pFrame,
                /* [in] */ IOleInPlaceUIWindow __RPC_FAR* pDoc)
        {
            return S_FALSE;
        }

        virtual HRESULT STDMETHODCALLTYPE HideUI(CDuiWebBrowser* pWeb)
        {
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE UpdateUI(CDuiWebBrowser* pWeb)
        {
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE EnableModeless(CDuiWebBrowser* pWeb,
                /* [in] */ BOOL fEnable)
        {
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate(CDuiWebBrowser* pWeb,
                /* [in] */ BOOL fActivate)
        {
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(CDuiWebBrowser* pWeb,
                /* [in] */ BOOL fActivate)
        {
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE ResizeBorder(CDuiWebBrowser* pWeb,
                /* [in] */ LPCRECT prcBorder,
                /* [in] */ IOleInPlaceUIWindow __RPC_FAR* pUIWindow,
                /* [in] */ BOOL fRameWindow)
        {
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(CDuiWebBrowser* pWeb,
                /* [in] */ LPMSG lpMsg,
                /* [in] */ const GUID __RPC_FAR* pguidCmdGroup,
                /* [in] */ DWORD nCmdID)
        {
            return S_FALSE;
        }

        virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath(CDuiWebBrowser* pWeb,
                /* [out] */ LPOLESTR __RPC_FAR* pchKey,
                /* [in] */ DWORD dw)
        {
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE GetDropTarget(CDuiWebBrowser* pWeb,
                /* [in] */ IDropTarget __RPC_FAR* pDropTarget,
                /* [out] */ IDropTarget __RPC_FAR* __RPC_FAR* ppDropTarget)
        {
            return E_NOTIMPL;
        }

        virtual HRESULT STDMETHODCALLTYPE GetExternal(CDuiWebBrowser* pWeb,
                /* [out] */ IDispatch __RPC_FAR* __RPC_FAR* ppDispatch)
        {
            return E_NOTIMPL;
        }

        virtual HRESULT STDMETHODCALLTYPE TranslateUrl(CDuiWebBrowser* pWeb,
                /* [in] */ DWORD dwTranslate,
                /* [in] */ OLECHAR __RPC_FAR* pchURLIn,
                /* [out] */ OLECHAR __RPC_FAR* __RPC_FAR* ppchURLOut)
        {
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE FilterDataObject(CDuiWebBrowser* pWeb,
                /* [in] */ IDataObject __RPC_FAR* pDO,
                /* [out] */ IDataObject __RPC_FAR* __RPC_FAR* ppDORet)
        {
            return S_OK;
        }

        // 	virtual HRESULT STDMETHODCALLTYPE GetOverrideKeyPath(
        // 		/* [annotation][out] */
        // 		__deref_out  LPOLESTR *pchKey,
        // 		/* [in] */ DWORD dw)
        // 	{
        // 		return E_NOTIMPL;
        // 	}

        // IDownloadManager
        virtual HRESULT STDMETHODCALLTYPE Download(CDuiWebBrowser* pWeb,
                /* [in] */ IMoniker* pmk,
                /* [in] */ IBindCtx* pbc,
                /* [in] */ DWORD dwBindVerb,
                /* [in] */ LONG grfBINDF,
                /* [in] */ BINDINFO* pBindInfo,
                /* [in] */ LPCOLESTR pszHeaders,
                /* [in] */ LPCOLESTR pszRedir,
                /* [in] */ UINT uiCP)
        {
            return S_OK;
        }
    };

    class DUILIB_API CDuiWebBrowser
        : public CDuiActiveX
        , public IDocHostUIHandler
        , public IServiceProvider
        , public IOleCommandTarget
        , public IDispatch
        , public ITranslateAccelerator
    {
        DECLARE_DUICONTROL(CDuiWebBrowser)
    public:
        CDuiWebBrowser(void);
        virtual ~CDuiWebBrowser(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString strName) OVERRIDE;

        CDuiString GetHomePage() const;
        void SetHomePage(LPCTSTR lpszUrl);

        BOOL IsAutoNavigation();
        void SetAutoNavigation(BOOL bAuto = TRUE);

        void SetWebBrowserEventHandler(CWebBrowserEventHandler* pEventHandler);
        void Navigate2(LPCTSTR lpszUrl);
        void Refresh();
        void Refresh2(int Level);
        void GoBack();
        void GoForward();

        void NavigateUrl(LPCTSTR lpszUrl);
        void NavigateHomePage();

        IWebBrowser2* GetWebBrowser2() const;
        IDispatch* GetHtmlWindow();

        BOOL DoCreateControl() OVERRIDE;
        void ReleaseControl() OVERRIDE;

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        // ITranslateAccelerator
        // Duilib消息分发给WebBrowser
        LRESULT TranslateAccelerator(MSG* pMsg) OVERRIDE;

        // IUnknown
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();
        STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObject);

        // IDispatch
        HRESULT STDMETHODCALLTYPE GetTypeInfoCount(__RPC__out UINT* pctinfo);
        HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, __RPC__deref_out_opt ITypeInfo** ppTInfo);
        HRESULT STDMETHODCALLTYPE GetIDsOfNames(__RPC__in REFIID riid, __RPC__in_ecount_full(cNames) LPOLESTR* rgszNames, UINT cNames, LCID lcid, __RPC__out_ecount_full(cNames) DISPID* rgDispId);
        HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);

        // IDocHostUIHandler
        STDMETHOD(ShowContextMenu)(DWORD dwID, POINT* pptPosition, IUnknown* pCommandTarget, IDispatch* pDispatchObjectHit);
        STDMETHOD(GetHostInfo)(DOCHOSTUIINFO* pInfo);
        STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc);
        STDMETHOD(HideUI)();
        STDMETHOD(UpdateUI)();
        STDMETHOD(EnableModeless)(BOOL fEnable);
        STDMETHOD(OnDocWindowActivate)(BOOL fActivate);
        STDMETHOD(OnFrameWindowActivate)(BOOL fActivate);
        STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fFrameWindow);
        STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID);	//浏览器消息过滤
        STDMETHOD(GetOptionKeyPath)(LPOLESTR* pchKey, DWORD dwReserved);
        STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget, IDropTarget** ppDropTarget);
        STDMETHOD(GetExternal)(IDispatch** ppDispatch);
        STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut);
        STDMETHOD(FilterDataObject)(IDataObject* pDO, IDataObject** ppDORet);

        // IServiceProvider
        STDMETHOD(QueryService)(REFGUID guidService, REFIID riid, void** ppvObject);

        // IOleCommandTarget
        HRESULT STDMETHODCALLTYPE QueryStatus(__RPC__in_opt const GUID* pguidCmdGroup, ULONG cCmds, __RPC__inout_ecount_full(cCmds) OLECMD prgCmds[ ], __RPC__inout_opt OLECMDTEXT* pCmdText);
        HRESULT STDMETHODCALLTYPE Exec(__RPC__in_opt const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, __RPC__in_opt VARIANT* pvaIn, __RPC__inout_opt VARIANT* pvaOut);

        static DISPID FindId(IDispatch* pObj, LPOLESTR pName);
        static HRESULT InvokeMethod(IDispatch* pObj, LPOLESTR pMehtod, VARIANT* pVarResult, VARIANT* ps, int cArgs);
        static HRESULT GetProperty(IDispatch* pObj, LPOLESTR pName, VARIANT* pValue);
        static HRESULT SetProperty(IDispatch* pObj, LPOLESTR pName, VARIANT* pValue);
    protected:
        // DWebBrowserEvents2
        void BeforeNavigate2(IDispatch* pDisp, VARIANT*& url, VARIANT*& Flags, VARIANT*& TargetFrameName, VARIANT*& PostData, VARIANT*& Headers, VARIANT_BOOL*& Cancel);
        void NavigateError(IDispatch* pDisp, VARIANT*& url, VARIANT*& TargetFrameName, VARIANT*& StatusCode, VARIANT_BOOL*& Cancel);
        void NavigateComplete2(IDispatch* pDisp, VARIANT*& url);
        void ProgressChange(LONG nProgress, LONG nProgressMax);
        void NewWindow3(IDispatch** pDisp, VARIANT_BOOL*& Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl);
        void CommandStateChange(long Command, VARIANT_BOOL Enable);
        void TitleChange(BSTR bstrTitle);
        void DocumentComplete(IDispatch* pDisp, VARIANT*& url);

        HRESULT RegisterEventHandler(BOOL inAdvise);
    private:
        BOOL m_bAutoNavi;	// 是否启动时打开默认页面
        LONG m_dwRef;
        DWORD m_dwCookie;

        IWebBrowser2* m_pWebBrowser2; //浏览器指针
        IHTMLWindow2* _pHtmlWnd2;
        CWebBrowserEventHandler* m_pWebBrowserEventHandler;	//浏览器事件处理

        CDuiString m_sHomePage;	// 默认页面
    };

}