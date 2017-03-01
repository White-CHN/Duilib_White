// BaoFeng.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BaoFengFrame.h"


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    // 全局初始化
    CDuiPaintManager::Initialize(hInstance);
    // 资源类型
    CDuiPaintManager::SetResourceType(DUILIB_FILE);
    // 资源路径
    CDuiString strResourcePath = CDuiPaintManager::GetInstancePath();
    strResourcePath += _T("BaoFong\\");
    CDuiPaintManager::SetResourcePath(strResourcePath.GetData());

    // 创建主窗口
    CBaoFengFrame* pBaoFengFrame = new CBaoFengFrame;
    pBaoFengFrame->Create(NULL, _T("Duilib开源项目展示(By White-CHN)"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pBaoFengFrame->CenterWindow();
    // 消息循环
    CDuiPaintManager::MessageLoop();

    CDuiPaintManager::Uninitialize();
    return 0;
}

