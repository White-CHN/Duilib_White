#pragma once


namespace DuiLib {

	class CDuiMarkup;

	class DUILIB_API CDuiMarkupNode
	{
		friend class CDuiMarkup;
	public:
		CDuiMarkupNode();
		CDuiMarkupNode(CDuiMarkup* pOwner, int iPos);
		~CDuiMarkupNode(void);
	public:
		BOOL IsValid() const;

		CDuiMarkupNode GetParent();
		CDuiMarkupNode GetSibling();
		CDuiMarkupNode GetChild();
		CDuiMarkupNode GetChild(LPCTSTR pstrName);

		BOOL HasSiblings() const;
		BOOL HasChildren() const;
		LPCTSTR GetName() const;
		LPCTSTR GetValue() const;

		BOOL HasAttributes();
		BOOL HasAttribute(LPCTSTR pstrName);
		int GetAttributeCount();
		LPCTSTR GetAttributeName(int iIndex);
		LPCTSTR GetAttributeValue(int iIndex);
		LPCTSTR GetAttributeValue(LPCTSTR pstrName);
		BOOL GetAttributeValue(int iIndex, LPTSTR pstrValue, SIZE_T cchMax);
		BOOL GetAttributeValue(LPCTSTR pstrName, LPTSTR pstrValue, SIZE_T cchMax);
	private:
		void _MapAttributes();
	private:
		enum { MAX_XML_ATTRIBUTES = 64 };
	private:
		typedef struct
		{
			ULONG iName;
			ULONG iValue;
		} XMLATTRIBUTE;
	private:
		int m_iPos;
		int m_nAttributes;
		XMLATTRIBUTE m_aAttributes[MAX_XML_ATTRIBUTES];
		CDuiMarkup* m_pOwner;
	};

}

