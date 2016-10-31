// DuiDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	if( FAILED(hRes) ) return 0;
	hRes = ::OleInitialize(NULL);
	if( FAILED(hRes) ) return 0;
	// ������Ϣ����̨
	CDuiConsole Console ;
	// ��ʼ��UI������
	CDuiPaintManager::SetInstance(hInstance);

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
			strResourcePath += _T("skin\\duidemo\\");
			CDuiPaintManager::SetResourcePath(strResourcePath.GetData());
			// ������Դ������
			//CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	}
	OleUninitialize();
	::CoUninitialize();
	return 0;
}

