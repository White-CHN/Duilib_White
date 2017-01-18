// DuiDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SkinPictureBtn.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    // ������Ϣ����̨
#ifdef _DEBUG
    CDuiConsole Console ;
#endif // _DEBUG

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
            //CDuiPaintManager::SetResourceZip(_T("DuiDemo.zip"), TRUE, _T("123456"));
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
                        //CDuiPaintManager::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize, _T("123456"));
                        // ������Դ������
                        CDuiResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
                    }
                }
                ::FreeResource(hResource);
            }
        }
        break;
    }
    // ע��ؼ�
    REGIST_DUICONTROL(CDuiSkinPictureBtn);
    //
    CDemoFrame* pDemoFrame = new CDemoFrame;
    pDemoFrame->Create(NULL, _T("Duilib��Դ��Ŀչʾ(By White)"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pDemoFrame->CenterWindow();
    CDuiPaintManager::MessageLoop();	//��ģʽ�Ի���
    //pDemoFrame->ShowModal();			//ģʽ�Ի���
    CDuiPaintManager::Uninitialize();
    return 0;
}

