#include "StdAfx.h"
#include "MiniAdo.h"
#include "Utility.h"

miniAdo::miniAdo(void)
{
    m_bSetOpen = false;
    m_bConnOpen = false;

    m_ADORecordset = new CADORecordset;
}

miniAdo::~miniAdo(void)
{
    delete m_ADORecordset;
}
BOOL miniAdo::Execute(CString sql)
{
    HRESULT hr;
    try
    {

        m_ADORecordset->m_pConnection->CursorLocation = adUseClient;
        hr = m_ADORecordset->m_pConnection->Execute((LPCTSTR)sql, NULL, ADODB::adExecuteNoRecords);
    }
    catch(_com_error& e)
    {
        _bstr_t bs(e.Source());
        _bstr_t bD(e.Description());
        ATLTRACE("\tMean =%s \n", e.ErrorMessage());
        ATLTRACE("\tSource =%s \n", (LPCTSTR)bs);
        ATLTRACE("\tDescript=%s \n", (LPCTSTR)bD);

        CString msg;
        msg.Format(_T("%s [%s]"), (LPCTSTR)bD, (LPCTSTR)sql);

        CCommon::WriteLog(msg);

#ifdef _DEBUG
        AfxMessageBox(msg, MB_ICONHAND);
#endif

        return FALSE;
    }

    if(SUCCEEDED(hr))
    {
        return TRUE;
    }

    return FALSE;
}


int miniAdo::GetFieldValueAsInt(LPCTSTR sName, int defValue)
{
    int nValue;
    BOOL bFlag;
    bFlag = m_ADORecordset->GetFieldValue(sName, nValue);
    if(!bFlag)
    {
        return defValue;
    }
    return nValue;
}
CString miniAdo::GetFieldValueAsStr(LPCTSTR sName)
{
    CString sValue;
    BOOL bFlag;
    bFlag = m_ADORecordset->GetFieldValue(sName, sValue);

    return sValue;
}

CString   miniAdo::GetFieldValueAsDate(LPCTSTR sName, CString fmt)
{
    CString sValue;
    BOOL bFlag;
    bFlag = m_ADORecordset->GetFieldValue(sName, sValue, fmt);

    return sValue;
}

_variant_t miniAdo::GetFieldValueAsVariant(LPCTSTR sName)
{
    _variant_t var;
    if(!m_ADORecordset->GetFieldValue(sName, var))
    {
        var.vt = VT_NULL;
    }

    return var;
}

double miniAdo:: GetFieldValueAsDouble(LPCTSTR sName)
{
    double dblValue = 0.0;
    BOOL bFlag;
    bFlag = m_ADORecordset->GetFieldValue(sName, dblValue);
    return dblValue;
}


BOOL  miniAdo::GetFieldInfo(int nIndex, CADOFieldInfo& fldInfo)
{
    BOOL bFlag;
    bFlag = m_ADORecordset->GetFieldInfo(nIndex, &fldInfo);
    return bFlag;
}


BOOL   miniAdo::GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo& fldInfo)
{
    BOOL bFlag;
    bFlag = m_ADORecordset->GetFieldInfo(lpFieldName, &fldInfo);
    return bFlag;
}
FieldPtr miniAdo::GetFieldInfo(int nIndex)
{
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    FieldPtr pField = m_ADORecordset->m_pRecordset->Fields->GetItem(vtIndex);
    return pField;
}
long miniAdo::GetFieldCount()
{
    return m_ADORecordset->GetFieldCount();
}
BOOL  miniAdo::Open(ADODB::_ConnectionPtr _pConnection)
{
    if(_pConnection != NULL)
    {
        m_ADORecordset->Open(_pConnection);
        m_bConnOpen = TRUE;
        return TRUE;
    }
#ifdef _DEBUG
    ATLTRACE("Error connecting to data sources!");
#endif
    return FALSE;
}

BOOL miniAdo::OpenRecordset(CString sql, int nOption)
{
    BOOL bFlag = FALSE;
    m_bSetOpen = TRUE;
    bFlag = m_ADORecordset->Open(sql, nOption);
    m_bSetOpen = bFlag;
    return bFlag;
}
BOOL miniAdo::AddNew()
{
    BOOL bFlag;
    bFlag = m_ADORecordset->AddNew();
    return bFlag;
}
BOOL miniAdo::Update()
{
    BOOL bFlag;
    bFlag = m_ADORecordset->Update();
    return bFlag;
}
BOOL miniAdo::PutItem(const LPCTSTR indexname, const _variant_t& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

BOOL miniAdo::PutItem(const LPCTSTR indexname, const long& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

BOOL miniAdo::PutItem(const LPCTSTR indexname, const CString& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

BOOL miniAdo::PutItem(const LPCTSTR indexname, const double& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

BOOL miniAdo::PutItem(const LPCTSTR indexname, const bool& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

BOOL miniAdo::PutItem(const LPCTSTR indexname, const COleDateTime& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

/*
BOOL miniAdo::PutItem(const LPCTSTR indexname,const int& value)
{
	BOOL bFlag = FALSE;
	m_ADORecordset->Edit();
	bFlag=m_ADORecordset->SetFieldValue (indexname,value);
	return bFlag;
}
*/
//////////////////////////////////////////////////////////////////////////
BOOL miniAdo::PutItem(const long indexname, const _variant_t& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

BOOL miniAdo::PutItem(const long indexname, const long& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

BOOL miniAdo::PutItem(const long indexname, const CString& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

BOOL miniAdo::PutItem(const long indexname, const double& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

BOOL miniAdo::PutItem(const long indexname, const bool& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

BOOL miniAdo::PutItem(const long indexname, const COleDateTime& value)
{
    BOOL bFlag = FALSE;
    m_ADORecordset->Edit();
    bFlag = m_ADORecordset->SetFieldValue(indexname, value);
    return bFlag;
}

/*
BOOL miniAdo::PutItem(const long indexname,const int& value)
{
	BOOL bFlag = FALSE;
	m_ADORecordset->Edit();
	bFlag=m_ADORecordset->SetFieldValue (indexname,value);
	return bFlag;
}

*/
int miniAdo::GetRecCount()
{
    BOOL bFlag;
    bFlag = m_ADORecordset->GetRecordCount();
    return bFlag;
}

void miniAdo::CloseRecordset(void)
{
    if(m_bSetOpen)
    {
        m_ADORecordset->Close();
        m_bSetOpen = FALSE;
    }
}
BOOL miniAdo::get_EOF()
{
    if(!m_bSetOpen)
    {
        ATLTRACE("Not open the record set!");
        return TRUE;
    }
    return m_ADORecordset->IsEof();

}
BOOL miniAdo::get_BOF()
{
    if(!m_bSetOpen)
    {
        ATLTRACE("Not open the record set!");
        return TRUE;
    }
    return m_ADORecordset->IsBof();

}

void miniAdo::Prev()
{
    m_ADORecordset->MovePrevious();
}

void miniAdo::Last()
{
    m_ADORecordset->MoveLast() ;
}

void miniAdo::Next()
{
    m_ADORecordset->MoveNext() ;
}

void miniAdo::First()
{
    m_ADORecordset->MoveFirst();
}

BOOL miniAdo::OpenConnect(CString sServer, CString sData, CString sUser, CString sPass)
{
    m_ADORecordset->m_pConnection = NULL;
    HRESULT hr1, hr2;
    CString sql;
    try
    {
        sql.Format(_T("Provider=SQLOLEDB.1;Persist Security Info=False;User ID=%s;\
				   Password=%s;Initial Catalog=%s;Data Source=%s"), sUser, sPass, \
                   sData, sServer);
        hr1 = m_ADORecordset->m_pConnection.CreateInstance(__uuidof(ADODB::Connection));
        hr2 = m_ADORecordset->m_pConnection->Open((_bstr_t)sql, (_bstr_t)sUser, \
                (_bstr_t)sPass, ADODB::adConnectUnspecified);

        if(FAILED(hr1) || FAILED(hr2))
        {
            ATLTRACE("Error connecting to database!");

            CString sInfo;
            sInfo.Format(_T("Error connecting to database <%s>"), sql);
            CCommon::WriteLog(sInfo);

            return FALSE;
        }
    }
    catch(_com_error& e)
    {
        e.Description();
        ATLTRACE("Error connecting to database!");

        CString sInfo;
        sInfo.Format(_T("Error connecting to database <%s>"), sql);
        CCommon::WriteLog(sInfo);

        return FALSE;
    }
    return TRUE;
}
_ConnectionPtr miniAdo::GetActiveConnetion()
{
    if(m_ADORecordset->m_pConnection == NULL)
    {
        return NULL;
    }

    return m_ADORecordset->m_pConnection;
}



long	miniAdo::BeginTrans()
{
    m_ADORecordset->m_pConnection->BeginTrans();
    return 1;
}

long	miniAdo::CommitTrans()
{
    return m_ADORecordset->m_pConnection->CommitTrans();
}

long	miniAdo::RollbackTrans()
{
    return m_ADORecordset->m_pConnection->RollbackTrans();
}
