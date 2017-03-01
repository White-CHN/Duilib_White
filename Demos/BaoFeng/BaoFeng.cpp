// BaoFeng.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "BaoFengFrame.h"


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    // ȫ�ֳ�ʼ��
    CDuiPaintManager::Initialize(hInstance);
    // ��Դ����
    CDuiPaintManager::SetResourceType(DUILIB_FILE);
    // ��Դ·��
    CDuiString strResourcePath = CDuiPaintManager::GetInstancePath();
    strResourcePath += _T("BaoFong\\");
    CDuiPaintManager::SetResourcePath(strResourcePath.GetData());

    // ����������
    CBaoFengFrame* pBaoFengFrame = new CBaoFengFrame;
    pBaoFengFrame->Create(NULL, _T("Duilib��Դ��Ŀչʾ(By White-CHN)"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pBaoFengFrame->CenterWindow();
    // ��Ϣѭ��
    CDuiPaintManager::MessageLoop();

    CDuiPaintManager::Uninitialize();
    return 0;
}

