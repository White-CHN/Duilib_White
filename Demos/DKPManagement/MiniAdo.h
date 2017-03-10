#pragma once

#include "ado.h"
class miniAdo
{
public:
    miniAdo();
    ~miniAdo();
public:
    CADORecordset*		  m_ADORecordset;
    BOOL    m_bSetOpen;
    BOOL    m_bConnOpen;

public:

    int        GetFieldValueAsInt(LPCTSTR sName, int defVaule = 0);
    CString	   GetFieldValueAsStr(LPCTSTR sName);
    double	   GetFieldValueAsDouble(LPCTSTR sName);
    CString	   GetFieldValueAsDate(LPCTSTR sName, CString fmt = _T("%Y-%m-%d"));
    _variant_t GetFieldValueAsVariant(LPCTSTR sName);

    BOOL	   PutItem(const LPCTSTR indexname, const _variant_t& value);
    BOOL	   PutItem(const LPCTSTR indexname, const long& value);
    BOOL	   PutItem(const LPCTSTR indexname, const CString& value);
    BOOL	   PutItem(const LPCTSTR indexname, const double& value);
    BOOL	   PutItem(const LPCTSTR indexname, const bool& value);
    BOOL	   PutItem(const LPCTSTR indexname, const COleDateTime& value);
    //	BOOL	   PutItem(const LPCTSTR indexname,const int& value);

    BOOL	   PutItem(const long indexname, const _variant_t& value);
    BOOL	   PutItem(const long indexname, const long& value);
    BOOL	   PutItem(const long indexname, const CString& value);
    BOOL	   PutItem(const long indexname, const double& value);
    BOOL	   PutItem(const long indexname, const bool& value);
    BOOL	   PutItem(const long indexname, const COleDateTime& value);
    //	BOOL	   PutItem(const long indexname,const int& value);

    long		GetFieldCount();
    BOOL		GetFieldInfo(int nIndex, CADOFieldInfo& fldInfo);
    BOOL		GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo& fldInfo);
    FieldPtr	GetFieldInfo(int nIndex);

    BOOL		OpenConnect(CString sServer, CString sData, CString sUser, CString sPass);
    BOOL		Open(ADODB::_ConnectionPtr _pConnection);
    BOOL		OpenRecordset(CString sql, int nOption = CADORecordset::openUnknown);
    int			GetRecCount();
    void		CloseRecordset(void);

    BOOL		Execute(CString sql);

    long		BeginTrans();
    long		CommitTrans();
    long		RollbackTrans();

    BOOL		get_BOF();
    BOOL		get_EOF();

    BOOL		AddNew();
    BOOL		Update();
    void		Prev();
    void		Last();
    void		Next();
    void		First();

    //	BOOL		UpdateBatch();


    _ConnectionPtr GetActiveConnetion();
};


