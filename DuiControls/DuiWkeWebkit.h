#pragma once
#include "Wke/Include/wke.h"
#pragma comment(lib, "wke\\lib\\wke.lib")
namespace DuiLib
{

#define DUI_CTR_WKEWEBKIT                          (_T("WkeWebkit"))
#define EVENT_TICK_TIEMER_ID						42

#ifdef _UNICODE
    #define _tstring std::wstring
#else
    #define _tstring std::string
#endif

    class REND_DATA
    {
    public:
        REND_DATA()
            : pixels(NULL)
            , hDC(NULL)
            , hBitmap(NULL)
        {
            ZeroMemory(&rt, sizeof(rt));
            hDC = ::CreateCompatibleDC(0);
        }

        ~REND_DATA()
        {
            if(hDC)
            {
                DeleteDC(hDC);
            }

            if(hBitmap)
            {
                DeleteObject(hBitmap);
            }
        }
    public:
        void* pixels; //渲染的内存数据
        HDC hDC; //内存设备
        HBITMAP hBitmap; //位图
        RECT rt; //渲染的矩形区域
    };

    class CDuiWkeWebkit;

    class DUILIB_API IWkeCallback
    {
    public:
        virtual void OnWkeTitleChanged(CDuiWkeWebkit* webView, LPCTSTR title) {}
        virtual void OnWkeURLChanged(CDuiWkeWebkit* webView, LPCTSTR url) {}
        virtual void OnWkeAlertBox(CDuiWkeWebkit* webView, LPCTSTR msg) {}
        virtual bool OnWkeNavigation(CDuiWkeWebkit* webView, wkeNavigationType navigationType, LPCTSTR url)
        {
            return FALSE;
        }
        virtual wkeWebView OnWkeCreateView(CDuiWkeWebkit* webView, const wkeNewViewInfo* info)
        {
            return NULL;
        }
        virtual void OnWkeDocumentReady(CDuiWkeWebkit* webView, const wkeDocumentReadyInfo* info) {}
        virtual void OnWkeLoadingFinish(CDuiWkeWebkit* webView, const LPCTSTR url, wkeLoadingResult result, LPCTSTR failedReason) {}
        virtual LPCTSTR OnJS2Native(CDuiWkeWebkit* pWeb, LPCTSTR lpMethod, LPCTSTR lpContent, void* pListenObj)
        {
            return NULL;
        }
    };

    class DUILIB_API CDuiWkeWebkit :
        public CDuiControl
    {
        DECLARE_DUICONTROL(CDuiWkeWebkit)
    public:
        CDuiWkeWebkit(void);
        virtual ~CDuiWkeWebkit(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        void DoInit() OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedUpdate = TRUE) OVERRIDE;
        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;
        void DoEvent(CDuiEvent& event) OVERRIDE;

        wkeWebView GetWebView();

        //加载url
        void Navigate(LPCTSTR lpUrl);

        //加载html代码
        void LoadHtml(LPCTSTR lpHtml);

        //关闭webkit窗口
        void Close();

        //页面操作：后退、前进、停止、刷新
        void Back();
        void Forward();
        void Stop();
        void Reload();

        //设置主页
        void SetHomePage(LPCTSTR lpHomeUrl);

        //浏览主页
        void NavigateHomePage();

        //设置页面焦点
        void SetPageFocus();

        //设置监听对象
        void SetListenObj(void* pListenObj);

        // 设置回调接口
        void SetWkeCallback(IWkeCallback* pWkeCallback);

        //设置因网络原因导致页面加载失败后的提示页面
        void SetErrorInfoPageUrl(LPCTSTR lpErrUrl);

        //执行js代码
        void ExecuteJS(LPCTSTR lpJS);
        //初始化webkit
        static void InitializeWebkit();

        //反初始化webkit
        static void UninitializeWebkit();
    private:
        // 回调事件
        static void WKE_CALL OnWkeTitleChanged(wkeWebView webView, void* param, wkeString title);
        static void WKE_CALL OnWkeURLChanged(wkeWebView webView, void* param, wkeString url);
        static void WKE_CALL OnWkeAlertBox(wkeWebView webView, void* param, wkeString msg);
        static bool WKE_CALL OnWkeNavigation(wkeWebView webView, void* param, wkeNavigationType navigationType, wkeString url);
        static wkeWebView WKE_CALL OnWkeCreateView(wkeWebView webView, void* param, const wkeNewViewInfo* info);
        static void WKE_CALL OnWkeDocumentReady(wkeWebView webView, void* param, const wkeDocumentReadyInfo* info);
        static void WKE_CALL OnWkeLoadingFinish(wkeWebView webView, void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason);
        //内置一个js与本地的函数映射
        static jsValue JS_CALL JsToNative(jsExecState es);
    private:
        void* m_pListenObj; //监听对象
        IWkeCallback* m_pWkeCallback;	// 回调接口
        wkeWebView	 m_pWebView;

        TCHAR m_chHomeUrl[1024]; //主页的url
        TCHAR m_chCurPageUrl[1024]; //当前页面的url
        TCHAR m_chErrUrl[1024]; //错误提示页面的url

        REND_DATA m_RendData;
        static map<wkeWebView, CDuiWkeWebkit*> m_mapWke2UI; //建立Wke核心到CDuiWkeWebkit的映射关系
    };

}