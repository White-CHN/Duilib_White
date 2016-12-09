// DuiDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void CALLBACK OneShotTimer(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    SYSTEMTIME now;
    GetLocalTime(&now);
    DUI_TRACE("<%d-%d-%d %d:%d:%d:%d> uTimerID[%d] uMsg[%d]",
              now.wYear, now.wMonth, now.wDay,
              now.wHour, now.wMinute, now.wSecond, now.wMilliseconds, uTimerID, uMsg);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    // 调试信息控制台
    CDuiConsole Console ;
    MMRESULT m = timeSetEvent(
                     1,               // 以毫秒指定事件的周期。
                     1,               // 以毫秒指定延时的精度，数值越小定时器事件分辨率越高。缺省值为1ms。
                     OneShotTimer,    //
                     NULL,       // 存放用户提供的回调数据
                     TIME_PERIODIC | TIME_CALLBACK_FUNCTION | TIME_KILL_SYNCHRONOUS);
    timeKillEvent(m);
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
                        // 加载资源管理器
                        CDuiResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
                    }
                }
                ::FreeResource(hResource);
            }
        }
        break;
    }
    CDemoFrame* pDemoFrame = new CDemoFrame;
    pDemoFrame->Create(NULL, _T("Duilib开源项目展示(By White)"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
    pDemoFrame->CenterWindow();
    CDuiPaintManager::MessageLoop();
    CDuiPaintManager::Uninitialize();
    return 0;
}

