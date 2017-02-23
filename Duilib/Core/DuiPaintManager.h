#pragma once

namespace DuiLib
{
#define CARET_TIMERID	0x1999

    typedef CDuiControl* (*LPCREATECONTROL)(LPCTSTR pstrType);

    enum DUILIB_RESTYPE
    {
        DUILIB_FILE		=  1,		// 来自磁盘文件
        DUILIB_ZIP			,		// 来自磁盘zip压缩包
        DUILIB_RESOURCE		,		// 来自资源
        DUILIB_ZIPRESOURCE	,		// 来自资源的zip压缩包
    };

    enum DUILIB_EVENTTYPE
    {
        DUIEVENT__FIRST = 1,
        DUIEVENT__KEYBEGIN,
        DUIEVENT_KEYDOWN,
        DUIEVENT_KEYUP,
        DUIEVENT_CHAR,
        DUIEVENT_SYSKEYDOWN,
        DUIEVENT_SYSKEYUP,
        DUIEVENT__KEYEND,
        DUIEVENT__MOUSEBEGIN,
        DUIEVENT_MOUSEMOVE,
        DUIEVENT_MOUSELEAVE,
        DUIEVENT_MOUSEENTER,
        DUIEVENT_MOUSEHOVER,
        DUIEVENT_BUTTONDOWN,
        DUIEVENT_BUTTONUP,
        DUIEVENT_RBUTTONDOWN,
        DUIEVENT_RBUTTONUP,
        DUIEVENT_MBUTTONDOWN,
        DUIEVENT_MBUTTONUP,
        DUIEVENT_DBLCLICK,
        DUIEVENT_CONTEXTMENU,
        DUIEVENT_SCROLLWHEEL,
        DUIEVENT__MOUSEEND,
        DUIEVENT_KILLFOCUS,
        DUIEVENT_SETFOCUS,
        DUIEVENT_WINDOWSIZE,
        DUIEVENT_SETCURSOR,
        DUIEVENT_TIMER,
        DUIEVENT__LAST,
    };

    class CShortCut
    {
    public:
        CShortCut();
    public:
        TCHAR m_ch;
        BOOL m_bPickNext;
    };

    class CDuiEvent
    {
    public:
        CDuiEvent();
    public:
        TCHAR chKey;
        WORD wKeyState;
        DWORD dwTimestamp;
        int Type;
        WPARAM wParam;
        LPARAM lParam;
        CDuiControl* pSender;
        CDuiPoint ptMouse;
    };


    class CDuiNotify
    {
    public:
        CDuiNotify();
    public:
        DWORD dwTimestamp;
        WPARAM wParam;
        LPARAM lParam;
        CDuiControl* pSender;
        CDuiPoint ptMouse;
        CDuiString sType;
        CDuiString sVirtualWnd;
    };

    class CDuiFontInfo
    {
    public:
        CDuiFontInfo();
    public:
        BOOL bBold;
        BOOL bUnderline;
        BOOL bItalic;
        int iSize;
        HFONT hFont;
        TEXTMETRIC tm;
        CDuiString sFontName;
    };

    class CDuiResInfo
    {
    public:
        CDuiResInfo();
    public:
        DWORD m_dwDefaultDisabledColor;
        DWORD m_dwDefaultFontColor;
        DWORD m_dwDefaultLinkFontColor;
        DWORD m_dwDefaultLinkHoverFontColor;
        DWORD m_dwDefaultSelectedBkColor;
        CStdStringPtrMap m_CustomFonts;
        CStdStringPtrMap m_ImageHash;
        CStdStringPtrMap m_AttrHash;
        CStdStringPtrMap m_StyleHash;
        CStdStringPtrMap m_DrawInfoHash;
        CDuiFontInfo m_DefaultFontInfo;
    };

    class CDuiImageInfo
    {
    public:
        CDuiImageInfo();
    public:
        BOOL bAlpha;
        BOOL bUseHSL;
        DWORD dwMask;
        int nX;
        int nY;
        HBITMAP hBitmap;
        LPBYTE pBits;
        LPBYTE pSrcBits;
        CDuiString sResType;
    };

    class CDuiTabInfo
    {
    public:
        CDuiTabInfo();
    public:
        BOOL bForward;
        BOOL bNextIsIt;
        CDuiControl* pFocus;
        CDuiControl* pLast;
    };

    class CDuiTimerInfo
    {
    public:
        CDuiTimerInfo();
    public:
        BOOL bKilled;
        UINT nLocalID;
        UINT uWinTimer;
        HWND hWnd;
        CDuiControl* pSender;
    };

    class CDuiPaintManager;
    class CDrawInfo
    {
    public:
        CDrawInfo();
    public:
        void Parse(LPCTSTR lpImage, LPCTSTR lpModify, CDuiPaintManager* pPaintManager);
    public:
        BYTE uFade;
        BOOL bHole;
        BOOL bTiledX;
        BOOL bTiledY;
        BOOL bHSL;
        DWORD dwMask;

        CDuiRect rcDest;
        CDuiRect rcSource;
        CDuiRect rcCorner;
        CDuiString sDrawString;
        CDuiString sDrawModify;
        CDuiString sImageName;
        CDuiString sResType;
    };

    class ITranslateAccelerator
    {
    public:
        virtual LRESULT TranslateAccelerator(MSG* pMsg) = 0;
    };

    class INotify
    {
    public:
        virtual void Notify(CDuiNotify& msg) = 0;
    };

    class IMessageFilter
    {
    public:
        virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) = 0;
    };

    class CDuiRichEdit;
    class DUILIB_API CDuiPaintManager : public CIDropTarget
    {
    public:
        CDuiPaintManager(void);
        virtual ~CDuiPaintManager(void);
    public:
        void Init(HWND hWnd);
        void NeedUpdate();
        BOOL Invalidate();
        BOOL Invalidate(RECT& rcItem);

        HDC GetPaintDC() const;
        HWND GetPaintWindow() const;

        void SetPainting(BOOL bIsPainting);

        //属性设置与获取
        POINT GetMousePos() const;
        CDuiSize GetClientSize() const;
        CDuiSize GetInitSize() const;
        void SetInitSize(int cx, int cy);

        RECT GetSizeBox();
        void SetSizeBox(RECT& rcSizeBox);
        RECT GetCaptionRect();
        void SetCaptionRect(RECT& rcCaption);

        SIZE GetRoundCorner() const;
        void SetRoundCorner(int cx, int cy);

        SIZE GetMinInfo() const;
        void SetMinInfo(int cx, int cy);

        SIZE GetMaxInfo() const;
        void SetMaxInfo(int cx, int cy);

        BOOL IsShowUpdateRect() const;
        void SetShowUpdateRect(BOOL bShow);

        BYTE GetOpacity() const;
        void SetOpacity(BYTE nOpacity);

        BOOL IsLayered() const;
        void SetLayered(BOOL bLayered);

        DWORD GetDefaultDisabledColor() const;
        void SetDefaultDisabledColor(DWORD dwColor, BOOL bShared = FALSE);

        DWORD GetDefaultFontColor() const;
        void SetDefaultFontColor(DWORD dwColor, BOOL bShared = FALSE);

        DWORD GetDefaultLinkFontColor() const;
        void SetDefaultLinkFontColor(DWORD dwColor, BOOL bShared = FALSE);

        DWORD GetDefaultLinkHoverFontColor() const;
        void SetDefaultLinkHoverFontColor(DWORD dwColor, BOOL bShared = FALSE);

        DWORD GetDefaultSelectedBkColor() const;
        void SetDefaultSelectedBkColor(DWORD dwColor, BOOL bShared = FALSE);
        // 样式管理
        void AddStyle(LPCTSTR pName, LPCTSTR pstrStyle, BOOL bShared = FALSE);
        LPCTSTR GetStyle(LPCTSTR pName) const;
        void RemoveAllStyle();
        //焦点相关
        CDuiControl* GetFocus() const;
        void SetFocus(CDuiControl* pControl);
        void SetFocusNeeded(CDuiControl* pControl);
        // 拖拽
        BOOL InitDragDrop();
        static WORD DIBNumColors(void* pv);
        static WORD ColorTableSize(LPVOID lpv);
        virtual bool OnDrop(FORMATETC* pFmtEtc, STGMEDIUM& medium, DWORD* pdwEffect);
        // RichEdit光标
        CDuiRichEdit* GetCurrentCaretObject();
        BOOL CreateCaret(HBITMAP hBmp, int nWidth, int nHeight);
        BOOL ShowCaret(BOOL bShow);
        BOOL SetCaretPos(CDuiRichEdit* pRichEdit, int x, int y);
        void DrawCaret(HDC hDC, const RECT& rcPaint);

        //字体相关
        CDuiFontInfo* GetDefaultFontInfo();
        void SetDefaultFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic, BOOL bShared = FALSE);
        HFONT AddFont(int id, LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic, BOOL bShared = FALSE);
        void AddFontArray(LPCTSTR pstrPath);
        HFONT GetFont(int id);
        HFONT GetFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic);
        void RemoveAllFonts();
        CDuiFontInfo* GetFontInfo(int id);
        CDuiFontInfo* GetFontInfo(HFONT hFont);

        //鼠标捕获
        BOOL IsCaptured();
        void SetCapture();
        void ReleaseCapture();
        //虚拟窗口
        void UsedVirtualWnd(BOOL bUsed);
        //Gdiplus相关
        BOOL IsUseGdiplusText() const;
        int GetGdiplusTextRenderingHint() const;
        void SetUseGdiplusText(BOOL bUse);
        void SetGdiplusTextRenderingHint(int trh);

        BOOL SetNextTabControl(BOOL bForward = TRUE);

        CDPI* GetDPIObj();
        void RebuildFont(CDuiFontInfo* pFontInfo);
        void ResetDPIAssets();
        void SetDPI(int iDPI);

        CDuiShadow* GetShadow();

        BOOL AddNotifier(INotify* pNotifier);
        void SendNotify(CDuiNotify& Msg, BOOL bAsync = FALSE);
        void SendNotify(CDuiControl* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bAsync = FALSE);
        BOOL RemoveNotifier(INotify* pNotifier);

        void AddDelayedCleanup(CDuiControl* pControl);

        void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList, BOOL bShared = FALSE);
        LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
        void RemoveAllDefaultAttributeList();

        BOOL InitControls(CDuiControl* pControl, CDuiControl* pParent = NULL);
        BOOL AttachDialog(CDuiControl* pControl);
        void ReapObjects(CDuiControl* pControl);

        BOOL AddPaintChildWnd(HWND hChildWnd);
        BOOL RemovePaintChildWnd(HWND hChildWnd);

        BOOL AddPostPaint(CDuiControl* pControl);
        BOOL RemovePostPaint(CDuiControl* pControl);

        CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
        BOOL AddOptionGroup(LPCTSTR pStrGroupName, CDuiControl* pControl);
        void RemoveOptionGroup(LPCTSTR pStrGroupName, CDuiControl* pControl);
        void RemoveAllOptionGroups();

        CDuiControl* GetRoot() const;
        CDuiControl* FindControl(POINT pt) const;
        CDuiControl* FindControl(LPCTSTR pstrName) const;
        CDuiControl* FindSubControlByName(CDuiControl* pParent, LPCTSTR pstrName) const;

        const CDuiImageInfo* GetImage(LPCTSTR bitmap);
        const CDuiImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0, BOOL bUseHSL = FALSE, HINSTANCE instance = NULL);
        const CDuiImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0, BOOL bUseHSL = FALSE, BOOL bShared = FALSE, HINSTANCE instance = NULL);
        const CDuiImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, BOOL bAlpha, BOOL bShared = FALSE);
        void RemoveImage(LPCTSTR bitmap, BOOL bShared = FALSE);
        void RemoveAllImages();

        const CDrawInfo* GetDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify);
        void RemoveAllDrawInfos();

        BOOL AddPreMessageFilter(IMessageFilter* pFilter);
        BOOL RemovePreMessageFilter(IMessageFilter* pFilter);
        BOOL PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

        BOOL AddMessageFilter(IMessageFilter* pFilter);
        BOOL RemoveMessageFilter(IMessageFilter* pFilter);

        BOOL AddTranslateAccelerator(ITranslateAccelerator* pTranslateAccelerator);
        BOOL RemoveTranslateAccelerator(ITranslateAccelerator* pTranslateAccelerator);
        BOOL TranslateAccelerator(LPMSG pMsg);

        BOOL SetTimer(CDuiControl* pControl, UINT nTimerID, UINT uElapse);
        BOOL KillTimer(CDuiControl* pControl, UINT nTimerID);
        void KillTimer(CDuiControl* pControl);
        void RemoveAllTimers();

        BOOL IsForceUseSharedRes() const;
        void SetForceUseSharedRes(BOOL bForce);

        CStdPtrArray* GetFoundControls();

        LRESULT OnDuiNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        BOOL MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

        static BOOL Initialize(HINSTANCE hInstance);
        static void RemoveAllShared();
        static void Uninitialize();

        static HINSTANCE GetInstance();
        static DWORD GetMainThreadID();

        static CDuiString GetInstancePath();

        static HINSTANCE GetResourceDll();

        static int GetResourceType();
        static void SetResourceType(int iResourceType);

        static const CDuiString& GetResourcePath();
        static void SetResourcePath(LPCTSTR pStrPath);

        static const CDuiString& GetResourceZipPwd();

        static const CDuiString& GetResourceZip();
        static void SetResourceZip(LPVOID pVoid, unsigned int len, LPCTSTR password = NULL);
        static void SetResourceZip(LPCTSTR pstrZip, BOOL bCachedResourceZip = FALSE, LPCTSTR password = NULL);

        static BOOL IsCachedResourceZip();
        static HANDLE GetResourceZipHandle();

        static BOOL GetHSL(short* H, short* S, short* L);
        static UINT MapKeyState();

        static BOOL TranslateMessage(const LPMSG pMsg);
        static BOOL IsIdleMessage(const LPMSG pMsg);
        static int MessageLoop();

        static void GetChildWndRect(HWND hWnd, HWND hChildWnd, RECT& rcChildWnd);
        //插件
        static BOOL LoadPlugin(LPCTSTR pstrModuleName);
        static CStdPtrArray* GetPlugins();
    protected:
        static CDuiControl* CALLBACK __FindControlFromNameHash(CDuiControl* pThis, LPVOID pData);
        static CDuiControl* CALLBACK __FindControlsFromUpdate(CDuiControl* pThis, LPVOID pData);
        static CDuiControl* CALLBACK __FindControlFromTab(CDuiControl* pThis, LPVOID pData);
        static CDuiControl* CALLBACK __FindControlFromPoint(CDuiControl* pThis, LPVOID pData);
        static CDuiControl* CALLBACK __FindControlFromShortcut(CDuiControl* pThis, LPVOID pData);
        static CDuiControl* CALLBACK __FindControlFromName(CDuiControl* pThis, LPVOID pData);
    private:
        HWND m_hWndPaint;						//绘制窗口的句柄
        HWND m_hwndTooltip;						//提示消息

        HDC m_hDcPaint;							//绘制窗口的设备上下文环境HDC
        HDC m_hDcOffscreen;						//m_hDcPaint的内存缓冲区

        HBITMAP m_hbmpOffscreen;				//与设备无关位图的句柄
        HBITMAP m_hDragBitmap;					//拖拽位图句柄

        BYTE m_nOpacity;						//alpha数值

        BOOL m_bUpdateNeeded;					//第一次更新界面标志位
        BOOL m_bLayered;						//是否分层
        BOOL m_bForceUseSharedRes;				//是否将资源添加到共享的属性中
        BOOL m_bUsedVirtualWnd;					//虚拟窗口
        BOOL m_bIsPainting;						//是否正在绘制
        BOOL m_bOffscreenPaint;
        BOOL m_bShowUpdateRect;					//是否显示窗口边框(默认红色)
        BOOL m_bFirstLayout;					//第一次界面绘制完的标志位
        BOOL m_bMouseCapture;					//是否设置鼠标捕获
        BOOL m_bDragMode;						//是否拖拽
        BOOL m_bUseGdiplusText;					//gdiplustext属性,是否用gdi+渲染文字
        BOOL m_bCaretActive;					//光标相关
        BOOL m_bMouseTracking;					//鼠标是否在本窗口区域
        BOOL m_bCaretShowing;

        int m_trh;								//textrenderinghint属性,gdi+渲染文字提示

        UINT m_uTimerID;

        ULONG_PTR m_gdiplusToken;				//Gdiplus相关

        CDPI* m_pDPI;
        BYTE* m_pOffscreenBits;
        CDuiControl* m_pRoot;					//xml 根节点
        CDuiControl* m_pFocus;					//当前焦点控件
        CDuiControl* m_pEventClick;				//当前点击的控件
        CDuiControl* m_pEventHover;				//当前鼠标停留的控件
        CDuiControl* m_pEventKey;				//键盘键值
        GdiplusStartupInput* m_pGdiplusStartupInput;
        CDuiRichEdit* m_currentCaretObject;

        TOOLINFO m_ToolTip;

        CDuiResInfo m_ResInfo;

        CDuiPoint m_ptLastMousePos;					//鼠标坐标

        CDuiRect m_rtCaret;							//光标相关
        CDuiRect m_rcLayeredUpdate;
        CDuiRect m_rcSizeBox;						//sizebox属性
        CDuiRect m_rcCaption;						//caption属性

        CDuiSize m_szInitWindowSize;				//size属性
        CDuiSize m_szRoundCorner;					//roundcorner属性
        CDuiSize m_szMinWindow;						//mininfo属性
        CDuiSize m_szMaxWindow;						//maxinfo属性

        CStdPtrArray m_aTimers;					//定时器
        CStdPtrArray m_aMessageFilters;
        CStdPtrArray m_aTranslateAccelerator;
        CStdPtrArray m_aPreMessageFilters;		//拦截在DispatchMessage消息之前的消息，注册的窗口句柄
        CStdPtrArray m_aNotifiers;				//
        CStdPtrArray m_aAsyncNotify;			//
        CStdPtrArray m_aDelayedCleanup;
        CStdPtrArray m_aPostPaintControls;
        CStdPtrArray m_aFoundControls;
        CStdPtrArray m_aChildWnds;
        CStdPtrArray m_aFonts;

        CStdStringPtrMap m_mNameHash;
        CStdStringPtrMap m_mOptionGroup;

        CDuiShadow m_shadow;					// 窗口阴影

        static HINSTANCE m_hInstance;			//应用程序实例
        static DWORD m_dwMainThreadID;
        static HINSTANCE m_hResourceInstance;	//资源实例
        static HMODULE m_hMsimg32Module;		//msimg32.dll
        static BOOL m_bCachedResourceZip;
        static HANDLE m_hResourceZip;			//zip句柄
        static int m_iResourceType;				//资源加载的方式
        static CDuiString m_strResourcePath;	//资源目录路径
        static CDuiString m_strResourceZip;		//资源文件名
        static CDuiString m_pStrResourceZipPwd;	//zip密码
        static CStdPtrArray m_aPreMessages;		//CDuiPaintManager句柄
        static CStdPtrArray m_aPlugins;			//插件
        static CDuiResInfo m_SharedResInfo;		//共享的所有属性

        static short m_Hue;
        static short m_Saturation;
        static short m_Lightness;
        static BOOL m_bUseHSL;
    };

}
