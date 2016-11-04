// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "..\DuiLib\Duilib.h"

using namespace DuiLib;

#ifdef _WIN64
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib, "..\\lib\\x64\\DuiLib_d.lib")
		#else
			#pragma comment(lib, "..\\lib\\x64\\DuiLibA_d.lib")
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib, "..\\lib\\x64\\DuiLib.lib")
		#else
			#pragma comment(lib, "..\\lib\\x64\\DuiLibA.lib")
		#endif
	#endif
#else
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib, "..\\lib\\win32\\DuiLib_d.lib")
		#else
			#pragma comment(lib, "..\\lib\\win32\\DuiLibA_d.lib")
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib, "..\\lib\\win32\\DuiLib.lib")
		#else
			#pragma comment(lib, "..\\lib\\win32\\DuiLibA.lib")
		#endif
	#endif
#endif
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include "DemoFrame.h"
