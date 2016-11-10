#include "StdAfx.h"
#include "DuiDefine.h"

namespace DuiLib
{



    void DUILIB_API DuiTrace(LPCTSTR pstrFormat, ...)
    {
#ifdef _DEBUG
        TCHAR szBuffer[2048] = {0};
        va_list args;
        va_start(args, pstrFormat);
        _vsntprintf(szBuffer, 2048, pstrFormat, args);
        va_end(args);
        _tprintf(szBuffer);
#endif	// _DEBUG
    }

}