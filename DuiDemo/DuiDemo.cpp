// DuiDemo.cpp : �������̨Ӧ�ó������ڵ㡣
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
    CDuiConsole Console ;// ������Ϣ����̨
    // ��ʼ��UI������
    CDuiPaintManager::Initialize(hInstance);
    // ��Դ����
#ifdef _DEBUG
    CDuiPaintManager::SetResourceType(DUILIB_FILE);
#else
    CDuiPaintManager::SetResourceType(DUILIB_ZIPRESOURCE);
#endif
    // ��Դ·��
    CDuiString strResourcePath = CDuiPaintManager::GetInstancePath();
    // ������Դ
    strResourcePath += _T("skin\\");
    CDuiPaintManager::SetResourcePath(strResourcePath.GetData());
    switch(CDuiPaintManager::GetResourceType())
    {
        case DUILIB_FILE:
        {
            // ������Դ������
            //CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
            break;
        }
    }
    CDemoFrame* pDemoFrame = new CDemoFrame;
    pDemoFrame->Create(NULL, _T("duilibʹ�����Ӽ�����By White��"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
    pDemoFrame->CenterWindow();
    CDuiPaintManager::MessageLoop();
    DUI_FREE_POINT(pDemoFrame);
    CDuiPaintManager::Uninitialize();
    OleUninitialize();
    ::CoUninitialize();
    return 0;
}

