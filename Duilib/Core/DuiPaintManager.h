#pragma once

namespace DuiLib {

	enum DUILIB_RESTYPE
	{
		DUILIB_FILE		=  1,		// ���Դ����ļ�
		DUILIB_ZIP			,		// ���Դ���zipѹ����
		DUILIB_RESOURCE		,		// ������Դ
		DUILIB_ZIPRESOURCE	,		// ������Դ��zipѹ����
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
