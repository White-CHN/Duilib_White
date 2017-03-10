#include "StdAfx.h"
#include "Utility.h"
#include "MiniAdo.h"

int           g_Tracecount = 0;
SYSTEMTIME    g_LastTime;


CString Convert::GetCurrentDate()
{
    CTime dt = CTime::GetCurrentTime();
    CString str;
    str.Format(_T("%.4d-%.2d-%.2d"), dt.GetYear(), dt.GetMonth(), dt.GetDay());
    return str;

}

CString Convert::GetCurrentDateTime()
{
    CTime dt = CTime::GetCurrentTime();
    CString str;
    str.Format(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d"), dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond());
    return str;

}

BOOL Convert::StrToNum(CString str, int& nResult)
{
    str.TrimLeft();
    str.TrimRight();
    if(str.GetLength() <= 0)
    {
        return FALSE;
    }

    TCHAR* stop;
    nResult = _tcstol(str, &stop, 10);
    if((*stop) || nResult == LONG_MAX || nResult == LONG_MIN)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

BOOL Convert::StrToNum(CString str, int& nResult, CString& sStop)
{
    str.TrimLeft();
    str.TrimRight();
    if(str.GetLength() <= 0)
    {
        return FALSE;
    }

    TCHAR* stop;

    nResult = _tcstol(str, &stop, 10);
    if(stop != NULL)
    {
        sStop = stop;
    }

    if((*stop) || nResult == LONG_MAX || nResult == LONG_MIN)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

CString Convert::FormatScaleAnno(CString sScale)//格式化比例尺注记
{
    //格式化比例尺分母
    int len = sScale.GetLength();
    int nsize = len / 3;
    int size = len % 3;
    CString stemp = sScale;
    if(len <= 3)
    {
        return sScale;
    }
    for(int i = 0; i < nsize; i++)
    {
        if(size > 0 && i == 0)
        {
            sScale = stemp.Left(size) + " " + stemp.Mid(size, 3);
        }
        else
        {
            sScale = sScale + stemp.Mid(size + 3 * i, 3);
        }
    }
    return sScale;
}

BOOL Convert::StrToNum(CString str, float& fResult, CString& sStop)
{
    str.TrimLeft();
    str.TrimRight();
    if(str.GetLength() <= 0)
    {
        return FALSE;
    }

    TCHAR* stop;
    fResult = (float)_tcstod(str.GetBuffer(), &stop);

    if(stop != NULL)
    {
        sStop = stop;
    }

    if((*stop)  ||	 fResult == +HUGE_VAL || fResult == -HUGE_VAL)
    {
        return 	 FALSE;
    }
    else
    {
        return      TRUE;
    }

}

BOOL Convert::StrToNum(CString str, float& fResult)
{
    str.TrimLeft();
    str.TrimRight();
    if(str.GetLength() <= 0)
    {
        return FALSE;
    }

    TCHAR* stop;
    fResult = (float)_tcstod(str.GetBuffer(), &stop);

    if((*stop) || fResult == +HUGE_VAL || fResult == -HUGE_VAL)
    {
        return 	 FALSE;
    }
    else
    {
        return      TRUE;
    }

}

BOOL Convert::StrToNum(CString str, double& fResult)
{
    str.TrimLeft();
    str.TrimRight();
    if(str.GetLength() <= 0)
    {
        return FALSE;
    }

    TCHAR* stop;
    fResult = _tcstod(str.GetBuffer(), &stop);

    if((*stop) || fResult == +HUGE_VAL || fResult == -HUGE_VAL)
    {
        return 	 FALSE;
    }
    else
    {
        return      TRUE;
    }

}

int Convert::Str2Num(CString str, int defResult)
{
    if(str.IsEmpty())
    {
        return   defResult;
    }
    int n;
    if(Convert::StrToNum(str, n))
    {
        return n;
    }
    else
    {
        return defResult;
    }
}

BOOL Convert::VarToStr(VARIANT var, CString& str)
{
    str.Empty();

    try
    {
        switch(var.vt)
        {
            case VT_DATE:
            {
                COleDateTime dt(var);
                if(dt.GetStatus() == COleDateTime::valid)
                {
                    if(dt.m_dt > 0)  // 解决出现1899-的问题
                    {
                        if(dt.GetHour() == 0 && dt.GetMinute() == 0 && dt.GetSecond() == 0)
                            //str=dt.Format("%Y-%m-%d");
                        {
                            str.Format(_T("%.4d-%.2d-%.2d"), dt.GetYear(), dt.GetMonth(), dt.GetDay());
                        }
                        else
                        {
                            str.Format(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d"), dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond());
                        }
                    }
                }
            }
            break;
            case VT_BSTR:
                str = (LPCTSTR)(_bstr_t)var.bstrVal;
                break;
            case VT_I2:
            case VT_UI2:
                str.Format(_T("%d"), var.iVal);
                break;
            case VT_I4:
                str.Format(_T("%d"), var.lVal);
                break;
            case VT_R4:
                str.Format(_T("%.8f"), var.fltVal + 0.000000001); //2014.06.12
                {
                    int  nDigit = str.GetLength();
                    int i = 0;
                    for(; i < 9 ; i++)
                    {
                        if(str[nDigit - i - 1] != '.' && str[nDigit - i - 1] != '0')
                        {
                            break;
                        }
                    }

                    str = str.Left(nDigit - i);
                }

                break;
            case VT_R8:
                str.Format(_T("%.8f"), var.dblVal + 0.000000001); // 精度
                {
                    int  nDigit = str.GetLength();
                    int i = 0;
                    for(; i < 9 ; i++)
                    {
                        if(str[nDigit - i - 1] != '.' && str[nDigit - i - 1] != '0')
                        {
                            break;
                        }
                    }

                    str = str.Left(nDigit - i);
                }


                break;
            default:
                return FALSE;
                break;
        }
    }
    catch(...)
    {
        ATLTRACE("\n convert error");
    }


    str.TrimLeft();
    str.TrimRight();
    return TRUE;
}

BOOL Convert::VarToNum(VARIANT var, int& num)
{
    CString str;
    if(!Convert::VarToStr(var, str))
    {
        return FALSE;
    }
    if(!Convert::StrToNum(str, num))
    {
        return FALSE;
    }

    return TRUE;

}

int Convert::Var2Num(VARIANT var, int defValue)
{
    CString str;

    if(!Convert::VarToStr(var, str))
    {
        return defValue;
    }

    int num = defValue;
    if(!Convert::StrToNum(str, num))
    {
        return defValue;
    }

    return num;

}

double Convert::Var2Double(VARIANT var, double defValue)
{
    CString str;

    if(!Convert::VarToStr(var, str))
    {
        return defValue;
    }

    double num = defValue;
    if(!Convert::StrToNum(str, num))
    {
        return defValue;
    }

    return num;
}

CString	Convert::Var2Str(VARIANT var, CString defValue)
{
    CString str;

    if(!Convert::VarToStr(var, str))
    {
        return defValue;
    }

    return str;
}

BOOL Convert::VarToNum(VARIANT var, double& fResult)
{
    CString str;
    if(!Convert::VarToStr(var, str))
    {
        return FALSE;
    }
    if(!Convert::StrToNum(str, fResult))
    {
        return FALSE;
    }

    return TRUE;
}


BOOL Convert::FormatToDate(CString str, CString& sDate)
{
    sDate = "";

    COleDateTime dt;

    if(Convert::StrToDate(str, dt))
    {
        sDate.Format(_T("%.4d-%.2d-%.2d"), dt.GetYear(), dt.GetMonth(), dt.GetDay());
        return true;
    }

    return false;
}

BOOL Convert::StrToDate(CString str, COleDateTime& dt)
{
    str.TrimLeft();
    str.TrimRight();
    if(str.GetLength() <= 0)
    {
        return FALSE;
    }
    CStringArray sTokens;
    CCommon::GetTokens(str, sTokens, _T("- :.,/"));

    CString s;
    int year = 2000, mon = 0, day = 0, hh = 0, mm = 0, ss = 0;

    if(sTokens.GetSize() >= 3)
    {
        s = sTokens[0];
        if(!Convert::StrToNum(s, year))
        {
            return false;
        }
        s = sTokens[1];
        if(!Convert::StrToNum(s, mon))
        {
            return false;
        }
        s = sTokens[2];
        if(!Convert::StrToNum(s, day))
        {
            return false;
        }
    }

    if(sTokens.GetSize() >= 6)
    {
        s = sTokens[3];
        if(!Convert::StrToNum(s, hh))
        {
            return false;
        }
        s = sTokens[4];
        if(!Convert::StrToNum(s, mm))
        {
            return false;
        }
        s = sTokens[5];
        if(!Convert::StrToNum(s, ss))
        {
            return false;
        }
    }
    if(sTokens.GetSize() == 1 && str.GetLength() == 8)
    {
        year = _ttoi(str.Left(4));
        mon = _ttoi(str.Mid(4, 2));
        day = _ttoi(str.Right(2));
    }

    return (dt.SetDateTime(year, mon, day, hh, mm, ss) == 0);


}

int    Convert::DateDiff(CString sDate1, CString sDate2)
{

    COleDateTime dt1;
    COleDateTime dt2;
    COleDateTimeSpan dlt;
    StrToDate(sDate1, dt1);
    StrToDate(sDate2, dt2);

    dlt = dt1 - dt2;

    return dlt.GetDays();
}

CString  Convert::DateAdd(CString sDate1, int nDay)
{
    COleDateTime dt1;
    StrToDate(sDate1, dt1);

    COleDateTimeSpan dlt;
    dlt.SetDateTimeSpan(nDay, 0, 0, 0);
    COleDateTime dt2 = dt1 + dlt;

    CString str;
    str.Format(_T("%.4d-%.2d-%.2d"), dt2.GetYear(), dt2.GetMonth(), dt2.GetDay());

    return str;
}

void Convert::CopyStrToChars(const TCHAR* pSrc, TCHAR* pDest, int bound)
{
    memset(pDest, 0, bound);
    int len = min((int)_tcslen(pSrc), bound);
    memcpy(pDest, pSrc, len);
}

//从字符组
void Convert::CopyCharsToStr(const TCHAR* pSrc, CString& pDest, int bound)
{
    TCHAR* buf = new TCHAR[bound + 1];
    memset(buf, 0, bound + 1);
    memcpy(buf, pSrc, bound);
    pDest = buf;
    delete [] buf;
    buf = NULL;
}

CString Convert::NumToStr(int nID)
{
    CString str;
    str.Format(_T("%d"), nID);
    return str;
}
CString Convert::DoubleToStr(double d, int nPrecise)
{
    CString str;
    switch(nPrecise)
    {
        case 0:
            str.Format(_T("%.0f"), d);
            break;
        case 1:
            str.Format(_T("%.1f"), d);
            break;
        case 2:
            str.Format(_T("%.2f"), d);
            break;
        case 3:
            str.Format(_T("%.3f"), d);
            break;
        case 4:
            str.Format(_T("%.4f"), d);
            break;
        case 5:
            str.Format(_T("%.5f"), d);
            break;
        case 6:
            str.Format(_T("%.6f"), d);
            break;
        case 7:
            str.Format(_T("%.7f"), d);
            break;
        case 8:
            str.Format(_T("%.8f"), d);
            break;
        case 9:
            str.Format(_T("%.9f"), d);
            break;
        default:
            str.Format(_T("%f"), d);
            break;
    }

    return str;
}
CString Convert::NumToStr(DWORD nID)
{
    CString str;
    str.Format(_T("%d"), (int)nID);
    return str;
}

CString Convert::NumToStr(float fValue, int DotNum)
{
    CString str;

    switch(DotNum)
    {
        case 1:
            str.Format(_T("%.1f"), fValue);
            break;

        case 2:
            str.Format(_T("%.2f"), fValue);
            break;

        case 3:
            str.Format(_T("%.3f"), fValue);
            break;

        default:
            str.Format(_T("%f"), fValue);
            break;
    }

    return str;
}

CString Convert::NumToStr(double fValue, int DotNum)
{
    CString str;

    switch(DotNum)
    {
        case 1:
            str.Format(_T("%.1f"), fValue);
            break;

        case 2:
            str.Format(_T("%.2f"), fValue);
            break;

        case 3:
            str.Format(_T("%.3f"), fValue);
            break;

        default:
            str.Format(_T("%f"), fValue);
            break;
    }

    return str;
}
void	Convert::StrToVar(CString sText, _variant_t* vValue, VARTYPE vtType)
{
    //	sText.TrimLeft();
    //	sText.TrimRight();

    vValue->Clear();
    if(vtType == VT_DATE)
    {
        vValue->vt = VT_NULL;//2015.10.09 Add
    }
    if(sText.IsEmpty())
    {
        return;    //2015.08.28 Add
    }

    switch(vtType)
    {
        case VT_I1: //  整数
        case VT_UI1:
        {
            int num;
            if(Convert::StrToNum(sText, num))
            {
                *vValue = (TCHAR)num;
            }
            else
            {
                vValue->vt = VT_NULL;
            }
        }
        break;
        case VT_I2:
        case VT_UI2:
        {
            int num;
            if(Convert::StrToNum(sText, num))
            {
                *vValue = (short)num;
            }
            else
            {
                vValue->vt = VT_NULL;
            }
        }

        break;
        case VT_I4:
        {
            int num;
            if(Convert::StrToNum(sText, num))
            {
                *vValue = (long)num;
            }
            else
            {
                vValue->vt = VT_NULL;
            }

        }

        break;
        case VT_R4:
        case VT_R8:     // 小数
        {
            double num;
            if(Convert::StrToNum(sText, num))
            {
                vValue->vt = VT_R8;
                vValue->dblVal = num;
            }
            else
            {
                vValue->vt = VT_NULL;
            }
        }
        break;

        case VT_DATE:     // 日期
        {
            COleDateTime dt;
            if(Convert::StrToDate(sText, dt))
            {
                vValue->vt = VT_DATE;
                vValue->date = dt;
            }
            else
            {
                vValue->vt = VT_NULL;
            }
        }
        break;

        case VT_BSTR:     // 日期
        {
            vValue->vt = VT_BSTR;
            vValue->bstrVal = sText.AllocSysString(); //de
            break;//2015.08.28 Add
        }
        default:    //  文本
            if(sText.IsEmpty())
            {
                vValue->vt = VT_NULL;
            }
            else
            {
                //vValue=(_bstr_t)(LPCTSTR)sText;
                vValue->vt = VT_BSTR;
                vValue->bstrVal = sText.AllocSysString(); //del by zyw
                // vValue->bstrVal=(_bstr_t)sText;//add by zyw  2005.1.18
            }
            break;
    }
}

CString  Convert::SingleStrToMulitStr(CString strContent, int nOneRowLength)
{
    int slong = 0;
    int pos = 0;
    int posbak = 0;
    int fds = 0;
    CString stemp;
    TCHAR* p  = strContent.GetBuffer();
    while(*p)
    {
        posbak = pos;
        if((*p) & 0x80)  //全角
        {
            p += 2;
            pos = pos + 2;

        }
        else
        {
            p++;
            pos++;
        }
        if(pos > nOneRowLength && fds == 0)
        {
            stemp = strContent.Left(posbak) + _T("\r\n");
            slong = posbak;
            fds = 1;
        }
        if(pos > (slong + nOneRowLength) && fds == 1)
        {
            stemp = stemp + strContent.Mid(slong, posbak - slong) + _T("\r\n");
            slong = posbak;
            fds = 2;
            break;
        }

    }
    strContent = stemp + strContent.Right(strContent.GetLength() - slong);
    return strContent;
}


COLORREF Convert::StringToCOLORREF(CString sVal, CString sToken)
{
    CStringArray aToken;

    CCommon::GetTokens(sVal, aToken, sToken);

    if(aToken.GetSize() != 3)
    {
        return NULL;
    }

    return RGB(_ttoi(aToken.GetAt(0)), _ttoi(aToken.GetAt(1)), _ttoi(aToken.GetAt(2)));

}

void Convert::StrToVar2(CString sValue, _variant_t* vVal, VARTYPE vtType, int nFldLen, VARIANT_BOOL bNullDisable)
{
    CString sInfo;

    if(sValue.IsEmpty() && (bNullDisable == VARIANT_TRUE))
    {
        if(vtType == VT_I2 || vtType == VT_I4 || vtType == VT_R4 || vtType == VT_R8)
        {
            sValue = "0";
        }
        else if(vtType == VT_DATE)
        {
            sValue = "1989-11-30";
        }
        else if(vtType == VT_BSTR)
        {
            sValue = "-";
        }
    }

    // 检查超长的字段
    if(vtType == VT_BSTR)
    {
        if(sValue.GetLength() > nFldLen)
        {
            sInfo.Format(_T("字段[%s]超过长度!"), sValue);
            sValue = sValue.Left(nFldLen);
        }
    }

    StrToVar(sValue, vVal, vtType);
}


int CCommon::GetTokens(const CString string, CStringArray& sToken, const CString seps)
{
    CString sItem;
    CString str;
    str += string;

    TCHAR* token = _tcstok(str.GetBuffer(), seps);

    while(token != NULL)
    {
        sItem = token;

        sToken.Add(sItem);
        token = _tcstok(NULL, seps);
    }

    return (int)sToken.GetSize();
}

int  CCommon::GetTokens2(CString strParent, CStringArray& aToken, CString strFind)
{
    if(strParent == "")
    {
        return 0;
    }
    int nPos = 0;

    CString sTemp, sLeft, sRight;
    sTemp = strParent;

    nPos = sTemp.Find(strFind);


    while(nPos >= 0)
    {
        sLeft = sTemp.Left(nPos);
        sRight = sTemp.Mid(nPos + 1);
        aToken.Add(sLeft);
        sTemp = sRight;
        nPos = sTemp.Find(strFind);
    }

    aToken.Add(sTemp);

    return (int)aToken.GetSize();
}

void CCommon::MsgLastError()
{

    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
    );
    ATLTRACE((LPCTSTR)lpMsgBuf);
    LocalFree(lpMsgBuf);
}
bool CCommon::TrimIntStr(CString str, CString& sNewStr)
{
    int len = str.GetLength();
    int j = 0;
    for(int i = 0; i < len; i++)
    {
        TCHAR c = str.GetAt(i);
        if((c >= '0' && c <= '9'))
        {
            sNewStr.Insert(j++, c);
        }
    }

    return sNewStr.GetLength() != str.GetLength();
}

// 得到整数字符串
bool CCommon::TrimFloatStr(CString str, CString& sNewStr)
{
    int len = str.GetLength();
    int j = 0;
    for(int i = 0; i < len; i++)
    {
        TCHAR c = str.GetAt(i);
        if((c >= '0' && c <= '9') || c == '.')
        {
            sNewStr.Insert(j++, c);
        }
    }

    return sNewStr.GetLength() != str.GetLength();
}

bool CCommon::TrimDateStr(CString str, CString& sNewStr)
{
    int len = str.GetLength();
    int j = 0;
    for(int i = 0; i < len; i++)
    {
        TCHAR c = str.GetAt(i);
        if((c >= '0' && c <= '9') || c == '-')
        {
            sNewStr.Insert(j++, c);
        }
    }

    return sNewStr.GetLength() != str.GetLength();

}

bool CCommon::TrimDateTimeStr(CString str, CString& sNewStr)
{
    int len = str.GetLength();
    int j = 0;
    for(int i = 0; i < len; i++)
    {
        TCHAR c = str.GetAt(i);
        if((c >= '0' && c <= '9') || c == '-' || c == ' ' || c == ':')
        {
            sNewStr.Insert(j++, c);
        }
    }

    return sNewStr.GetLength() != str.GetLength();

}

bool CCommon::FormatDateStr(CString str)
{
    int len = str.GetLength();
    if(len < 8 || len > 10)
    {
        return false;
    }

    CStringArray sToken;
    GetTokens(str, sToken, _T("-"));
    if(sToken[0].GetLength() < 4)
    {
        return false;
    }

    for(int i = 0 ; i < len ; i++)
    {
        TCHAR c = str.GetAt(i);
        if(!((c >= '0' && c <= '9') || c == '-'))
        {
            return false;
        }
    }

    return true;
}

CString CCommon::GetAppPath()
{
    TCHAR path_buffer[_MAX_PATH];
    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[_MAX_DIR];
    TCHAR fname[_MAX_FNAME];
    TCHAR ext[_MAX_EXT];

    HINSTANCE hInstance = AfxGetApp()->m_hInstance;

    GetModuleFileName(hInstance, path_buffer, _MAX_PATH);
    TCHAR* p = path_buffer;
    _tsplitpath(p, drive, dir, fname, ext);

    CString sPath;
    sPath = drive;
    sPath += dir;
    return sPath;
}
//返回sFilePath相对sCurPath的相对路径
CString CCommon::GetRelativePath(CString sCurPath, CString sFilePath)
{
    //路径统一"\\"替换为"/"
    if(sCurPath.IsEmpty() || sFilePath.IsEmpty())
    {
        return sFilePath;
    }
    sCurPath.Replace(_T("\\"), _T("/"));
    CStringArray sCurPathArr;
    GetTokens2(sCurPath, sCurPathArr, _T("/"));
    CStringArray sFilePathArr;
    GetTokens2(sFilePath, sFilePathArr, _T("/"));
    if(sCurPathArr.GetAt(0) != sFilePathArr.GetAt(0))//如果盘符不等直接返回绝对路径
    {
        return sFilePath;
    }
    int nSize1 = sCurPathArr.GetCount();
    int nSize2 = sFilePathArr.GetCount();
    int nSize = min(nSize1, nSize2);
    int nEql = nSize;
    for(int i = 1; i < nSize; i++)
    {
        CString str1 = sCurPathArr.GetAt(i);
        CString str2 = sFilePathArr.GetAt(i);
        if(str1 != str2)
        {
            nEql = i;
            break;
        }
    }
    CString str;
    if(nEql == nSize1 && nEql == nSize2)
    {
        return _T("./");
    }
    else if(nEql < nSize1)
    {
        for(int  i = nEql ; i < nSize1; i++)
        {
            str = str + _T("../");
        }
        for(int i = nEql; i < nSize2; i++)
        {
            str = str + sFilePathArr.GetAt(i);
        }
    }
    else
    {
        str = "./";
        for(int i = nEql; i < nSize2; i++)
        {
            str = str + sFilePathArr.GetAt(i);
        }
    }
    return str;
}

void CCommon::WriteLog(CString sInfo, bool bDate, LPCTSTR objName, LPCTSTR sPrjName)
{

    if(g_Tracecount == 0)
    {
        GetSystemTime(&g_LastTime);
    }

    SYSTEMTIME    stTime;
    GetSystemTime(&stTime);

    CString str = sInfo;
    CString temp;

    if(objName != NULL)
    {
        str += objName;
    }

    if(sPrjName != NULL)
    {
        temp.Format(_T("(%s)"), sPrjName);
        str += temp;
    }


    CString path;

    path.Format(_T("%slog%04d%02d%02d.txt"), GetAppPath(), stTime.wYear, stTime.wMonth, stTime.wDay);

    try
    {

        FILE* f = _tfopen(path, _T("a+"));
        g_Tracecount;
        CString sInfo;

        int diffT = (stTime.wHour - g_LastTime.wHour) * 3600 * 1000
                    + (stTime.wMinute - g_LastTime.wMinute) * 60 * 1000
                    + (stTime.wSecond - g_LastTime.wSecond) * 1000
                    + stTime.wMilliseconds - g_LastTime.wMilliseconds;

        if(bDate)
            sInfo.Format(_T("%d | (%06d) | %02d:%02d:%02d.%03d | %s\n"),
                         ++g_Tracecount, diffT, stTime.wHour, stTime.wMinute, stTime.wSecond,
                         stTime.wMilliseconds, str);
        else
        {
            sInfo.Format(_T("%s"), str);
        }

        g_LastTime = stTime;

        _fputts(sInfo.GetBuffer(), f);
        fclose(f);
    }
    catch(...)
    {

    }
}

CString CCommon::GetSubString(CString strParent, CString strFind, int nStart, int nLen)
{
    //strParent="001-002-00123-12",strFind="-"
    //nStart=0,nLen=1时,return "001";nSart=3,Len=1,返回第4区间
    //nStart>3时,return ""
    //nStart=2,nLen<0时，return "00123-12"
    int i;
    if(strFind == "")
    {
        //待查找的字符串为空，则返回Mid(nStart, nLen)
        return strParent.Mid(nStart, nLen);;
    }

    int nFindLen = strFind.GetLength();
    int pos1 = 0;		//待查找的前点
    int pos2 = 0;		//待查找的后点
    if(nStart == 0)
    {
        pos1 = 0;
    }
    else
    {
        //查找前点
        for(i = 0; i < nStart; i++)
        {
            pos1 = strParent.Find(strFind, pos1);
            if(pos1 < 0)
            {
                return _T("");
            }
            pos1 += nFindLen;
        }
    }

    pos2 = pos1;
    if(nLen < 0) //当nLen<0时，将找到的第nStart个字符串后的所有字符返回
    {
        return strParent.Mid(pos1);
    }

    for(i = 0; i < nLen; i++)
    {
        pos2 = strParent.Find(strFind, pos2);
        if(pos2 < 0)
        {
            return strParent.Mid(pos1);
        }
        pos2 += nFindLen;
    }
    return strParent.Mid(pos1, pos2 - pos1 - nFindLen);
}

BOOL CCommon::CheckLXDH(CString sLxdh)
{
    int i = sLxdh.Find(_T("-"));
    if(i > 0)
    {
        if((sLxdh.GetLength() > 0) && (sLxdh.GetLength() > 12))
        {
            AfxMessageBox(_T("联系电话格式不正确"));
            return FALSE;
        }
    }
    else
    {
        if((sLxdh.GetLength() > 0) && (sLxdh.GetLength() > 11))
        {
            AfxMessageBox(_T("联系电话格式不正确"));
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CCommon::CheckSfz(CString sSfzh)
{
    int no[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    TCHAR id[] = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};
    //1 0 X 9 8 7 6 5 4 3 2   校验码
    BOOL error = TRUE;
    int i = 0, wi = 0, sum = 0;
    TCHAR* p = sSfzh.GetBuffer();
    for(; i < sSfzh.GetLength() - 1; i++)
    {
        wi = (*(p + i) - '0') * no[i];
        sum += wi;
    }
    if(*(p + i) < '0' || *(p + i) > '9')
    {
        if(*(p + i) != 'X' && *(p + i) != 'x')
        {
            error = FALSE; //身份证最后一位输入错误
        }
    }
    wi = sum % 11;
    if(*(p + 17) == 'x' || *(p + 17) == 'X')  //最后一位为'x'或'X';
    {
        if(id[wi] != 'x' && id[wi] != 'X')
        {
            error = FALSE;
        }
    }
    else if(id[wi] != *(p + 17)) //判断计算出的身份证校验码与输入是否相符
    {
        error = FALSE;
    }

    if(error == FALSE)
    {
        return FALSE;
    }

    if(sSfzh.GetLength() != 15 && sSfzh.GetLength() != 18)
    {
        return FALSE;
    }
    int year, month, day;
    if(sSfzh.GetLength() == 15)
    {

        year = _ttoi(_T("19") + sSfzh.Mid(6, 2));
        month = _ttoi(sSfzh.Mid(8, 2));
        day = _ttoi(sSfzh.Mid(10, 2));
    }
    else
    {
        year = _ttoi(sSfzh.Mid(6, 4));
        month = _ttoi(sSfzh.Mid(10, 2));
        day = _ttoi(sSfzh.Mid(12, 2));
    }
    if(month > 12)
    {
        return FALSE;
    }
    if(year < 0 || month <= 0 || day <= 0)
    {
        return FALSE;
    }
    if(month == 2)
    {
        if(day > 29 && (year % 4 == 0 && year % 100 != 0 || year % 400 == 0))
        {
            return FALSE;
        }
        else if(day > 28)
        {
            return FALSE;
        }
    }
    else if((month == 1 || month == 3 || month == 5 || month == 7 ||
             month == 8 || month == 10 || month == 12) && day > 31)
    {
        return FALSE;
    }
    else if(day > 30)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CCommon::CheckZJHM(CString sZjlx, CString sZjhm)
{
    //	if ((sZjlx=="居民身份证")&&(sZjhm.GetLength()!=15)&&(sZjhm.GetLength()!=18))
    if(sZjlx == _T("居民身份证"))
    {
        if((sZjhm != "") && (sZjhm.GetLength() != 15) && (sZjhm.GetLength() != 18))
        {
            AfxMessageBox(_T("证件号码格式不正确"));
            return FALSE;
        }
    }
    else if((sZjhm != _T("")) && (sZjlx == _T("")))
    {
        AfxMessageBox(_T("请先选择证件类型!"));
        return FALSE;
    }
    return TRUE;
}

BOOL CCommon::TestDate(TCHAR* strdate) //strdate 是合法日期返回true，非法返回 false
{

    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    //_stscanf(strdate, "%04X-%02X-%02X %02X:%02X:%02X", &year, &month, &day, &hour, &minute, &second);
    _stscanf(strdate, _T("%04X%02X%02X "), &year, &month, &day);
    if(year < 0 || month < 0 || day < 0 || hour < 0 || minute < 0 || second < 0)
    {
        return false;    //防止字符串中有不是1—9之间
    }
    //的数如200A-01-02 0A:0A:0A
    if(hour > 23 || second > 60 || minute > 60)
    {
        return false;
    }
    if(month == 0 || day == 0)
    {
        return false;
    }
    if(month <= 12 && day <= 31)
    {
        if(year % 400 == 0 && month == 2 && day <= 29)
        {
            return true;    //闰年
        }
        else if(year % 100 != 0 && year % 4 == 0 && month == 2 && day <= 29)
        {
            return true;    //闰年
        }
        else if((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day <= 31)
        {
            return true;
        }
        else if((month == 2 || month == 4 || month == 6 || month == 9 || month == 11) && day <= 30)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

}

BOOL CCommon::MakeDir(CString sFileName)
{
    CString sTemp;
    int nPos = 3;
    BOOL bOK = FALSE;

    while(nPos > 2)
    {
        nPos = sFileName.Find(_T("\\"), nPos);
        sTemp = sFileName.Left(nPos);
        bOK = ::CreateDirectory(sTemp, NULL);
        ++nPos;
    }
    return bOK;
}

//方法传入参数:十进制度
BOOL CCommon::FormatDecimalDegree(double number, CString& sDDMMSS)
{
    int nDu = (int)number;
    int nFen = (int)((number - nDu) * 60.0);
    double fMiao = (number - nDu) * 3600.0 - nFen * 60.0;
    sDDMMSS.Format(_T("%d°%d′%.3f″"), nDu, nFen, fMiao);
    return TRUE;
}

BOOL MakeDir(CString strFileName)
{
    CString strTemp;
    int pos = 3;

    BOOL bOk = FALSE;

    while(pos > 2)
    {
        pos = strFileName.Find(_T("\\"), pos);
        strTemp = strFileName.Left(pos);
        bOk = CreateDirectory(strTemp, 0);
        pos++;
    }
    return bOk;
}

BOOL CCommon::FileExist(const TCHAR* pszFileName)
{
    BOOL bExist = FALSE;
    HANDLE hFile;

    if(NULL != pszFileName)
    {
        //	 Use the preferred Win32 API call and not the older OpenFile.
        hFile = CreateFile(
                    pszFileName,
                    GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    0,
                    0);

        if(hFile != INVALID_HANDLE_VALUE)
        {
            //			 If the handle is valid then the file exists.
            CloseHandle(hFile);
            bExist = TRUE;
        }
    }

    return (bExist);
}