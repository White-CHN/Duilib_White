// DuiDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	if( FAILED(hRes) ) return 0;
	hRes = ::OleInitialize(NULL);
	if( FAILED(hRes) ) return 0;
	// 调试信息控制台
	CDuiConsole Console ;
	// 初始化UI管理器
	CDuiPaintManager::SetInstance(hInstance);

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
			strResourcePath += _T("skin\\duidemo\\");
			CDuiPaintManager::SetResourcePath(strResourcePath.GetData());
			// 加载资源管理器
			//CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	}
	OleUninitialize();
	::CoUninitialize();
	return 0;
}

