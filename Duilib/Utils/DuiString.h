#pragma once

namespace DuiLib
{
    class DUILIB_API CDuiString
    {
    public:
        CDuiString(void);
        CDuiString(const TCHAR ch);
        CDuiString(const CDuiString& src);
        CDuiString(LPCTSTR lpStr, int iLen = -1);
        ~CDuiString(void);
    public:
        operator LPCTSTR() const;
        TCHAR operator[](int index) const;
        const CDuiString& operator=(const CDuiString& src);
        const CDuiString& operator=(const TCHAR ch);
        const CDuiString& operator=(LPCTSTR lpStr);
#ifdef _UNICODE
        const CDuiString& operator=(LPCSTR lpStr);
        const CDuiString& operator+=(LPCSTR lpStr);
#else
        const CDuiString& operator=(LPCWSTR lpwStr);
        const CDuiString& operator+=(LPCWSTR lpwStr);
#endif
        CDuiString operator+(const CDuiString& src) const;
        CDuiString operator+(LPCTSTR lpStr) const;
        const CDuiString& operator+=(const CDuiString& src);
        const CDuiString& operator+=(LPCTSTR lpStr);
        const CDuiString& operator+=(const TCHAR ch);

        bool operator == (LPCTSTR lpStr) const;
        bool operator != (LPCTSTR lpStr) const;
        bool operator <= (LPCTSTR lpStr) const;
        bool operator < (LPCTSTR lpStr) const;
        bool operator >= (LPCTSTR lpStr) const;
        bool operator > (LPCTSTR lpStr) const;
    public:
        enum { MAX_LOCAL_STRING_LEN = 63 };
    public:
        int GetLength() const;

        void SetEmpty();
        BOOL IsEmpty() const;

        TCHAR GetAt(int index) const;
        void SetAt(int index, TCHAR ch);

        void Append(LPCTSTR pstr);

        void Assign(LPCTSTR lpStr, int iLength = -1);

        LPCTSTR GetData() const;

        int Compare(LPCTSTR lpStr) const;
        int CompareNoCase(LPCTSTR lpStr) const;

        void MakeUpper();
        void MakeLower();

        CDuiString Left(int iLength) const;
        CDuiString Mid(int iPos, int iLength = -1) const;
        CDuiString Right(int iLength) const;

        int Find(TCHAR ch, int iPos = 0) const;
        int Find(LPCTSTR lpStrSub, int iPos = 0) const;
        int ReverseFind(TCHAR ch) const;
        int Replace(LPCTSTR lpStrFrom, LPCTSTR lpStrTo);

        int __cdecl Format(LPCTSTR lpStrFormat, ...);
        int __cdecl SmallFormat(LPCTSTR lpStrFormat, ...);
    private:
        int __cdecl InnerFormat(LPCTSTR lpStrFormat, va_list Args);
    private:
        LPTSTR m_lpStr;
        TCHAR m_strBuffer[MAX_LOCAL_STRING_LEN + 1];
    };




}

