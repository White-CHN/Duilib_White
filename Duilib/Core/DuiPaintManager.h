#pragma once

namespace DuiLib {

	enum DUILIB_RESTYPE
	{
		DUILIB_FILE		=  1,		// 来自磁盘文件
		DUILIB_ZIP			,		// 来自磁盘zip压缩包
		DUILIB_RESOURCE		,		// 来自资源
		DUILIB_ZIPRESOURCE	,		// 来自资源的zip压缩包
	};

	class DUILIB_API CDuiPaintManager
	{
	public:
		CDuiPaintManager(void);
		~CDuiPaintManager(void);
	public:
		static HINSTANCE GetInstance();
		static void SetInstance(HINSTANCE hInstance);
		static CDuiString GetInstancePath();

		static int GetResourceType();
		static void SetResourceType(int iResourceType);
		static void SetResourcePath(LPCTSTR pStrPath);
		

		
	private:
		static HINSTANCE m_hInstance;

		static int m_iResourceType;
		static CDuiString m_pStrResourcePath;
	};

}
