#include "StdAfx.h"
#include "DuiWebBrowser.h"
#include <atlcomcli.h>
#include <mshtml.h>
#include <ExDispid.h>
namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiWebBrowser)

    CDuiWebBrowser::CDuiWebBrowser(void)
        : m_bAutoNavi(FALSE)
        , m_dwRef(0)
        , m_dwCookie(0)
        , m_pWebBrowser2(NULL)
        , _pHtmlWnd2(NULL)
        , m_pWebBrowserEventHandler(NULL)
    {
        SetCLSID(CLSID_WebBrowser);
        m_sHomePage.Empty();
    }


    CDuiWebBrowser::~CDuiWebBrowser(void)
    {
        ReleaseControl();
    }


    CDuiString CDuiWebBrowser::GetClass() const
    {
        return DUI_CTR_WEBBROWSER;
    }

    LPVOID CDuiWebBrowser::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_WEBBROWSER) == 0)
        {
            return static_cast<CDuiWebBrowser*>(this);
        }
        return CDuiActiveX::GetInterface(pstrName);
    }


    CDuiString CDuiWebBrowser::GetHomePage() const
    {
        return m_sHomePage;
    }

    void CDuiWebBrowser::SetHomePage(LPCTSTR lpszUrl)
    {
        if(m_sHomePage == lpszUrl)
        {
            return;
        }
        m_sHomePage = lpszUrl;
    }

    BOOL CDuiWebBrowser::IsAutoNavigation()
    {
        return m_bAutoNavi;
    }

    void CDuiWebBrowser::SetAutoNavigation(BOOL bAuto /*= TRUE*/)
    {
        if(m_bAutoNavi == bAuto)
        {
            return;
        }

        m_bAutoNavi = bAuto;
    }

    void CDuiWebBrowser::SetWebBrowserEventHandler(CWebBrowserEventHandler* pEventHandler)
    {
        if(pEventHandler != NULL && m_pWebBrowserEventHandler != pEventHandler)
        {
            m_pWebBrowserEventHandler = pEventHandler;
        }
    }

    void CDuiWebBrowser::Navigate2(LPCTSTR lpszUrl)
    {
        if(lpszUrl == NULL)
        {
            return;
        }

        if(m_pWebBrowser2)
        {
            CDuiVariant url;
            url.vt = VT_BSTR;
            url.bstrVal = T2BSTR(lpszUrl);
            HRESULT hr = m_pWebBrowser2->Navigate2(&url, NULL, NULL, NULL, NULL);
        }
    }

    void CDuiWebBrowser::Refresh()
    {
        if(m_pWebBrowser2)
        {
            m_pWebBrowser2->Refresh();
        }
    }

    void CDuiWebBrowser::Refresh2(int Level)
    {
        if(m_pWebBrowser2)
        {
            CDuiVariant vLevel;
            vLevel.vt = VT_I4;
            vLevel.intVal = Level;
            m_pWebBrowser2->Refresh2(&vLevel);
        }
    }

    void CDuiWebBrowser::GoBack()
    {
        if(m_pWebBrowser2)
        {
            m_pWebBrowser2->GoBack();
        }
    }

    void CDuiWebBrowser::GoForward()
    {
        if(m_pWebBrowser2)
        {
            m_pWebBrowser2->GoForward();
        }
    }

    void CDuiWebBrowser::NavigateUrl(LPCTSTR lpszUrl)
    {
        if(m_pWebBrowser2 && lpszUrl)
        {
            m_pWebBrowser2->Navigate((BSTR)SysAllocString(T2BSTR(lpszUrl)), NULL, NULL, NULL, NULL);
        }
    }

    void CDuiWebBrowser::NavigateHomePage()
    {
        if(!m_sHomePage.IsEmpty())
        {
            NavigateUrl(m_sHomePage);
        }
    }

    IWebBrowser2* CDuiWebBrowser::GetWebBrowser2() const
    {
        return m_pWebBrowser2;
    }

    IDispatch* CDuiWebBrowser::GetHtmlWindow()
    {
        IDispatch* pDp =  NULL;
        HRESULT hr;
        if(m_pWebBrowser2)
        {
            hr = m_pWebBrowser2->get_Document(&pDp);
        }

        if(FAILED(hr))
        {
            return NULL;
        }

        CComQIPtr<IHTMLDocument2> pHtmlDoc2 = pDp;

        if(pHtmlDoc2 == NULL)
        {
            return NULL;
        }

        hr = pHtmlDoc2->get_parentWindow(&_pHtmlWnd2);

        if(FAILED(hr))
        {
            return NULL;
        }

        IDispatch* pHtmlWindown = NULL;
        hr = _pHtmlWnd2->QueryInterface(IID_IDispatch, (void**)&pHtmlWindown);
        if(FAILED(hr))
        {
            return NULL;
        }

        return pHtmlWindown;
    }

    BOOL CDuiWebBrowser::DoCreateControl()
    {
        if(!CDuiActiveX::DoCreateControl())
        {
            return false;
        }
        GetManager()->AddTranslateAccelerator(this);
        GetControl(IID_IWebBrowser2, (LPVOID*)&m_pWebBrowser2);
        if(m_bAutoNavi && !m_sHomePage.IsEmpty())
        {
            this->Navigate2(m_sHomePage);
        }
        RegisterEventHandler(TRUE);
        return true;
    }


    void CDuiWebBrowser::ReleaseControl()
    {
        SetCreated(FALSE);
        GetManager()->RemoveTranslateAccelerator(this);
        RegisterEventHandler(FALSE);
    }

    void CDuiWebBrowser::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("homepage")) == 0)
        {
            m_sHomePage = pstrValue;
        }
        else if(_tcsicmp(pstrName, _T("autonavi")) == 0)
        {
            m_bAutoNavi = (_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else
        {
            CDuiActiveX::SetAttribute(pstrName, pstrValue);
        }
    }

    LRESULT CDuiWebBrowser::TranslateAccelerator(MSG* pMsg)
    {
        if(pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST)
        {
            return S_FALSE;
        }

        if(m_pWebBrowser2 == NULL)
        {
            return E_NOTIMPL;
        }

        // 当前Web窗口不是焦点,不处理加速键
        BOOL bIsChild = FALSE;
        HWND hTempWnd = NULL;
        HWND hWndFocus = ::GetFocus();

        hTempWnd = hWndFocus;
        while(hTempWnd != NULL)
        {
            if(hTempWnd == GetHostWindow())
            {
                bIsChild = TRUE;
                break;
            }
            hTempWnd = ::GetParent(hTempWnd);
        }
        if(!bIsChild)
        {
            return S_FALSE;
        }

        IOleInPlaceActiveObject* pObj;
        if(FAILED(m_pWebBrowser2->QueryInterface(IID_IOleInPlaceActiveObject, (LPVOID*)&pObj)))
        {
            return S_FALSE;
        }

        HRESULT hResult = pObj->TranslateAccelerator(pMsg);
        pObj->Release();
        return hResult;
    }

    STDMETHODIMP_(ULONG) CDuiWebBrowser::AddRef()
    {
        InterlockedIncrement(&m_dwRef);
        return m_dwRef;
    }

    STDMETHODIMP_(ULONG) CDuiWebBrowser::Release()
    {
        ULONG ulRefCount = InterlockedDecrement(&m_dwRef);
        return ulRefCount;
    }

    STDMETHODIMP CDuiWebBrowser::QueryInterface(REFIID riid, LPVOID* ppvObject)
    {
        *ppvObject = NULL;

        if(riid == IID_IDocHostUIHandler)
        {
            *ppvObject = static_cast<IDocHostUIHandler*>(this);
        }
        else if(riid == IID_IDispatch)
        {
            *ppvObject = static_cast<IDispatch*>(this);
        }
        else if(riid == IID_IServiceProvider)
        {
            *ppvObject = static_cast<IServiceProvider*>(this);
        }
        else if(riid == IID_IOleCommandTarget)
        {
            *ppvObject = static_cast<IOleCommandTarget*>(this);
        }

        if(*ppvObject != NULL)
        {
            AddRef();
        }
        return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
    }

    STDMETHODIMP CDuiWebBrowser::GetTypeInfoCount(UINT* iTInfo)
    {
        *iTInfo = 0;
        return E_NOTIMPL;
    }

    STDMETHODIMP CDuiWebBrowser::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP CDuiWebBrowser::GetIDsOfNames(REFIID riid, OLECHAR** rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP CDuiWebBrowser::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
    {
        if((riid != IID_NULL))
        {
            return E_INVALIDARG;
        }

        switch(dispIdMember)
        {
            case DISPID_BEFORENAVIGATE2:
                BeforeNavigate2(
                    pDispParams->rgvarg[6].pdispVal,
                    pDispParams->rgvarg[5].pvarVal,
                    pDispParams->rgvarg[4].pvarVal,
                    pDispParams->rgvarg[3].pvarVal,
                    pDispParams->rgvarg[2].pvarVal,
                    pDispParams->rgvarg[1].pvarVal,
                    pDispParams->rgvarg[0].pboolVal);
                break;
            case DISPID_COMMANDSTATECHANGE:
                CommandStateChange(
                    pDispParams->rgvarg[1].lVal,
                    pDispParams->rgvarg[0].boolVal);
                break;
            case DISPID_NAVIGATECOMPLETE2:
                NavigateComplete2(
                    pDispParams->rgvarg[1].pdispVal,
                    pDispParams->rgvarg[0].pvarVal);
                break;
            case DISPID_NAVIGATEERROR:
                NavigateError(
                    pDispParams->rgvarg[4].pdispVal,
                    pDispParams->rgvarg[3].pvarVal,
                    pDispParams->rgvarg[2].pvarVal,
                    pDispParams->rgvarg[1].pvarVal,
                    pDispParams->rgvarg[0].pboolVal);
                break;
            case DISPID_STATUSTEXTCHANGE:
                break;
            //  	case DISPID_NEWWINDOW2:
            //  		break;
            case DISPID_NEWWINDOW3:
                NewWindow3(
                    pDispParams->rgvarg[4].ppdispVal,
                    pDispParams->rgvarg[3].pboolVal,
                    pDispParams->rgvarg[2].uintVal,
                    pDispParams->rgvarg[1].bstrVal,
                    pDispParams->rgvarg[0].bstrVal);
                break;
            case DISPID_TITLECHANGE:
            {
                TitleChange(pDispParams->rgvarg[0].bstrVal);
                break;
            }
            case DISPID_DOCUMENTCOMPLETE:
            {
                DocumentComplete(
                    pDispParams->rgvarg[1].pdispVal,
                    pDispParams->rgvarg[0].pvarVal);

                break;
            }
            default:
                return DISP_E_MEMBERNOTFOUND;
        }
        return S_OK;
    }

    STDMETHODIMP CDuiWebBrowser::ShowContextMenu(DWORD dwID, POINT* pptPosition, IUnknown* pCommandTarget, IDispatch* pDispatchObjectHit)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->ShowContextMenu(this, dwID, pptPosition, pCommandTarget, pDispatchObjectHit);
        }
        return S_FALSE;
    }

    STDMETHODIMP CDuiWebBrowser::GetHostInfo(DOCHOSTUIINFO* pInfo)
    {
        if(pInfo != NULL)
        {
            pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_NO3DOUTERBORDER;
        }
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->GetHostInfo(this, pInfo);
        }
        return S_OK;
    }

    STDMETHODIMP CDuiWebBrowser::ShowUI(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->ShowUI(this, dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
        }
        return S_OK;
    }

    STDMETHODIMP CDuiWebBrowser::HideUI()
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->HideUI(this);
        }
        return S_OK;
    }

    STDMETHODIMP CDuiWebBrowser::UpdateUI()
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->UpdateUI(this);
        }
        return S_OK;
    }

    STDMETHODIMP CDuiWebBrowser::EnableModeless(BOOL fEnable)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->EnableModeless(this, fEnable);
        }
        return E_NOTIMPL;
    }

    STDMETHODIMP CDuiWebBrowser::OnDocWindowActivate(BOOL fActivate)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->OnDocWindowActivate(this, fActivate);
        }
        return E_NOTIMPL;
    }

    STDMETHODIMP CDuiWebBrowser::OnFrameWindowActivate(BOOL fActivate)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->OnFrameWindowActivate(this, fActivate);
        }
        return E_NOTIMPL;
    }

    STDMETHODIMP CDuiWebBrowser::ResizeBorder(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fFrameWindow)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->ResizeBorder(this, prcBorder, pUIWindow, fFrameWindow);
        }
        return E_NOTIMPL;
    }

    STDMETHODIMP CDuiWebBrowser::TranslateAccelerator(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->TranslateAccelerator(this, lpMsg, pguidCmdGroup, nCmdID);
        }
        return S_FALSE;
    }

    STDMETHODIMP CDuiWebBrowser::GetOptionKeyPath(LPOLESTR* pchKey, DWORD dwReserved)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->GetOptionKeyPath(this, pchKey, dwReserved);
        }
        return E_NOTIMPL;
    }

    STDMETHODIMP CDuiWebBrowser::GetDropTarget(IDropTarget* pDropTarget, IDropTarget** ppDropTarget)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->GetDropTarget(this, pDropTarget, ppDropTarget);
        }
        return S_FALSE;	// 使用系统拖拽
    }

    STDMETHODIMP CDuiWebBrowser::GetExternal(IDispatch** ppDispatch)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->GetExternal(this, ppDispatch);
        }
        return S_FALSE;
    }

    STDMETHODIMP CDuiWebBrowser::TranslateUrl(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->TranslateUrl(this, dwTranslate, pchURLIn, ppchURLOut);
        }
        else
        {
            *ppchURLOut = 0;
            return E_NOTIMPL;
        }
    }

    STDMETHODIMP CDuiWebBrowser::FilterDataObject(IDataObject* pDO, IDataObject** ppDORet)
    {
        if(m_pWebBrowserEventHandler)
        {
            return m_pWebBrowserEventHandler->FilterDataObject(this, pDO, ppDORet);
        }
        else
        {
            *ppDORet = 0;
            return E_NOTIMPL;
        }
    }

    STDMETHODIMP CDuiWebBrowser::QueryService(REFGUID guidService, REFIID riid, void** ppvObject)
    {
        HRESULT hr = E_NOINTERFACE;
        *ppvObject = NULL;

        if(guidService == SID_SDownloadManager && riid == IID_IDownloadManager)
        {
            *ppvObject = this;
            return S_OK;
        }

        return hr;
    }

    HRESULT STDMETHODCALLTYPE CDuiWebBrowser::QueryStatus(__RPC__in_opt const GUID* pguidCmdGroup, ULONG cCmds, __RPC__inout_ecount_full(cCmds) OLECMD prgCmds[ ], __RPC__inout_opt OLECMDTEXT* pCmdText)
    {
        HRESULT hr = OLECMDERR_E_NOTSUPPORTED;
        return hr;
    }

    HRESULT STDMETHODCALLTYPE CDuiWebBrowser::Exec(__RPC__in_opt const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, __RPC__in_opt VARIANT* pvaIn, __RPC__inout_opt VARIANT* pvaOut)
    {
        HRESULT hr = S_OK;

        if(pguidCmdGroup && IsEqualGUID(*pguidCmdGroup, CGID_DocHostCommandHandler))
        {

            switch(nCmdID)
            {

                case OLECMDID_SHOWSCRIPTERROR:
                {
                    IHTMLDocument2*             pDoc = NULL;
                    IHTMLWindow2*               pWindow = NULL;
                    IHTMLEventObj*              pEventObj = NULL;
                    BSTR                        rgwszNames[5] =
                    {
                        SysAllocString(L"errorLine"),
                        SysAllocString(L"errorCharacter"),
                        SysAllocString(L"errorCode"),
                        SysAllocString(L"errorMessage"),
                        SysAllocString(L"errorUrl")
                    };
                    DISPID                      rgDispIDs[5];
                    VARIANT                     rgvaEventInfo[5];
                    DISPPARAMS                  params;
                    BOOL                        fContinueRunningScripts = true;
                    int                         i;

                    params.cArgs = 0;
                    params.cNamedArgs = 0;

                    // Get the document that is currently being viewed.
                    hr = pvaIn->punkVal->QueryInterface(IID_IHTMLDocument2, (void**) &pDoc);
                    // Get document.parentWindow.
                    hr = pDoc->get_parentWindow(&pWindow);
                    pDoc->Release();
                    // Get the window.event object.
                    hr = pWindow->get_event(&pEventObj);
                    // Get the error info from the window.event object.
                    for(i = 0; i < 5; i++)
                    {
                        // Get the property's dispID.
                        hr = pEventObj->GetIDsOfNames(IID_NULL, &rgwszNames[i], 1,
                                                      LOCALE_SYSTEM_DEFAULT, &rgDispIDs[i]);
                        // Get the value of the property.
                        hr = pEventObj->Invoke(rgDispIDs[i], IID_NULL,
                                               LOCALE_SYSTEM_DEFAULT,
                                               DISPATCH_PROPERTYGET, &params, &rgvaEventInfo[i],
                                               NULL, NULL);
                        SysFreeString(rgwszNames[i]);
                    }

                    // At this point, you would normally alert the user with
                    // the information about the error, which is now contained
                    // in rgvaEventInfo[]. Or, you could just exit silently.

                    (*pvaOut).vt = VT_BOOL;
                    if(fContinueRunningScripts)
                    {
                        // Continue running scripts on the page.
                        (*pvaOut).boolVal = VARIANT_TRUE;
                    }
                    else
                    {
                        // Stop running scripts on the page.
                        (*pvaOut).boolVal = VARIANT_FALSE;
                    }
                    break;
                }
                default:
                    hr = OLECMDERR_E_NOTSUPPORTED;
                    break;
            }
        }
        else
        {
            hr = OLECMDERR_E_UNKNOWNGROUP;
        }
        return (hr);
    }

    DISPID CDuiWebBrowser::FindId(IDispatch* pObj, LPOLESTR pName)
    {
        DISPID id = 0;
        if(FAILED(pObj->GetIDsOfNames(IID_NULL, &pName, 1, LOCALE_SYSTEM_DEFAULT, &id)))
        {
            id = -1;
        }
        return id;
    }

    HRESULT CDuiWebBrowser::InvokeMethod(IDispatch* pObj, LPOLESTR pMehtod, VARIANT* pVarResult, VARIANT* ps, int cArgs)
    {
        DISPID dispid = FindId(pObj, pMehtod);
        if(dispid == -1)
        {
            return E_FAIL;
        }

        DISPPARAMS dispparams;
        dispparams.cArgs = cArgs;
        dispparams.rgvarg = ps;
        dispparams.cNamedArgs = 0;
        dispparams.rgdispidNamedArgs = NULL;

        return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, pVarResult, NULL, NULL);
    }

    HRESULT CDuiWebBrowser::GetProperty(IDispatch* pObj, LPOLESTR pName, VARIANT* pValue)
    {
        DISPID dispid = FindId(pObj, pName);
        if(dispid == -1)
        {
            return E_FAIL;
        }

        DISPPARAMS ps;
        ps.cArgs = 0;
        ps.rgvarg = NULL;
        ps.cNamedArgs = 0;
        ps.rgdispidNamedArgs = NULL;

        return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &ps, pValue, NULL, NULL);
    }

    HRESULT CDuiWebBrowser::SetProperty(IDispatch* pObj, LPOLESTR pName, VARIANT* pValue)
    {
        DISPID dispid = FindId(pObj, pName);
        if(dispid == -1)
        {
            return E_FAIL;
        }

        DISPPARAMS ps;
        ps.cArgs = 1;
        ps.rgvarg = pValue;
        ps.cNamedArgs = 0;
        ps.rgdispidNamedArgs = NULL;

        return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &ps, NULL, NULL, NULL);
    }

    void CDuiWebBrowser::BeforeNavigate2(IDispatch* pDisp, VARIANT*& url, VARIANT*& Flags, VARIANT*& TargetFrameName, VARIANT*& PostData, VARIANT*& Headers, VARIANT_BOOL*& Cancel)
    {
        if(m_pWebBrowserEventHandler)
        {
            m_pWebBrowserEventHandler->BeforeNavigate2(this, pDisp, url, Flags, TargetFrameName, PostData, Headers, Cancel);
        }
    }

    void CDuiWebBrowser::NavigateError(IDispatch* pDisp, VARIANT*& url, VARIANT*& TargetFrameName, VARIANT*& StatusCode, VARIANT_BOOL*& Cancel)
    {
        if(m_pWebBrowserEventHandler)
        {
            m_pWebBrowserEventHandler->NavigateError(this, pDisp, url, TargetFrameName, StatusCode, Cancel);
        }
    }

    void CDuiWebBrowser::NavigateComplete2(IDispatch* pDisp, VARIANT*& url)
    {
        CComPtr<IDispatch> spDoc;
        m_pWebBrowser2->get_Document(&spDoc);

        if(spDoc)
        {
            CComQIPtr<ICustomDoc, &IID_ICustomDoc> spCustomDoc(spDoc);
            if(spCustomDoc)
            {
                spCustomDoc->SetUIHandler(this);
            }
        }

        if(m_pWebBrowserEventHandler)
        {
            m_pWebBrowserEventHandler->NavigateComplete2(this, pDisp, url);
        }
    }

    void CDuiWebBrowser::ProgressChange(LONG nProgress, LONG nProgressMax)
    {
        if(m_pWebBrowserEventHandler)
        {
            m_pWebBrowserEventHandler->ProgressChange(this, nProgress, nProgressMax);
        }
    }

    void CDuiWebBrowser::NewWindow3(IDispatch** pDisp, VARIANT_BOOL*& Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl)
    {
        if(m_pWebBrowserEventHandler)
        {
            m_pWebBrowserEventHandler->NewWindow3(this, pDisp, Cancel, dwFlags, bstrUrlContext, bstrUrl);
        }
    }
    void CDuiWebBrowser::CommandStateChange(long Command, VARIANT_BOOL Enable)
    {
        if(m_pWebBrowserEventHandler)
        {
            m_pWebBrowserEventHandler->CommandStateChange(this, Command, Enable);
        }
    }

    void CDuiWebBrowser::TitleChange(BSTR bstrTitle)
    {
        if(m_pWebBrowserEventHandler)
        {
            m_pWebBrowserEventHandler->TitleChange(this, bstrTitle);
        }
    }

    void CDuiWebBrowser::DocumentComplete(IDispatch* pDisp, VARIANT*& url)
    {
        if(m_pWebBrowserEventHandler)
        {
            m_pWebBrowserEventHandler->DocumentComplete(this, pDisp, url);
        }
    }

    HRESULT CDuiWebBrowser::RegisterEventHandler(BOOL inAdvise)
    {
        CComPtr<IWebBrowser2> pWebBrowser;
        CComPtr<IConnectionPointContainer>  pCPC;
        CComPtr<IConnectionPoint> pCP;
        HRESULT hr = GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);
        if(FAILED(hr))
        {
            return hr;
        }
        hr = pWebBrowser->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
        if(FAILED(hr))
        {
            return hr;
        }
        hr = pCPC->FindConnectionPoint(DIID_DWebBrowserEvents2, &pCP);
        if(FAILED(hr))
        {
            return hr;
        }

        if(inAdvise)
        {
            hr = pCP->Advise((IDispatch*)this, &m_dwCookie);
        }
        else
        {
            hr = pCP->Unadvise(m_dwCookie);
        }
        return hr;
    }

}