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
        void* pixels; //��Ⱦ���ڴ�����
        HDC hDC; //�ڴ��豸
        HBITMAP hBitmap; //λͼ
        RECT rt; //��Ⱦ�ľ�������
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

        //����url
        void Navigate(LPCTSTR lpUrl);

        //����html����
        void LoadHtml(LPCTSTR lpHtml);

        //�ر�webkit����
        void Close();

        //ҳ����������ˡ�ǰ����ֹͣ��ˢ��
        void Back();
        void Forward();
        void Stop();
        void Reload();

        //������ҳ
        void SetHomePage(LPCTSTR lpHomeUrl);

        //�����ҳ
        void NavigateHomePage();

        //����ҳ�潹��
        void SetPageFocus();

        //���ü�������
        void SetListenObj(void* pListenObj);

        // ���ûص��ӿ�
        void SetWkeCallback(IWkeCallback* pWkeCallback);

        //����������ԭ����ҳ�����ʧ�ܺ����ʾҳ��
        void SetErrorInfoPageUrl(LPCTSTR lpErrUrl);

        //ִ��js����
        void ExecuteJS(LPCTSTR lpJS);
        //��ʼ��webkit
        static void InitializeWebkit();

        //����ʼ��webkit
        static void UninitializeWebkit();
    private:
        // �ص��¼�
        static void WKE_CALL OnWkeTitleChanged(wkeWebView webView, void* param, wkeString title);
        static void WKE_CALL OnWkeURLChanged(wkeWebView webView, void* param, wkeString url);
        static void WKE_CALL OnWkeAlertBox(wkeWebView webView, void* param, wkeString msg);
        static bool WKE_CALL OnWkeNavigation(wkeWebView webView, void* param, wkeNavigationType navigationType, wkeString url);
        static wkeWebView WKE_CALL OnWkeCreateView(wkeWebView webView, void* param, const wkeNewViewInfo* info);
        static void WKE_CALL OnWkeDocumentReady(wkeWebView webView, void* param, const wkeDocumentReadyInfo* info);
        static void WKE_CALL OnWkeLoadingFinish(wkeWebView webView, void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason);
        //����һ��js�뱾�صĺ���ӳ��
        static jsValue JS_CALL JsToNative(jsExecState es);
    private:
        void* m_pListenObj; //��������
        IWkeCallback* m_pWkeCallback;	// �ص��ӿ�
        wkeWebView	 m_pWebView;

        TCHAR m_chHomeUrl[1024]; //��ҳ��url
        TCHAR m_chCurPageUrl[1024]; //��ǰҳ���url
        TCHAR m_chErrUrl[1024]; //������ʾҳ���url

        REND_DATA m_RendData;
        static map<wkeWebView, CDuiWkeWebkit*> m_mapWke2UI; //����Wke���ĵ�CDuiWkeWebkit��ӳ���ϵ
    };

}