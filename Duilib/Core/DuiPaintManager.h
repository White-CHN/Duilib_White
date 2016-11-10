#pragma once

namespace DuiLib
{

    enum DUILIB_RESTYPE
    {
        DUILIB_FILE		=  1,		// 来自磁盘文件
        DUILIB_ZIP			,		// 来自磁盘zip压缩包
        DUILIB_RESOURCE		,		// 来自资源
        DUILIB_ZIPRESOURCE	,		// 来自资源的zip压缩包
    };

    typedef enum EVENTTYPE_UI
    {
        UIEVENT__FIRST = 1,
        UIEVENT__KEYBEGIN,
        UIEVENT_KEYDOWN,
        UIEVENT_KEYUP,
        UIEVENT_CHAR,
        UIEVENT_SYSKEY,
        UIEVENT__KEYEND,
        UIEVENT__MOUSEBEGIN,
        UIEVENT_MOUSEMOVE,
        UIEVENT_MOUSELEAVE,
        UIEVENT_MOUSEENTER,
        UIEVENT_MOUSEHOVER,
        UIEVENT_BUTTONDOWN,
        UIEVENT_BUTTONUP,
        UIEVENT_RBUTTONDOWN,
        UIEVENT_RBUTTONUP,
        UIEVENT_MBUTTONDOWN,
        UIEVENT_MBUTTONUP,
        UIEVENT_DBLCLICK,
        UIEVENT_CONTEXTMENU,
        UIEVENT_SCROLLWHEEL,
        UIEVENT__MOUSEEND,
        UIEVENT_KILLFOCUS,
        UIEVENT_SETFOCUS,
        UIEVENT_WINDOWSIZE,
        UIEVENT_SETCURSOR,
        UIEVENT_TIMER,
        UIEVENT__LAST,
    };

    // Structure for notifications from the system
    // to the control implementation.
    typedef struct tagTEventUI
    {
        int Type;
        CDuiControl* pSender;
        DWORD dwTimestamp;
        POINT ptMouse;
        TCHAR chKey;
        WORD wKeyState;
        WPARAM wParam;
        LPARAM lParam;
    } TEventUI;

    // Structure for notifications to the outside world
    typedef struct tagTNotifyUI
    {
        CDuiString sType;
        CDuiString sVirtualWnd;
        CDuiControl* pSender;
        DWORD dwTimestamp;
        POINT ptMouse;
        WPARAM wParam;
        LPARAM lParam;
    } TNotifyUI;

    typedef struct tagTFontInfo
    {
        HFONT hFont;
        CDuiString sFontName;
        int iSize;
        BOOL bBold;
        BOOL bUnderline;
        BOOL bItalic;
        TEXTMETRIC tm;
    } TFontInfo;

    typedef struct tagTResInfo
    {
        DWORD m_dwDefaultDisabledColor;
        DWORD m_dwDefaultFontColor;
        DWORD m_dwDefaultLinkFontColor;
        DWORD m_dwDefaultLinkHoverFontColor;
        DWORD m_dwDefaultSelectedBkColor;
        TFontInfo m_DefaultFontInfo;
        CStdStringPtrMap m_CustomFonts;
        CStdStringPtrMap m_ImageHash;
        CStdStringPtrMap m_AttrHash;
        CStdStringPtrMap m_StyleHash;
        CStdStringPtrMap m_DrawInfoHash;
    } TResInfo;

    class CDuiPaintManager;

    typedef struct tagTDrawInfo
    {
        tagTDrawInfo();
        void Parse(LPCTSTR pStrImage, LPCTSTR pStrModify, CDuiPaintManager* paintManager);
        void Clear();

        CDuiString sDrawString;
        CDuiString sDrawModify;
        CDuiString sImageName;
        CDuiString sResType;
        RECT rcDest;
        RECT rcSource;
        RECT rcCorner;
        DWORD dwMask;
        BYTE uFade;
        BOOL bHole;
        BOOL bTiledX;
        BOOL bTiledY;
        BOOL bHSL;
    } TDrawInfo;

    typedef struct tagTImageInfo
    {
        HBITMAP hBitmap;
        LPBYTE pBits;
        LPBYTE pSrcBits;
        int nX;
        int nY;
        BOOL bAlpha;
        BOOL bUseHSL;
        CDuiString sResType;
        DWORD dwMask;
    } TImageInfo;

    typedef struct tagFINDTABINFO
    {
        CDuiControl* pFocus;
        CDuiControl* pLast;
        BOOL bForward;
        BOOL bNextIsIt;
    } FINDTABINFO;

    class ITranslateAccelerator
    {
    public:
        virtual LRESULT TranslateAccelerator(MSG* pMsg) = 0;
    };

    // Listener interface
    class INotifyUI
    {
    public:
        virtual void Notify(TNotifyUI& msg) = 0;
    };

    // MessageFilter interface
    class IMessageFilterUI
    {
    public:
        virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) = 0;
    };

    typedef CDuiControl* (*LPCREATECONTROL)(LPCTSTR pstrType);

    class DUILIB_API CDuiPaintManager
    {
    public:
        CDuiPaintManager(void);
        ~CDuiPaintManager(void);

    public:
        void Init(HWND hWnd);
        void NeedUpdate();
        void Invalidate();
        void Invalidate(RECT& rcItem);

        HDC GetPaintDC() const;

        POINT GetMousePos() const;

        void SetPainting(BOOL bIsPainting);

        BOOL MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
        BOOL AddPreMessageFilter(IMessageFilterUI* pFilter);

        HWND GetPaintWindow() const;

        void RemoveImage(LPCTSTR bitmap, BOOL bShared = FALSE);

        //属性设置与获取
        void SetInitSize(int cx, int cy);

        RECT& GetSizeBox();
        void SetSizeBox(RECT& rcSizeBox);
        RECT& GetCaptionRect();
        void SetCaptionRect(RECT& rcCaption);

        SIZE GetRoundCorner() const;
        void SetRoundCorner(int cx, int cy);

        SIZE GetMinInfo() const;
        void SetMinInfo(int cx, int cy);

        SIZE GetMaxInfo() const;
        void SetMaxInfo(int cx, int cy);

        void SetShowUpdateRect(BOOL bShow);


        void SetOpacity(BYTE nOpacity);

        void SetLayered(BOOL bLayered);

        void SetLayeredOpacity(BYTE nOpacity);

        DWORD GetDefaultDisabledColor() const;
        void SetDefaultDisabledColor(DWORD dwColor, BOOL bShared = FALSE);

        DWORD GetDefaultFontColor() const;
        void SetDefaultFontColor(DWORD dwColor, BOOL bShared = FALSE);

        void SetDefaultLinkFontColor(DWORD dwColor, BOOL bShared = FALSE);
        void SetDefaultLinkHoverFontColor(DWORD dwColor, BOOL bShared = FALSE);
        void SetDefaultSelectedBkColor(DWORD dwColor, BOOL bShared = FALSE);
        // 样式管理
        void AddStyle(LPCTSTR pName, LPCTSTR pstrStyle, BOOL bShared = FALSE);
        LPCTSTR GetStyle(LPCTSTR pName) const;

        //焦点相关
        CDuiControl* GetFocus() const;
        void SetFocus(CDuiControl* pControl);

        // 光标
        void DrawCaret(HDC hDC, const RECT& rcPaint);

        //字体相关
        TFontInfo* GetDefaultFontInfo();
        void SetDefaultFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic, BOOL bShared = false);
        HFONT AddFont(int id, LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic, BOOL bShared = FALSE);
        HFONT GetFont(int id);
        HFONT GetFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic);
        TFontInfo* GetFontInfo(int id);
        TFontInfo* GetFontInfo(HFONT hFont);

        void SetCapture();

        //虚拟窗口
        void UsedVirtualWnd(BOOL bUsed);
        //Gdiplus相关
        BOOL IsUseGdiplusText() const;
        int GetGdiplusTextRenderingHint() const;
        void SetUseGdiplusText(BOOL bUse);
        void SetGdiplusTextRenderingHint(int trh);

        BOOL SetNextTabControl(BOOL bForward = TRUE);

        CDPI* GetDPIObj();

        CDuiShadow* GetShadow();

        BOOL AddNotifier(INotifyUI* pNotifier);
        void SendNotify(TNotifyUI& Msg, BOOL bAsync = FALSE);
        void SendNotify(CDuiControl* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bAsync = FALSE);

        void AddDelayedCleanup(CDuiControl* pControl);

        DWORD GetDefaultLinkFontColor() const;
        DWORD GetDefaultLinkHoverFontColor() const;
        DWORD GetDefaultSelectedBkColor() const;

        void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList, BOOL bShared = FALSE);
        LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;


        BOOL InitControls(CDuiControl* pControl, CDuiControl* pParent = NULL);
        BOOL AttachDialog(CDuiControl* pControl);

        CDuiControl* FindControl(POINT pt) const;
        CDuiControl* FindControl(LPCTSTR pstrName) const;

        BOOL IsLayered();
        const TImageInfo* GetImage(LPCTSTR bitmap);
        const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0, BOOL bUseHSL = FALSE, HINSTANCE instance = NULL);
        const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0, BOOL bUseHSL = FALSE, BOOL bShared = FALSE, HINSTANCE instance = NULL);
        const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, BOOL bAlpha, BOOL bShared = FALSE);

        const TDrawInfo* GetDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify);
    private:
        BOOL TranslateAccelerator(LPMSG pMsg);
        BOOL PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

    public:
        static HINSTANCE GetInstance();
        static BOOL Initialize(HINSTANCE hInstance);
        static void Uninitialize();
        static CDuiString GetInstancePath();
        static HINSTANCE GetResourceDll();
        static int GetResourceType();
        static const CDuiString& GetResourcePath();
        static const CDuiString& GetResourceZip();
        static void SetResourceType(int iResourceType);
        static void SetResourcePath(LPCTSTR pStrPath);

        static BOOL IsCachedResourceZip();
        static HANDLE GetResourceZipHandle();

        static BOOL GetHSL(short* H, short* S, short* L);

        static BOOL TranslateMessage(const LPMSG pMsg);
        static void MessageLoop();

        static void GetChildWndRect(HWND hWnd, HWND hChildWnd, RECT& rcChildWnd);
        //插件
        static BOOL LoadPlugin(LPCTSTR pstrModuleName);
        static CStdPtrArray* GetPlugins();
    private:
        CStdPtrArray* GetFoundControls();
        static CDuiControl* CALLBACK __FindControlFromNameHash(CDuiControl* pThis, LPVOID pData);
        static CDuiControl* CALLBACK __FindControlsFromUpdate(CDuiControl* pThis, LPVOID pData);
        static CDuiControl* CALLBACK __FindControlFromTab(CDuiControl* pThis, LPVOID pData);
        static CDuiControl* CALLBACK __FindControlFromPoint(CDuiControl* pThis, LPVOID pData);
    private:
        HWND m_hWndPaint;						//所属的窗体的句柄
        HDC m_hDcPaint;							//所属的窗体的HDC
        HDC m_hDcOffscreen;
        HBITMAP m_hbmpOffscreen;

        BYTE m_nOpacity;						//alpha数值

        BOOL m_bUpdateNeeded;
        BOOL m_bLayered;
        BOOL m_bForceUseSharedRes;
        BOOL m_bUsedVirtualWnd;					//虚拟窗口
        BOOL m_bIsPainting;
        BOOL m_bFocusNeeded;
        BOOL m_bOffscreenPaint;
        BOOL m_bShowUpdateRect;					//窗口红色边框
        BOOL m_bFirstLayout;
        BOOL m_bLayeredChanged;
        BOOL m_bMouseCapture;					//是否设置鼠标捕获

        BOOL m_bUseGdiplusText;					//gdiplustext属性,是否用gdi+渲染文字
        int m_trh;								//textrenderinghint属性,gdi+渲染文字提示
        ULONG_PTR m_gdiplusToken;				//Gdiplus相关
        Gdiplus::GdiplusStartupInput* m_pGdiplusStartupInput;

        BYTE* m_pOffscreenBits;

        CDuiControl* m_pRoot;					//xml 根节点
        CDuiControl* m_pFocus;					//当前焦点控件
        CDuiControl* m_pEventClick;				//当前点击的控件

        CDPI* m_pDPI;

        POINT m_ptLastMousePos;					//鼠标坐标
        RECT m_rcLayeredUpdate;
        RECT m_rcSizeBox;						//sizebox属性
        RECT m_rcCaption;						//caption属性
        SIZE m_szInitWindowSize;				//size属性
        SIZE m_szRoundCorner;					//roundcorner属性
        SIZE m_szMinWindow;						//mininfo属性
        SIZE m_szMaxWindow;						//maxinfo属性



        CStdPtrArray m_aMessageFilters;
        CStdPtrArray m_aTranslateAccelerator;
        CStdPtrArray m_aPreMessageFilters;		//拦截在DispatchMessage消息之前的消息，注册的窗口句柄
        CStdPtrArray m_aNotifiers;				//
        CStdPtrArray m_aAsyncNotify;			//

        CStdPtrArray m_aDelayedCleanup;

        CStdPtrArray m_aPostPaintControls;
        CStdPtrArray m_aFoundControls;

        CStdPtrArray m_aChildWnds;

        CStdStringPtrMap m_mNameHash;

        TResInfo m_ResInfo;


        CDuiShadow m_shadow;						// 窗口阴影

        static HINSTANCE m_hInstance;			//应用程序实例
        static HINSTANCE m_hResourceInstance;	//资源实例
        static HMODULE m_hMsimg32Module;		//msimg32.dll
        static BOOL m_bCachedResourceZip;
        static HANDLE m_hResourceZip;
        static int m_iResourceType;				//资源加载的方式
        static CDuiString m_strResourcePath;	//资源目录路径
        static CDuiString m_strResourceZip;	//资源文件名
        static CStdPtrArray m_aPreMessages;		//所有CDuiPaintManager句柄
        static CStdPtrArray m_aPlugins;			//所有插件
        static TResInfo m_SharedResInfo;		//默认属性

        static short m_H;
        static short m_S;
        static short m_L;
        static BOOL m_bUseHSL;

    };

}
