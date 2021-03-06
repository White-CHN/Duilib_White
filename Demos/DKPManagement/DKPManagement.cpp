
// DKPManagement.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DKPManagement.h"
#include "DKPManageFrame.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif


// CDKPManagementApp

BEGIN_MESSAGE_MAP(CDKPManagementApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDKPManagementApp 构造

CDKPManagementApp::CDKPManagementApp()
{
    // 支持重新启动管理器
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

    // TODO: 在此处添加构造代码，
    // 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDKPManagementApp 对象

CDKPManagementApp theApp;


// CDKPManagementApp 初始化

BOOL CDKPManagementApp::InitInstance()
{
    // 如果一个运行在 Windows XP 上的应用程序清单指定要
    // 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
    //则需要 InitCommonControlsEx()。否则，将无法创建窗口。
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // 将它设置为包括所有要在应用程序中使用的
    // 公共控件类。
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();


    AfxEnableControlContainer();

    // 创建 shell 管理器，以防对话框包含
    // 任何 shell 树视图控件或 shell 列表视图控件。
    CShellManager* pShellManager = new CShellManager;

    // 标准初始化
    // 如果未使用这些功能并希望减小
    // 最终可执行文件的大小，则应移除下列
    // 不需要的特定初始化例程
    // 更改用于存储设置的注册表项
    // TODO: 应适当修改该字符串，
    // 例如修改为公司或组织名
    SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

    // 全局初始化
    CDuiPaintManager::Initialize(m_hInstance);
    // 资源类型
#ifdef _DEBUG
    CDuiPaintManager::SetResourceType(DUILIB_FILE);
#else
    CDuiPaintManager::SetResourceType(DUILIB_ZIPRESOURCE);
#endif
    switch(CDuiPaintManager::GetResourceType())
    {
        case DUILIB_FILE:
        {
            // 资源路径
            CDuiString strResourcePath = CDuiPaintManager::GetInstancePath();
            strResourcePath += _T("DKPManagement\\");
            CDuiPaintManager::SetResourcePath(strResourcePath.GetData());
        }
        break;
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
                    }
                }
                ::FreeResource(hResource);
            }
        }
        break;
    }
    // 创建主窗口
    CDKPManageFrame* pFrame = new CDKPManageFrame;
    pFrame->Create(NULL, _T("Aion DKP Management System"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->CenterWindow();
    m_pMainWnd = CWnd::FromHandle(*pFrame);
    // 消息循环
    CDuiPaintManager::MessageLoop();

    CDuiPaintManager::Uninitialize();

    // 删除上面创建的 shell 管理器。
    if(pShellManager != NULL)
    {
        delete pShellManager;
    }

    // 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
    //  而不是启动应用程序的消息泵。
    return FALSE;
}

