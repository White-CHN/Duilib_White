#pragma once

namespace DuiLib
{

    class DUILIB_API CDuiConsole
    {
    public:
        CDuiConsole();
        ~CDuiConsole();
    };

    class DUILIB_API STRINGorID
    {
    public:
        STRINGorID(LPCTSTR lpString);
        STRINGorID(UINT nID);
    public:
        LPCTSTR m_lpstr;
    };

    class DUILIB_API CDuiSize : public tagSIZE
    {
    public:
        CDuiSize();
        CDuiSize(const SIZE& src);
        CDuiSize(const RECT rc);
        CDuiSize(int cx, int cy);
    };

    class DUILIB_API CDuiRect : public tagRECT
    {
    public:
        CDuiRect();
        CDuiRect(const RECT& src);
        CDuiRect(int iLeft, int iTop, int iRight, int iBottom);

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