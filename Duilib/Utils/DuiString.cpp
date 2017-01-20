#include "StdAfx.h"
#include "DuiString.h"

namespace DuiLib
{
    CDuiString::CDuiString(void)
        : m_lpStr(m_strBuffer)
    {
        ZeroMemory(m_strBuffer, sizeof(m_strBuffer));
    }


    CDuiString::CDuiString(const TCHAR ch)
        : m_lpStr(m_strBuffer)
    {
        ZeroMemory(m_strBuffer, sizeof(m_strBuffer));
        m_strBuffer[0] = ch;
    }

    CDuiString::CDuiString(const CDuiString& src)
        : m_lpStr(m_strBuffer)
    {
        ZeroMemory(m_strBuffer, sizeof(m_strBuffer));
        Assign(src.m_lpStr);
    }

    CDuiString::CDuiString(LPCTSTR lpStr, int iLen /*= -1*/)
        : m_lpStr(m_strBuffer)
    {
        ASSERT(!::IsBadStringPtr(lpStr, -1) || lpStr == NULL);
        ZeroMemory(m_strBuffer, sizeof(m_strBuffer));
        Assign(lpStr, iLen);
    }

    CDuiString::~CDuiString(void)
    {
        if(m_lpStr != m_strBuffer)
        {
            free(m_lpStr);
        }
    }

    CDuiString::operator LPCTSTR() const
    {
        return m_lpStr;
    }

    TCHAR CDuiString::operator[](int index) const
    {
        return m_lpStr[index];
    }

    const CDuiString& CDuiString::operator=(const CDuiString& src)
    {
        Assign(src);
        return *this;
    }

    const CDuiString& CDuiString::operator=(const TCHAR ch)
    {
        Empty();
        m_strBuffer[0] = ch;
        m_strBuffer[1] = _T('\0');
        return *this;
    }

    const CDuiString& CDuiString::operator=(LPCTSTR lpStr)
    {
        if(lpStr)
        {
            ASSERT(!::IsBadStringPtr(lpStr, -1));
            Assign(lpStr);
        }
        else
        {
            Empty();
        }
        return *this;
    }

#ifdef _UNICODE

    const CDuiString& CDuiString::operator=(LPCSTR lpStr)
    {
        if(lpStr)
        {
            ASSERT(!::IsBadStringPtrA(lpStr, -1));
            int cchStr = (int) strlen(lpStr) + 1;
            LPWSTR pwstr = (LPWSTR) _alloca(cchStr);
            if(pwstr != NULL)
            {
                ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr) ;
            }
            Assign(pwstr);
        }
        else
        {
            Empty();
        }
        return *this;
    }

    const CDuiString& CDuiString::operator+=(LPCSTR lpStr)
    {
        if(lpStr)
        {
            ASSERT(!::IsBadStringPtrA(lpStr, -1));
            int cchStr = (int) strlen(lpStr) + 1;
            LPWSTR pwstr = (LPWSTR) _alloca(cchStr);
            if(pwstr != NULL)
            {
                ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr) ;
            }
            Append(pwstr);
        }
        return *this;
    }

#else
    const CDuiString& CDuiString::operator=(LPCWSTR lpwStr)
    {
        if(lpwStr)
        {
            ASSERT(!::IsBadStringPtrW(lpwStr, -1));
            int cchStr = ((int) wcslen(lpwStr) * 2) + 1;
            LPSTR pstr = (LPSTR) _alloca(cchStr);
            if(pstr != NULL)
            {
                ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
            }
            Assign(pstr);
        }
        else
        {
            Empty();
        }
        return *this;
    }

    const CDuiString& CDuiString::operator+=(LPCWSTR lpwStr)
    {
        if(lpwStr)
        {
            ASSERT(!::IsBadStringPtrW(lpwStr, -1));
            int cchStr = ((int) wcslen(lpwStr) * 2) + 1;
            LPSTR pstr = (LPSTR) _alloca(cchStr);
            if(pstr != NULL)
            {
                ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
            }
            Append(pstr);
        }
        return *this;
    }

#endif // _UNICODE


    CDuiString CDuiString::operator+(const CDuiString& src) const
    {
        CDuiString strTemp = *this;
        strTemp.Append(src);
        return strTemp;
    }

    CDuiString CDuiString::operator+(LPCTSTR lpStr) const
    {
        if(lpStr)
        {
            ASSERT(!::IsBadStringPtr(lpStr, -1));
            CDuiString strTemp = *this;
            strTemp.Append(lpStr);
            return strTemp;
        }
        return *this;
    }

    const CDuiString& CDuiString::operator+=(const CDuiString& src)
    {
        Append(src);
        return *this;
    }

    const CDuiString& CDuiString::operator+=(LPCTSTR lpStr)
    {
        if(lpStr)
        {
            ASSERT(!::IsBadStringPtr(lpStr, -1));
            Append(lpStr);
        }
        return *this;
    }

    const CDuiString& CDuiString::operator+=(const TCHAR ch)
    {
        TCHAR str[] = { ch, _T('\0') };
        Append(str);
        return *this;
    }

    bool CDuiString::operator == (LPCTSTR lpStr) const
    {
        return (Compare(lpStr) == 0);
    }

    bool CDuiString::operator != (LPCTSTR lpStr) const
    {
        return (Compare(lpStr) != 0);
    }

    bool CDuiString::operator <= (LPCTSTR lpStr) const
    {
        return (Compare(lpStr) <= 0);
    }
    bool CDuiString::operator < (LPCTSTR lpStr) const
    {
        return (Compare(lpStr) <  0);
    }

    bool CDuiString::operator >= (LPCTSTR lpStr) const
    {
        return (Compare(lpStr) >= 0);
    }

    bool CDuiString::operator > (LPCTSTR lpStr) const
    {
        return (Compare(lpStr) >  0);
    }

    int CDuiString::GetLength() const
    {
        return (int) _tcslen(m_lpStr);
    }

    void CDuiString::Empty()
    {
        if(m_lpStr != m_strBuffer)
        {
            free(m_lpStr);
        }
        m_lpStr = m_strBuffer;
        ZeroMemory(m_strBuffer, sizeof(m_strBuffer));
    }

    BOOL CDuiString::IsEmpty() const
    {
        return  _tcslen(m_lpStr) == 0 ? TRUE : FALSE;
    }

    TCHAR CDuiString::GetAt(int index) const
    {
        if(index < 0 || index >= GetLength())
        {
            return _T('\0');
        }
        return m_lpStr[index];
    }

    void CDuiString::SetAt(int index, TCHAR ch)
    {
        if(index < 0 || index >= GetLength())
        {
            return ;
        }
        m_lpStr[index] = ch;
    }

    void CDuiString::Append(LPCTSTR lpStr)
    {
        int iNewLength = GetLength() + (int) _tcslen(lpStr);
        if(iNewLength >= MAX_LOCAL_STRING_LEN)
        {
            if(m_lpStr == m_strBuffer)
            {
                m_lpStr = static_cast<LPTSTR>(malloc((iNewLength + 1) * sizeof(TCHAR)));
                _tcscpy(m_lpStr, m_strBuffer);
                _tcscat(m_lpStr, lpStr);
            }
            else
            {
                m_lpStr = static_cast<LPTSTR>(realloc(m_lpStr, (iNewLength + 1) * sizeof(TCHAR)));
                _tcscat(m_lpStr, lpStr);
            }
        }
        else
        {
            if(m_lpStr != m_strBuffer)
            {
                free(m_lpStr);
                m_lpStr = m_strBuffer;
            }
            _tcscat(m_strBuffer, lpStr);
        }
    }

    void CDuiString::Assign(LPCTSTR lpStr, int iLength /*= -1*/)
    {
        if(lpStr == NULL)
        {
            lpStr = _T("");
        }
        iLength = (iLength < 0 ? (int) _tcslen(lpStr) : iLength);
        if(iLength < MAX_LOCAL_STRING_LEN)
        {
            if(m_lpStr != m_strBuffer)
            {
                free(m_lpStr);
                m_lpStr = m_strBuffer;
            }
        }
        else if(iLength > GetLength() || m_lpStr == m_strBuffer)
        {
            if(m_lpStr == m_strBuffer)
            {
                m_lpStr = NULL;
            }
            m_lpStr = static_cast<LPTSTR>(realloc(m_lpStr, (iLength + 1) * sizeof(TCHAR)));
        }
        _tcsncpy(m_lpStr, lpStr, iLength);
        m_lpStr[iLength] = _T('\0');
    }

    LPCTSTR CDuiString::GetData() const
    {
        return m_lpStr;
    }

    int CDuiString::Compare(LPCTSTR lpStr) const
    {
        return _tcscmp(m_lpStr, lpStr);
    }

    int CDuiString::CompareNoCase(LPCTSTR lpStr) const
    {
        return _tcsicmp(m_lpStr, lpStr);
    }

    void CDuiString::MakeUpper()
    {
        _tcsupr(m_lpStr);
    }

    void CDuiString::MakeLower()
    {
        _tcslwr(m_lpStr);
    }

    CDuiString CDuiString::Left(int iLength) const
    {
        if(iLength < 0)
        {
            iLength = 0;
        }
        if(iLength > GetLength())
        {
            iLength = GetLength();
        }
        return CDuiString(m_lpStr, iLength);
    }

    CDuiString CDuiString::Mid(int iPos, int iLength /*= -1*/) const
    {
        if(iLength < 0)
        {
            iLength = GetLength() - iPos;
        }
        if(iPos + iLength > GetLength())
        {
            iLength = GetLength() - iPos;
        }
        if(iLength <= 0)
        {
            return CDuiString();
        }
        return CDuiString(m_lpStr + iPos, iLength);
    }

    CDuiString CDuiString::Right(int iLength) const
    {
        int iPos = GetLength() - iLength;
        if(iPos < 0)
        {
            iPos = 0;
            iLength = GetLength();
        }
        return CDuiString(m_lpStr + iPos, iLength);
    }

    int CDuiString::Find(TCHAR ch, int iPos /*= 0*/) const
    {
        if(iPos < 0 || iPos >= GetLength())
        {
            return -1;
        }
        LPCTSTR lpStr = _tcschr(m_lpStr + iPos, ch);
        return (lpStr == NULL) ? -1 : (int)(lpStr - m_lpStr);
    }

    int CDuiString::Find(LPCTSTR lpStrSub, int iPos /*= 0*/) const
    {
        ASSERT(!::IsBadStringPtr(lpStrSub, -1));
        if(iPos < 0 || iPos >= GetLength() || lpStrSub == NULL)
        {
            return -1;
        }
        LPCTSTR lpStr = _tcsstr(m_lpStr + iPos, lpStrSub);
        return (lpStr == NULL) ? -1 : (int)(lpStr - m_lpStr);
    }

    int CDuiString::ReverseFind(TCHAR ch) const
    {
        LPCTSTR lpStr = _tcsrchr(m_lpStr, ch);
        return (lpStr == NULL) ? -1 : (int)(lpStr - m_lpStr);
    }

    int CDuiString::Replace(LPCTSTR lpStrFrom, LPCTSTR lpStrTo)
    {
        if(lpStrFrom == NULL || lpStrTo == NULL)
        {
            return -1;
        }
        CDuiString strTemp;
        int iCount = 0;
        int iPos = Find(lpStrFrom);
        if(iPos < 0)
        {
            return 0;
        }
        int iFrom = (int) _tcslen(lpStrFrom);
        int iTo = (int) _tcslen(lpStrTo);
        while(iPos >= 0)
        {
            strTemp = Left(iPos);
            strTemp += lpStrTo;
            strTemp += Mid(iPos + iFrom);
            Assign(strTemp);
            iPos = Find(lpStrFrom, iPos + iTo);
            iCount++;
        }
        return iCount;
    }

    int __cdecl CDuiString::Format(LPCTSTR lpStrFormat, ...)
    {
        if(lpStrFormat == NULL)
        {
            return -1;
        }
        va_list Args;
        va_start(Args, lpStrFormat);
        int iRet = InnerFormat(lpStrFormat, Args);
        va_end(Args);
        return iRet;
    }

    int __cdecl CDuiString::SmallFormat(LPCTSTR lpStrFormat, ...)
    {
        if(lpStrFormat == NULL)
        {
            return -1;
        }
        CDuiString strFormat = lpStrFormat;
        TCHAR strBuffer[MAX_LOCAL_STRING_LEN + 1] = { 0 };
        va_list argList;
        va_start(argList, lpStrFormat);
        int iRet = ::_vsntprintf(strBuffer, sizeof(strBuffer), strFormat, argList);
        va_end(argList);
        Assign(strBuffer);
        return iRet;
    }

    int __cdecl CDuiString::InnerFormat(LPCTSTR lpStrFormat, va_list Args)
    {
#if _MSC_VER <= 1400
        TCHAR* szBuffer = NULL;
        int size = 512, nLen, counts;
        szBuffer = (TCHAR*)malloc(size);
        ZeroMemory(szBuffer, size);
        while(TRUE)
        {
            counts = size / sizeof(TCHAR);
            nLen = _vsntprintf(szBuffer, counts, lpStrFormat, Args);
            if(nLen != -1 && nLen < counts)
            {
                break;
            }
            if(nLen == -1)
            {
                size *= 2;
            }
            else
            {
                size += 1 * sizeof(TCHAR);
            }
            if((szBuffer = (TCHAR*)realloc(szBuffer, size)) != NULL)
            {
                ZeroMemory(szBuffer, size);
            }
            else
            {
                break;
            }
        }
        Assign(szBuffer);
        free(szBuffer);
        return nLen;
#else
        int nLen = _vsntprintf(NULL, 0, lpStrFormat, Args);
        int totalLen = (nLen + 1) * sizeof(TCHAR);
        TCHAR* szBuffer = (TCHAR*)malloc(totalLen);
        ZeroMemory(szBuffer, totalLen);
        nLen = _vsntprintf(szBuffer, nLen + 1, lpStrFormat, Args);
        Assign(szBuffer);
        free(szBuffer);
        return nLen;
#endif
    }

}
