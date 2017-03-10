#include "StdAfx.h"

#include "ado.h"
#include "utility.h"



#pragma warning( push )
#pragma warning( disable : 4310 )
#pragma warning( disable : 4244 )

#define ChunkSize 100
///////////////////////////////////////////////////////
//
// CADODatabase Class
//

DWORD CADODatabase::GetRecordCount(_RecordsetPtr m_pRs)
{
    DWORD numRows = 0;

    numRows = m_pRs->GetRecordCount();

    if(numRows == -1)
    {
        if(m_pRs->EndOfFile != VARIANT_TRUE)
        {
            m_pRs->MoveFirst();
        }

        while(m_pRs->EndOfFile != VARIANT_TRUE)
        {
            numRows++;
            m_pRs->MoveNext();
        }
        if(numRows > 0)
        {
            m_pRs->MoveFirst();
        }
    }
    return numRows;
}

BOOL CADODatabase::Open(LPCTSTR lpstrConnection, LPCTSTR lpstrUserID, LPCTSTR lpstrPassword)
{
    HRESULT hr = S_OK;

    if(IsOpen())
    {
        Close();
    }

    if(_tcsicmp(lpstrConnection, _T("")) != 0)
    {
        m_strConnection = lpstrConnection;
    }

    ATLASSERT(!m_strConnection.IsEmpty());
    CoInitialize(NULL);
    m_pConnection.CreateInstance(__uuidof(Connection));
    try
    {
        if(m_nConnectionTimeout != 0)
        {
            m_pConnection->PutConnectionTimeout(m_nConnectionTimeout);
        }
        hr = m_pConnection->Open(_bstr_t(m_strConnection), _bstr_t(lpstrUserID), _bstr_t(lpstrPassword), NULL);
        return hr == S_OK;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }

}

void CADODatabase::dump_com_error(_com_error& e)
{
    CString ErrorStr;


    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());
    ErrorStr.Format(_T("CADODataBase Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
                    e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription);
    m_strLastError = _T("Connection String = " + GetConnectionString() + '\n' + ErrorStr);
    m_dwLastError = e.Error();
    CADOHelper::SendErrorMsg(m_pConnection);
#ifdef _DEBUG
    ATLTRACE(ErrorStr);
#endif
}

BOOL CADODatabase::IsOpen()
{
    if(m_pConnection)
    {
        return m_pConnection->GetState() != adStateClosed;
    }
    return FALSE;
}

void CADODatabase::Close()
{
    if(IsOpen())
    {
        m_pConnection->Close();
    }
}


///////////////////////////////////////////////////////
//
// CADORecordset Class
//

CADORecordset::CADORecordset()
{
    m_pRecordset = NULL;
    m_pCmd = NULL;
    m_strQuery = _T("");
    m_strLastError = _T("");
    m_dwLastError = 0;
    m_pRecBinding = NULL;
    m_pRecordset.CreateInstance(__uuidof(Recordset));
    m_pCmd.CreateInstance(__uuidof(Command));
    m_nEditStatus = CADORecordset::dbEditNone;
    m_nSearchDirection = CADORecordset::searchForward;
}

CADORecordset::CADORecordset(CADODatabase* pAdoDatabase)
{
    m_pRecordset = NULL;
    m_pCmd = NULL;
    m_strQuery = _T("");
    m_strLastError = _T("");
    m_dwLastError = 0;
    m_pRecBinding = NULL;
    m_pRecordset.CreateInstance(__uuidof(Recordset));
    m_pCmd.CreateInstance(__uuidof(Command));
    m_nEditStatus = CADORecordset::dbEditNone;
    m_nSearchDirection = CADORecordset::searchForward;

    m_pConnection = pAdoDatabase->GetActiveConnection();
}
BOOL CADORecordset::Open(_ConnectionPtr mpdb)
{
    Close();
    if(mpdb == NULL)
    {
        return FALSE;
    }
    m_pConnection = mpdb;

    return TRUE;
}
BOOL CADORecordset::Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec, int nOption)
{
    Close();

    if(_tcsicmp(lpstrExec, _T("")) != 0)
    {
        m_strQuery = lpstrExec;
    }

    ATLASSERT(!m_strQuery.IsEmpty());

    if(m_pConnection == NULL)
    {
        m_pConnection = mpdb;
    }

    m_strQuery.TrimLeft();
    BOOL bIsSelect = (m_strQuery.Mid(0, (int)_tcslen(_T("Select "))).CompareNoCase(_T("select ")) == 0 && openUnknown == nOption);

    try
    {
        m_pRecordset->CursorType = adOpenStatic;
        m_pRecordset->CursorLocation = adUseClient;
        if(bIsSelect || nOption == openQuery || nOption == openUnknown)
            m_pRecordset->Open(m_strQuery.GetBuffer(), _variant_t((IDispatch*)mpdb, TRUE),
                               adOpenStatic, adLockOptimistic, adCmdUnknown);
        else if(nOption == openTable)
            m_pRecordset->Open(m_strQuery.GetBuffer(), _variant_t((IDispatch*)mpdb, TRUE),
                               adOpenKeyset, adLockOptimistic, adCmdTable);
        else if(nOption == openStoredProc)
        {
            m_pCmd->ActiveConnection = mpdb;
            m_pCmd->CommandText = _bstr_t(m_strQuery);
            m_pCmd->CommandType = adCmdStoredProc;
            m_pConnection->CursorLocation = adUseClient;

            m_pRecordset = m_pCmd->Execute(NULL, NULL, adCmdText);
        }
        else
        {
            ATLTRACE("Unknown parameter. %d", nOption);
            return FALSE;
        }
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }

    return m_pRecordset != NULL;
}

BOOL CADORecordset::Open(LPCTSTR lpstrExec, int nOption)
{


    if(m_pConnection == NULL)
    {
        CString sInfo;
        sInfo.Format(_T("数据连接错误 <%s>"), lpstrExec);
        AfxMessageBox(sInfo, MB_ICONHAND);
        // Vito CCommon::WriteLog(sInfo);
        return FALSE;
    }

    if(m_pConnection->GetState() == adStateClosed)
    {
        CString sInfo;
        sInfo.Format(_T("数据连接错误 <%s>"), lpstrExec);
        AfxMessageBox(sInfo, MB_ICONHAND);
        // Vito CCommon::WriteLog(sInfo);
        return FALSE;
    }


    return Open(m_pConnection, lpstrExec, nOption);
}


BOOL CADORecordset::Requery()
{
    if(IsOpen())
    {
        try
        {
            m_pRecordset->Requery(adExecuteRecord);
        }
        catch(_com_error& e)
        {
            dump_com_error(e);
            return FALSE;
        }
    }
    return TRUE;
}


BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, double& dbValue)
{
    double val = (double)NULL;
    _variant_t vtFld;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
        switch(vtFld.vt)
        {
            case VT_R4:
                val = vtFld.fltVal;
                break;
            case VT_R8:
                val = vtFld.dblVal;
                break;
            case VT_DECIMAL:
                //Corrected by Jos?Carlos Mart韓ez Gal醤
                //val = vtFld.decVal.Lo32;
                //val *= (vtFld.decVal.sign == 128)? -1 : 1;
                //val /= pow(10, vtFld.decVal.scale);
                val = (double)vtFld;
                break;
            case VT_UI1:
                val = vtFld.iVal;
                break;
            case VT_I2:
            case VT_I4:
                val = vtFld.lVal;
                break;
            case VT_INT:
                val = vtFld.intVal;
                break;
            case VT_NULL:
            case VT_EMPTY:
                val = 0;
                break;
            default:
                val = vtFld.dblVal;
        }
        dbValue = val;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}


BOOL CADORecordset::GetFieldValue(int nIndex, double& dbValue)
{
    double val = (double)NULL;
    _variant_t vtFld;
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
        switch(vtFld.vt)
        {
            case VT_R4:
                val = vtFld.fltVal;
                break;
            case VT_R8:
                val = vtFld.dblVal;
                break;
            case VT_DECIMAL:
                //Corrected by Jos?Carlos Mart韓ez Gal醤
                //val = vtFld.decVal.Lo32;
                //val *= (vtFld.decVal.sign == 128)? -1 : 1;
                //val /= pow(10, vtFld.decVal.scale);
                val = (double)vtFld;
                break;
            case VT_UI1:
                val = vtFld.iVal;
                break;
            case VT_I2:
            case VT_I4:
                val = vtFld.lVal;
                break;
            case VT_INT:
                val = vtFld.intVal;
                break;
            case VT_NULL:
            case VT_EMPTY:
                val = 0;
                break;
            default:
                val = 0;
        }
        dbValue = val;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}


BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, long& lValue)
{
    long val = (long)NULL;
    _variant_t vtFld;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
        if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
        {
            val = vtFld.lVal;
        }
        lValue = val;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(int nIndex, long& lValue)
{
    long val = (long)NULL;
    _variant_t vtFld;
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
        if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
        {
            val = vtFld.lVal;
        }
        lValue = val;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}


BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, unsigned long& ulValue)
{
    long val = (long)NULL;
    _variant_t vtFld;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
        if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
        {
            val = vtFld.ulVal;
        }
        ulValue = val;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(int nIndex, unsigned long& ulValue)
{
    long val = (long)NULL;
    _variant_t vtFld;
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
        if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
        {
            val = vtFld.ulVal;
        }
        ulValue = val;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, int& nValue)
{
    int val = NULL;
    _variant_t vtFld;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
        switch(vtFld.vt)
        {
            case VT_BOOL:
                val = vtFld.boolVal;
                break;
            case VT_I2:
            case VT_UI1:
                val = vtFld.iVal;
                break;
            case VT_INT:
            case  VT_I4:
                val = vtFld.intVal;
                break;
            case VT_NULL:
            case VT_EMPTY:
                val = 0;
                break;
            case VT_DECIMAL:
                //val = (long)vtFld.decVal.Lo32;
                val = (double)vtFld;

                break;
            default:
                val = (long)vtFld;
                break;
        }
        nValue = val;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(int nIndex, int& nValue)
{
    int val = (int)NULL;
    _variant_t vtFld;
    _variant_t vtIndex;

    vtIndex.vt = VT_I4;
    vtIndex.iVal = nIndex;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
        switch(vtFld.vt)
        {
            case VT_BOOL:
                val = vtFld.boolVal;
                break;
            case VT_I2:
            case VT_UI1:
                val = vtFld.iVal;
                break;
            case VT_INT:
            case  VT_I4:
                val = vtFld.intVal;
                break;
            case VT_NULL:
            case VT_EMPTY:
                val = 0;
                break;
            default:
                val = vtFld.iVal;
        }
        nValue = val;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, CString& strValue, CString strDateFormat)
{
    CString str = _T("");
    _variant_t vtFld;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
        switch(vtFld.vt)
        {
            case VT_R4:
                str = CADOHelper::DblToStr(vtFld.fltVal);
                break;
            case VT_R8:
                str = CADOHelper::DblToStr(vtFld.dblVal);
                break;
            case VT_BSTR:
                str = vtFld.bstrVal;
                break;
            case VT_I2:
            case VT_UI1:
                str = CADOHelper::IntToStr(vtFld.iVal);
                break;
            case VT_INT:
                str = CADOHelper::IntToStr(vtFld.intVal);
                break;
            case VT_I4:
                str = CADOHelper::LongToStr(vtFld.lVal);
                break;
            case VT_UI4:
                str = CADOHelper::ULongToStr(vtFld.ulVal);
                break;
            case VT_DECIMAL:
            {
                //Corrected by Jos?Carlos Mart韓ez Gal醤
                //double val = vtFld.decVal.Lo32;
                //val *= (vtFld.decVal.sign == 128)? -1 : 1;
                //val /= pow(10, vtFld.decVal.scale);
                double val = (double)vtFld;

                str = CADOHelper::DblToStr(val);
            }
            break;

            case VT_DATE:
            {
                COleDateTime dt(vtFld);

                if(strDateFormat.IsEmpty())
                {
                    strDateFormat = _T("%Y-%m-%d %H:%M:%S");
                }
                //str = dt.Format(strDateFormat);
                str.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), dt.GetYear(), dt.GetMonth(), dt.GetDay(), \
                           dt.GetHour(), dt.GetMinute(), dt.GetSecond());
            }
            break;
            case VT_EMPTY:
            case VT_NULL:
                str.Empty();
                break;
            default:
                str.Empty();
                return FALSE;
        }
        strValue = str;
        strValue.TrimLeft();
        strValue.TrimRight();

        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(int nIndex, CString& strValue, CString strDateFormat)
{
    CString str = _T("");
    _variant_t vtFld;
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
        switch(vtFld.vt)
        {
            case VT_R4:
                str = CADOHelper::DblToStr(vtFld.fltVal);
                break;
            case VT_R8:
                str = CADOHelper::DblToStr(vtFld.dblVal);
                break;
            case VT_BSTR:
                str = vtFld.bstrVal;
                break;
            case VT_I2:
            case VT_UI1:
                str = CADOHelper::IntToStr(vtFld.iVal);
                break;
            case VT_INT:
                str = CADOHelper::IntToStr(vtFld.intVal);
                break;
            case VT_I4:
                str = CADOHelper::LongToStr(vtFld.lVal);
                break;
            case VT_UI4:
                str = CADOHelper::ULongToStr(vtFld.ulVal);
                break;
            case VT_DECIMAL:
            {
                //Corrected by Jos?Carlos Mart韓ez Gal醤
//			double val = vtFld.decVal.Lo32;
//			val *= (vtFld.decVal.sign == 128)? -1 : 1;
//			val /= pow(10, vtFld.decVal.scale);
                double val = (double)vtFld;

                str = CADOHelper::DblToStr(val);
            }
            break;
            case VT_DATE:
            {
                COleDateTime dt(vtFld);

                if(strDateFormat.IsEmpty())
                {
                    strDateFormat = _T("%Y-%m-%d %H:%M:%S");
                }
                //str = dt.Format(strDateFormat);
                str.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), dt.GetYear(), dt.GetMonth(), dt.GetDay(), \
                           dt.GetHour(), dt.GetMinute(), dt.GetSecond());

            }
            break;
            case VT_EMPTY:
            case VT_NULL:
                str.Empty();
                break;
            default:
                str.Empty();
                return FALSE;
        }
        strValue = str;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, COleDateTime& time)
{
    _variant_t vtFld;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
        switch(vtFld.vt)
        {
            case VT_DATE:
            {
                COleDateTime dt(vtFld);
                time = dt;
            }
            break;
            case VT_EMPTY:
            case VT_NULL:
                time.SetStatus(COleDateTime::null);
                break;
            default:
                return FALSE;
        }
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(int nIndex, COleDateTime& time)
{
    _variant_t vtFld;
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
        switch(vtFld.vt)
        {
            case VT_DATE:
            {
                COleDateTime dt(vtFld);
                time = dt;
            }
            break;
            case VT_EMPTY:
            case VT_NULL:
                time.SetStatus(COleDateTime::null);
                break;
            default:
                return FALSE;
        }
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, bool& bValue)
{
    _variant_t vtFld;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
        switch(vtFld.vt)
        {
            case VT_BOOL:
                bValue = vtFld.boolVal == TRUE ? true : false;
                break;
            case VT_EMPTY:
            case VT_NULL:
                bValue = false;
                break;
            default:
                return FALSE;
        }
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(int nIndex, bool& bValue)
{
    _variant_t vtFld;
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
        switch(vtFld.vt)
        {
            case VT_BOOL:
                bValue = vtFld.boolVal == TRUE ? true : false;
                break;
            case VT_EMPTY:
            case VT_NULL:
                bValue = false;
                break;
            default:
                return FALSE;
        }
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

//BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, COleCurrency& cyValue)
//{
//	_variant_t vtFld;
//
//	try
//	{
//		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
//		switch(vtFld.vt)
//		{
//		case VT_CY:
//			cyValue = (CURRENCY)vtFld.cyVal;
//			break;
//		case VT_EMPTY:
//		case VT_NULL:
//			{
//			cyValue = COleCurrency();
//			cyValue.m_status = COleCurrency::null;
//			}
//			break;
//		default:
//			return FALSE;
//		}
//		return TRUE;
//	}
//	catch(_com_error &e)
//	{
//		dump_com_error(e);
//		return FALSE;
//	}
//}
//
//BOOL CADORecordset::GetFieldValue(int nIndex, COleCurrency& cyValue)
//{
//	_variant_t vtFld;
//	_variant_t vtIndex;
//
//	vtIndex.vt = VT_I2;
//	vtIndex.iVal = nIndex;
//
//	try
//	{
//		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
//		switch(vtFld.vt)
//		{
//		case VT_CY:
//			cyValue = (CURRENCY)vtFld.cyVal;
//			break;
//		case VT_EMPTY:
//		case VT_NULL:
//			{
//			cyValue = COleCurrency();
//			cyValue.m_status = COleCurrency::null;
//			}
//			break;
//		default:
//			return FALSE;
//		}
//		return TRUE;
//	}
//	catch(_com_error &e)
//	{
//		dump_com_error(e);
//		return FALSE;
//	}
//}
//
BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, _variant_t& vtValue)
{
    try
    {
        vtValue = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::GetFieldValue(int nIndex, _variant_t& vtValue)
{
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    try
    {
        vtValue = m_pRecordset->Fields->GetItem(vtIndex)->Value;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}



CString CADORecordset::GetFieldValueAsStr(LPCTSTR lpFieldName)
{
    _variant_t vtValue;

    GetFieldValue(lpFieldName, vtValue);

    return Convert::Var2Str(vtValue);

}

CString CADORecordset::GetFieldValueAsStr(long nIndex)
{
    _variant_t vtValue;

    GetFieldValue(nIndex, vtValue);

    return Convert::Var2Str(vtValue);

}


BOOL CADORecordset::IsFieldNull(LPCTSTR lpFieldName)
{
    _variant_t vtFld;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
        return vtFld.vt == VT_NULL;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::IsFieldNull(int nIndex)
{
    _variant_t vtFld;
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
        return vtFld.vt == VT_NULL;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::IsFieldEmpty(LPCTSTR lpFieldName)
{
    _variant_t vtFld;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
        return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::IsFieldEmpty(int nIndex)
{
    _variant_t vtFld;
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    try
    {
        vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
        return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::SetFieldEmpty(LPCTSTR lpFieldName)
{
    _variant_t vtFld;
    vtFld.vt = VT_EMPTY;

    return PutFieldValue(lpFieldName, vtFld);
}

BOOL CADORecordset::SetFieldEmpty(int nIndex)
{
    _variant_t vtFld;
    vtFld.vt = VT_EMPTY;

    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    return PutFieldValue(vtIndex, vtFld);
}


DWORD CADORecordset::GetRecordCount()
{
    DWORD nRows = 0;

    nRows = m_pRecordset->GetRecordCount();

    if(nRows == -1)
    {
        nRows = 0;
        if(m_pRecordset->EndOfFile != VARIANT_TRUE)
        {
            m_pRecordset->MoveFirst();
        }

        while(m_pRecordset->EndOfFile != VARIANT_TRUE)
        {
            nRows++;
            m_pRecordset->MoveNext();
        }
        if(nRows > 0)
        {
            m_pRecordset->MoveFirst();
        }
    }

    return nRows;
}

BOOL CADORecordset::IsOpen()
{
    if(m_pRecordset != NULL && IsConnectionOpen())
    {
        return m_pRecordset->GetState() != adStateClosed;
    }
    return FALSE;
}

void CADORecordset::Close()
{

    if(IsOpen())
    {
        try
        {
            if(m_nEditStatus != dbEditNone)
            {
                CancelUpdate();
            }



            CString stFld = (LPCTSTR)m_pRecordset->GetSort();

            // 白涛  2004/10/29 修改
            // 原因  关联查询记录集关闭出错
            if(!stFld.IsEmpty())
            {
                m_pRecordset->PutSort(_T(""));
            }


            m_pRecordset->Close();
        }
        catch(_com_error& e)
        {
            dump_com_error(e);
        }
    }

}


BOOL CADODatabase::Execute(LPCTSTR lpstrExec)
{
    ATLASSERT(m_pConnection != NULL);
    ATLASSERT(_tcsicmp(lpstrExec, _T("")) != 0);
    _variant_t vRecords;

    m_nRecordsAffected = 0;

    try
    {
        m_pConnection->CursorLocation = adUseClient;
        m_pConnection->Execute(_bstr_t(lpstrExec), &vRecords, adExecuteNoRecords);
        m_nRecordsAffected = vRecords.iVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::RecordBinding(CADORecordBinding& pAdoRecordBinding)
{
    HRESULT hr;
    m_pRecBinding = NULL;

    //Open the binding interface.
    if(FAILED(hr = m_pRecordset->QueryInterface(__uuidof(IADORecordBinding), (LPVOID*)&m_pRecBinding)))
    {
        _com_issue_error(hr);
        return FALSE;
    }

    //Bind the recordset to class
    if(FAILED(hr = m_pRecBinding->BindToRecordset(&pAdoRecordBinding)))
    {
        _com_issue_error(hr);
        return FALSE;
    }
    return TRUE;
}

BOOL CADORecordset::GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo)
{
    FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

    return GetFieldInfo(pField, fldInfo);
}

BOOL CADORecordset::GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo)
{
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

    return GetFieldInfo(pField, fldInfo);
}


BOOL CADORecordset::GetFieldInfo(FieldPtr pField, CADOFieldInfo* fldInfo)
{
    memset(fldInfo, 0, sizeof(CADOFieldInfo));

    _tcscpy(fldInfo->m_strName, (LPCTSTR)pField->GetName());
    fldInfo->m_lDefinedSize = pField->GetDefinedSize();
    fldInfo->m_nType = pField->GetType();
    fldInfo->m_lAttributes = pField->GetAttributes();
    if(!IsEof())
    {
        fldInfo->m_lSize = pField->GetActualSize();
    }
    return TRUE;
}

BOOL CADORecordset::GetChunk(LPCTSTR lpFieldName, CString& strValue)
{
    FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

    return GetChunk(pField, strValue);
}

BOOL CADORecordset::GetChunk(int nIndex, CString& strValue)
{
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

    return GetChunk(pField, strValue);
}


BOOL CADORecordset::GetChunk(FieldPtr pField, CString& strValue)
{
    CString str = _T("");
    long lngSize, lngOffSet = 0;
    _variant_t varChunk;

    lngSize = pField->ActualSize;

    str.Empty();
    while(lngOffSet < lngSize)
    {
        try
        {
            varChunk = pField->GetChunk(ChunkSize);

            str += varChunk.bstrVal;
            lngOffSet += ChunkSize;
        }
        catch(_com_error& e)
        {
            dump_com_error(e);
            return FALSE;
        }
    }

    lngOffSet = 0;
    strValue = str;
    return TRUE;
}

BOOL CADORecordset::GetChunk(LPCTSTR lpFieldName, LPVOID lpData)
{
    FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

    return GetChunk(pField, lpData);
}

BOOL CADORecordset::GetChunk(int nIndex, LPVOID lpData)
{
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

    return GetChunk(pField, lpData);
}

BOOL CADORecordset::GetChunk(FieldPtr pField, LPVOID lpData)
{
    long lngSize, lngOffSet = 0;
    _variant_t varChunk;
    UCHAR chData;
    HRESULT hr;
    long lBytesCopied = 0;

    lngSize = pField->ActualSize;

    while(lngOffSet < lngSize)
    {
        try
        {
            varChunk = pField->GetChunk(ChunkSize);

            //Copy the data only upto the Actual Size of Field.
            for(long lIndex = 0; lIndex <= (ChunkSize - 1); lIndex++)
            {
                hr = SafeArrayGetElement(varChunk.parray, &lIndex, &chData);
                if(SUCCEEDED(hr))
                {
                    //Take BYTE by BYTE and advance Memory Location
                    //hr = SafeArrayPutElement((SAFEARRAY FAR*)lpData, &lBytesCopied ,&chData);
                    ((UCHAR*)lpData)[lBytesCopied] = chData;
                    lBytesCopied++;
                }
                else
                {
                    break;
                }
            }
            lngOffSet += ChunkSize;
        }
        catch(_com_error& e)
        {
            dump_com_error(e);
            return FALSE;
        }
    }

    lngOffSet = 0;
    return TRUE;
}

BOOL CADORecordset::AppendChunk(LPCTSTR lpFieldName, LPVOID lpData, UINT nBytes)
{

    FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

    return AppendChunk(pField, lpData, nBytes);
}


BOOL CADORecordset::AppendChunk(int nIndex, LPVOID lpData, UINT nBytes)
{
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

    return AppendChunk(pField, lpData, nBytes);
}

BOOL CADORecordset::AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes)
{
    HRESULT hr;
    _variant_t varChunk;
    long lngOffset = 0;
    UCHAR chData;
    SAFEARRAY FAR* psa = NULL;
    SAFEARRAYBOUND rgsabound[1];

    try
    {
        //Create a safe array to store the array of BYTES
        rgsabound[0].lLbound = 0;
        rgsabound[0].cElements = nBytes;
        psa = SafeArrayCreate(VT_UI1, 1, rgsabound);

        while(lngOffset < (long)nBytes)
        {
            chData	= ((UCHAR*)lpData)[lngOffset];
            hr = SafeArrayPutElement(psa, &lngOffset, &chData);

            if(FAILED(hr))
            {
                return FALSE;
            }

            lngOffset++;
        }
        lngOffset = 0;

        //Assign the Safe array  to a variant.
        varChunk.vt = VT_ARRAY | VT_UI1;
        varChunk.parray = psa;

        hr = pField->AppendChunk(varChunk);

        if(SUCCEEDED(hr))
        {
            return TRUE;
        }
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }

    return FALSE;
}

CString CADORecordset::GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows)
{
    lpNull;

    _bstr_t varOutput;
    _bstr_t varNull("");
    _bstr_t varCols("\t");
    _bstr_t varRows("\r");

    if(_tcsclen(lpCols) != 0)
    {
        varCols = _bstr_t(lpCols);
    }

    if(_tcsclen(lpRows) != 0)
    {
        varRows = _bstr_t(lpRows);
    }

    if(numRows == 0)
    {
        numRows = (long)GetRecordCount();
    }

    varOutput = m_pRecordset->GetString(adClipString, numRows, varCols, varRows, varNull);

    return (LPCTSTR)varOutput;
}

void CADORecordset::Edit()
{
    m_nEditStatus = dbEdit;
}

BOOL CADORecordset::AddNew()
{
    m_nEditStatus = dbEditNone;
    if(m_pRecordset->AddNew() != S_OK)
    {
        return FALSE;
    }

    m_nEditStatus = dbEditNew;
    return TRUE;
}

BOOL CADORecordset::AddNew(CADORecordBinding& pAdoRecordBinding)
{
    try
    {
        if(m_pRecBinding->AddNew(&pAdoRecordBinding) != S_OK)
        {
            return FALSE;
        }
        else
        {
            m_pRecBinding->Update(&pAdoRecordBinding);
            return TRUE;
        }

    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::Update()
{
    BOOL bret = TRUE;

    if(m_nEditStatus != dbEditNone)
    {

        try
        {
            if(m_pRecordset->Update() != S_OK)
            {
                bret = FALSE;
            }
        }
        catch(_com_error& e)
        {
            dump_com_error(e);
            bret = FALSE;
        }

        if(!bret)
        {
            m_pRecordset->CancelUpdate();
        }
        m_nEditStatus = dbEditNone;
    }
    return bret;
}

void CADORecordset::CancelUpdate()
{
    m_pRecordset->CancelUpdate();
    m_nEditStatus = dbEditNone;
}
BOOL CADORecordset::SetFieldValue(int nIndex, CString strValue)
{
    _variant_t vtFld;
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    if(!strValue.IsEmpty())
    {
        vtFld.vt = VT_BSTR;
    }
    else
    {
        vtFld.vt = VT_NULL;
    }

    //Corrected by Giles Forster 10/03/2001
    vtFld.bstrVal = strValue.AllocSysString();//del by zyw
    //vtFld.bstrVal = (_bstr_t)strValue;//add by zyw  2005.1.18


    return PutFieldValue(vtIndex, vtFld);
}


BOOL CADORecordset::SetFieldValueEx(LPCTSTR lpFieldName, CString strValue)
{
    if(m_nEditStatus == dbEditNone)
    {
        return FALSE;
    }

    try
    {
        CADOHelper::PutRsItem(m_pRecordset, lpFieldName, strValue);
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, CString strValue)
{
    _variant_t vtFld;

    if(!strValue.IsEmpty())
    {
        vtFld.vt = VT_BSTR;
    }
    else
    {
        vtFld.vt = VT_NULL;
    }

    //Corrected by Giles Forster 10/03/2001
    vtFld.bstrVal = strValue.AllocSysString();//del by zyw
    //vtFld.bstrVal = (_bstr_t)strValue;//add by zyw  2005.1.18

    return PutFieldValue(lpFieldName, vtFld);
}

BOOL CADORecordset::SetFieldValue(int nIndex, int nValue)
{
    _variant_t vtFld;

    vtFld.vt = VT_I4;
    vtFld.lVal = nValue;

    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    return PutFieldValue(vtIndex, vtFld);
}

BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, int nValue)
{
    _variant_t vtFld;

    vtFld.vt = VT_I4;
    vtFld.lVal = nValue;


    return PutFieldValue(lpFieldName, vtFld);
}

BOOL CADORecordset::SetFieldValue(int nIndex, long lValue)
{
    _variant_t vtFld;
    vtFld.vt = VT_I4;
    vtFld.lVal = lValue;

    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    return PutFieldValue(vtIndex, vtFld);

}

BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, long lValue)
{
    _variant_t vtFld;
    vtFld.vt = VT_I4;
    vtFld.lVal = lValue;

    return PutFieldValue(lpFieldName, vtFld);
}

BOOL CADORecordset::SetFieldValue(int nIndex, unsigned long ulValue)
{
    _variant_t vtFld;
    vtFld.vt = VT_UI4;
    vtFld.ulVal = ulValue;

    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    return PutFieldValue(vtIndex, vtFld);

}

BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, unsigned long ulValue)
{
    _variant_t vtFld;
    vtFld.vt = VT_UI4;
    vtFld.ulVal = ulValue;

    return PutFieldValue(lpFieldName, vtFld);
}

BOOL CADORecordset::SetFieldValue(int nIndex, double dblValue)
{
    _variant_t vtFld;
    vtFld.vt = VT_R8;
    vtFld.dblVal = dblValue;

    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    return PutFieldValue(vtIndex, vtFld);
}

BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, double dblValue)
{
    _variant_t vtFld;
    vtFld.vt = VT_R8;
    vtFld.dblVal = dblValue;

    return PutFieldValue(lpFieldName, vtFld);
}

BOOL CADORecordset::SetFieldValue(int nIndex, COleDateTime time)
{
    _variant_t vtFld;
    vtFld.vt = VT_DATE;
    vtFld.date = time;

    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    return PutFieldValue(vtIndex, vtFld);
}

BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, COleDateTime time)
{
    _variant_t vtFld;
    vtFld.vt = VT_DATE;
    vtFld.date = time;

    return PutFieldValue(lpFieldName, vtFld);
}



BOOL CADORecordset::SetFieldValue(int nIndex, bool bValue)
{
    _variant_t vtFld;
    vtFld.vt = VT_BOOL;
    vtFld.boolVal = bValue;

    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    return PutFieldValue(vtIndex, vtFld);
}

BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, bool bValue)
{
    _variant_t vtFld;
    vtFld.vt = VT_BOOL;
    vtFld.boolVal = bValue;

    return PutFieldValue(lpFieldName, vtFld);
}


//BOOL CADORecordset::SetFieldValue(int nIndex, COleCurrency cyValue)
//{
//	if(cyValue.m_status == COleCurrency::invalid)
//		return FALSE;
//
//	_variant_t vtFld;
//
//	vtFld.vt = VT_CY;
//	vtFld.cyVal = cyValue.m_cur;
//
//	_variant_t vtIndex;
//
//	vtIndex.vt = VT_I2;
//	vtIndex.iVal = nIndex;
//
//	return PutFieldValue(vtIndex, vtFld);
//}
//
//BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, COleCurrency cyValue)
//{
//	if(cyValue.m_status == COleCurrency::invalid)
//		return FALSE;
//
//	_variant_t vtFld;
//
//	vtFld.vt = VT_CY;
//	vtFld.cyVal = cyValue.m_cur;
//
//	return PutFieldValue(lpFieldName, vtFld);
//}
//
BOOL CADORecordset::SetFieldValue(int nIndex, _variant_t vtValue)
{
    _variant_t vtIndex;

    vtIndex.vt = VT_I2;
    vtIndex.iVal = nIndex;

    return PutFieldValue(vtIndex, vtValue);
}

BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, _variant_t vtValue)
{
    return PutFieldValue(lpFieldName, vtValue);
}


BOOL CADORecordset::SetBookmark()
{
    if(m_varBookmark.vt != VT_EMPTY)
    {
        m_pRecordset->Bookmark = m_varBookmark;
        return TRUE;
    }
    return FALSE;
}

BOOL CADORecordset::Delete()
{
    if(m_pRecordset->Delete(adAffectCurrent) != S_OK)
    {
        return FALSE;
    }

    if(m_pRecordset->Update() != S_OK)
    {
        return FALSE;
    }

    m_nEditStatus = dbEditNone;
    return TRUE;
}

BOOL CADORecordset::Find(LPCTSTR lpFind, int nSearchDirection)
{

    m_strFind = lpFind;
    m_nSearchDirection = nSearchDirection;

    ATLASSERT(!m_strFind.IsEmpty());

    if(m_nSearchDirection == searchForward)
    {
        m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchForward, "");
        if(!IsEof())
        {
            m_varBookFind = m_pRecordset->Bookmark;
            return TRUE;
        }
    }
    else if(m_nSearchDirection == searchBackward)
    {
        m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchBackward, "");
        if(!IsBof())
        {
            m_varBookFind = m_pRecordset->Bookmark;
            return TRUE;
        }
    }
    else
    {
        ATLTRACE("Unknown parameter. %d", nSearchDirection);
        m_nSearchDirection = searchForward;
    }
    return FALSE;
}

BOOL CADORecordset::FindFirst(LPCTSTR lpFind)
{
    m_pRecordset->MoveFirst();
    return Find(lpFind);
}

BOOL CADORecordset::FindNext()
{
    if(m_nSearchDirection == searchForward)
    {
        m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchForward, m_varBookFind);
        if(!IsEof())
        {
            m_varBookFind = m_pRecordset->Bookmark;
            return TRUE;
        }
    }
    else
    {
        m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchBackward, m_varBookFind);
        if(!IsBof())
        {
            m_varBookFind = m_pRecordset->Bookmark;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CADORecordset::PutFieldValue(LPCTSTR lpFieldName, _variant_t vtFld)
{
    if(m_nEditStatus == dbEditNone)
    {
        return FALSE;
    }

    try
    {
        m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}


BOOL CADORecordset::PutFieldValue(_variant_t vtIndex, _variant_t vtFld)
{
    if(m_nEditStatus == dbEditNone)
    {
        return FALSE;
    }

    try
    {
        m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::Clone(CADORecordset& pRs)
{
    try
    {
        pRs.m_pRecordset = m_pRecordset->Clone(adLockUnspecified);
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::SetFilter(LPCTSTR strFilter)
{
    ATLASSERT(IsOpen());

    try
    {
        m_pRecordset->PutFilter(strFilter);
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::SetSort(LPCTSTR strCriteria)
{
    ATLASSERT(IsOpen());

    try
    {
        m_pRecordset->PutSort(strCriteria);
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::SaveAsXML(LPCTSTR lpstrXMLFile)
{
    HRESULT hr;

    ATLASSERT(IsOpen());

    try
    {
        hr = m_pRecordset->Save(lpstrXMLFile, adPersistXML);
        return hr == S_OK;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::OpenXML(LPCTSTR lpstrXMLFile)
{
    HRESULT hr = S_OK;

    if(IsOpen())
    {
        Close();
    }

    try
    {
        hr = m_pRecordset->Open(lpstrXMLFile, "Provider=MSPersist;", adOpenForwardOnly, adLockOptimistic, adCmdFile);
        return hr == S_OK;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADORecordset::Execute(CADOCommand* pAdoCommand)
{
    if(IsOpen())
    {
        Close();
    }

    ATLASSERT(!pAdoCommand->GetText().IsEmpty());
    try
    {
        m_pConnection->CursorLocation = adUseClient;
        m_pRecordset = pAdoCommand->GetCommand()->Execute(NULL, NULL, pAdoCommand->GetType());
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

void CADORecordset::dump_com_error(_com_error& e)
{
    CString ErrorStr;


    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());
    ErrorStr.Format(_T("CADORecordset Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
                    e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription);
    m_strLastError = _T("Query = " + GetQuery() + '\n' + ErrorStr);
    m_dwLastError = e.Error();
    CADOHelper::SendErrorMsg(m_pConnection);
#ifdef _DEBUG
    ATLTRACE(ErrorStr);
    ATLASSERT(FALSE);
#endif

    MessageBoxEx(NULL, m_strLastError, _T("错误"), MB_ICONHAND, 0);

    // Vito CCommon::WriteLog(m_strLastError);

}


///////////////////////////////////////////////////////
//
// CADOCommad Class
//

CADOCommand::CADOCommand(CADODatabase* pAdoDatabase, CString strCommandText, int nCommandType)
{
    m_pCommand = NULL;
    m_pCommand.CreateInstance(__uuidof(Command));
    m_strCommandText = strCommandText;
    //m_pCommand->CommandText = m_strCommandText.AllocSysString();//del by zyw
    m_pCommand->CommandText = (_bstr_t) m_strCommandText; //add by zyw  2005.1.18
    m_nCommandType = nCommandType;
    m_pCommand->CommandType = (CommandTypeEnum)m_nCommandType;
    m_pCommand->ActiveConnection = pAdoDatabase->GetActiveConnection();
    m_nRecordsAffected = 0;
}

BOOL CADOCommand::AddParameter(CADOParameter* pAdoParameter)
{
    ATLASSERT(pAdoParameter->GetParameter() != NULL);

    try
    {
        m_pCommand->Parameters->Append(pAdoParameter->GetParameter());
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, int nValue)
{

    _variant_t vtValue;

    vtValue.vt = VT_I2;
    vtValue.iVal = nValue;

    return AddParameter(strName, nType, nDirection, lSize, vtValue);
}

BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, long lValue)
{

    _variant_t vtValue;

    vtValue.vt = VT_I4;
    vtValue.lVal = lValue;

    return AddParameter(strName, nType, nDirection, lSize, vtValue);
}

BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, double dblValue, int nPrecision, int nScale)
{

    _variant_t vtValue;

    vtValue.vt = VT_R8;
    vtValue.dblVal = dblValue;

    return AddParameter(strName, nType, nDirection, lSize, vtValue, nPrecision, nScale);
}

BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, CString strValue)
{
    _variant_t vtValue;

    vtValue.vt = VT_BSTR;
    vtValue.bstrVal = strValue.AllocSysString();//del by zyw
    //vtValue.bstrVal = (_bstr_t)strValue;//add by zyw  2005.1.18

    return AddParameter(strName, nType, nDirection, lSize, vtValue);
}

BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, COleDateTime time)
{

    _variant_t vtValue;

    vtValue.vt = VT_DATE;
    vtValue.date = time;

    return AddParameter(strName, nType, nDirection, lSize, vtValue);
}


BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, _variant_t vtValue, int nPrecision, int nScale)
{
    try
    {
        //_ParameterPtr pParam = m_pCommand->CreateParameter(strName.AllocSysString(), (ADODB::DataTypeEnum)nType, (ADODB::ParameterDirectionEnum)nDirection, lSize, vtValue);//del by zyw
        _ParameterPtr pParam = m_pCommand->CreateParameter((_bstr_t)strName, (ADODB::DataTypeEnum)nType, (ADODB::ParameterDirectionEnum)nDirection, lSize, vtValue);//add by zyw  2005.1.18
        pParam->PutPrecision(nPrecision);
        pParam->PutNumericScale(nScale);
        m_pCommand->Parameters->Append(pParam);

        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}


void CADOCommand::SetText(CString strCommandText)
{
    ATLASSERT(!strCommandText.IsEmpty());

    m_strCommandText = strCommandText;
    //m_pCommand->CommandText = m_strCommandText.AllocSysString();//del by zyw
    m_pCommand->CommandText = (_bstr_t) m_strCommandText; //add by zyw  2005.1.18
}

void CADOCommand::SetType(int nCommandType)
{
    m_nCommandType = nCommandType;
    m_pCommand->CommandType = (CommandTypeEnum)m_nCommandType;
}

BOOL CADOCommand::Execute()
{
    _variant_t vRecords;
    m_nRecordsAffected = 0;
    try
    {
        m_pCommand->Execute(&vRecords, NULL, adCmdStoredProc);
        m_nRecordsAffected = vRecords.iVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

void CADOCommand::dump_com_error(_com_error& e)
{
    CString ErrorStr;


    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());
    ErrorStr.Format(_T("CADOCommand Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
                    e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription);
    m_strLastError = ErrorStr;
    m_dwLastError = e.Error();
    CADOHelper::SendErrorMsg(m_pCommand->ActiveConnection);
#ifdef _DEBUG
    ATLTRACE(ErrorStr);
#endif
}


///////////////////////////////////////////////////////
//
// CADOParameter Class
//

CADOParameter::CADOParameter(int nType, long lSize, int nDirection, CString strName)
{
    m_pParameter = NULL;
    m_pParameter.CreateInstance(__uuidof(Parameter));
    m_strName = _T("");
    m_pParameter->Direction = (ADODB::ParameterDirectionEnum)nDirection;
    m_strName = strName;
    //m_pParameter->Name = m_strName.AllocSysString();//del by zyw
    m_pParameter->Name = (_bstr_t) m_strName; //add by zyw  2005.1.18
    m_pParameter->Type = (ADODB::DataTypeEnum)nType;
    m_pParameter->Size = lSize;
    m_nType = nType;
}

BOOL CADOParameter::SetValue(int nValue)
{
    _variant_t vtVal;

    ATLASSERT(m_pParameter != NULL);

    vtVal.vt = VT_I2;
    vtVal.iVal = nValue;

    try
    {
        if(m_pParameter->Size == 0)
        {
            m_pParameter->Size = sizeof(int);
        }

        m_pParameter->Value = vtVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}


BOOL CADOParameter::SetValue(long lValue)
{
    _variant_t vtVal;

    ATLASSERT(m_pParameter != NULL);

    vtVal.vt = VT_I4;
    vtVal.lVal = lValue;

    try
    {
        if(m_pParameter->Size == 0)
        {
            m_pParameter->Size = sizeof(long);
        }

        m_pParameter->Value = vtVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOParameter::SetValue(double dblValue)
{
    _variant_t vtVal;

    ATLASSERT(m_pParameter != NULL);

    vtVal.vt = VT_R8;
    vtVal.dblVal = dblValue;

    try
    {
        if(m_pParameter->Size == 0)
        {
            m_pParameter->Size = sizeof(double);
        }

        m_pParameter->Value = vtVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOParameter::SetValue(CString strValue)
{
    _variant_t vtVal;

    ATLASSERT(m_pParameter != NULL);

    if(!strValue.IsEmpty())
    {
        vtVal.vt = VT_BSTR;
    }
    else
    {
        vtVal.vt = VT_NULL;
    }

    //Corrected by Giles Forster 10/03/2001
    vtVal.bstrVal = strValue.AllocSysString();//del by zyw
    //vtVal.bstrVal = (_bstr_t)strValue;//add by zyw  2005.1.18

    try
    {
        if(m_pParameter->Size == 0)
        {
            m_pParameter->Size = sizeof(TCHAR) * strValue.GetLength();
        }

        m_pParameter->Value = vtVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOParameter::SetValue(COleDateTime time)
{
    _variant_t vtVal;

    ATLASSERT(m_pParameter != NULL);

    vtVal.vt = VT_DATE;
    vtVal.date = time;

    try
    {
        if(m_pParameter->Size == 0)
        {
            m_pParameter->Size = sizeof(DATE);
        }

        m_pParameter->Value = vtVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOParameter::SetValue(_variant_t vtValue)
{

    ATLASSERT(m_pParameter != NULL);

    try
    {
        if(m_pParameter->Size == 0)
        {
            m_pParameter->Size = sizeof(VARIANT);
        }

        m_pParameter->Value = vtValue;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOParameter::GetValue(int& nValue)
{
    _variant_t vtVal;
    int nVal = 0;

    try
    {
        vtVal = m_pParameter->Value;

        switch(vtVal.vt)
        {
            case VT_BOOL:
                nVal = vtVal.boolVal;
                break;
            case VT_I2:
            case VT_UI1:
                nVal = vtVal.iVal;
                break;
            case VT_INT:
                nVal = vtVal.intVal;
                break;
            case VT_NULL:
            case VT_EMPTY:
                nVal = 0;
                break;
            default:
                nVal = vtVal.iVal;
        }
        nValue = nVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOParameter::GetValue(long& lValue)
{
    _variant_t vtVal;
    long lVal = 0;

    try
    {
        vtVal = m_pParameter->Value;
        if(vtVal.vt != VT_NULL && vtVal.vt != VT_EMPTY)
        {
            lVal = vtVal.lVal;
        }
        lValue = lVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOParameter::GetValue(double& dbValue)
{
    _variant_t vtVal;
    double dblVal;
    try
    {
        vtVal = m_pParameter->Value;
        switch(vtVal.vt)
        {
            case VT_R4:
                dblVal = vtVal.fltVal;
                break;
            case VT_R8:
                dblVal = vtVal.dblVal;
                break;
            case VT_DECIMAL:
                //Corrected by Jos?Carlos Mart韓ez Gal醤
                //dblVal = vtVal.decVal.Lo32;
                //dblVal *= (vtVal.decVal.sign == 128)? -1 : 1;
                //dblVal /= pow(10, vtVal.decVal.scale);
                dblVal = (double)vtVal;

                break;
            case VT_UI1:
                dblVal = vtVal.iVal;
                break;
            case VT_I2:
            case VT_I4:
                dblVal = vtVal.lVal;
                break;
            case VT_INT:
                dblVal = vtVal.intVal;
                break;
            case VT_NULL:
            case VT_EMPTY:
                dblVal = 0;
                break;
            default:
                dblVal = 0;
        }
        dbValue = dblVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOParameter::GetValue(CString& strValue, CString strDateFormat)
{
    _variant_t vtVal;
    CString strVal = _T("");

    try
    {
        vtVal = m_pParameter->Value;
        switch(vtVal.vt)
        {
            case VT_R4:
                strVal = CADOHelper::DblToStr(vtVal.fltVal);
                break;
            case VT_R8:
                strVal = CADOHelper::DblToStr(vtVal.dblVal);
                break;
            case VT_BSTR:
                strVal = vtVal.bstrVal;
                break;
            case VT_I2:
            case VT_UI1:
                strVal = CADOHelper::IntToStr(vtVal.iVal);
                break;
            case VT_INT:
                strVal = CADOHelper::IntToStr(vtVal.intVal);
                break;
            case VT_I4:
                strVal = CADOHelper::LongToStr(vtVal.lVal);
                break;
            case VT_DECIMAL:
            {
                //Corrected by Jos?Carlos Mart韓ez Gal醤
                //double val = vtVal.decVal.Lo32;
                //val *= (vtVal.decVal.sign == 128)? -1 : 1;
                //val /= pow(10, vtVal.decVal.scale);
                double val = (double)vtVal;
                strVal = CADOHelper::DblToStr(val);
            }
            break;
            case VT_DATE:
            {
                COleDateTime dt(vtVal);

                if(strDateFormat.IsEmpty())
                {
                    strDateFormat = _T("%Y-%m-%d %H:%M:%S");
                }
                //strVal = dt.Format(strDateFormat);
                strVal.Format(_T("%.4d-%.2d-%.2d %.2d-%.2d-%.2d"), dt.GetYear(), dt.GetMonth(), dt.GetDay(), \
                              dt.GetHour(), dt.GetMinute(), dt.GetSecond());
            }
            break;
            case VT_EMPTY:
            case VT_NULL:
                strVal.Empty();
                break;
            default:
                strVal.Empty();
                return FALSE;
        }
        strValue = strVal;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOParameter::GetValue(COleDateTime& time)
{
    _variant_t vtVal;

    try
    {
        vtVal = m_pParameter->Value;
        switch(vtVal.vt)
        {
            case VT_DATE:
            {
                COleDateTime dt(vtVal);
                time = dt;
            }
            break;
            case VT_EMPTY:
            case VT_NULL:
                time.SetStatus(COleDateTime::null);
                break;
            default:
                return FALSE;
        }
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}

BOOL CADOParameter::GetValue(_variant_t& vtValue)
{
    try
    {
        vtValue = m_pParameter->Value;
        return TRUE;
    }
    catch(_com_error& e)
    {
        dump_com_error(e);
        return FALSE;
    }
}


void CADOParameter::dump_com_error(_com_error& e)
{
    CString ErrorStr;


    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());
    ErrorStr.Format(_T("CADOParameter Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
                    e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription);
    m_strLastError = ErrorStr;
    m_dwLastError = e.Error();
#ifdef _DEBUG
    ATLTRACE(ErrorStr);
#endif
}

//IMPLEMENT_DYNAMIC(CADOException, CException)

//CADOException::CADOException(int nCause, CString strErrorString) : CException(TRUE)
//{
//	m_nCause = nCause;
//	m_strErrorString = strErrorString;
//}
//
//CADOException::~CADOException()
//{
//
//}
//
//int CADOException::GetError(int nADOError)
//{
//	switch (nADOError)
//	{
//	case noError:
//		return CADOException::noError;
//		break;
//	default:
//		return CADOException::Unknown;
//	}
//
//}
//
CString CADOHelper::IntToStr(int nVal)
{
    CString strRet;
    TCHAR buff[10];

    _itot(nVal, buff, 10);
    strRet = buff;
    return strRet;
}

CString CADOHelper::LongToStr(long lVal)
{
    CString strRet;
    TCHAR buff[20];

    _ltot(lVal, buff, 10);
    strRet = buff;
    return strRet;
}

CString CADOHelper::ULongToStr(unsigned long ulVal)
{
    CString strRet;
    TCHAR buff[20];

    _ultot(ulVal, buff, 10);
    strRet = buff;
    return strRet;

}


CString CADOHelper::DblToStr(float fltVal)
{
    CString strRet = _T("");
    strRet.Format(_T("%g"), fltVal);
    return strRet;
}

//void CADOHelper::ThrowADOException(int nADOError, CString strErrorString)
//{
//	throw new CADOException(nADOError, strErrorString);
//}
//

//检测错误发生时连接的状态Add by Hury
void CADOHelper::SendErrorMsg(_ConnectionPtr pConnection)
{
    //Define Other Variables
    if(NULL == pConnection)
    {
        return;
    }
    _bstr_t  strError;
    ErrorPtr  pErr = NULL;
    CString ErrorStr , strItem , strTmp , strSqlState;
    BOOL bCommuFail = FALSE;
    try
    {
        // Enumerate Errors collection and display
        // properties of each Error object.
        long nCount = pConnection->Errors->Count;

        // Collection ranges from 0 to nCount - 1.
        for(long i = 0; i < nCount; i++)
        {
            pErr = pConnection->Errors->GetItem(i);
            strTmp.Format(_T("Error #%d\n"), pErr->Number);
            strItem += strTmp;
            strTmp.Format(_T(" %s\n"), (LPCSTR)pErr->Description);
            strItem += strTmp;
            strTmp.Format(_T(" (Source: %s)\n"), (LPCSTR)pErr->Source);
            strItem += strTmp;
            strTmp.Format(_T(" (SQL State: %s)\n"), (LPCSTR)pErr->SQLState);
            strSqlState.Format(_T("%s"), (LPCSTR)pErr->SQLState);
            if(strSqlState.Left(2).CompareNoCase(_T("08")) == 0)
            {
                bCommuFail		= TRUE;
            }

            strItem += strTmp;
            strTmp.Format(_T(" (NativeError: %d)\n"), pErr->NativeError);
            strItem += strTmp;
            if((LPCSTR)pErr->GetHelpFile() == NULL)
            {
                strTmp.Format(_T("\tNo Help file available\n"));
                strItem += strTmp;
            }
            else
            {
                strTmp.Format(_T("\t(HelpFile: %s\n)") , pErr->HelpFile);
                strItem += strTmp;
                strTmp.Format(_T("\t(HelpContext: %s\n)") , pErr->HelpContext);
                strItem += strTmp;
            }
            ErrorStr += strItem + _T("\r\n");
        }
#ifdef _DEBUG
        ATLTRACE(ErrorStr);
#endif

        if(bCommuFail)
        {
//			CWnd *pMainWnd = AfxGetMainWnd();
//			pMainWnd ? pMainWnd->PostMessage(WM_ADOMSG, 0, 0) :0 ;
        }
    }
    catch(...)
    {

    }
}


VARTYPE CADOHelper::GetVarType(ADODB::DataTypeEnum adoType)
{
    switch(adoType)
    {
        case ADODB::adEmpty:
            return VT_EMPTY;
        case ADODB::adBigInt:
            return VT_I4 ;
        case ADODB::adUnsignedBigInt:
            return VT_I4 ;
        case adSingle :
            return VT_R8 ;
        case adCurrency :
            return VT_EMPTY ;
        case ADODB::adDecimal :
            // ATLASSERT(FALSE);
            return VT_R8 ;
        case ADODB::adNumeric :
            return VT_R8 ;
        case ADODB::adBoolean :
            return VT_I4 ;
        case ADODB::adError :
            return VT_EMPTY ;
        case ADODB::adUserDefined :
            return VT_EMPTY ;
        case adVariant :
            return VT_VARIANT ;
        case adIDispatch :
            return VT_EMPTY ;
        case ADODB::adIUnknown :
            return VT_EMPTY ;
        case adGUID :
            return VT_EMPTY ;
        case ADODB::adBSTR :
            return VT_BSTR ;
        case ADODB::adTinyInt :
            return VT_I1 ;
        case ADODB::adSmallInt :
            return VT_I2 ;
        case ADODB::adUnsignedTinyInt :
            return VT_UI1 ;
        case ADODB::adUnsignedSmallInt :
            return VT_UI2 ;
        case ADODB::adUnsignedInt :
            return VT_I4 ;
        case ADODB::adDouble :
            return VT_R8 ;
        case ADODB::adDate :
            return VT_DATE ;
        case adDBDate :
            return VT_DATE ;
        case ADODB::adDBTime :
            return VT_DATE ;
        case ADODB::adDBTimeStamp :
            return VT_DATE ;
        case ADODB::adChar :
            return VT_BSTR ;
        case ADODB::adVarChar :
            return VT_BSTR ;
        case ADODB::adLongVarChar :
            return VT_BSTR ;
        case ADODB::adWChar :
            return VT_BSTR ;
        case ADODB::adVarWChar :
            return VT_BSTR ;
        case ADODB::adLongVarWChar :
            return VT_BSTR ;
        case ADODB::adInteger :
            return VT_I4 ;
        case ADODB::adBinary :
            return VT_ARRAY | VT_UI1 ;
        case ADODB::adVarBinary :
            return VT_ARRAY | VT_UI1 ;
        case ADODB::adLongVarBinary :
            return VT_ARRAY | VT_UI1 ;
        case ADODB::adChapter :
            return VT_EMPTY ;
        case ADODB::adFileTime :
            return VT_EMPTY ;
        case ADODB::adPropVariant :
            return VT_EMPTY ;
        case ADODB::adVarNumeric :
            return VT_R8 ;
    }

    ATLASSERT(FALSE);

    return VT_EMPTY;
}


void CADOHelper::PutRsItem(_RecordsetPtr  rs, CString fieldName, CString fieldValue)
{
    FieldPtr field = rs->GetFields()->GetItem((LPCTSTR)fieldName);
    ADODB::DataTypeEnum dtType = field->GetType(); //字段的数据类型

    VARTYPE vtType = GetVarType(dtType);

    _variant_t  vt;

    Convert::StrToVar(fieldValue, &vt, vtType);

    _variant_t vtf = (_bstr_t)(LPCTSTR)fieldName;
    rs->Fields->GetItem(vtf)->PutValue(vt);//vtf

}

void CADOHelper::PutRsItem(_RecordsetPtr  rs, CString fieldName, int fieldValue)
{

    FieldPtr field = rs->GetFields()->GetItem((LPCTSTR)fieldName);
    ADODB::DataTypeEnum dtType = field->GetType(); //字段数据类型
    VARTYPE vtType = GetVarType(dtType);

    _variant_t vt;
    CString ss = Convert::NumToStr(fieldValue);
    Convert::StrToVar(ss, &vt, vtType);
    _variant_t vtf = (_bstr_t)(LPCTSTR)fieldName;
    rs->Fields->GetItem(vtf)->PutValue(vt);

}

void CADOHelper::PutRsItem(_RecordsetPtr  rs, CString fieldName, double fieldValue)
{
    FieldPtr field = rs->GetFields()->GetItem((LPCTSTR)fieldName);
    ADODB::DataTypeEnum dtType = field->GetType(); //字段数据类型
    VARTYPE vtType = GetVarType(dtType);
    _variant_t vt;
    _variant_t t = fieldValue;
    CString fvalue;
    Convert::VarToStr(t, fvalue);
    Convert::StrToVar(fvalue, &vt, vtType);
    _variant_t vtf = (_bstr_t)(LPCTSTR)fieldName;
    rs->Fields->GetItem(vtf)->PutValue(vt);
}


void CADOHelper::PutFieldValue(FieldPtr field, const _variant_t& vValue)
{
    ADODB::DataTypeEnum dtType = field->GetType(); //字段的数据类型

    VARTYPE vtType = GetVarType(dtType);

    _variant_t  vt;

    if(vtType == vValue.vt)
    {
        vt = vValue;
    }
    else
    {
        CString str;
        Convert::VarToStr(vValue, str);
        Convert::StrToVar(str, &vt, vtType);
    }

    field->PutValue(vt);

    return ;
}



#pragma warning( pop )



