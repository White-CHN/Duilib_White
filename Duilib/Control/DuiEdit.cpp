#include "StdAfx.h"
#include "DuiEdit.h"

namespace DuiLib
{
    CDuiEditWnd::CDuiEditWnd(void)
        : m_bInit(FALSE)
        , m_bDrawCaret(FALSE)
        , m_hBkBrush(NULL)
        , m_pOwner(NULL)
    {

    }

    CDuiEditWnd::~CDuiEditWnd(void)
    {
        if(m_hBkBrush != NULL)
        {
            DeleteObject(m_hBkBrush);
            m_hBkBrush = NULL;
        }
    }

    LPCTSTR CDuiEditWnd::GetWindowClassName() const
    {
        return _T("CDuiEditWnd");
    }

    LPCTSTR CDuiEditWnd::GetSuperClassName() const
    {
        return WC_EDIT;
    }

    RECT CDuiEditWnd::CalPos()
    {
        if(m_pOwner == NULL)
        {
            CDuiRect rcPos;
            return rcPos;
        }
        CDuiRect rcPos = m_pOwner->GetPos();
        RECT rcInset = m_pOwner->GetTextPadding();
        rcPos.left += rcInset.left;
        rcPos.top += rcInset.top;
        rcPos.right -= rcInset.right;
        rcPos.bottom -= rcInset.bottom;
        LONG lEditHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
        if(lEditHeight < rcPos.GetHeight())
        {
            rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
            rcPos.bottom = rcPos.top + lEditHeight;
        }
        CDuiControl* pParent = m_pOwner;
        RECT rcParent = {0};
        while(pParent = pParent->GetParent())
        {
            if(!pParent->IsVisible())
            {
                rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
                break;
            }
            rcParent = pParent->GetClientPos();
            if(!::IntersectRect(&rcPos, &rcPos, &rcParent))
            {
                rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
                break;
            }
        }

        return rcPos;
    }

    BOOL CDuiEditWnd::Init(CDuiEdit* pOwner)
    {
        m_pOwner = pOwner;
        RECT rcPos = CalPos();
        UINT uStyle = 0;
        if(m_pOwner->GetManager()->IsLayered())
        {
            uStyle = WS_POPUP | ES_AUTOHSCROLL | WS_VISIBLE;
            RECT rcWnd = {0};
            ::GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWnd);
            rcPos.left += rcWnd.left;
            rcPos.right += rcWnd.left;
            rcPos.top += rcWnd.top - 1;
            rcPos.bottom += rcWnd.top - 1;
        }
        else
        {
            uStyle = WS_CHILD | ES_AUTOHSCROLL;
        }

        UINT uTextStyle = m_pOwner->GetTextStyle();
        if(uTextStyle & DT_LEFT)
        {
            uStyle |= ES_LEFT;
        }
        else if(uTextStyle & DT_CENTER)
        {
            uStyle |= ES_CENTER;
        }
        else if(uTextStyle & DT_RIGHT)
        {
            uStyle |= ES_RIGHT;
        }
        if(m_pOwner->IsPasswordMode())
        {
            uStyle |= ES_PASSWORD;
        }
        HWND hWnd = Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);

        HFONT hFont = NULL;
        int iFontIndex = m_pOwner->GetFont();
        if(iFontIndex != -1)
        {
            hFont = m_pOwner->GetManager()->GetFont(iFontIndex);
        }
        if(hFont == NULL)
        {
            hFont = m_pOwner->GetManager()->GetDefaultFontInfo()->hFont;
        }

        SetWindowFont(hWnd, hFont, TRUE);
        Edit_LimitText(hWnd, m_pOwner->GetMaxChar());
        if(m_pOwner->IsPasswordMode())
        {
            Edit_SetPasswordChar(hWnd, m_pOwner->GetPasswordChar());
        }
        Edit_SetText(hWnd, m_pOwner->GetText());
        Edit_SetModify(hWnd, FALSE);
        SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
        Edit_Enable(hWnd, m_pOwner->IsEnabled());
        Edit_SetReadOnly(hWnd, m_pOwner->IsReadOnly());

        //Styls
        LONG styleValue = ::GetWindowLong(hWnd, GWL_STYLE);
        styleValue |= pOwner->GetWindowStyls();
        ::SetWindowLong(hWnd, GWL_STYLE, styleValue);
        ::ShowWindow(hWnd, SW_SHOWNOACTIVATE);

        int cchLen = ::GetWindowTextLength(hWnd);
        if(cchLen <= 0)
        {
            cchLen = 1;
        }
        ::SetFocus(hWnd);
        ::SendMessage(hWnd, EM_SETSEL, 0, cchLen);
        m_bInit = TRUE;
        return TRUE;
    }

    void CDuiEditWnd::OnFinalMessage(HWND hWnd)
    {
        m_pOwner->Invalidate();
        // Clear reference and die
        if(m_hBkBrush != NULL)
        {
            ::DeleteObject(m_hBkBrush);
            m_hBkBrush = NULL;
        }
        if(m_pOwner->GetManager()->IsLayered())
        {
            m_pOwner->GetManager()->RemovePaintChildWnd(hWnd);
        }
        m_pOwner->EmptyEditWnd();
        delete this;
    }

    LRESULT CDuiEditWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(wParam == TIMERID_CARET)
        {
            m_bDrawCaret = !m_bDrawCaret;
            RECT rcClient;
            ::GetClientRect(GetHWND(), &rcClient);
            ::InvalidateRect(GetHWND(), &rcClient, FALSE);
            return 0;
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiEditWnd::OnPrint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LRESULT lRes = 0;
        if(m_pOwner->GetManager()->IsLayered())
        {
            lRes = __super::HandleMessage(uMsg, wParam, lParam);
            if(m_pOwner->IsEnabled() && m_bDrawCaret)
            {
                RECT rcClient;
                ::GetClientRect(GetHWND(), &rcClient);
                POINT ptCaret;
                ::GetCaretPos(&ptCaret);
                RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x, ptCaret.y + rcClient.bottom - rcClient.top };
                CRenderEngine::DrawLine((HDC)wParam, rcCaret, 1, 0xFF000000);
            }
            return lRes;
        }
        bHandled = FALSE;
        return lRes;
    }

    LRESULT CDuiEditWnd::OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF)
        {
            return NULL;
        }
        ::SetBkMode((HDC)wParam, TRANSPARENT);

        DWORD dwTextColor = 0 ;
        if(m_pOwner->GetNativeEditTextColor() != 0x000000)
        {
            dwTextColor = m_pOwner->GetNativeEditTextColor();
        }
        else
        {
            dwTextColor = m_pOwner->GetTextColor();
        }

        ::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));
        if(m_hBkBrush == NULL)
        {
            DWORD clrColor = m_pOwner->GetNativeEditBkColor();
            m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
        }
        return (LRESULT)m_hBkBrush;
    }

    LRESULT CDuiEditWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(TCHAR(wParam) == VK_RETURN)
        {
            m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);
        }
        else if(TCHAR(wParam) == VK_TAB)
        {
            if(m_pOwner->GetManager()->IsLayered())
            {
                m_pOwner->GetManager()->SetNextTabControl();
            }
        }
        return 0;
    }

    LRESULT CDuiEditWnd::OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(!m_bInit || m_pOwner == NULL)
        {
            return 0;
        }
        // Copy text back
        int iLen = ::GetWindowTextLength(GetHWND()) + 1;
        LPTSTR pstr = static_cast<LPTSTR>(_alloca(iLen * sizeof(TCHAR)));
        ASSERT(pstr);
        if(pstr == NULL)
        {
            return 0;
        }
        ZeroMemory(pstr, iLen * sizeof(TCHAR));
        ::GetWindowText(GetHWND(), pstr, iLen);
        m_pOwner->SetText(pstr);
        m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
        if(m_pOwner->GetManager()->IsLayered())
        {
            m_pOwner->Invalidate();
        }
        return 0;
    }

    LRESULT CDuiEditWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
        {
            return OnEditChanged(uMsg, wParam, lParam, bHandled);
        }
        else if(GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE)
        {
            RECT rcClient;
            ::GetClientRect(GetHWND(), &rcClient);
            ::InvalidateRect(GetHWND(), &rcClient, FALSE);
        }
        return 0;
    }

    LRESULT CDuiEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        PostMessage(WM_CLOSE);
        return ::DefWindowProc(GetHWND(), uMsg, wParam, lParam);
    }

    LRESULT CDuiEditWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(m_pOwner->GetManager()->IsLayered())
        {
            m_pOwner->GetManager()->AddPaintChildWnd(GetHWND());
            ::SetTimer(GetHWND(), TIMERID_CARET, ::GetCaretBlinkTime(), NULL);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch(uMsg)
        {
            case WM_CREATE:
                lRes = OnCreate(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KILLFOCUS:
                lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
                break;
            case OCM_COMMAND:
                lRes = OnCommand(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KEYDOWN:
                lRes = OnKeyDown(uMsg, wParam, lParam, bHandled);
                break;
            case OCM__BASE + WM_CTLCOLOREDIT:
            case OCM__BASE + WM_CTLCOLORSTATIC:
                lRes = OnCtlColor(uMsg, wParam, lParam, bHandled);
                break;
            case WM_PRINT:
                lRes = OnPrint(uMsg, wParam, lParam, bHandled);
                break;
            case WM_TIMER:
                lRes = OnTimer(uMsg, wParam, lParam, bHandled);
                break;
            default:
                bHandled = FALSE;
                break;
        }
        if(bHandled)
        {
            return lRes;
        }
        return __super::HandleMessage(uMsg, wParam, lParam);
    }

    /////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiEdit)

    CDuiEdit::CDuiEdit(void)
        : m_cPasswordChar(_T('*'))
        , m_dwEditbkColor(0xFFFFFFFF)
        , m_dwEditTextColor(0x00000000)
        , m_dwTipValueColor(0xFFBAC0C5)
        , m_bReadOnly(FALSE)
        , m_bPasswordMode(FALSE)
        , m_iWindowStyls(0)
        , m_uMaxChar(255)
        , m_uButtonState(0)
        , m_pEditWnd(NULL)
    {
        SetTextPadding(CDuiRect(4, 3, 4, 3));
        SetBkColor(0xFFFFFFFF);
    }


    CDuiEdit::~CDuiEdit(void)
    {
    }


    LPCTSTR CDuiEdit::GetClass() const
    {
        return DUI_CTR_EDIT;
    }

    LPVOID CDuiEdit::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_EDIT) == 0)
        {
            return static_cast<CDuiEdit*>(this);
        }
        return __super::GetInterface(pstrName);
    }

    UINT CDuiEdit::GetControlFlags() const
    {
        if(!IsEnabled())
        {
            return CDuiControl::GetControlFlags();
        }

        return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
    }

    void CDuiEdit::EmptyEditWnd()
    {
        m_pEditWnd = NULL;
    }

    BOOL CDuiEdit::IsReadOnly() const
    {
        return m_bReadOnly;
    }

    void CDuiEdit::SetReadOnly(BOOL bReadOnly)
    {
        if(m_bReadOnly == bReadOnly)
        {
            return;
        }

        m_bReadOnly = bReadOnly;
        if(m_pEditWnd != NULL)
        {
            Edit_SetReadOnly(m_pEditWnd->GetHWND(), m_bReadOnly);
        }
        Invalidate();
    }

    void CDuiEdit::SetNumberOnly(BOOL bNumberOnly)
    {
        if(bNumberOnly)
        {
            m_iWindowStyls |= ES_NUMBER;
        }
        else
        {
            m_iWindowStyls &= ~ES_NUMBER;
        }
    }

    BOOL CDuiEdit::IsPasswordMode() const
    {
        return m_bPasswordMode;
    }

    void CDuiEdit::SetPasswordMode(BOOL bPasswordMode)
    {
        if(m_bPasswordMode == bPasswordMode)
        {
            return;
        }
        m_bPasswordMode = bPasswordMode;
        Invalidate();
        if(m_pEditWnd != NULL)
        {
            LONG styleValue = ::GetWindowLong(m_pEditWnd->GetHWND(), GWL_STYLE);
            bPasswordMode ? styleValue |= ES_PASSWORD : styleValue &= ~ES_PASSWORD;
            ::SetWindowLong(m_pEditWnd->GetHWND(), GWL_STYLE, styleValue);
        }
    }

    TCHAR CDuiEdit::GetPasswordChar() const
    {
        return m_cPasswordChar;
    }

    void CDuiEdit::SetPasswordChar(TCHAR cPasswordChar)
    {
        if(m_cPasswordChar == cPasswordChar)
        {
            return;
        }
        m_cPasswordChar = cPasswordChar;
        if(m_pEditWnd != NULL)
        {
            Edit_SetPasswordChar(m_pEditWnd->GetHWND(), m_cPasswordChar);
        }
        Invalidate();
    }

    UINT CDuiEdit::GetMaxChar() const
    {
        return m_uMaxChar;
    }

    void CDuiEdit::SetMaxChar(UINT uMax)
    {
        m_uMaxChar = uMax;
        if(m_pEditWnd != NULL)
        {
            Edit_LimitText(m_pEditWnd->GetHWND(), m_uMaxChar);
        }
    }

    int CDuiEdit::GetWindowStyls() const
    {
        return m_iWindowStyls;
    }

    LPCTSTR CDuiEdit::GetNormalImage() const
    {
        return m_sNormalImage;
    }

    void CDuiEdit::SetNormalImage(LPCTSTR pStrImage)
    {
        m_sNormalImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiEdit::GetHotImage() const
    {
        return m_sHotImage;
    }

    void CDuiEdit::SetHotImage(LPCTSTR pStrImage)
    {
        m_sHotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiEdit::GetFocusedImage() const
    {
        return m_sFocusedImage;
    }

    void CDuiEdit::SetFocusedImage(LPCTSTR pStrImage)
    {
        m_sFocusedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiEdit::GetDisabledImage() const
    {
        return m_sDisabledImage;
    }

    void CDuiEdit::SetDisabledImage(LPCTSTR pStrImage)
    {
        m_sDisabledImage = pStrImage;
        Invalidate();
    }

    DWORD CDuiEdit::GetNativeEditBkColor() const
    {
        return m_dwEditbkColor;
    }

    void CDuiEdit::SetNativeEditBkColor(DWORD dwBkColor)
    {
        m_dwEditbkColor = dwBkColor;
    }

    DWORD CDuiEdit::GetNativeEditTextColor() const
    {
        return m_dwEditTextColor;
    }

    void CDuiEdit::SetNativeEditTextColor(LPCTSTR pStrColor)
    {
        if(*pStrColor == _T('#'))
        {
            pStrColor = ::CharNext(pStrColor);
        }
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

        m_dwEditTextColor = clrColor;
    }

    LPCTSTR CDuiEdit::GetTipValue() const
    {
        if(!IsResourceText())
        {
            return m_sTipValue;
        }
        return CDuiResourceManager::GetInstance()->GetText(m_sTipValue);
    }

    void CDuiEdit::SetTipValue(LPCTSTR pStrTipValue)
    {
        m_sTipValue	= pStrTipValue;
    }

    DWORD CDuiEdit::GetTipValueColor() const
    {
        return m_dwTipValueColor;
    }

    void CDuiEdit::SetTipValueColor(LPCTSTR pStrColor)
    {
        if(*pStrColor == _T('#'))
        {
            pStrColor = ::CharNext(pStrColor);
        }
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

        m_dwTipValueColor = clrColor;
    }

    void CDuiEdit::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("readonly")) == 0)
        {
            SetReadOnly(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("numberonly")) == 0)
        {
            SetNumberOnly(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("password")) == 0)
        {
            SetPasswordMode(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("passwordchar")) == 0)
        {
            SetPasswordChar(*pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("maxchar")) == 0)
        {
            SetMaxChar(_ttoi(pstrValue));
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
        else if(_tcsicmp(pstrName, _T("tipvalue")) == 0)
        {
            SetTipValue(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("tipvaluecolor")) == 0)
        {
            SetTipValueColor(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("nativetextcolor")) == 0)
        {
            SetNativeEditTextColor(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("nativebkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetNativeEditBkColor(clrColor);
        }
        else
        {
            __super::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiEdit::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        __super::SetPos(rc, bNeedInvalidate);
        if(m_pEditWnd != NULL)
        {
            RECT rcPos = m_pEditWnd->CalPos();
            ::SetWindowPos(m_pEditWnd->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left,
                           rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }

    void CDuiEdit::PaintStatusImage(HDC hDC)
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

    void CDuiEdit::PaintText(HDC hDC)
    {
        DWORD mCurTextColor = GetTextColor();

        if(GetTextColor() == 0)
        {
            mCurTextColor =  GetManager()->GetDefaultFontColor();
            SetTextColor(GetManager()->GetDefaultFontColor());
        }
        if(GetDisabledTextColor() == 0)
        {
            SetDisabledTextColor(GetManager()->GetDefaultDisabledColor());
        }

        CDuiString sDrawText = GetText();
        CDuiString sTipValue = GetTipValue();
        if(sDrawText == sTipValue || sDrawText == _T(""))
        {
            mCurTextColor = m_dwTipValueColor;
            sDrawText = sTipValue;
        }
        else
        {
            CDuiString sTemp = sDrawText;
            if(m_bPasswordMode)
            {
                sDrawText.SetEmpty();
                LPCTSTR pStr = sTemp.GetData();
                while(*pStr != _T('\0'))
                {
                    sDrawText += m_cPasswordChar;
                    pStr = ::CharNext(pStr);
                }
            }
        }

        RECT rc = GetPos();
        rc.left += GetTextPadding().left;
        rc.right -= GetTextPadding().right;
        rc.top += GetTextPadding().top;
        rc.bottom -= GetTextPadding().bottom;
        if(IsEnabled())
        {
            CRenderEngine::DrawText(hDC, GetManager(), rc, sDrawText, mCurTextColor, \
                                    GetFont(), DT_SINGLELINE | GetTextStyle());
        }
        else
        {
            CRenderEngine::DrawText(hDC, GetManager(), rc, sDrawText, GetDisabledTextColor(), \
                                    GetFont(), DT_SINGLELINE | GetTextStyle());
        }
    }

    void CDuiEdit::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(GetParent() != NULL)
            {
                GetParent()->DoEvent(event);
            }
            else
            {
                __super::DoEvent(event);
            }
            return;
        }

        if(event.Type == UIEVENT_SETCURSOR && IsEnabled())
        {
            ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
            return;
        }
        if(event.Type == UIEVENT_WINDOWSIZE)
        {
            if(m_pEditWnd != NULL)
            {
                GetManager()->SetFocusNeeded(this);
            }
        }
        if(event.Type == UIEVENT_SCROLLWHEEL)
        {
            if(m_pEditWnd != NULL)
            {
                return;
            }
        }
        if(event.Type == UIEVENT_SETFOCUS && IsEnabled())
        {
            if(m_pEditWnd)
            {
                return;
            }
            m_pEditWnd = new CDuiEditWnd();
            ASSERT(m_pEditWnd);
            m_pEditWnd->Init(this);

            Invalidate();
        }
        if(event.Type == UIEVENT_KILLFOCUS && IsEnabled())
        {
            Invalidate();
        }
        if(event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN)
        {
            if(IsEnabled())
            {
                GetManager()->ReleaseCapture();
                if(IsFocused() && m_pEditWnd == NULL)
                {
                    m_pEditWnd = new CDuiEditWnd();
                    ASSERT(m_pEditWnd);
                    m_pEditWnd->Init(this);

                    if(PtInRect(&GetPos(), event.ptMouse))
                    {
                        int nSize = GetWindowTextLength(*m_pEditWnd);
                        if(nSize == 0)
                        {
                            nSize = 1;
                        }

                        Edit_SetSel(*m_pEditWnd, 0, nSize);
                    }
                    Invalidate();
                }
                else if(m_pEditWnd != NULL)
                {
#if 1
                    int nSize = GetWindowTextLength(*m_pEditWnd);
                    if(nSize == 0)
                    {
                        nSize = 1;
                    }

                    Edit_SetSel(*m_pEditWnd, 0, nSize);
#else
                    POINT pt = event.ptMouse;
                    pt.x -= m_rcItem.left + m_rcTextPadding.left;
                    pt.y -= m_rcItem.top + m_rcTextPadding.top;
                    ::SendMessage(*m_pEditWnd, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
                }
            }
            return;
        }
        if(event.Type == UIEVENT_MOUSEMOVE)
        {
            return;
        }
        if(event.Type == UIEVENT_BUTTONUP)
        {
            return;
        }
        if(event.Type == UIEVENT_CONTEXTMENU)
        {
            return;
        }
        if(event.Type == UIEVENT_MOUSEENTER)
        {
            if(IsEnabled())
            {
                m_uButtonState |= UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_MOUSELEAVE)
        {
            if(IsEnabled())
            {
                m_uButtonState &= ~UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        __super::DoEvent(event);
    }

}

