#pragma once

class Convert
{
public:
    static BOOL StrToNum(CString str, double& fResult);
    static int  Str2Num(CString str, int defResult = 0);
    static BOOL StrToNum(CString str, int& nResult, CString& sStop);
    static BOOL StrToNum(CString str, int& nResult);
    static BOOL StrToNum(CString str, float& fResult);
    static BOOL StrToNum(CString str, float& fResult, CString& sStop);
    static BOOL VarToStr(VARIANT var, CString& str);
    static BOOL VarToNum(VARIANT var, int& num);
    static BOOL VarToNum(VARIANT var, double& fResult);
    static BOOL StrToDate(CString str, COleDateTime& dt);

    static CString	Var2Str(VARIANT var, CString defValue = _T(""));
    static int		Var2Num(VARIANT var, int defValue = 0);
    static double	Var2Double(VARIANT var, double defValue = 0);
    //static CString  GetFwjgMc(int nId);

    static BOOL		FormatToDate(CString str, CString& sDate);
    static CString	GetCurrentDate();
    static CString	GetCurrentDateTime();
    static CString	NumToStr(int nID);
    static CString  NumToStr(double fValue, int DotNum = 0);
    static CString	DoubleToStr(double d, int nPrecise = 3);
    static CString	NumToStr(DWORD nID);
    static CString	NumToStr(float fValue, int DotNum = 0);
    static int		DateDiff(CString sDate1, CString sDate2);
    static CString	DateAdd(CString sDate1, int nDay);
    static void		StrToVar(CString sText, _variant_t* vValue, VARTYPE vtType);
    static void		StrToFeatureVar(CString sText, _variant_t* vValue, int type);
    static bool		IsTokenInStr(CString sStr, CString sFind);  // 是否在某一字符串中
    //static long		strToZdno(CString strZd);

    //从字符串中拷贝字符到字符组
    static void		CopyStrToChars(const TCHAR* pSrc, TCHAR* pDest, int bound);
    static void		CopyCharsToStr(const TCHAR* pSrc, CString& pDest, int bound);
    //static int		Tbstr2TbID(CString TbNo);
    //static int		ZdNoStr2ZdID(CString ZdNo);
    //static CString	ZdID2ZdStr(int ZdNo);
    //static CString	LocalToStr(int nLocalNO);
    //static long		strToLocalid(CString str);
    //static void		FormatSLNo(CString &SLNo, _ConnectionPtr conn);
    //static int		ReadSLNoLength(_ConnectionPtr conn);
    static COLORREF StringToCOLORREF(CString sVal, CString sToken);
    static CString	SingleStrToMulitStr(CString strContent, int nOneRowLength); //单行字符串换多行

    static void     StrToVar2(CString sValue, _variant_t* vVal, VARTYPE vtType, int nFldLen, VARIANT_BOOL bNullDisable);
    static CString FormatScaleAnno(CString sScale); //格式化比例尺注记
    static CString HZtoPY(CString str); //汉字输入转换拼音首字母
    static int FindCharPosInWString(CString str, TCHAR ch);////获取字符在宽字符串中的位置，汉字算一个字符
private:
    static TCHAR convert(wchar_t n);
    static bool In(wchar_t start, wchar_t end, wchar_t code);
};

class CCommon
{
public:
    static	 int   GetTokens(const CString string, CStringArray& sToken, const CString seps = _T("\r\n"));
    static	 int   GetTokens2(CString strParent, CStringArray& aToken, CString strFind);
    static	 void  MsgLastError();
    static	 bool  TrimDateTimeStr(CString str, CString& sNewStr);
    static	 bool  TrimDateStr(CString str, CString& sNewStr);
    static	 bool  TrimFloatStr(CString str, CString& sNewStr);
    static	 bool  TrimIntStr(CString str, CString& sNewStr);
    static  CString GetSubString(CString strParent, CString strFind, int nStart, int nLen = 1);
    static  bool   FormatDateStr(CString str);
    //static void	   SetMessage(HWND pMsgWnd,CString sMsg);
    static void WriteLog(CString sInfo, bool bDate = true, LPCTSTR objName = NULL, LPCTSTR sPrjName = NULL);
    static CString GetAppPath();
    static BOOL FileExist(const TCHAR* pszFileName);
    static BOOL CheckLXDH(CString sLxdh);
    static BOOL CheckSfz(CString sSfzh);
    static BOOL CheckZJHM(CString sZjlx, CString sZjhm);
    static BOOL TestDate(TCHAR* strdate); //strdate 是合法日期返回true，非法返回 false
    static CString GetRelativePath(CString sCurPath, CString sFilePath);
    static BOOL MakeDir(CString sFileName);
    static BOOL FormatDecimalDegree(double number, CString& sDDMMSS);
};

#define GET_STRING_VALUE1(VT)				(LPCWSTR)( (VT.vt == VT_NULL || VT.vt == VT_EMPTY) ? L"":  VT.bstrVal)
#define GET_SHORT_VALUE1(VT,def_value)		( (VT.vt != VT_I2 ) ? (short)def_value:  VT.iVal)
#define GET_DOUBLE_VALUE1(VT,def_value)		( (VT.vt != VT_R8) ? (double)def_value:  VT.dblVal)
#define GET_LONG_VALUE1(VT,def_value)		( (VT.vt != VT_I4) ?  (long) def_value:  VT.lVal)

#define GET_STRING_VALUE(VT)				(LPCWSTR)( (VT.vt == VT_NULL || VT.vt == VT_EMPTY) ? L"":  VT.bstrVal)
#define GET_SHORT_VALUE(VT,def_value)		( (VT.vt != VT_I2 ) ? (short)def_value:   (short)VT)
#define GET_DOUBLE_VALUE(VT,def_value)		( (VT.vt != VT_R8) ? (double)def_value:  (double)VT)
#define GET_FLOAT_VALUE(VT,def_value)		( (VT.vt != VT_R4) ?  (float)def_value:   (float)VT)
#define GET_LONG_VALUE(VT,def_value)		( (VT.vt != VT_I4) ?  (long) def_value:    (long)VT)

#define GET_STRING_VALUE_EX(VT)				(LPCWSTR)( (VT.vt == VT_NULL || VT.vt == VT_EMPTY) ? L"":  (LPCWSTR)(_bstr_t)VT)

BOOL ReadPrjString(std::string sFilePath, std::string& sInfo);
BOOL WritePrjString(std::string sFilePath, std::string& sInfo);
BOOL ChangeConnString(CString& strConnStr);