// DuiDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SkinPictureBtn.h"


//多线程消息循环,仿WTL
class CSDIThreadManager
{
public:
    // thread init param
    struct _RunData
    {
        LPTSTR lpstrCmdLine;
        int nCmdShow;
    };

    // thread proc
    static DWORD WINAPI RunThread(LPVOID lpData)
    {
        _RunData* pData = (_RunData*)lpData;
        // 创建主窗口
        CDemoFrame* pDemoFrame = new CDemoFrame;
        pDemoFrame->Create(NULL, _T("Duilib开源项目展示(By White-CHN)"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
        pDemoFrame->CenterWindow();
        // 消息循环
        CDuiPaintManager::MessageLoop();	//非模式对话框
        //pDemoFrame->ShowModal();			//模式对话框

        delete pData;

        return 0;
    }

    DWORD m_dwCount;
    HANDLE m_arrThreadHandles[MAXIMUM_WAIT_OBJECTS - 1];

    CSDIThreadManager() : m_dwCount(0)
    {
        ZeroMemory(m_arrThreadHandles, MAXIMUM_WAIT_OBJECTS - 1);
    }

    // Operations
    DWORD AddThread(LPTSTR lpstrCmdLine, int nCmdShow)
    {
        if(m_dwCount == (MAXIMUM_WAIT_OBJECTS - 1))
        {
            ::MessageBox(NULL, _T("ERROR: Cannot create ANY MORE threads!!!"), _T("SDI"), MB_OK);
            return 0;
        }

        _RunData* pData = new _RunData;
        pData->lpstrCmdLine = lpstrCmdLine;
        pData->nCmdShow = nCmdShow;
        DWORD dwThreadID;
        HANDLE hThread = ::CreateThread(NULL, 0, RunThread, pData, 0, &dwThreadID);
        if(hThread == NULL)
        {
            ::MessageBox(NULL, _T("ERROR: Cannot create thread!!!"), _T("SDI"), MB_OK);
            return 0;
        }

        m_arrThreadHandles[m_dwCount] = hThread;
        m_dwCount++;
        return dwThreadID;
    }

    void RemoveThread(DWORD dwIndex)
    {
        ::CloseHandle(m_arrThreadHandles[dwIndex]);
        if(dwIndex != (m_dwCount - 1))
        {
            m_arrThreadHandles[dwIndex] = m_arrThreadHandles[m_dwCount - 1];
        }
        m_dwCount--;
    }

    int Run(LPTSTR lpstrCmdLine, int nCmdShow)
    {
        MSG msg = {0};
        // force message queue to be created
        ::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

        AddThread(lpstrCmdLine, nCmdShow);

        int nRet = m_dwCount;
        DWORD dwRet;
        while(m_dwCount > 0)
        {
            dwRet = ::MsgWaitForMultipleObjects(m_dwCount, m_arrThreadHandles, FALSE, INFINITE, QS_ALLINPUT);

            if(dwRet == 0xFFFFFFFF)
            {
                ::MessageBox(NULL, _T("ERROR: Wait for multiple objects failed!!!"), _T("SDI"), MB_OK);
            }
            else if(dwRet >= WAIT_OBJECT_0 && dwRet <= (WAIT_OBJECT_0 + m_dwCount - 1))
            {
                RemoveThread(dwRet - WAIT_OBJECT_0);
            }
            else if(dwRet == (WAIT_OBJECT_0 + m_dwCount))
            {
                if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    if(msg.message == WM_USER)
                    {
                        AddThread(_T(""), SW_SHOWNORMAL);
                    }
                }
            }
            else
            {
                ::MessageBeep((UINT) - 1);
            }
        }

        return nRet;
    }
};


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    // 调试信息控制台
#ifdef _DEBUG
    CDuiConsole Console ;
#endif // _DEBUG

    // 全局初始化
    CDuiPaintManager::Initialize(hInstance);
    // 资源类型
#ifdef _DEBUG
    CDuiPaintManager::SetResourceType(DUILIB_FILE);
#else
    CDuiPaintManager::SetResourceType(DUILIB_ZIPRESOURCE);
#endif
    // 资源路径
    CDuiString strResourcePath = CDuiPaintManager::GetInstancePath();
    // 加载资源
    switch(CDuiPaintManager::GetResourceType())
    {
        case DUILIB_FILE:
        {
            strResourcePath += _T("DuiDemo\\");
            CDuiPaintManager::SetResourcePath(strResourcePath.GetData());
            // 加载资源管理器
            CDuiResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
            break;
        }
        case DUILIB_RESOURCE:
        {
            break;
        }
        case DUILIB_ZIP:
        {
            strResourcePath += _T("DuiDemo\\");
            CDuiPaintManager::SetResourcePath(strResourcePath.GetData());
            CDuiPaintManager::SetResourceZip(_T("DuiDemo.zip"), TRUE);
            //CDuiPaintManager::SetResourceZip(_T("DuiDemo.zip"), TRUE, _T("123456"));
            // 加载资源管理器
            CDuiResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
            break;
        }
        case DUILIB_ZIPRESOURCE:
        {
            HRSRC hResource = ::FindResource(CDuiPaintManager::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
            if(hResource != NULL)
            {
                DWORD dwSize = 0;
                HGLOBAL hGlobal = ::LoadResource(CDuiPaintManager::GetResourceDll(), hResource);
                if(hGlobal != NULL)
                {
                    dwSize = ::SizeofResource(CDuiPaintManager::GetResourceDll(), hResource);
                    if(dwSize > 0)
                    {
                        CDuiPaintManager::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
                        //CDuiPaintManager::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize, _T("123456"));
                        // 加载资源管理器
                        CDuiResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
                    }
                }
                ::FreeResource(hResource);
            }
        }
        break;
    }
    // 注册用户自定义控件
    REGIST_DUICONTROL(CDuiSkinPictureBtn);
#define SINGLETHREAD
#ifdef SINGLETHREAD
    // 单线程消息循环
    // 创建主窗口
    CDemoFrame* pDemoFrame = new CDemoFrame;
    pDemoFrame->Create(NULL, _T("Duilib开源项目展示(By White-CHN)"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pDemoFrame->CenterWindow();
    // 消息循环
    CDuiPaintManager::MessageLoop();	//非模式对话框
    //pDemoFrame->ShowModal();			//模式对话框
#else
    // 多线程消息循环
    CSDIThreadManager mgr;
    mgr.Run(lpstrCmdLine, nCmdShow);
#endif

    CDuiPaintManager::Uninitialize();
    return 0;
}

