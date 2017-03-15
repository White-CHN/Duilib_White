#include "stdafx.h"
#include "MySqlite.h"


const char* WcharToUtf8(const wchar_t* pwStr)
{
    if(pwStr == NULL)
    {
        return NULL;
    }

    int len = WideCharToMultiByte(CP_UTF8, 0, pwStr, -1, NULL, 0, NULL, NULL);
    if(len <= 0)
    {
        return NULL;
    }
    char* pStr = new char[len];
    WideCharToMultiByte(CP_UTF8, 0, pwStr, -1, pStr, len, NULL, NULL);
    return pStr;
}

const wchar_t* Utf8ToWchar(const char* pStr)
{
    if(pStr == NULL)
    {
        return NULL;
    }

    int len = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0);
    if(len <= 0)
    {
        return NULL;
    }
    wchar_t* pwStr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pwStr, len);
    return pwStr;
}



CMySqlite::CMySqlite(void)
    : m_pDB(NULL)
{
}


CMySqlite::~CMySqlite(void)
{
    if(m_pDB != NULL)
    {
        sqlite3_close(m_pDB);
        m_pDB = NULL;
    }
}

int CMySqlite::callback(void* data, int argc, char** argv, char** azColName)
{
    if(data == NULL)
    {
        return 0;
    }
    CRecords* pRecords = (CRecords*)data;
    CStringArray* pArray = new CStringArray;
    for(int i = 0; i < argc; i++)
    {
        // CString str;
        //str.Format(_T("[%s]=[%s]"), azColName[i], argv[i]);
        const TCHAR* pData = NULL;
#ifdef _UNICODE
        pData = Utf8ToWchar(argv[i]);
#else
        pData = argv[i];
#endif
        pArray->Add(pData);
#ifdef _UNICODE
        if(pData != NULL)
        {
            delete[] pData;
            pData = NULL;
        }
#endif
    }
    pRecords->m_vtData.push_back(pArray);
    pRecords->m_nCol = argc;

    return 0;
}

BOOL CMySqlite::Open(const TCHAR* zFilename)
{
    if(m_pDB != NULL)
    {
        return FALSE;
    }
    const char* pFileName = NULL;
    BOOL bRes = FALSE;
#ifdef _UNICODE
    pFileName = WcharToUtf8(zFilename);
#else
    pFileName = zFilename;
#endif
    int nRes = sqlite3_open(pFileName, &m_pDB);
    if(nRes != SQLITE_OK)
    {
        CString str;
        str.Format(_T("nRes[%d] filename=[%s]"), nRes, pFileName);
        AfxMessageBox(str);
        sqlite3_close(m_pDB);
        m_pDB = NULL;
        bRes = FALSE;
    }
    else
    {
        bRes = TRUE;
    }
#ifdef _UNICODE
    if(pFileName != NULL)
    {
        delete[] pFileName;
        pFileName = NULL;
    }
#endif
    return bRes;
}

BOOL CMySqlite::Execute(const TCHAR* sql, void* data)
{
    if(m_pDB == NULL)
    {
        return FALSE;
    }
    char* zErrMsg = NULL;
    int nRes = SQLITE_OK;
    const char* pSql = NULL;
#ifdef _UNICODE
    pSql = WcharToUtf8(sql);
#else
    pSql = sql;
#endif
    BOOL bRes = FALSE;
    if(data == NULL)
    {
        nRes = sqlite3_exec(m_pDB, pSql, NULL, NULL, &zErrMsg);
    }
    else
    {
        nRes = sqlite3_exec(m_pDB, pSql, callback, data, &zErrMsg);
    }
    if(nRes != SQLITE_OK)
    {
#if _DEBUG
        CString str;
        str.Format(_T("SQL error:[%s]"), zErrMsg);
        AfxMessageBox(str);
#endif
        sqlite3_free(zErrMsg);
        bRes = FALSE;
    }
    else
    {
        bRes = TRUE;
    }
#ifdef _UNICODE
    if(pSql != NULL)
    {
        delete[] pSql;
        pSql = NULL;
    }
#endif
    return bRes;
}

BOOL CMySqlite::IsOpen()
{
    return m_pDB == NULL ? FALSE : TRUE;
}
