#include "StdAfx.h"
#include "DuiPaintManager.h"

namespace DuiLib {

	HINSTANCE CDuiPaintManager::m_hInstance = NULL;
	int CDuiPaintManager::m_iResourceType = DUILIB_FILE;

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

	int CDuiPaintManager::GetResourceType()
	{
		return m_iResourceType;
	}

	void CDuiPaintManager::SetResourceType(int iResourceType)
	{
		m_iResourceType = iResourceType;
	}

}