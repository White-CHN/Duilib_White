#include "StdAfx.h"
#include "DuiRichEdit.h"
#include <Imm.h>
#pragma comment(lib,"imm32.lib")

namespace DuiLib
{
    EXTERN_C const IID IID_ITextServices =   // 8d33f740-cf58-11ce-a89d-00aa006cadc5
    {
        0x8d33f740,
        0xcf58,
        0x11ce,
        {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
    };

    EXTERN_C const IID IID_ITextHost =   /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
    {
        0xc5bdd8d0,
        0xd26e,
        0x11ce,
        {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
    };

    ////////////////////////////////////////////////////////////
    CTxtWinHost::CTxtWinHost(void)
        : chPasswordChar('*')
        , m_bRichText(FALSE)
        , m_bTransparent(FALSE)
        , m_bInplaceActive(FALSE)
        , m_bShowCaret(FALSE)
        , m_bTimer(FALSE)
        , m_bCaptured(FALSE)
        , m_bEnableAutoWordSel(FALSE)
        , m_bWordWrap(FALSE)
        , m_bAllowBeep(FALSE)
        , m_bSaveSelection(FALSE)
        , lSelBarWidth(0)
        , m_iLimitText(TEXTMAX)
        , laccelpos(-1)
        , cRefs(0)
        , m_dwStyle(0)
        , m_iCaretWidth(0)
        , m_iCaretHeight(0)
        , m_iCaretLastWidth(0)
        , m_iCaretLastHeight(0)
        , pserv(NULL)
        , m_pRichEdit(NULL)
    {
        ::ZeroMemory(&rcClient, sizeof(rcClient));
        ::ZeroMemory(&sizelExtent, sizeof(sizelExtent));
        ::ZeroMemory(&cf, sizeof(cf));
        ::ZeroMemory(&pf, sizeof(pf));
    }

    CTxtWinHost::~CTxtWinHost(void)
    {
        pserv->OnTxInPlaceDeactivate();
        //pserv->Release(); 剪切和复制后，关闭时会造成delete异常，暂时注释掉
    }


    HRESULT CTxtWinHost::InitDefaultCharFormat(CDuiRichEdit* re, CHARFORMAT2W* pcf, HFONT hfont)
    {
        memset(pcf, 0, sizeof(CHARFORMAT2W));
        if(!hfont)
        {
            hfont = re->GetManager()->GetFont(re->GetFont());
        }
        LOGFONT lf;
        ::GetObject(hfont, sizeof(LOGFONT), &lf);

        DWORD dwColor = re->GetTextColor();
        if(re->GetManager()->IsLayered())
        {
            CRenderEngine::CheckAlphaColor(dwColor);
        }
        pcf->cbSize = sizeof(CHARFORMAT2W);
        pcf->crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
        LONG yPixPerInch = GetDeviceCaps(re->GetManager()->GetPaintDC(), LOGPIXELSY);
        pcf->yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
        pcf->yOffset = 0;
        pcf->dwEffects = 0;
        pcf->dwMask = CFM_SIZE | CFM_OFFSET | CFM_FACE | CFM_CHARSET | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
        if(lf.lfWeight >= FW_BOLD)
        {
            pcf->dwEffects |= CFE_BOLD;
        }
        if(lf.lfItalic)
        {
            pcf->dwEffects |= CFE_ITALIC;
        }
        if(lf.lfUnderline)
        {
            pcf->dwEffects |= CFE_UNDERLINE;
        }
        pcf->bCharSet = lf.lfCharSet;
        pcf->bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
        _tcscpy(pcf->szFaceName, lf.lfFaceName);
#else
        //need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
        MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, pcf->szFaceName, LF_FACESIZE) ;
#endif

        return S_OK;
    }

    HRESULT CTxtWinHost::InitDefaultParaFormat(CDuiRichEdit* re, PARAFORMAT2* ppf)
    {
        memset(ppf, 0, sizeof(PARAFORMAT2));
        ppf->cbSize = sizeof(PARAFORMAT2);
        ppf->dwMask = PFM_ALL;
        ppf->wAlignment = PFA_LEFT;
        ppf->cTabCount = 1;
        ppf->rgxTabs[0] = lDefaultTab;

        return S_OK;
    }

    BOOL CTxtWinHost::Init(CDuiRichEdit* pRichEdit, const CREATESTRUCT* pcs)
    {
        IUnknown* pUnk = NULL;
        HRESULT hr = 0;

        m_pRichEdit = pRichEdit;
        // Initialize Reference count
        cRefs = 1;

        // Create and cache CHARFORMAT for this control
        if(FAILED(InitDefaultCharFormat(pRichEdit, &cf, NULL)))
        {
            goto err;
        }

        // Create and cache PARAFORMAT for this control
        if(FAILED(InitDefaultParaFormat(pRichEdit, &pf)))
        {
            goto err;
        }

        // edit controls created without a window are multiline by default
        // so that paragraph formats can be
        m_dwStyle = ES_MULTILINE;

        // edit controls are rich by default
        m_bRichText = pRichEdit->IsRich();

        m_iLimitText = pRichEdit->GetLimitText();

        if(pcs)
        {
            m_dwStyle = pcs->style;
            if(!(m_dwStyle & (ES_AUTOHSCROLL | WS_HSCROLL)))
            {
                m_bWordWrap = TRUE;
            }
        }

        if(!(m_dwStyle & ES_LEFT))
        {
            if(m_dwStyle & ES_CENTER)
            {
                pf.wAlignment = PFA_CENTER;
            }
            else if(m_dwStyle & ES_RIGHT)
            {
                pf.wAlignment = PFA_RIGHT;
            }
        }

        m_bInplaceActive = TRUE;

        PCreateTextServices TextServicesProc = NULL;
#ifdef _UNICODE
        HMODULE hmod = LoadLibrary(_T("Msftedit.dll"));
#else
        HMODULE hmod = LoadLibrary(_T("Riched20.dll"));
#endif
        if(hmod)
        {
            TextServicesProc = (PCreateTextServices)GetProcAddress(hmod, "CreateTextServices");
        }
        if(TextServicesProc)
        {
            HRESULT hr = TextServicesProc(NULL, this, &pUnk);
        }

        hr = pUnk->QueryInterface(IID_ITextServices, (void**)&pserv);

        // Whether the previous call succeeded or failed we are done
        // with the private interface.
        pUnk->Release();

        if(FAILED(hr))
        {
            goto err;
        }

        // Set window text
        if(pcs && pcs->lpszName)
        {
#ifdef _UNICODE
            if(FAILED(pserv->TxSetText((TCHAR*)pcs->lpszName)))
            {
                goto err;
            }
#else
            size_t iLen = _tcslen(pcs->lpszName);
            LPWSTR lpText = new WCHAR[iLen + 1];
            ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
            ::MultiByteToWideChar(CP_ACP, 0, pcs->lpszName, -1, (LPWSTR)lpText, iLen) ;
            if(FAILED(pserv->TxSetText((LPWSTR)lpText)))
            {
                DUI_FREE_ARRAY(lpText);
                goto err;
            }
            DUI_FREE_ARRAY(lpText);
#endif
        }

        return TRUE;
err:
        return FALSE;
    }

    ITextServices* CTxtWinHost::GetTextServices() const
    {
        return pserv;
    }


    BOOL CTxtWinHost::IsCaptured()
    {
        return m_bCaptured;
    }

    void CTxtWinHost::SetReadOnly(BOOL bReadOnly)
    {
        if(bReadOnly)
        {
            m_dwStyle |= ES_READONLY;
        }
        else
        {
            m_dwStyle &= ~ES_READONLY;
        }

        pserv->OnTxPropertyBitsChange(TXTBIT_READONLY, bReadOnly ? TXTBIT_READONLY : 0);
    }

    void CTxtWinHost::SetRichTextFlag(BOOL bRichText)
    {
        m_bRichText = bRichText;
        pserv->OnTxPropertyBitsChange(TXTBIT_RICHTEXT, bRichText ? TXTBIT_RICHTEXT : 0);
    }


    void CTxtWinHost::SetColor(DWORD dwColor)
    {
        CRenderEngine::CheckAlphaColor(dwColor);
        cf.crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
        pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, TXTBIT_CHARFORMATCHANGE);
    }

    void CTxtWinHost::LimitText(LONG nChars)
    {
        m_iLimitText = nChars;
        if(m_iLimitText <= 0)
        {
            m_iLimitText = TEXTMAX;
        }
        pserv->OnTxPropertyBitsChange(TXTBIT_MAXLENGTHCHANGE, TXTBIT_MAXLENGTHCHANGE);
    }

    void CTxtWinHost::GetControlRect(LPRECT prc)
    {
        prc->top = rcClient.top;
        prc->bottom = rcClient.bottom;
        prc->left = rcClient.left;
        prc->right = rcClient.right;
    }

    void CTxtWinHost::SetClientRect(RECT* prc)
    {
        rcClient = *prc;

        LONG xPerInch = ::GetDeviceCaps(m_pRichEdit->GetManager()->GetPaintDC(), LOGPIXELSX);
        LONG yPerInch =	::GetDeviceCaps(m_pRichEdit->GetManager()->GetPaintDC(), LOGPIXELSY);
        sizelExtent.cx = MulDiv(rcClient.right - rcClient.left, HIMETRIC_PER_INCH, xPerInch);
        sizelExtent.cy = MulDiv(rcClient.bottom - rcClient.top, HIMETRIC_PER_INCH, yPerInch);
        //pserv->OnTxPropertyBitsChange(TXTBIT_VIEWINSETCHANGE, TXTBIT_VIEWINSETCHANGE);  解决动画过程中循环调用setpos
    }

    void CTxtWinHost::SetTransparent(BOOL bTransparent)
    {
        m_bTransparent = bTransparent;

        // notify text services of property change
        pserv->OnTxPropertyBitsChange(TXTBIT_BACKSTYLECHANGE, 0);
    }


    BOOL CTxtWinHost::DoSetCursor(RECT* prc, POINT* pt)
    {
        RECT rc = prc ? *prc : rcClient;

        // Is this in our rectangle?
        if(PtInRect(&rc, *pt))
        {
            RECT* prcClient = (!m_bInplaceActive || prc) ? &rc : NULL;
            pserv->OnTxSetCursor(DVASPECT_CONTENT,	-1, NULL, NULL,  m_pRichEdit->GetManager()->GetPaintDC(), NULL, prcClient, pt->x, pt->y);

            return TRUE;
        }

        return FALSE;
    }


    void CTxtWinHost::SetCharFormat(CHARFORMAT2W& c)
    {
        cf = c;
    }

    void CTxtWinHost::SetParaFormat(PARAFORMAT2& p)
    {
        pf = p;
    }

    HRESULT CTxtWinHost::OnTxInPlaceActivate(LPCRECT prcClient)
    {
        m_bInplaceActive = TRUE;

        HRESULT hr = pserv->OnTxInPlaceActivate(prcClient);

        if(FAILED(hr))
        {
            m_bInplaceActive = FALSE;
        }

        return hr;
    }

    ULONG STDMETHODCALLTYPE CTxtWinHost::AddRef(void)
    {
        return ++cRefs;
    }

    ULONG STDMETHODCALLTYPE CTxtWinHost::Release(void)
    {
        ULONG c_Refs = --cRefs;

        if(c_Refs == 0)
        {
            delete this;
        }

        return c_Refs;
    }

    HRESULT STDMETHODCALLTYPE CTxtWinHost::QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR* __RPC_FAR* ppvObject)
    {
        HRESULT hr = E_NOINTERFACE;
        *ppvObject = NULL;

        if(IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITextHost))
        {
            AddRef();
            *ppvObject = (ITextHost*) this;
            hr = S_OK;
        }

        return hr;
    }

    HDC CTxtWinHost::TxGetDC()
    {
        return m_pRichEdit->GetManager()->GetPaintDC();
    }

    int CTxtWinHost::TxReleaseDC(HDC hdc)
    {
        return 1;
    }

    BOOL CTxtWinHost::TxShowScrollBar(int fnBar, BOOL fShow)
    {
        CDuiScrollBar* pVerticalScrollBar = m_pRichEdit->GetVerticalScrollBar();
        CDuiScrollBar* pHorizontalScrollBar = m_pRichEdit->GetHorizontalScrollBar();
        if(fnBar == SB_VERT && pVerticalScrollBar)
        {
            pVerticalScrollBar->SetVisible(fShow == TRUE);
        }
        else if(fnBar == SB_HORZ && pHorizontalScrollBar)
        {
            pHorizontalScrollBar->SetVisible(fShow == TRUE);
        }
        else if(fnBar == SB_BOTH)
        {
            if(pVerticalScrollBar)
            {
                pVerticalScrollBar->SetVisible(fShow == TRUE);
            }
            if(pHorizontalScrollBar)
            {
                pHorizontalScrollBar->SetVisible(fShow == TRUE);
            }
        }
        return TRUE;
    }

    BOOL CTxtWinHost::TxEnableScrollBar(int fuSBFlags, int fuArrowflags)
    {
        if(fuSBFlags == SB_VERT)
        {
            m_pRichEdit->EnableScrollBar(true, m_pRichEdit->GetHorizontalScrollBar() != NULL);
            m_pRichEdit->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
        }
        else if(fuSBFlags == SB_HORZ)
        {
            m_pRichEdit->EnableScrollBar(m_pRichEdit->GetVerticalScrollBar() != NULL, true);
            m_pRichEdit->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
        }
        else if(fuSBFlags == SB_BOTH)
        {
            m_pRichEdit->EnableScrollBar(true, true);
            m_pRichEdit->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
            m_pRichEdit->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
        }
        return TRUE;
    }

    BOOL CTxtWinHost::TxSetScrollRange(int fnBar, LONG nMinPos, int nMaxPos, BOOL fRedraw)
    {
        CDuiScrollBar* pVerticalScrollBar = m_pRichEdit->GetVerticalScrollBar();
        CDuiScrollBar* pHorizontalScrollBar = m_pRichEdit->GetHorizontalScrollBar();
        if(fnBar == SB_VERT && pVerticalScrollBar)
        {
            if(nMaxPos - nMinPos - rcClient.bottom + rcClient.top <= 0)
            {
                pVerticalScrollBar->SetVisible(false);
            }
            else
            {
                pVerticalScrollBar->SetVisible(true);
                pVerticalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.bottom + rcClient.top);
            }
        }
        else if(fnBar == SB_HORZ && pHorizontalScrollBar)
        {
            if(nMaxPos - nMinPos - rcClient.right + rcClient.left <= 0)
            {
                pHorizontalScrollBar->SetVisible(false);
            }
            else
            {
                pHorizontalScrollBar->SetVisible(true);
                pHorizontalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.right + rcClient.left);
            }
        }
        return TRUE;
    }

    BOOL CTxtWinHost::TxSetScrollPos(int fnBar, int nPos, BOOL fRedraw)
    {
        CDuiScrollBar* pVerticalScrollBar = m_pRichEdit->GetVerticalScrollBar();
        CDuiScrollBar* pHorizontalScrollBar = m_pRichEdit->GetHorizontalScrollBar();
        if(fnBar == SB_VERT && pVerticalScrollBar)
        {
            pVerticalScrollBar->SetScrollPos(nPos);
        }
        else if(fnBar == SB_HORZ && pHorizontalScrollBar)
        {
            pHorizontalScrollBar->SetScrollPos(nPos);
        }
        return TRUE;
    }

    void CTxtWinHost::TxInvalidateRect(LPCRECT prc, BOOL fMode)
    {
        if(prc == NULL)
        {
            m_pRichEdit->GetManager()->Invalidate(rcClient);
            return;
        }
        RECT rc = *prc;
        m_pRichEdit->GetManager()->Invalidate(rc);
    }

    void CTxtWinHost::TxViewChange(BOOL fUpdate)
    {
        if(m_pRichEdit->OnTxViewChanged(fUpdate))
        {
            m_pRichEdit->Invalidate();
        }
    }


    BOOL CTxtWinHost::TxCreateCaret(HBITMAP hbmp, int xWidth, int yHeight)
    {
        m_iCaretWidth = xWidth;
        m_iCaretHeight = yHeight;
        if(m_pRichEdit->GetManager()->IsLayered())
        {
            return m_pRichEdit->GetManager()->CreateCaret(hbmp, xWidth, yHeight);
        }
        else
        {
            return ::CreateCaret(m_pRichEdit->GetManager()->GetPaintWindow(), hbmp, xWidth, yHeight);
        }
    }

    BOOL CTxtWinHost::TxShowCaret(BOOL fShow)
    {
        m_bShowCaret = fShow;
        if(m_pRichEdit->GetManager()->IsLayered())
        {
            if(m_pRichEdit->GetManager()->GetCurrentCaretObject() == m_pRichEdit)
            {
                if((m_pRichEdit->IsReadOnly() || !m_pRichEdit->Activate()))
                {
                    m_pRichEdit->GetManager()->ShowCaret(FALSE);
                    return TRUE;
                }
            }

            return m_pRichEdit->GetManager()->ShowCaret(fShow == TRUE);
        }
        else
        {
            if(fShow)
            {
                return ::ShowCaret(m_pRichEdit->GetManager()->GetPaintWindow());
            }
            else
            {
                return ::HideCaret(m_pRichEdit->GetManager()->GetPaintWindow());
            }
        }
    }

    BOOL CTxtWinHost::TxSetCaretPos(int x, int y)
    {
        if(m_pRichEdit->GetManager()->IsLayered())
        {
            m_pRichEdit->GetManager()->SetCaretPos(m_pRichEdit, x, y);
            return true;
        }
        else
        {
            return ::SetCaretPos(x, y);
        }
    }

    BOOL CTxtWinHost::TxSetTimer(UINT idTimer, UINT uTimeout)
    {
        m_bTimer = TRUE;
        return m_pRichEdit->SetTimer(idTimer, uTimeout) == TRUE;
    }

    void CTxtWinHost::TxKillTimer(UINT idTimer)
    {
        m_pRichEdit->KillTimer(idTimer);
        m_bTimer = FALSE;
    }

    void CTxtWinHost::TxScrollWindowEx(int dx, int dy, LPCRECT lprcScroll,	LPCRECT lprcClip,	HRGN hrgnUpdate, LPRECT lprcUpdate,	UINT fuScroll)
    {
        return;
    }

    void CTxtWinHost::TxSetCapture(BOOL fCapture)
    {
        if(fCapture)
        {
            m_pRichEdit->GetManager()->SetCapture();
        }
        else
        {
            m_pRichEdit->GetManager()->ReleaseCapture();
        }
        m_bCaptured = fCapture;
    }

    void CTxtWinHost::TxSetFocus()
    {
        m_pRichEdit->SetFocus();
    }

    void CTxtWinHost::TxSetCursor(HCURSOR hcur,	BOOL fText)
    {
        ::SetCursor(hcur);
    }

    BOOL CTxtWinHost::TxScreenToClient(LPPOINT lppt)
    {
        return ::ScreenToClient(m_pRichEdit->GetManager()->GetPaintWindow(), lppt);
    }

    BOOL CTxtWinHost::TxClientToScreen(LPPOINT lppt)
    {
        return ::ClientToScreen(m_pRichEdit->GetManager()->GetPaintWindow(), lppt);
    }

    HRESULT CTxtWinHost::TxActivate(LONG* plOldState)
    {
        return S_OK;
    }

    HRESULT CTxtWinHost::TxDeactivate(LONG lNewState)
    {
        return S_OK;
    }

    HRESULT CTxtWinHost::TxGetClientRect(LPRECT prc)
    {
        *prc = rcClient;
        GetControlRect(prc);
        return NOERROR;
    }

    HRESULT CTxtWinHost::TxGetViewInset(LPRECT prc)
    {
        prc->left = prc->right = prc->top = prc->bottom = 0;
        return NOERROR;
    }

    HRESULT CTxtWinHost::TxGetCharFormat(const CHARFORMATW** ppCF)
    {
        *ppCF = &cf;
        return NOERROR;
    }

    HRESULT CTxtWinHost::TxGetParaFormat(const PARAFORMAT** ppPF)
    {
        *ppPF = &pf;
        return NOERROR;
    }

    COLORREF CTxtWinHost::TxGetSysColor(int nIndex)
    {
        DWORD dwColor = ::GetSysColor(nIndex);
        CRenderEngine::CheckAlphaColor(dwColor);
        return dwColor;
    }

    HRESULT CTxtWinHost::TxGetBackStyle(TXTBACKSTYLE* pstyle)
    {
        *pstyle = !m_bTransparent ? TXTBACK_OPAQUE : TXTBACK_TRANSPARENT;
        return NOERROR;
    }

    HRESULT CTxtWinHost::TxGetMaxLength(DWORD* pLength)
    {
        *pLength = m_iLimitText;
        return NOERROR;
    }

    HRESULT CTxtWinHost::TxGetScrollBars(DWORD* pdwScrollBar)
    {
        *pdwScrollBar =  m_dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL |
                                      ES_AUTOHSCROLL | ES_DISABLENOSCROLL);

        return NOERROR;
    }

    HRESULT CTxtWinHost::TxGetPasswordChar(TCHAR* pch)
    {
#ifdef _UNICODE
        *pch = chPasswordChar;
#else
        ::WideCharToMultiByte(CP_ACP, 0, &chPasswordChar, 1, pch, 1, NULL, NULL) ;
#endif
        return NOERROR;
    }

    HRESULT CTxtWinHost::TxGetAcceleratorPos(LONG* pcp)
    {
        *pcp = laccelpos;
        return S_OK;
    }

    HRESULT CTxtWinHost::OnTxCharFormatChange(const CHARFORMATW* pcf)
    {
        return S_OK;
    }

    HRESULT CTxtWinHost::OnTxParaFormatChange(const PARAFORMAT* ppf)
    {
        return S_OK;
    }

    HRESULT CTxtWinHost::TxGetPropertyBits(DWORD dwMask, DWORD* pdwBits)
    {
        DWORD dwProperties = 0;

        if(m_bRichText)
        {
            dwProperties = TXTBIT_RICHTEXT;
        }

        if(m_dwStyle & ES_MULTILINE)
        {
            dwProperties |= TXTBIT_MULTILINE;
        }

        if(m_dwStyle & ES_READONLY)
        {
            dwProperties |= TXTBIT_READONLY;
        }

        if(m_dwStyle & ES_PASSWORD)
        {
            dwProperties |= TXTBIT_USEPASSWORD;
        }

        if(!(m_dwStyle & ES_NOHIDESEL))
        {
            dwProperties |= TXTBIT_HIDESELECTION;
        }

        if(m_bEnableAutoWordSel)
        {
            dwProperties |= TXTBIT_AUTOWORDSEL;
        }

        if(m_bWordWrap)
        {
            dwProperties |= TXTBIT_WORDWRAP;
        }

        if(m_bAllowBeep)
        {
            dwProperties |= TXTBIT_ALLOWBEEP;
        }

        if(m_bSaveSelection)
        {
            dwProperties |= TXTBIT_SAVESELECTION;
        }

        *pdwBits = dwProperties & dwMask;
        return NOERROR;
    }


    HRESULT CTxtWinHost::TxNotify(DWORD iNotify, void* pv)
    {
        if(iNotify == EN_REQUESTRESIZE)
        {
            RECT rc;
            REQRESIZE* preqsz = (REQRESIZE*)pv;
            GetControlRect(&rc);
            rc.bottom = rc.top + preqsz->rc.bottom;
            rc.right  = rc.left + preqsz->rc.right;
            SetClientRect(&rc);
        }
        m_pRichEdit->OnTxNotify(iNotify, pv);
        return S_OK;
    }

    HIMC CTxtWinHost::TxImmGetContext()
    {
        return NULL;
    }

    void CTxtWinHost::TxImmReleaseContext(HIMC himc)
    {

    }

    HRESULT CTxtWinHost::TxGetExtent(LPSIZEL lpExtent)
    {
        *lpExtent = sizelExtent;
        return S_OK;
    }

    HRESULT	CTxtWinHost::TxGetSelectionBarWidth(LONG* plSelBarWidth)
    {
        *plSelBarWidth = lSelBarWidth;
        return S_OK;
    }

    //////////////////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiRichEdit)

    CDuiRichEdit::CDuiRichEdit(void)
        : m_bWantTab(TRUE)
        , m_bWantReturn(TRUE)
        , m_bWantCtrlReturn(TRUE)
        , m_bRich(TRUE)
        , m_bReadOnly(FALSE)
        , m_bVScrollBarFixing(FALSE)
#ifndef _UNICODE
        , m_fAccumulateDBC(TRUE)
#else
        , m_fAccumulateDBC(FALSE)
#endif
        , m_lTwhStyle(ES_MULTILINE)
        , m_dwTipValueColor(0xFFBAC0C5)
        , m_dwTextColor(0)
        , m_iFont(-1)
        , m_iLimitText(TEXTMAX)
        , m_uTipValueAlign(DT_SINGLELINE | DT_LEFT)
        , m_uButtonState(0)
        , m_chLeadByte(0)
        , m_pTwh(NULL)
    {

    }


    CDuiRichEdit::~CDuiRichEdit(void)
    {
        if(m_pTwh)
        {
            m_pTwh->Release();
            m_pTwh = NULL;
            GetManager()->RemoveMessageFilter(this);
        }
    }

    CDuiString CDuiRichEdit::GetClass() const
    {
        return DUI_CTR_RICHEDIT;
    }

    LPVOID CDuiRichEdit::GetInterface(CDuiString strName)
    {
        if(strName == DUI_CTR_RICHEDIT)
        {
            return static_cast<CDuiRichEdit*>(this);
        }
        return CDuiContainer::GetInterface(strName);
    }

    UINT CDuiRichEdit::GetControlFlags() const
    {
        if(!IsEnabled())
        {
            return CDuiControl::GetControlFlags();
        }

        return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
    }


    long CDuiRichEdit::GetTextLength(DWORD dwFlags /*= GTL_DEFAULT*/) const
    {
        GETTEXTLENGTHEX textLenEx;
        textLenEx.flags = dwFlags;
#ifdef _UNICODE
        textLenEx.codepage = 1200;
#else
        textLenEx.codepage = CP_ACP;
#endif
        LRESULT lResult = 0;
        TxSendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&textLenEx, 0, &lResult);
        return (long)lResult;
    }

    CDuiString CDuiRichEdit::GetText() const
    {
        long lLen = GetTextLength(GTL_DEFAULT);
        LPTSTR lpText = NULL;
        GETTEXTEX gt;
        gt.flags = GT_DEFAULT;
#ifdef _UNICODE
        gt.cb = sizeof(TCHAR) * (lLen + 1) ;
        gt.codepage = 1200;
        lpText = new TCHAR[lLen + 1];
        ::ZeroMemory(lpText, (lLen + 1) * sizeof(TCHAR));
#else
        gt.cb = sizeof(TCHAR) * lLen * 2 + 1;
        gt.codepage = CP_ACP;
        lpText = new TCHAR[lLen * 2 + 1];
        ::ZeroMemory(lpText, (lLen * 2 + 1) * sizeof(TCHAR));
#endif
        gt.lpDefaultChar = NULL;
        gt.lpUsedDefChar = NULL;
        TxSendMessage(EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)lpText, 0);
        CDuiString sText(lpText);
        DUI_FREE_ARRAY(lpText);
        return sText;
    }

    void CDuiRichEdit::SetText(LPCTSTR pstrText)
    {
        CDuiContainer::SetText(pstrText);
        if(!m_pTwh)
        {
            return;
        }
        m_pTwh->SetColor(m_dwTextColor);
        SetSel(0, -1);
        ReplaceSel(pstrText, FALSE);
    }


    void CDuiRichEdit::SetEnabled(BOOL bEnabled)
    {
        if(IsEnabled() == bEnabled)
        {
            return;
        }

        if(m_pTwh)
        {
            m_pTwh->SetColor(bEnabled ? m_dwTextColor : GetManager()->GetDefaultDisabledColor());
        }

        CDuiContainer::SetEnabled(bEnabled);
    }

    LONG CDuiRichEdit::GetWinStyle()
    {
        return m_lTwhStyle;
    }

    void CDuiRichEdit::SetWinStyle(LONG lStyle)
    {
        m_lTwhStyle = lStyle;
    }

    BOOL CDuiRichEdit::IsAccumulateDBCMode()
    {
        return m_fAccumulateDBC;
    }


    void CDuiRichEdit::SetAccumulateDBCMode(BOOL bDBCMode)
    {
        m_fAccumulateDBC = bDBCMode;
    }

    BOOL CDuiRichEdit::IsMultiLine()
    {
        return (m_lTwhStyle & ES_MULTILINE) == ES_MULTILINE;
    }

    void CDuiRichEdit::SetMultiLine(BOOL bMultiLine)
    {
        if(!bMultiLine)
        {
            m_lTwhStyle &= ~ES_MULTILINE;
        }
        else
        {
            m_lTwhStyle |= ES_MULTILINE;
        }
    }

    BOOL CDuiRichEdit::IsReadOnly()
    {
        return m_bReadOnly;
    }

    void CDuiRichEdit::SetReadOnly(BOOL bReadOnly /*= TRUE*/)
    {
        m_bReadOnly = bReadOnly;
        if(m_pTwh)
        {
            m_pTwh->SetReadOnly(bReadOnly);
        }
    }

    BOOL CDuiRichEdit::IsWantTab() const
    {
        return m_bWantTab;
    }

    void CDuiRichEdit::SetWantTab(BOOL bWantTab /*= TRUE*/)
    {
        m_bWantTab = bWantTab;
    }

    BOOL CDuiRichEdit::IsWantReturn() const
    {
        return m_bWantReturn;
    }

    void CDuiRichEdit::SetWantReturn(BOOL bWantReturn /*= TRUE*/)
    {
        m_bWantReturn = bWantReturn;
    }

    BOOL CDuiRichEdit::IsWantCtrlReturn() const
    {
        return m_bWantCtrlReturn;
    }

    void CDuiRichEdit::SetWantCtrlReturn(BOOL bWantCtrlReturn /*= TRUE*/)
    {
        m_bWantCtrlReturn = bWantCtrlReturn;
    }

    BOOL CDuiRichEdit::IsRich() const
    {
        return m_bRich;
    }

    void CDuiRichEdit::SetRich(BOOL bRich /*= TRUE*/)
    {
        m_bRich = bRich;
        if(m_pTwh)
        {
            m_pTwh->SetRichTextFlag(bRich);
        }
    }

    int CDuiRichEdit::GetFont() const
    {
        return m_iFont;
    }

    void CDuiRichEdit::SetFont(int index)
    {
        m_iFont = index;
    }

    DWORD CDuiRichEdit::GetTextColor() const
    {
        return m_dwTextColor;
    }

    void CDuiRichEdit::SetTextColor(DWORD dwTextColor)
    {
        m_dwTextColor = dwTextColor;
        if(m_pTwh)
        {
            m_pTwh->SetColor(dwTextColor);
        }
    }

    int CDuiRichEdit::GetLimitText() const
    {
        return m_iLimitText;
    }

    void CDuiRichEdit::SetLimitText(int iChars)
    {
        m_iLimitText = iChars;
        if(m_pTwh)
        {
            m_pTwh->LimitText(m_iLimitText);
        }
    }

    LPCTSTR CDuiRichEdit::GetNormalImage() const
    {
        return m_sNormalImage;
    }

    void CDuiRichEdit::SetNormalImage(LPCTSTR pStrImage)
    {
        if(m_sNormalImage == pStrImage)
        {
            return;
        }
        m_sNormalImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiRichEdit::GetHotImage() const
    {
        return m_sHotImage;
    }

    void CDuiRichEdit::SetHotImage(LPCTSTR pStrImage)
    {
        if(m_sHotImage == pStrImage)
        {
            return;
        }
        m_sHotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiRichEdit::GetFocusedImage() const
    {
        return m_sFocusedImage;
    }

    void CDuiRichEdit::SetFocusedImage(LPCTSTR pStrImage)
    {
        if(m_sFocusedImage == pStrImage)
        {
            return;
        }
        m_sFocusedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiRichEdit::GetDisabledImage() const
    {
        return m_sDisabledImage;
    }

    void CDuiRichEdit::SetDisabledImage(LPCTSTR pStrImage)
    {
        if(m_sDisabledImage == pStrImage)
        {
            return;
        }
        m_sDisabledImage = pStrImage;
        Invalidate();
    }

    RECT CDuiRichEdit::GetTextPadding() const
    {
        return m_rcTextPadding;
    }

    void CDuiRichEdit::SetTextPadding(RECT rc)
    {
        if(m_rcTextPadding == rc)
        {
            return;
        }
        m_rcTextPadding = rc;
        Invalidate();
    }

    LPCTSTR CDuiRichEdit::GetTipValue() const
    {
        return m_sTipValue.GetData();
    }

    void CDuiRichEdit::SetTipValue(LPCTSTR pStrTipValue)
    {
        m_sTipValue	= pStrTipValue;
    }

    DWORD CDuiRichEdit::GetTipValueColor() const
    {
        return m_dwTipValueColor;
    }

    void CDuiRichEdit::SetTipValueColor(DWORD dwColor)
    {
        if(m_dwTipValueColor == dwColor)
        {
            return;
        }
        m_dwTipValueColor = dwColor;
    }

    UINT CDuiRichEdit::GetTipValueAlign() const
    {
        return m_uTipValueAlign;
    }

    void CDuiRichEdit::SetTipValueAlign(UINT uAlign)
    {
        m_uTipValueAlign = uAlign;
        if(GetText().IsEmpty())
        {
            Invalidate();
        }
    }

    void CDuiRichEdit::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("vscrollbar")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_lTwhStyle |= ES_DISABLENOSCROLL | WS_VSCROLL;
            }
        }
        else if(_tcsicmp(pstrName, _T("autovscroll")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_lTwhStyle |= ES_AUTOVSCROLL;
            }
        }
        else if(_tcsicmp(pstrName, _T("hscrollbar")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_lTwhStyle |= ES_DISABLENOSCROLL | WS_HSCROLL;
            }
        }
        else if(_tcsicmp(pstrName, _T("autohscroll")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_lTwhStyle |= ES_AUTOHSCROLL;
            }
        }
        else if(_tcsicmp(pstrName, _T("wanttab")) == 0)
        {
            SetWantTab(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("wantreturn")) == 0)
        {
            SetWantReturn(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("wantctrlreturn")) == 0)
        {
            SetWantCtrlReturn(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("rich")) == 0)
        {
            SetRich(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("multiline")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("false")) == 0)
            {
                m_lTwhStyle &= ~ES_MULTILINE;
            }
        }
        else if(_tcsicmp(pstrName, _T("readonly")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_lTwhStyle |= ES_READONLY;
                m_bReadOnly = true;
            }
        }
        else if(_tcsicmp(pstrName, _T("password")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_lTwhStyle |= ES_PASSWORD;
            }
        }
        else if(_tcsicmp(pstrName, _T("align")) == 0)
        {
            if(_tcsstr(pstrValue, _T("left")) != NULL)
            {
                m_lTwhStyle &= ~(ES_CENTER | ES_RIGHT);
                m_lTwhStyle |= ES_LEFT;
            }
            if(_tcsstr(pstrValue, _T("center")) != NULL)
            {
                m_lTwhStyle &= ~(ES_LEFT | ES_RIGHT);
                m_lTwhStyle |= ES_CENTER;
            }
            if(_tcsstr(pstrValue, _T("right")) != NULL)
            {
                m_lTwhStyle &= ~(ES_LEFT | ES_CENTER);
                m_lTwhStyle |= ES_RIGHT;
            }
        }
        else if(_tcsicmp(pstrName, _T("font")) == 0)
        {
            SetFont(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("textcolor")) == 0)
        {
            while(*pstrValue > _T('\0') && *pstrValue <= _T(' '))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("maxchar")) == 0)
        {
            SetLimitText(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("normalimage")) == 0)
        {
            SetNormalImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("hotimage")) == 0)
        {
            SetHotImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("focusedimage")) == 0)
        {
            SetFocusedImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("disabledimage")) == 0)
        {
            SetDisabledImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("textpadding")) == 0)
        {
            RECT rcTextPadding = { 0 };
            LPTSTR pstr = NULL;
            rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetTextPadding(rcTextPadding);
        }
        else if(_tcsicmp(pstrName, _T("tipvalue")) == 0)
        {
            SetTipValue(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("tipvaluecolor")) == 0)
        {
            if(*pstrName == _T('#'))
            {
                pstrName = ::CharNext(pstrName);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrName, &pstr, 16);
            SetTipValueColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("tipvaluealign")) == 0)
        {
            if(_tcsstr(pstrValue, _T("left")) != NULL)
            {
                m_uTipValueAlign = DT_SINGLELINE | DT_LEFT;
            }
            if(_tcsstr(pstrValue, _T("center")) != NULL)
            {
                m_uTipValueAlign = DT_SINGLELINE | DT_CENTER;
            }
            if(_tcsstr(pstrValue, _T("right")) != NULL)
            {
                m_uTipValueAlign = DT_SINGLELINE | DT_RIGHT;
            }
        }
        else
        {
            CDuiContainer::SetAttribute(pstrName, pstrValue);
        }
    }

    // 多行非rich格式的richedit有一个滚动条bug，在最后一行是空行时，LineDown和SetScrollPos无法滚动到最后
    // 引入iPos就是为了修正这个bug

    void CDuiRichEdit::SetScrollPos(SIZE szPos, BOOL bMsg /*= TRUE*/)
    {
        int cx = 0;
        int cy = 0;
        if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
        {
            int iLastScrollPos = GetVerticalScrollBar()->GetScrollPos();
            GetVerticalScrollBar()->SetScrollPos(szPos.cy);
            cy = GetVerticalScrollBar()->GetScrollPos() - iLastScrollPos;
        }
        if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
        {
            int iLastScrollPos = GetHorizontalScrollBar()->GetScrollPos();
            GetHorizontalScrollBar()->SetScrollPos(szPos.cx);
            cx = GetHorizontalScrollBar()->GetScrollPos() - iLastScrollPos;
        }
        if(cy != 0)
        {
            int iPos = 0;
            if(m_pTwh && !m_bRich && GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
            {
                iPos = GetVerticalScrollBar()->GetScrollPos();
            }
            WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, GetVerticalScrollBar()->GetScrollPos());
            TxSendMessage(WM_VSCROLL, wParam, 0L, 0);
            if(m_pTwh && !m_bRich && GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
            {
                if(cy > 0 && GetVerticalScrollBar()->GetScrollPos() <= iPos)
                {
                    GetVerticalScrollBar()->SetScrollPos(iPos);
                }
            }
        }
        if(cx != 0)
        {
            WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, GetHorizontalScrollBar()->GetScrollPos());
            TxSendMessage(WM_HSCROLL, wParam, 0L, 0);
        }
    }


    void CDuiRichEdit::LineUp()
    {
        TxSendMessage(WM_VSCROLL, SB_LINEUP, 0L, 0);
    }

    void CDuiRichEdit::LineDown()
    {
        int iPos = 0;
        if(m_pTwh && !m_bRich && GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
        {
            iPos = GetVerticalScrollBar()->GetScrollPos();
        }
        TxSendMessage(WM_VSCROLL, SB_LINEDOWN, 0L, 0);
        if(m_pTwh && !m_bRich && GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
        {
            if(GetVerticalScrollBar()->GetScrollPos() <= iPos)
            {
                GetVerticalScrollBar()->SetScrollPos(GetVerticalScrollBar()->GetScrollRange());
            }
        }
    }


    void CDuiRichEdit::PageUp()
    {
        TxSendMessage(WM_VSCROLL, SB_PAGEUP, 0L, 0);
    }


    void CDuiRichEdit::PageDown()
    {
        TxSendMessage(WM_VSCROLL, SB_PAGEDOWN, 0L, 0);
    }


    void CDuiRichEdit::HomeUp()
    {
        TxSendMessage(WM_VSCROLL, SB_TOP, 0L, 0);
    }


    void CDuiRichEdit::EndDown()
    {
        TxSendMessage(WM_VSCROLL, SB_BOTTOM, 0L, 0);
    }


    void CDuiRichEdit::LineLeft()
    {
        TxSendMessage(WM_HSCROLL, SB_LINELEFT, 0L, 0);
    }


    void CDuiRichEdit::LineRight()
    {
        TxSendMessage(WM_HSCROLL, SB_LINERIGHT, 0L, 0);
    }


    void CDuiRichEdit::PageLeft()
    {
        TxSendMessage(WM_HSCROLL, SB_PAGELEFT, 0L, 0);
    }


    void CDuiRichEdit::PageRight()
    {
        TxSendMessage(WM_HSCROLL, SB_PAGERIGHT, 0L, 0);
    }


    void CDuiRichEdit::HomeLeft()
    {
        TxSendMessage(WM_HSCROLL, SB_LEFT, 0L, 0);
    }


    void CDuiRichEdit::EndRight()
    {
        TxSendMessage(WM_HSCROLL, SB_RIGHT, 0L, 0);
    }

    void CDuiRichEdit::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::SetPos(rc, bNeedInvalidate);
        rc = GetPos();

        rc.left += GetInset().left;
        rc.top += GetInset().top;
        rc.right -= GetInset().right;
        rc.bottom -= GetInset().bottom;
        BOOL bVScrollBarVisiable = FALSE;
        if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
        {
            bVScrollBarVisiable = TRUE;
            rc.right -= GetVerticalScrollBar()->GetFixedWidth();
        }
        if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
        {
            rc.bottom -= GetHorizontalScrollBar()->GetFixedHeight();
        }

        if(m_pTwh)
        {
            RECT rcRich = rc;
            rcRich.left += m_rcTextPadding.left;
            rcRich.right -= m_rcTextPadding.right;
            rcRich.top += m_rcTextPadding.top;
            rcRich.bottom -= m_rcTextPadding.bottom;
            m_pTwh->SetClientRect(&rcRich);
            if(bVScrollBarVisiable && (!GetVerticalScrollBar()->IsVisible() || m_bVScrollBarFixing))
            {
                LONG lWidth = rcRich.right - rcRich.left + GetVerticalScrollBar()->GetFixedWidth();
                LONG lHeight = 0;
                SIZEL szExtent = { -1, -1 };
                m_pTwh->GetTextServices()->TxGetNaturalSize(
                    DVASPECT_CONTENT,
                    GetManager()->GetPaintDC(),
                    NULL,
                    NULL,
                    TXTNS_FITTOCONTENT,
                    &szExtent,
                    &lWidth,
                    &lHeight);
                if(lHeight > rcRich.bottom - rcRich.top)
                {
                    GetVerticalScrollBar()->SetVisible(true);
                    GetVerticalScrollBar()->SetScrollPos(0);
                    m_bVScrollBarFixing = true;
                }
                else
                {
                    if(m_bVScrollBarFixing)
                    {
                        GetVerticalScrollBar()->SetVisible(false);
                        m_bVScrollBarFixing = false;
                    }
                }
            }
        }

        if(GetVerticalScrollBar() != NULL && GetVerticalScrollBar()->IsVisible())
        {
            RECT rcScrollBarPos = { rc.right, rc.top, rc.right + GetVerticalScrollBar()->GetFixedWidth(), rc.bottom};
            GetVerticalScrollBar()->SetPos(rcScrollBarPos);
        }
        if(GetHorizontalScrollBar() != NULL && GetHorizontalScrollBar()->IsVisible())
        {
            RECT rcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + GetHorizontalScrollBar()->GetFixedHeight()};
            GetHorizontalScrollBar()->SetPos(rcScrollBarPos);
        }

        SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
        if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
        {
            szAvailable.cx += GetHorizontalScrollBar()->GetScrollRange();
        }

        int nAdjustables = 0;
        int cxFixed = 0;
        int nEstimateNum = 0;
        for(int it1 = 0; it1 < GetCount(); it1++)
        {
            CDuiControl* pControl = GetItemAt(it1);
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                continue;
            }
            SIZE sz = pControl->EstimateSize(szAvailable);
            if(sz.cx == 0)
            {
                nAdjustables++;
            }
            else
            {
                if(sz.cx < pControl->GetMinWidth())
                {
                    sz.cx = pControl->GetMinWidth();
                }
                if(sz.cx > pControl->GetMaxWidth())
                {
                    sz.cx = pControl->GetMaxWidth();
                }
            }
            cxFixed += sz.cx +  pControl->GetPadding().left + pControl->GetPadding().right;
            nEstimateNum++;
        }
        cxFixed += (nEstimateNum - 1) * GetChildPadding();

        int cxExpand = 0;
        int cxNeeded = 0;
        if(nAdjustables > 0)
        {
            cxExpand = MAX(0, (szAvailable.cx - cxFixed) / nAdjustables);
        }
        // Position the elements
        SIZE szRemaining = szAvailable;
        int iPosX = rc.left;
        if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
        {
            iPosX -= GetHorizontalScrollBar()->GetScrollPos();
        }
        int iAdjustable = 0;
        int cxFixedRemaining = cxFixed;
        for(int it2 = 0; it2 < GetCount(); it2++)
        {
            CDuiControl* pControl = GetItemAt(it2);
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                SetFloatPos(it2);
                continue;
            }
            RECT rcPadding = pControl->GetPadding();
            szRemaining.cx -= rcPadding.left;
            SIZE sz = pControl->EstimateSize(szRemaining);
            if(sz.cx == 0)
            {
                iAdjustable++;
                sz.cx = cxExpand;

                if(sz.cx < pControl->GetMinWidth())
                {
                    sz.cx = pControl->GetMinWidth();
                }
                if(sz.cx > pControl->GetMaxWidth())
                {
                    sz.cx = pControl->GetMaxWidth();
                }
            }
            else
            {
                if(sz.cx < pControl->GetMinWidth())
                {
                    sz.cx = pControl->GetMinWidth();
                }
                if(sz.cx > pControl->GetMaxWidth())
                {
                    sz.cx = pControl->GetMaxWidth();
                }

            }

            sz.cy = pControl->GetFixedHeight();
            if(sz.cy == 0)
            {
                sz.cy = rc.bottom - rc.top - rcPadding.top - rcPadding.bottom;
            }
            if(sz.cy < 0)
            {
                sz.cy = 0;
            }
            if(sz.cy < pControl->GetMinHeight())
            {
                sz.cy = pControl->GetMinHeight();
            }
            if(sz.cy > pControl->GetMaxHeight())
            {
                sz.cy = pControl->GetMaxHeight();
            }

            RECT rcCtrl = { iPosX + rcPadding.left, rc.top + rcPadding.top, iPosX + sz.cx + rcPadding.left , rc.top + rcPadding.top + sz.cy};
            pControl->SetPos(rcCtrl, true);
            iPosX += sz.cx + GetChildPadding() + rcPadding.left + rcPadding.right;
            cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
            szRemaining.cx -= sz.cx + GetChildPadding() + rcPadding.right;
        }
        cxNeeded += (nEstimateNum - 1) * GetChildPadding();
        if(GetHorizontalScrollBar() != NULL)
        {
            if(cxNeeded > rc.right - rc.left)
            {
                if(GetHorizontalScrollBar()->IsVisible())
                {
                    GetHorizontalScrollBar()->SetScrollRange(cxNeeded - (rc.right - rc.left));
                }
                else
                {
                    GetHorizontalScrollBar()->SetVisible(true);
                    GetHorizontalScrollBar()->SetScrollRange(cxNeeded - (rc.right - rc.left));
                    GetHorizontalScrollBar()->SetScrollPos(0);
                    rc.bottom -= GetHorizontalScrollBar()->GetFixedHeight();
                }
            }
        }
    }


    void CDuiRichEdit::Move(SIZE szOffset, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiContainer::Move(szOffset, bNeedInvalidate);
        if(m_pTwh != NULL)
        {
            RECT rc = GetPos();
            rc.left += GetInset().left;
            rc.top += GetInset().top;
            rc.right -= GetInset().right;
            rc.bottom -= GetInset().bottom;

            if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
            {
                rc.right -= GetVerticalScrollBar()->GetFixedWidth();
            }
            if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
            {
                rc.bottom -= GetHorizontalScrollBar()->GetFixedHeight();
            }
            m_pTwh->SetClientRect(&rc);
        }
    }

    void CDuiRichEdit::DoEvent(CDuiEvent& event)
    {
        if(!IsMouseEnabled() && event.Type > DUIEVENT__MOUSEBEGIN && event.Type < DUIEVENT__MOUSEEND)
        {
            if(GetParent() != NULL)
            {
                GetParent()->DoEvent(event);
            }
            else
            {
                CDuiContainer::DoEvent(event);
            }
            return;
        }
        if(event.Type == DUIEVENT_SETCURSOR && IsEnabled())
        {
            if(m_pTwh && m_pTwh->DoSetCursor(NULL, &event.ptMouse))
            {
                return;
            }
        }
        else if(event.Type == DUIEVENT_SETFOCUS)
        {
            if(m_pTwh)
            {
                m_pTwh->OnTxInPlaceActivate(NULL);
                m_pTwh->GetTextServices()->TxSendMessage(WM_SETFOCUS, 0, 0, 0);
            }
            SetFocused(TRUE);
            Invalidate();
            return;
        }
        else if(event.Type == DUIEVENT_KILLFOCUS)
        {
            if(m_pTwh)
            {
                m_pTwh->OnTxInPlaceActivate(NULL);
                m_pTwh->GetTextServices()->TxSendMessage(WM_KILLFOCUS, 0, 0, 0);
            }
            SetFocused(FALSE);
            Invalidate();
            return;
        }
        else if(event.Type == DUIEVENT_TIMER)
        {
            if(m_pTwh)
            {
                m_pTwh->GetTextServices()->TxSendMessage(WM_TIMER, event.wParam, event.lParam, 0);
            }
            return;
        }
        else if(event.Type == DUIEVENT_SCROLLWHEEL)
        {
            if((event.wKeyState & MK_CONTROL) != 0)
            {
                return;
            }
        }
        else if(event.Type == DUIEVENT_BUTTONDOWN || event.Type == DUIEVENT_DBLCLICK)
        {
            return;
        }
        else if(event.Type == DUIEVENT_MOUSEMOVE)
        {
            return;
        }
        else if(event.Type == DUIEVENT_BUTTONUP)
        {
            return;
        }
        else if(event.Type == DUIEVENT_MOUSEENTER)
        {
            if(IsEnabled())
            {
                m_uButtonState |= UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        else if(event.Type == DUIEVENT_MOUSELEAVE)
        {
            if(IsEnabled())
            {
                m_uButtonState &= ~UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        if(event.Type > DUIEVENT__KEYBEGIN && event.Type < DUIEVENT__KEYEND)
        {
            return;
        }
        CDuiContainer::DoEvent(event);
    }


    void CDuiRichEdit::PaintStatusImage(HDC hDC)
    {
        if(IsFocused())
        {
            m_uButtonState |= UISTATE_FOCUSED;
        }
        else
        {
            m_uButtonState &= ~ UISTATE_FOCUSED;
        }
        if(!IsEnabled())
        {
            m_uButtonState |= UISTATE_DISABLED;
        }
        else
        {
            m_uButtonState &= ~ UISTATE_DISABLED;
        }

        if((m_uButtonState & UISTATE_DISABLED) != 0)
        {
            if(!m_sDisabledImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sDisabledImage)) {}
                else
                {
                    return;
                }
            }
        }
        else if((m_uButtonState & UISTATE_FOCUSED) != 0)
        {
            if(!m_sFocusedImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sFocusedImage)) {}
                else
                {
                    return;
                }
            }
        }
        else if((m_uButtonState & UISTATE_HOT) != 0)
        {
            if(!m_sHotImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sHotImage)) {}
                else
                {
                    return;
                }
            }
        }

        if(!m_sNormalImage.IsEmpty())
        {
            if(!DrawImage(hDC, (LPCTSTR)m_sNormalImage)) {}
            else
            {
                return;
            }
        }
    }

    void CDuiRichEdit::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        RECT rcTemp = { 0 };
        if(!::IntersectRect(&rcTemp, &rcPaint, &GetPos()))
        {
            return;
        }

        CRenderClip clip;
        CRenderClip::GenerateClip(hDC, rcTemp, clip);
        CDuiControl::DoPaint(hDC, rcPaint);

        if(m_pTwh)
        {
            RECT rc;
            m_pTwh->GetControlRect(&rc);
            // Remember wparam is actually the hdc and lparam is the update
            // rect because this message has been preprocessed by the window.
            m_pTwh->GetTextServices()->TxDraw(
                DVASPECT_CONTENT,  		// Draw Aspect
                /*-1*/0,				// Lindex
                NULL,					// Info for drawing optimazation
                NULL,					// target device information
                hDC,			        // Draw device HDC
                NULL, 				   	// Target device HDC
                (RECTL*)&rc,			// Bounding client rectangle
                NULL, 		            // Clipping rectangle for metafiles
                (RECT*)&rcPaint,		// Update rectangle
                NULL, 	   				// Call back function
                NULL,					// Call back parameter
                0);				        // What view of the object
            if(m_bVScrollBarFixing)
            {
                LONG lWidth = rc.right - rc.left + GetVerticalScrollBar()->GetFixedWidth();
                LONG lHeight = 0;
                SIZEL szExtent = { -1, -1 };
                m_pTwh->GetTextServices()->TxGetNaturalSize(
                    DVASPECT_CONTENT,
                    GetManager()->GetPaintDC(),
                    NULL,
                    NULL,
                    TXTNS_FITTOCONTENT,
                    &szExtent,
                    &lWidth,
                    &lHeight);
                if(lHeight <= rc.bottom - rc.top)
                {
                    NeedUpdate();
                }
            }
        }

        if(GetCount() > 0)
        {
            RECT rc = GetPos();
            rc.left += GetInset().left;
            rc.top += GetInset().top;
            rc.right -= GetInset().right;
            rc.bottom -= GetInset().bottom;
            if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
            {
                rc.right -= GetVerticalScrollBar()->GetFixedWidth();
            }
            if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
            {
                rc.bottom -= GetHorizontalScrollBar()->GetFixedHeight();
            }

            if(!::IntersectRect(&rcTemp, &rcPaint, &rc))
            {
                for(int it = 0; it < GetCount(); it++)
                {
                    CDuiControl* pControl = GetItemAt(it);
                    if(!pControl->IsVisible())
                    {
                        continue;
                    }
                    if(!::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()))
                    {
                        continue;
                    }
                    if(pControl ->IsFloat())
                    {
                        if(!::IntersectRect(&rcTemp, &GetPos(), &pControl->GetPos()))
                        {
                            continue;
                        }
                        pControl->DoPaint(hDC, rcPaint);
                    }
                }
            }
            else
            {
                CRenderClip childClip;
                CRenderClip::GenerateClip(hDC, rcTemp, childClip);
                for(int it = 0; it < GetCount(); it++)
                {
                    CDuiControl* pControl = GetItemAt(it);
                    if(!pControl->IsVisible())
                    {
                        continue;
                    }
                    if(!::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()))
                    {
                        continue;
                    }
                    if(pControl ->IsFloat())
                    {
                        if(!::IntersectRect(&rcTemp, &GetPos(), &pControl->GetPos()))
                        {
                            continue;
                        }
                        CRenderClip::UseOldClipBegin(hDC, childClip);
                        pControl->DoPaint(hDC, rcPaint);
                        CRenderClip::UseOldClipEnd(hDC, childClip);
                    }
                    else
                    {
                        if(!::IntersectRect(&rcTemp, &rc, &pControl->GetPos()))
                        {
                            continue;
                        }
                        pControl->DoPaint(hDC, rcPaint);
                    }
                }
            }
        }

        if(GetVerticalScrollBar() != NULL && GetVerticalScrollBar()->IsVisible())
        {
            if(::IntersectRect(&rcTemp, &rcPaint, &GetVerticalScrollBar()->GetPos()))
            {
                GetVerticalScrollBar()->DoPaint(hDC, rcPaint);
            }
        }

        if(GetHorizontalScrollBar() != NULL && GetHorizontalScrollBar()->IsVisible())
        {
            if(::IntersectRect(&rcTemp, &rcPaint, &GetHorizontalScrollBar()->GetPos()))
            {
                GetHorizontalScrollBar()->DoPaint(hDC, rcPaint);
            }
        }
        // 绘制提示文字
        CDuiString sDrawText = GetText();
        if(sDrawText.IsEmpty() && !IsFocused())
        {
            DWORD dwTextColor = GetTipValueColor();
            CDuiString sTipValue = GetTipValue();
            RECT rc = GetPos();
            rc.left += GetTextPadding().left;
            rc.right -= GetTextPadding().right;
            rc.top += GetTextPadding().top;
            rc.bottom -= GetTextPadding().bottom;
            UINT uTextAlign = GetTipValueAlign();
            if(IsMultiLine())
            {
                uTextAlign |= DT_TOP;
            }
            else
            {
                uTextAlign |= DT_VCENTER;
            }
            CRenderEngine::DrawText(hDC, GetManager(), rc, sTipValue, dwTextColor, m_iFont, uTextAlign);
        }
    }

    void CDuiRichEdit::DoInit()
    {
        CREATESTRUCT cs;
        cs.style = m_lTwhStyle;
        cs.x = 0;
        cs.y = 0;
        cs.cy = 0;
        cs.cx = 0;
        CDuiString strText = CDuiControl::GetText();
        cs.lpszName = strText.GetData();
        if(m_pTwh == NULL)
        {
            m_pTwh = new CTxtWinHost();
            if(m_pTwh && m_pTwh->Init(this, &cs))
            {
                m_pTwh->SetTransparent(TRUE);
                LRESULT lResult = 0;
                m_pTwh->GetTextServices()->TxSendMessage(EM_SETLANGOPTIONS, 0, 0, &lResult);
                m_pTwh->GetTextServices()->TxSendMessage(EM_SETEVENTMASK, 0, ENM_DROPFILES | ENM_LINK | ENM_CHANGE, &lResult);
                m_pTwh->OnTxInPlaceActivate(NULL);
                GetManager()->AddMessageFilter(this);
                if(!IsEnabled())
                {
                    m_pTwh->SetColor(GetManager()->GetDefaultDisabledColor());
                }
            }
        }
    }

    BOOL CDuiRichEdit::OnTxViewChanged(BOOL bUpdate)
    {
        return TRUE;
    }

    void CDuiRichEdit::OnTxNotify(DWORD iNotify, void* pv)
    {
        switch(iNotify)
        {
            case EN_CHANGE:
            {
                GetManager()->SendNotify(this, DUI_MSGTYPE_TEXTCHANGED);
                break;
            }
            case EN_DROPFILES:
            case EN_MSGFILTER:
            case EN_OLEOPFAILED:
            case EN_PROTECTED:
            case EN_SAVECLIPBOARD:
            case EN_SELCHANGE:
            case EN_STOPNOUNDO:
            case EN_LINK:
            case EN_OBJECTPOSITIONS:
            case EN_DRAGDROPDONE:
            {
                if(pv)                        // Fill out NMHDR portion of pv
                {
                    LONG nId =  GetWindowLong(GetManager()->GetPaintWindow(), GWL_ID);
                    NMHDR*  phdr = (NMHDR*)pv;
                    phdr->hwndFrom = GetManager()->GetPaintWindow();
                    phdr->idFrom = nId;
                    phdr->code = iNotify;

                    if(SendMessage(GetManager()->GetPaintWindow(), WM_NOTIFY, (WPARAM) nId, (LPARAM) pv))
                    {
                        //hr = S_FALSE;
                    }
                }
            }
            break;
        }
    }


    HRESULT CDuiRichEdit::TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT* plresult) const
    {
        if(m_pTwh)
        {
            if(msg == WM_KEYDOWN && TCHAR(wparam) == VK_RETURN)
            {
                if(!m_bWantReturn || (::GetKeyState(VK_CONTROL) < 0 && !m_bWantCtrlReturn))
                {
                    if(GetManager() != NULL)
                    {
                        GetManager()->SendNotify((CDuiControl*)this, DUI_MSGTYPE_RETURN);
                    }
                    return S_OK;
                }
            }
            return m_pTwh->GetTextServices()->TxSendMessage(msg, wparam, lparam, plresult);
        }
        return S_FALSE;
    }

    BOOL CDuiRichEdit::GetModify() const
    {
        if(!m_pTwh)
        {
            return FALSE;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_GETMODIFY, 0, 0, &lResult);
        return (BOOL)lResult == TRUE;
    }

    void CDuiRichEdit::SetModify(BOOL bModified /*= TRUE*/) const
    {
        TxSendMessage(EM_SETMODIFY, bModified, 0, 0);
    }

    BOOL CDuiRichEdit::CanUndo()
    {
        if(!m_pTwh)
        {
            return FALSE;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_CANUNDO, 0, 0, &lResult);
        return (BOOL)lResult == TRUE;
    }

    BOOL CDuiRichEdit::CanRedo()
    {
        if(!m_pTwh)
        {
            return FALSE;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_CANREDO, 0, 0, &lResult);
        return (BOOL)lResult == TRUE;
    }

    BOOL CDuiRichEdit::CanPaste()
    {
        if(!m_pTwh)
        {
            return FALSE;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_CANPASTE, 0, 0, &lResult);
        return (BOOL)lResult == TRUE;
    }

    BOOL CDuiRichEdit::Redo()
    {
        if(!m_pTwh)
        {
            return FALSE;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_REDO, 0, 0, &lResult);
        return (BOOL)lResult == TRUE;
    }

    BOOL CDuiRichEdit::Undo()
    {
        if(!m_pTwh)
        {
            return FALSE;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_UNDO, 0, 0, &lResult);
        return (BOOL)lResult == TRUE;
    }


    void CDuiRichEdit::Clear()
    {
        SetSel(0, -1);
        TxSendMessage(WM_CLEAR, 0, 0, 0);
    }

    void CDuiRichEdit::Copy()
    {
        TxSendMessage(WM_COPY, 0, 0, 0);
    }

    void CDuiRichEdit::Cut()
    {
        TxSendMessage(WM_CUT, 0, 0, 0);
    }

    void CDuiRichEdit::Paste()
    {
        TxSendMessage(WM_PASTE, 0, 0, 0);
    }


    WORD CDuiRichEdit::GetSelectionType() const
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_SELECTIONTYPE, 0, 0, &lResult);
        return (WORD)lResult;
    }


    CDuiString CDuiRichEdit::GetSelText() const
    {
        if(!m_pTwh)
        {
            return CDuiString();
        }
        CHARRANGE cr;
        cr.cpMin = cr.cpMax = 0;
        TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0);
        LPWSTR lpText = NULL;
        lpText = new WCHAR[cr.cpMax - cr.cpMin + 1];
        ::ZeroMemory(lpText, (cr.cpMax - cr.cpMin + 1) * sizeof(WCHAR));
        TxSendMessage(EM_GETSELTEXT, 0, (LPARAM)lpText, 0);
        CDuiString sText;
        sText = (LPCWSTR)lpText;
        DUI_FREE_ARRAY(lpText);
        return sText;
    }


    CDuiString CDuiRichEdit::GetTextRange(long nStartChar, long nEndChar) const
    {
        TEXTRANGEW tr = { 0 };
        tr.chrg.cpMin = nStartChar;
        tr.chrg.cpMax = nEndChar;
        LPWSTR lpText = NULL;
        lpText = new WCHAR[nEndChar - nStartChar + 1];
        ::ZeroMemory(lpText, (nEndChar - nStartChar + 1) * sizeof(WCHAR));
        tr.lpstrText = lpText;
        TxSendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr, 0);
        CDuiString sText;
        sText = (LPCWSTR)lpText;
        DUI_FREE_ARRAY(lpText);
        return sText;
    }


    int CDuiRichEdit::InsertText(long nInsertAfterChar, LPCTSTR lpstrText, BOOL bCanUndo /*= FALSE*/)
    {
        int nRet = SetSel(nInsertAfterChar, nInsertAfterChar);
        ReplaceSel(lpstrText, bCanUndo);
        return nRet;
    }

    int CDuiRichEdit::AppendText(LPCTSTR lpstrText, BOOL bCanUndo /*= FALSE*/)
    {
        int nRet = SetSel(-1, -1);
        ReplaceSel(lpstrText, bCanUndo);
        return nRet;
    }

    void CDuiRichEdit::HideSelection(BOOL bHide /*= TRUE*/, BOOL bChangeStyle /*= FALSE*/)
    {
        TxSendMessage(EM_HIDESELECTION, bHide, bChangeStyle, 0);
    }


    void CDuiRichEdit::ScrollCaret()
    {
        TxSendMessage(EM_SCROLLCARET, 0, 0, 0);
    }

    void CDuiRichEdit::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo)
    {
#ifdef _UNICODE
        TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText, 0);
#else
        int iLen = _tcslen(lpszNewText);
        LPWSTR lpText = new WCHAR[iLen + 1];
        ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
        ::MultiByteToWideChar(CP_ACP, 0, lpszNewText, -1, (LPWSTR)lpText, iLen) ;
        TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpText, 0);
        DUI_FREE_ARRAY(lpText);
#endif
    }

    void CDuiRichEdit::GetSel(CHARRANGE& cr) const
    {
        TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0);
    }

    int CDuiRichEdit::SetSel(CHARRANGE& cr)
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult);
        return (int)lResult;
    }

    void CDuiRichEdit::GetSel(long& nStartChar, long& nEndChar) const
    {
        CHARRANGE cr;
        TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0);
        nStartChar = cr.cpMin;
        nEndChar = cr.cpMax;
    }

    int CDuiRichEdit::SetSel(long nStartChar, long nEndChar)
    {
        CHARRANGE cr;
        cr.cpMin = nStartChar;
        cr.cpMax = nEndChar;
        LRESULT lResult = 0;
        TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult);
        return (int)lResult;
    }

    int CDuiRichEdit::SetSelAll()
    {
        return SetSel(0, -1);
    }


    int CDuiRichEdit::SetSelNone()
    {
        return SetSel(-1, 0);
    }


    BOOL CDuiRichEdit::GetZoom(int& nNum, int& nDen) const
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_GETZOOM, (WPARAM)&nNum, (LPARAM)&nDen, &lResult);
        return (BOOL)lResult == TRUE;
    }

    BOOL CDuiRichEdit::SetZoom(int nNum, int nDen)
    {
        if(nNum < 0 || nNum > 64)
        {
            return FALSE;
        }
        if(nDen < 0 || nDen > 64)
        {
            return FALSE;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_SETZOOM, nNum, nDen, &lResult);
        return (BOOL)lResult == TRUE;
    }

    BOOL CDuiRichEdit::SetZoomOff()
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_SETZOOM, 0, 0, &lResult);
        return (BOOL)lResult == TRUE;
    }


    BOOL CDuiRichEdit::GetAutoURLDetect() const
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_GETAUTOURLDETECT, 0, 0, &lResult);
        return (BOOL)lResult == TRUE;
    }

    BOOL CDuiRichEdit::SetAutoURLDetect(BOOL bAutoDetect /*= TRUE*/)
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_AUTOURLDETECT, bAutoDetect, 0, &lResult);
        return (BOOL)lResult == FALSE;
    }


    DWORD CDuiRichEdit::GetEventMask() const
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_GETEVENTMASK, 0, 0, &lResult);
        return (DWORD)lResult;
    }

    DWORD CDuiRichEdit::SetEventMask(DWORD dwEventMask)
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_SETEVENTMASK, 0, dwEventMask, &lResult);
        return (DWORD)lResult;
    }


    DWORD CDuiRichEdit::GetDefaultCharFormat(CHARFORMAT2& cf) const
    {
        cf.cbSize = sizeof(CHARFORMAT2);
        LRESULT lResult = 0;
        TxSendMessage(EM_GETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
        return (DWORD)lResult;
    }

    BOOL CDuiRichEdit::SetDefaultCharFormat(CHARFORMAT2& cf)
    {
        if(!m_pTwh)
        {
            return FALSE;
        }
        cf.cbSize = sizeof(CHARFORMAT2);
        LRESULT lResult = 0;
        TxSendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
        if((BOOL)lResult == TRUE)
        {
            CHARFORMAT2W cfw;
            cfw.cbSize = sizeof(CHARFORMAT2W);
            TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cfw, 0);
            m_pTwh->SetCharFormat(cfw);
            return TRUE;
        }
        return FALSE;
    }


    DWORD CDuiRichEdit::GetSelectionCharFormat(CHARFORMAT2& cf) const
    {
        cf.cbSize = sizeof(CHARFORMAT2);
        LRESULT lResult = 0;
        TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cf, &lResult);
        return (DWORD)lResult;
    }

    BOOL CDuiRichEdit::SetSelectionCharFormat(CHARFORMAT2& cf)
    {
        if(GetManager()->IsLayered())
        {
            CRenderEngine::CheckAlphaColor(cf.crTextColor);
            CRenderEngine::CheckAlphaColor(cf.crBackColor);
        }
        if(!m_pTwh)
        {
            return FALSE;
        }
        cf.cbSize = sizeof(CHARFORMAT2);
        LRESULT lResult = 0;
        TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &lResult);
        return (BOOL)lResult == TRUE;
    }


    BOOL CDuiRichEdit::SetWordCharFormat(CHARFORMAT2& cf)
    {
        if(!m_pTwh)
        {
            return FALSE;
        }
        cf.cbSize = sizeof(CHARFORMAT2);
        LRESULT lResult = 0;
        TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf, &lResult);
        return (BOOL)lResult == TRUE;
    }


    DWORD CDuiRichEdit::GetParaFormat(PARAFORMAT2& pf) const
    {
        pf.cbSize = sizeof(PARAFORMAT2);
        LRESULT lResult = 0;
        TxSendMessage(EM_GETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
        return (DWORD)lResult;
    }


    BOOL CDuiRichEdit::SetParaFormat(PARAFORMAT2& pf)
    {
        if(!m_pTwh)
        {
            return FALSE;
        }
        pf.cbSize = sizeof(PARAFORMAT2);
        LRESULT lResult = 0;
        TxSendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
        if((BOOL)lResult == TRUE)
        {
            m_pTwh->SetParaFormat(pf);
            return TRUE;
        }
        return FALSE;
    }

    int CDuiRichEdit::GetLineCount() const
    {
        if(!m_pTwh)
        {
            return 0;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_GETLINECOUNT, 0, 0, &lResult);
        return (int)lResult;
    }

    CDuiString CDuiRichEdit::GetLine(int nIndex, int nMaxLength) const
    {
        LPWSTR lpText = NULL;
        lpText = new WCHAR[nMaxLength + 1];
        ::ZeroMemory(lpText, (nMaxLength + 1) * sizeof(WCHAR));
        *(LPWORD)lpText = (WORD)nMaxLength;
        TxSendMessage(EM_GETLINE, nIndex, (LPARAM)lpText, 0);
        CDuiString sText;
        sText = (LPCWSTR)lpText;
        DUI_FREE_ARRAY(lpText);
        return sText;
    }

    int CDuiRichEdit::LineIndex(int nLine) const
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_LINEINDEX, nLine, 0, &lResult);
        return (int)lResult;
    }

    int CDuiRichEdit::LineLength(int nLine) const
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_LINELENGTH, nLine, 0, &lResult);
        return (int)lResult;
    }

    BOOL CDuiRichEdit::LineScroll(int nLines, int nChars)
    {
        LRESULT lResult = 0;
        TxSendMessage(EM_LINESCROLL, nChars, nLines, &lResult);
        return (BOOL)lResult == TRUE;
    }

    long CDuiRichEdit::LineFromChar(long nIndex) const
    {
        if(!m_pTwh)
        {
            return 0L;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_EXLINEFROMCHAR, 0, nIndex, &lResult);
        return (long)lResult;
    }

    CDuiPoint CDuiRichEdit::PosFromChar(UINT nChar) const
    {
        POINTL pt;
        TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, nChar, 0);
        return CDuiPoint(pt.x, pt.y);
    }

    int CDuiRichEdit::CharFromPos(CDuiPoint pt) const
    {
        POINTL ptl = {pt.x, pt.y};
        if(!m_pTwh)
        {
            return 0;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_CHARFROMPOS, 0, (LPARAM)&ptl, &lResult);
        return (int)lResult;
    }

    void CDuiRichEdit::EmptyUndoBuffer()
    {
        TxSendMessage(EM_EMPTYUNDOBUFFER, 0, 0, 0);
    }

    UINT CDuiRichEdit::SetUndoLimit(UINT nLimit)
    {
        if(!m_pTwh)
        {
            return 0;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_SETUNDOLIMIT, (WPARAM) nLimit, 0, &lResult);
        return (UINT)lResult;
    }

    long CDuiRichEdit::StreamIn(int nFormat, EDITSTREAM& es)
    {
        if(!m_pTwh)
        {
            return 0L;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_STREAMIN, nFormat, (LPARAM)&es, &lResult);
        return (long)lResult;
    }

    long CDuiRichEdit::StreamOut(int nFormat, EDITSTREAM& es)
    {
        if(!m_pTwh)
        {
            return 0L;
        }
        LRESULT lResult = 0;
        TxSendMessage(EM_STREAMOUT, nFormat, (LPARAM)&es, &lResult);
        return (long)lResult;
    }

    LRESULT CDuiRichEdit::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(!IsVisible() || !IsEnabled())
        {
            return 0;
        }
        if(!IsMouseEnabled() && uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST)
        {
            return 0;
        }
        if(uMsg == WM_MOUSEWHEEL && (LOWORD(wParam) & MK_CONTROL) == 0)
        {
            return 0;
        }

        if(uMsg == WM_IME_COMPOSITION)
        {
            // 解决微软输入法位置异常的问题
            HIMC hIMC = ImmGetContext(GetManager()->GetPaintWindow());
            if(hIMC)
            {
                POINT point;
                GetCaretPos(&point);

                COMPOSITIONFORM Composition;
                Composition.dwStyle = CFS_POINT;
                Composition.ptCurrentPos.x = point.x;
                Composition.ptCurrentPos.y = point.y;
                ImmSetCompositionWindow(hIMC, &Composition);

                ImmReleaseContext(GetManager()->GetPaintWindow(), hIMC);
            }

            return 0;
        }

        BOOL bWasHandled = TRUE;
        if((uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) || uMsg == WM_SETCURSOR)
        {
            if(!m_pTwh->IsCaptured())
            {
                switch(uMsg)
                {
                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP:
                    case WM_LBUTTONDBLCLK:
                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP:
                    {
                        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                        CDuiControl* pHover = GetManager()->FindControl(pt);
                        if(pHover != this)
                        {
                            bWasHandled = FALSE;
                            return 0;
                        }
                    }
                    break;
                }
            }
            // Mouse message only go when captured or inside rect
            DWORD dwHitResult = m_pTwh->IsCaptured() ? HITRESULT_HIT : HITRESULT_OUTSIDE;
            if(dwHitResult == HITRESULT_OUTSIDE)
            {
                RECT rc;
                m_pTwh->GetControlRect(&rc);
                POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                if(uMsg == WM_MOUSEWHEEL)
                {
                    ::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
                }
                if(::PtInRect(&rc, pt) && !GetManager()->IsCaptured())
                {
                    dwHitResult = HITRESULT_HIT;
                }
            }
            if(dwHitResult != HITRESULT_HIT)
            {
                return 0;
            }
            if(uMsg == WM_SETCURSOR)
            {
                bWasHandled = false;
            }
            else if(uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK || uMsg == WM_RBUTTONDOWN)
            {
                SetFocus();
            }
        }
#ifdef _UNICODE
        else if(uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST)
        {
#else
        else if((uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) || uMsg == WM_CHAR || uMsg == WM_IME_CHAR)
        {
#endif
            if(!IsFocused())
            {
                return 0;
            }
        }
        else if(uMsg == WM_CONTEXTMENU)
        {
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            POINT ptClient = pt;
            ::ScreenToClient(GetManager()->GetPaintWindow(), &ptClient);
            CDuiControl* pHover = GetManager()->FindControl(ptClient);
            if(pHover != this)
            {
                bWasHandled = FALSE;
                return 0;
            }

            //创建一个弹出式菜单
            HMENU hPopMenu = CreatePopupMenu();
            AppendMenu(hPopMenu, 0, ID_RICH_UNDO, _T("撤销(&U)"));
            AppendMenu(hPopMenu, 0, ID_RICH_REDO, _T("重做(&R)"));
            AppendMenu(hPopMenu, MF_SEPARATOR, 0, _T(""));
            AppendMenu(hPopMenu, 0, ID_RICH_CUT, _T("剪切(&X)"));
            AppendMenu(hPopMenu, 0, ID_RICH_COPY, _T("复制(&C)"));
            AppendMenu(hPopMenu, 0, ID_RICH_PASTE, _T("粘帖(&V)"));
            AppendMenu(hPopMenu, 0, ID_RICH_CLEAR, _T("清空(&L)"));
            AppendMenu(hPopMenu, MF_SEPARATOR, 0, _T(""));
            AppendMenu(hPopMenu, 0, ID_RICH_SELECTALL, _T("全选(&A)"));

            //初始化菜单项
            UINT uUndo = (CanUndo() ? 0 : MF_GRAYED);
            EnableMenuItem(hPopMenu, ID_RICH_UNDO, MF_BYCOMMAND | uUndo);
            UINT uRedo = (CanRedo() ? 0 : MF_GRAYED);
            EnableMenuItem(hPopMenu, ID_RICH_REDO, MF_BYCOMMAND | uRedo);
            UINT uSel = ((GetSelectionType() != SEL_EMPTY) ? 0 : MF_GRAYED);
            UINT uReadonly = IsReadOnly() ? MF_GRAYED : 0;
            EnableMenuItem(hPopMenu, ID_RICH_CUT, MF_BYCOMMAND | uSel | uReadonly);
            EnableMenuItem(hPopMenu, ID_RICH_COPY, MF_BYCOMMAND | uSel);
            EnableMenuItem(hPopMenu, ID_RICH_CLEAR, MF_BYCOMMAND | uSel | uReadonly);
            EnableMenuItem(hPopMenu, ID_RICH_PASTE, MF_BYCOMMAND | uReadonly);

            TrackPopupMenu(hPopMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, GetManager()->GetPaintWindow(), NULL);
            DestroyMenu(hPopMenu);
        }
        else if(uMsg == WM_COMMAND)
        {
            bHandled = FALSE;
            if(!IsFocused())
            {
                return 0;
            }
            UINT uCmd = (UINT)wParam;
            switch(uCmd)
            {
                case ID_RICH_UNDO:
                {
                    Undo();
                    break;
                }
                case ID_RICH_REDO:
                {
                    Redo();
                    break;
                }
                case ID_RICH_CUT:
                {
                    Cut();
                    break;
                }
                case ID_RICH_COPY:
                {
                    Copy();
                    break;
                }
                case ID_RICH_PASTE:
                {
                    Paste();
                    break;
                }
                case ID_RICH_CLEAR:
                {
                    Clear();
                    break;
                }
                case ID_RICH_SELECTALL:
                {
                    SetSelAll();
                    break;
                }
                default:
                    break;
            }
        }
        else
        {
            switch(uMsg)
            {
                case WM_HELP:
                    bWasHandled = false;
                    break;
                default:
                    return 0;
            }
        }

        if(WM_CHAR == uMsg)
        {
#ifndef _UNICODE
            // check if we are waiting for 2 consecutive WM_CHAR messages
            if(IsAccumulateDBCMode())
            {
                if((GetKeyState(VK_KANA) & 0x1))
                {
                    // turn off accumulate mode
                    SetAccumulateDBCMode(FALSE);
                    m_chLeadByte = 0;
                }
                else
                {
                    if(!m_chLeadByte)
                    {
                        // This is the first WM_CHAR message,
                        // accumulate it if this is a LeadByte.  Otherwise, fall thru to
                        // regular WM_CHAR processing.
                        if(IsDBCSLeadByte((WORD)wParam))
                        {
                            // save the Lead Byte and don't process this message
                            m_chLeadByte = (WORD)wParam << 8 ;

                            //TCHAR a = (WORD)wParam << 8 ;
                            return 0;
                        }
                    }
                    else
                    {
                        // This is the second WM_CHAR message,
                        // combine the current byte with previous byte.
                        // This DBC will be handled as WM_IME_CHAR.
                        wParam |= m_chLeadByte;
                        uMsg = WM_IME_CHAR;

                        // setup to accumulate more WM_CHAR
                        m_chLeadByte = 0;
                    }
                }
            }
#endif
        }

        LRESULT lResult = 0;
        HRESULT Hr = TxSendMessage(uMsg, wParam, lParam, &lResult);
        if(Hr == S_OK)
        {
            bHandled = bWasHandled;
        }
        else if((uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) || uMsg == WM_CHAR || uMsg == WM_IME_CHAR)
        {
            bHandled = bWasHandled;
        }
        else if(uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST)
        {
            if(m_pTwh->IsCaptured())
            {
                bHandled = bWasHandled;
            }
        }
        return lResult;
    }


}