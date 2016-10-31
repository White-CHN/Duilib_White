#include "StdAfx.h"
#include "DuiUtils.h"

namespace DuiLib {


	CDuiConsole::CDuiConsole()
	{
#ifdef _DEBUG
		AllocConsole();
		freopen("CONOUT$", "w+t", stdout);
#endif // _DEBUG
	}

	CDuiConsole::~CDuiConsole()
	{
#ifdef _DEBUG
		FreeConsole();
#endif // _DEBUG
	}

}