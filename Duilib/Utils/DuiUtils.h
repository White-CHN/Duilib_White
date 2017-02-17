#pragma once

namespace DuiLib
{
    char* w2a(wchar_t* lpszSrc);
    wchar_t* a2w(char* lpszSrc);

    class DUILIB_API CDuiConsole
    {
    public:
        CDuiConsole(BOOL bOpen = TRUE);
        ~CDuiConsole();
    private:
        BOOL m_bOpen;
    };

    class DUILIB_API CDuiTimeInterval
    {
    public:
        CDuiTimeInterval();
        ~CDuiTimeInterval();
    private:
        DWORD m_dwMesc;
    };

    class DUILIB_API STRINGorID
    {
    public:
        STRINGorID(LPCTSTR lpString);
        STRINGorID(UINT nID);
    public:
        LPCTSTR m_lpstr;
    };

    template<class T>
    class CSafeRelease
    {
    public:
        CSafeRelease(T* p) : m_p(p) { };
        ~CSafeRelease()
        {
            if(m_p != NULL)
            {
                m_p->Release();
            }
        };
    public:
        T* Detach()
        {
            T* t = m_p;
            m_p = NULL;
            return t;
        };
    public:
        T* m_p;
    };

    class CDuiVariant : public VARIANT
    {
    public:
        CDuiVariant()
        {
            VariantInit(this);
        }
        CDuiVariant(int i)
        {
            VariantInit(this);
            this->vt = VT_I4;
            this->intVal = i;
        }
        CDuiVariant(float f)
        {
            VariantInit(this);
            this->vt = VT_R4;
            this->fltVal = f;
        }
        CDuiVariant(LPOLESTR s)
        {
            VariantInit(this);
            this->vt = VT_BSTR;
            this->bstrVal = s;
        }
        CDuiVariant(IDispatch* disp)
        {
            VariantInit(this);
            this->vt = VT_DISPATCH;
            this->pdispVal = disp;
        }

        ~CDuiVariant()
        {
            VariantClear(this);
        }
    };

    class DUILIB_API CDuiPoint : public tagPOINT
    {
    public:
        CDuiPoint();
        CDuiPoint(const POINT& src);
        CDuiPoint(int x, int y);
        CDuiPoint(LPARAM lParam);
        // Operations
        BOOL operator==(const POINT& point) const;
        BOOL operator!=(const POINT& point) const;
        void operator+=(const POINT& point);
        void operator-=(const POINT& point);
    };

    class DUILIB_API CDuiSize : public tagSIZE
    {
    public:
        CDuiSize();
        CDuiSize(const SIZE& src);
        CDuiSize(const RECT rc);
        CDuiSize(const int cx, const int cy);
        // Operations
        BOOL operator==(const SIZE& size) const;
        BOOL operator!=(const SIZE& size) const;
        void operator+=(const SIZE& size);
        void operator-=(const SIZE& size);

        // Operators returning CSize values
        CDuiSize operator+(const SIZE& size) const;
        CDuiSize operator-(const SIZE& size) const;

        void SetSize(const int CX, const int CY);
    };

    class DUILIB_API CDuiRect : public tagRECT
    {
    public:
        CDuiRect();
        CDuiRect(const RECT& src);
        CDuiRect(const LPCRECT lpSrcRect);
        CDuiRect(int iLeft, int iTop, int iRight, int iBottom);
        // Operations
        BOOL operator==(const RECT& rect) const;
        BOOL operator!=(const RECT& rect) const;
        void operator+=(const RECT& rect);
        void operator-=(const RECT& rect);


        int GetWidth() const;
        int GetHeight() const;
        void Empty();
        BOOL IsNull() const;
        void Join(const RECT& rc);
        void ResetOffset();
        void Normalize();
        void Offset(int cx, int cy);
        void Inflate(int cx, int cy);
        void Deflate(int cx, int cy);
        void Union(CDuiRect& rc);

        CDuiRect MulDiv(int nMultiplier, int nDivisor) const;
    };

    class DUILIB_API CDuiRectD
    {
    public:
        CDuiRectD();
        CDuiRectD(const CDuiRectD& src);
    public:
        double left;
        double top;
        double right;
        double bottom;
    };

    class DUILIB_API CStdPtrArray
    {
    public:
        CStdPtrArray(int iPreallocSize = 0);
        CStdPtrArray(const CStdPtrArray& src);
        ~CStdPtrArray();

        void Empty();
        void Resize(int iSize);
        BOOL IsEmpty() const;
        int Find(LPVOID pData) const;
        BOOL Add(LPVOID pData);
        BOOL SetAt(int iIndex, LPVOID pData);
        BOOL InsertAt(int iIndex, LPVOID pData);
        BOOL Remove(int iIndex);
        int GetSize() const;
        LPVOID* GetData();

        LPVOID GetAt(int iIndex) const;
        LPVOID operator[](int nIndex) const;

    protected:
        LPVOID* m_ppData;
        int m_iCount;
        int m_iAllocated;
    };

    struct TITEM
    {
        CDuiString Key;
        LPVOID Data;
        struct TITEM* pPrev;
        struct TITEM* pNext;
    };

    class DUILIB_API CStdStringPtrMap
    {
    public:
        CStdStringPtrMap(int nSize = 83);
        ~CStdStringPtrMap();

        void Resize(int nSize = 83);
        LPVOID Find(LPCTSTR key, BOOL optimize = TRUE) const;
        BOOL Insert(LPCTSTR key, LPVOID pData);
        LPVOID Set(LPCTSTR key, LPVOID pData);
        BOOL Remove(LPCTSTR key);
        void RemoveAll();
        int GetSize() const;
        LPCTSTR GetAt(int iIndex) const;
        LPCTSTR operator[](int nIndex) const;

    protected:
        TITEM** m_aT;
        int m_nBuckets;
        int m_nCount;
    };

}