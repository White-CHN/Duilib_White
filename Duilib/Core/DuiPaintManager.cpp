#include "StdAfx.h"
#include "DuiPaintManager.h"

namespace DuiLib {

	HINSTANCE CDuiPaintManager::m_hInstance = NULL;

	int CDuiPaintManager::m_iResourceType = DUILIB_FILE;
	CDuiString CDuiPaintManager::m_pStrResourcePath = _T("");

	CDuiPaintManager::CDuiPaintManager(void)
	{

	}


	CDuiPaintManager::~CDuiPaintManager(void)
	{
	}


	HINSTANCE CDuiPaintManager::GetInstance()
	{
		return m_hInstance;
	}

	void CDuiPaintManager::SetInstance(HINSTANCE hInstance)
	{
		m_hInstance = hInstance;
	}

	DuiLib::CDuiString CDuiPaintManager::GetInstancePath()
	{
		if( m_hInstance == NULL ) 
		{
			return _T('\0');
		}

		TCHAR tszModule[MAX_PATH + 1] = { 0 };
		::GetModuleFileName(m_hInstance, tszModule, MAX_PATH);
		CDuiString strInstancePath = tszModule;
		int pos = strInstancePath.ReverseFind(_T('\\'));
		if( pos >= 0 ) 
		{
			strInstancePath = strInstancePath.Left(pos + 1);
		}
		return strInstancePath;
	}

	int CDuiPaintManager::GetResourceType()
	{
		return m_iResourceType;
	}

	void CDuiPaintManager::SetResourceType(int iResourceType)
	{
		m_iResourceType = iResourceType;
	}

	void CDuiPaintManager::SetResourcePath(LPCTSTR pStrPath)
	{
		m_pStrResourcePath = pStrPath;
		if( m_pStrResourcePath.IsEmpty()) 
		{
			return;
		}
		TCHAR cEnd = m_pStrResourcePath.GetAt(m_pStrResourcePath.GetLength() - 1);
		if( cEnd != _T('\\') && cEnd != _T('/') ) 
		{
			m_pStrResourcePath += _T('\\');
		}
	}

}