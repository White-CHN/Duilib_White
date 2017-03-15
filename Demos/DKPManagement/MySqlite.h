#pragma once
#include "sqlite3.h"


class CRecords
{
public:
    CRecords()
        : m_nCol(0)
    {
        m_vtData.clear();
    }
    ~CRecords()
    {
        for(int i = 0; i < (int)m_vtData.size(); i++)
        {
            if(m_vtData[i] != NULL)
            {
                delete m_vtData[i];
                m_vtData[i] = NULL;
            }
        }
        m_vtData.clear();
    }
public:
    CString GetItem(int nRow, int nCol)
    {
        if(nRow < 0 || nCol < 0)
        {
            return _T("");
        }
        return m_vtData[nRow]->GetAt(nCol);
    }
    int GetCount()
    {
        if(m_vtData.size() == 0)
        {
            return 0;
        }
        else
        {
            return (m_vtData.size() * m_vtData[0]->GetCount());
        }
    }
    int GetRow()
    {
        return m_vtData.size();
    }
    int GetCol()
    {
        return m_nCol;
    }
public:
    int m_nCol;// ап
    vector<CStringArray*> m_vtData;
};

class CMySqlite
{
public:
    CMySqlite(void);
    ~CMySqlite(void);
public:
    static int callback(void* data, int argc, char** argv, char** azColName);
    BOOL Open(const TCHAR* zFilename);
    BOOL Execute(const TCHAR* sql, void* data = NULL);
    BOOL IsOpen();
private:
    sqlite3* m_pDB;
};

