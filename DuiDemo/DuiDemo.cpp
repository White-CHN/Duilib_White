// DuiDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CDuiPaintManager::SetInstance(hInstance);
	CDuiPaintManager::SetResourceType(DUILIB_FILE);
	return 0;
}

