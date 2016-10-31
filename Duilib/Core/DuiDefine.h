#pragma once

namespace DuiLib {


	//!检查并释放指针
#define UTILITY_FREE_POINT(x)			\
	{									\
		if (NULL != x)					\
		{								\
			delete x;					\
			x = NULL;					\
		}								\
	}

	//!检查并释放数组
#define UTILITY_FREE_ARRAY(x)			\
	{									\
		if (NULL != x)					\
		{								\
			delete[] x;					\
			x = NULL;					\
		}								\
	}

#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

#ifdef _DEBUG
	#ifndef DUI_TRACE
	#define DUI_TRACE(...) _tprintf(_T("\r\n[%s][Line:%d]"),__FUNCTIONW__,__LINE__);DuiTrace(__VA_ARGS__)
	#endif	
#else
	#ifndef DUI_TRACE
	#define DUI_TRACE 
	#endif
#endif	// _DEBUG

	void DUILIB_API DuiTrace(LPCTSTR pstrFormat, ...);















}