// DuiDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    HRESULT hRes = ::CoInitialize(NULL);
    if(FAILED(hRes))
    {
        return 0;
    }
    hRes = ::OleInitialize(NULL);
    if(FAILED(hRes))
    {
        return 0;
    }
    CDuiConsole Console ;// 调试信息控制台

    // 初始化UI管理器
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
            //CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
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
                        //CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
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
    DUI_FREE_POINT(pDemoFrame);
    CDuiPaintManager::Uninitialize();
    OleUninitialize();
    ::CoUninitialize();
    return 0;
}

