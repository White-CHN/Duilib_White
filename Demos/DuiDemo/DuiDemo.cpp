// DuiDemo.cpp : �������̨Ӧ�ó������ڵ㡣
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
    // ������Ϣ����̨
    CDuiConsole Console ;
    MMRESULT m = timeSetEvent(
                     1,               // �Ժ���ָ���¼������ڡ�
                     1,               // �Ժ���ָ����ʱ�ľ��ȣ���ֵԽС��ʱ���¼��ֱ���Խ�ߡ�ȱʡֵΪ1ms��
                     OneShotTimer,    //
                     NULL,       // ����û��ṩ�Ļص�����
                     TIME_PERIODIC | TIME_CALLBACK_FUNCTION | TIME_KILL_SYNCHRONOUS);
    timeKillEvent(m);
    // ȫ�ֳ�ʼ��
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
    switch(CDuiPaintManager::GetResourceType())
    {
        case DUILIB_FILE:
        {
            strResourcePath += _T("DuiDemo\\");
            CDuiPaintManager::SetResourcePath(strResourcePath.GetData());
            // ������Դ������
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
            // ������Դ������
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
                        // ������Դ������
                        CDuiResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
                    }
                }
                ::FreeResource(hResource);
            }
        }
        break;
    }
    CDemoFrame* pDemoFrame = new CDemoFrame;
    pDemoFrame->Create(NULL, _T("Duilib��Դ��Ŀչʾ(By White)"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
    pDemoFrame->CenterWindow();
    CDuiPaintManager::MessageLoop();
    CDuiPaintManager::Uninitialize();
    return 0;
}

