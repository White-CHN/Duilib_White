
// DKPManagement.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDKPManagementApp:
// �йش����ʵ�֣������ DKPManagement.cpp
//

class CDKPManagementApp : public CWinApp
{
public:
	CDKPManagementApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDKPManagementApp theApp;