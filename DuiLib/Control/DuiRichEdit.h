#pragma once
namespace DuiLib
{
#define ID_RICH_UNDO			101
#define ID_RICH_CUT				102
#define ID_RICH_COPY			103
#define ID_RICH_PASTE			104
#define ID_RICH_CLEAR			105
#define ID_RICH_SELECTALL		106
#define ID_RICH_REDO			107

#ifndef LY_PER_INCH
    #define LY_PER_INCH 1440
#endif

#ifndef HIMETRIC_PER_INCH
    #define HIMETRIC_PER_INCH 2540
#endif

#include <textserv.h>

#define TEXTMAX ((32 * 1024) - 1)

    // 注意：TxSendMessage和SendMessage是有区别的，TxSendMessage没有multibyte和unicode自动转换的功能，
    // 而richedit2.0内部是以unicode实现的，在multibyte程序中，必须自己处理unicode到multibyte的转换

    class CDuiRichEdit;

    class CTxtWinHost : public ITextHost
    {
    public:
        CTxtWinHost(void);
        virtual ~CTxtWinHost(void);
    public:
        HRESULT InitDefaultCharFormat(CDuiRichEdit* re, CHARFORMAT2W* pcf, HFONT hfont);
        HRESULT InitDefaultParaFormat(CDuiRichEdit* re, PARAFORMAT2* ppf);
        BOOL Init(CDuiRichEdit* pRichEdit, const CREATESTRUCT* pcs);
        ITextServices* GetTextServices() const;

        BOOL IsCaptured();
        void SetReadOnly(BOOL bReadOnly);
        void SetRichTextFlag(BOOL bRichText);
        void SetColor(DWORD dwColor);
        void LimitText(LONG nChars);
        void GetControlRect(LPRECT prc);
        void SetClientRect(RECT* prc);
        void SetTransparent(BOOL bTransparent);
        BOOL DoSetCursor(RECT* prc, POINT* pt);
        void SetCharFormat(CHARFORMAT2W& c);
        void SetParaFormat(PARAFORMAT2& p);
        HRESULT OnTxInPlaceActivate(LPCRECT prcClient);

        ULONG STDMETHODCALLTYPE AddRef(void) OVERRIDE;
        ULONG STDMETHODCALLTYPE Release(void) OVERRIDE;
        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR* __RPC_FAR* ppvObject) OVERRIDE;

        HDC 		TxGetDC() OVERRIDE;
        int			TxReleaseDC(HDC hdc) OVERRIDE;
        BOOL 		TxShowScrollBar(int fnBar, BOOL fShow) OVERRIDE;
        BOOL 		TxEnableScrollBar(int fuSBFlags, int fuArrowflags) OVERRIDE;
        BOOL 		TxSetScrollRange(int fnBar, LONG nMinPos, int nMaxPos, BOOL fRedraw) OVERRIDE;
        BOOL 		TxSetScrollPos(int fnBar, int nPos, BOOL fRedraw) OVERRIDE;
        void		TxInvalidateRect(LPCRECT prc, BOOL fMode) OVERRIDE;
        void 		TxViewChange(BOOL fUpdate) OVERRIDE;
        BOOL		TxCreateCaret(HBITMAP hbmp, int xWidth, int yHeight) OVERRIDE;
        BOOL		TxShowCaret(BOOL fShow) OVERRIDE;
        BOOL		TxSetCaretPos(int x, int y) OVERRIDE;
        BOOL 		TxSetTimer(UINT idTimer, UINT uTimeout) OVERRIDE;
        void 		TxKillTimer(UINT idTimer) OVERRIDE;
        void		TxScrollWindowEx(int dx, int dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll) OVERRIDE;
        void		TxSetCapture(BOOL fCapture) OVERRIDE;
        void		TxSetFocus() OVERRIDE;
        void 		TxSetCursor(HCURSOR hcur, BOOL fText) OVERRIDE;
        BOOL 		TxScreenToClient(LPPOINT lppt) OVERRIDE;
        BOOL		TxClientToScreen(LPPOINT lppt) OVERRIDE;
        HRESULT		TxActivate(LONG* plOldState) OVERRIDE;
        HRESULT		TxDeactivate(LONG lNewState) OVERRIDE;
        HRESULT		TxGetClientRect(LPRECT prc) OVERRIDE;
        HRESULT		TxGetViewInset(LPRECT prc) OVERRIDE;
        HRESULT 	TxGetCharFormat(const CHARFORMATW** ppCF) OVERRIDE;
        HRESULT		TxGetParaFormat(const PARAFORMAT** ppPF) OVERRIDE;
        COLORREF	TxGetSysColor(int nIndex) OVERRIDE;
        HRESULT		TxGetBackStyle(TXTBACKSTYLE* pstyle) OVERRIDE;
        HRESULT		TxGetMaxLength(DWORD* plength) OVERRIDE;
        HRESULT		TxGetScrollBars(DWORD* pdwScrollBar) OVERRIDE;
        HRESULT		TxGetPasswordChar(TCHAR* pch) OVERRIDE;
        HRESULT		TxGetAcceleratorPos(LONG* pcp) OVERRIDE;
        HRESULT		TxGetExtent(LPSIZEL lpExtent) OVERRIDE;
        HRESULT 	OnTxCharFormatChange(const CHARFORMATW* pcf) OVERRIDE;
        HRESULT		OnTxParaFormatChange(const PARAFORMAT* ppf) OVERRIDE;
        HRESULT		TxGetPropertyBits(DWORD dwMask, DWORD* pdwBits) OVERRIDE;
        HRESULT		TxNotify(DWORD iNotify, void* pv) OVERRIDE;
        HIMC		TxImmGetContext() OVERRIDE;
        void		TxImmReleaseContext(HIMC himc) OVERRIDE;
        HRESULT		TxGetSelectionBarWidth(LONG* lSelBarWidth) OVERRIDE;
    private:
        WCHAR chPasswordChar;		    // Password character
        BOOL m_bRichText;				// Whether control is rich text
        BOOL m_bTransparent;			// Whether control is transparent
        BOOL m_bInplaceActive;			// Whether control is inplace active
        BOOL m_bShowCaret;
        BOOL m_bTimer;					// A timer is set
        BOOL m_bCaptured;
        BOOL m_bEnableAutoWordSel;	// enable Word style auto word selection?
        BOOL m_bWordWrap;			// Whether control should word wrap
        BOOL m_bAllowBeep;			// Whether beep is allowed
        BOOL m_bSaveSelection;		// Whether to save the selection when inactive
        LONG lSelBarWidth;			// Width of the selection bar
        LONG m_iLimitText;				// maximum text size
        LONG laccelpos;				// Accelerator position
        ULONG cRefs;					// Reference Count
        DWORD m_dwStyle;				// style bits
        int m_iCaretWidth;
        int m_iCaretHeight;
        int m_iCaretLastWidth;
        int m_iCaretLastHeight;
        ITextServices* pserv;		    // pointer to Text Services object
        CDuiRichEdit* m_pRichEdit;
        RECT rcClient;				// Client Rect for this control
        SIZEL sizelExtent;			// Extent array
        CHARFORMAT2W cf;					// Default character format
        PARAFORMAT2	pf;					    // Default paragraph format
    };

    class DUILIB_API CDuiRichEdit
        : public CDuiContainer
        , public IMessageFilter
    {
        DECLARE_DUICONTROL(CDuiRichEdit)
    public:
        CDuiRichEdit(void);
        virtual ~CDuiRichEdit(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString strName) OVERRIDE;
        UINT GetControlFlags() const OVERRIDE;

        long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;
        CDuiString GetText() const OVERRIDE;
        void SetText(LPCTSTR pstrText) OVERRIDE;

        void SetEnabled(BOOL bEnabled);

        LONG GetWinStyle();
        void SetWinStyle(LONG lStyle);

        BOOL IsAccumulateDBCMode();
        void SetAccumulateDBCMode(BOOL bDBCMode);

        BOOL IsMultiLine();
        void SetMultiLine(BOOL bMultiLine);

        BOOL IsReadOnly();
        void SetReadOnly(BOOL bReadOnly = TRUE);

        BOOL IsWantTab() const;
        void SetWantTab(BOOL bWantTab = TRUE);

        BOOL IsWantReturn() const;
        void SetWantReturn(BOOL bWantReturn = TRUE);

        BOOL IsWantCtrlReturn() const;
        void SetWantCtrlReturn(BOOL bWantCtrlReturn = TRUE);

        BOOL IsRich() const;
        void SetRich(BOOL bRich = TRUE);

        int GetFont() const;
        void SetFont(int index);

        DWORD GetTextColor() const;
        void SetTextColor(DWORD dwTextColor);

        int GetLimitText() const;
        void SetLimitText(int iChars);

        LPCTSTR GetNormalImage() const;
        void SetNormalImage(LPCTSTR pStrImage);

        LPCTSTR GetHotImage() const;
        void SetHotImage(LPCTSTR pStrImage);

        LPCTSTR GetFocusedImage() const;
        void SetFocusedImage(LPCTSTR pStrImage);

        LPCTSTR GetDisabledImage() const;
        void SetDisabledImage(LPCTSTR pStrImage);

        RECT GetTextPadding() const;
        void SetTextPadding(RECT rc);

        LPCTSTR GetTipValue() const;
        void SetTipValue(LPCTSTR pStrTipValue);

        DWORD GetTipValueColor() const;
        void SetTipValueColor(DWORD dwColor);

        UINT GetTipValueAlign() const;
        void SetTipValueAlign(UINT uAlign);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void SetScrollPos(SIZE szPos, BOOL bMsg = TRUE) OVERRIDE;
        void LineUp() OVERRIDE;
        void LineDown() OVERRIDE;
        void PageUp() OVERRIDE;
        void PageDown() OVERRIDE;
        void HomeUp() OVERRIDE;
        void EndDown() OVERRIDE;
        void LineLeft() OVERRIDE;
        void LineRight() OVERRIDE;
        void PageLeft() OVERRIDE;
        void PageRight() OVERRIDE;
        void HomeLeft() OVERRIDE;
        void EndRight() OVERRIDE;

        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void Move(SIZE szOffset, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void DoEvent(CDuiEvent& event) OVERRIDE;
        void PaintStatusImage(HDC hDC) OVERRIDE;
        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;
        void DoInit() OVERRIDE;

        virtual BOOL OnTxViewChanged(BOOL bUpdate);
        virtual void OnTxNotify(DWORD iNotify, void* pv);
        virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT* plresult) const;

        BOOL GetModify() const;
        void SetModify(BOOL bModified = TRUE) const;

        BOOL CanUndo();
        BOOL CanRedo();
        BOOL CanPaste();
        BOOL Redo();
        BOOL Undo();
        void Clear();
        void Copy();
        void Cut();
        void Paste();

        WORD GetSelectionType() const;

        CDuiString GetSelText() const;
        CDuiString GetTextRange(long nStartChar, long nEndChar) const;
        int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, BOOL bCanUndo = FALSE);
        int AppendText(LPCTSTR lpstrText, BOOL bCanUndo = FALSE);

        void HideSelection(BOOL bHide = TRUE, BOOL bChangeStyle = FALSE);
        void ScrollCaret();

        void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo);

        void GetSel(CHARRANGE& cr) const;
        int SetSel(CHARRANGE& cr);

        void GetSel(long& nStartChar, long& nEndChar) const;
        int SetSel(long nStartChar, long nEndChar);

        int SetSelAll();
        int SetSelNone();

        BOOL GetZoom(int& nNum, int& nDen) const;
        BOOL SetZoom(int nNum, int nDen);
        BOOL SetZoomOff();

        BOOL GetAutoURLDetect() const;
        BOOL SetAutoURLDetect(BOOL bAutoDetect = TRUE);

        DWORD GetEventMask() const;
        DWORD SetEventMask(DWORD dwEventMask);

        DWORD GetDefaultCharFormat(CHARFORMAT2& cf) const;
        BOOL SetDefaultCharFormat(CHARFORMAT2& cf);

        DWORD GetSelectionCharFormat(CHARFORMAT2& cf) const;
        BOOL SetSelectionCharFormat(CHARFORMAT2& cf);

        BOOL SetWordCharFormat(CHARFORMAT2& cf);

        DWORD GetParaFormat(PARAFORMAT2& pf) const;
        BOOL SetParaFormat(PARAFORMAT2& pf);

        int GetLineCount() const;
        CDuiString GetLine(int nIndex, int nMaxLength) const;
        int LineIndex(int nLine = -1) const;
        int LineLength(int nLine = -1) const;
        BOOL LineScroll(int nLines, int nChars = 0);
        long LineFromChar(long nIndex) const;
        CDuiPoint PosFromChar(UINT nChar) const;
        int CharFromPos(CDuiPoint pt) const;
        void EmptyUndoBuffer();
        UINT SetUndoLimit(UINT nLimit);
        long StreamIn(int nFormat, EDITSTREAM& es);
        long StreamOut(int nFormat, EDITSTREAM& es);

        LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
    private:
        BOOL m_bWantTab;
        BOOL m_bWantReturn;
        BOOL m_bWantCtrlReturn;
        BOOL m_bRich;
        BOOL m_bReadOnly;
        BOOL m_bVScrollBarFixing;
        BOOL  m_fAccumulateDBC ; // TRUE - need to cumulate ytes from 2 WM_CHAR msgs

        LONG m_lTwhStyle;

        DWORD m_dwTipValueColor;
        DWORD m_dwTextColor;

        int m_iFont;
        int m_iLimitText;

        UINT m_uTipValueAlign;
        UINT m_uButtonState;
        UINT m_chLeadByte; // use when we are in _fAccumulateDBC mode

        CTxtWinHost* m_pTwh;

        CDuiRect m_rcTextPadding;
        CDuiString m_sTipValue;
        CDuiString m_sNormalImage;
        CDuiString m_sHotImage;
        CDuiString m_sFocusedImage;
        CDuiString m_sDisabledImage;
    };

}