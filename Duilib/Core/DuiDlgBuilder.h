#pragma once

namespace DuiLib {


	class IDialogBuilderCallback
	{
	public:
		virtual CDuiControl* CreateControl(LPCTSTR pstrClass) = 0;
	};

	class DUILIB_API CDuiDlgBuilder
	{
	public:
		CDuiDlgBuilder(void);
		~CDuiDlgBuilder(void);
	public:
		CDuiControl* Create(STRINGorID xml, LPCTSTR type = NULL, IDialogBuilderCallback* pCallback = NULL,
			CDuiPaintManager* pManager = NULL, CDuiControl* pParent = NULL);

		void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;
		void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;
	private:
		CDuiControl* Create(CDuiControl* pParent);
		CDuiControl* Parse(CDuiMarkupNode* pRoot,CDuiControl* pParent);
	private:
		CDuiPaintManager* m_pManager;
		IDialogBuilderCallback* m_pCallback;
		
		LPCTSTR m_pstrtype;
		CDuiMarkup m_xml;
	};



}