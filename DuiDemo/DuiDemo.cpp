// DuiDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CDuiPaintManager::SetInstance(hInstance);
	CDuiPaintManager::SetResourceType(DUILIB_FILE);
	return 0;
}

