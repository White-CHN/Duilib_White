// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
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
// TODO: 在此处引用程序需要的其他头文件

#include "DemoFrame.h"
