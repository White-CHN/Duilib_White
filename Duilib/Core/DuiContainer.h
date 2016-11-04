#pragma once

namespace DuiLib {

	class IContainer
	{
	public:
		virtual CDuiControl* GetItemAt(int iIndex) const = 0;
		virtual int GetItemIndex(CDuiControl* pControl) const  = 0;
		virtual BOOL SetItemIndex(CDuiControl* pControl, int iIndex)  = 0;
		virtual int GetCount() const = 0;
		virtual BOOL Add(CDuiControl* pControl) = 0;
		virtual BOOL AddAt(CDuiControl* pControl, int iIndex)  = 0;
		virtual BOOL Remove(CDuiControl* pControl) = 0;
		virtual BOOL RemoveAt(int iIndex)  = 0;
		virtual void RemoveAll() = 0;
	};

	class DUILIB_API CDuiContainer
		: public CDuiControl
		, public IContainer
	{
		DECLARE_DUICONTROL(CDuiContainer)
	public:
		CDuiContainer(void);
		virtual ~CDuiContainer(void);
	public:

		virtual LPVOID GetInterface(LPCTSTR pstrName);

		virtual LPCTSTR GetClass() const;

		virtual RECT GetInset() const;

		virtual UINT GetChildAlign() const;

		virtual UINT GetChildVAlign() const;

		virtual CDuiControl* GetItemAt(int iIndex) const;
		virtual int GetItemIndex(CDuiControl* pControl) const ;
		virtual BOOL SetItemIndex(CDuiControl* pControl, int iIndex);
		virtual int GetCount() const;
		virtual BOOL Add(CDuiControl* pControl);
		virtual BOOL AddAt(CDuiControl* pControl, int iIndex);
		virtual BOOL Remove(CDuiControl* pControl);
		virtual BOOL RemoveAt(int iIndex);
		virtual void RemoveAll();

		virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);
		virtual void DoPaint(HDC hDC, const RECT& rcPaint);
	protected:
		virtual void SetFloatPos(int iIndex);
	protected:
		BOOL m_bAutoDestroy;
		BOOL m_bDelayedDestroy;

		int m_iChildPadding;

		UINT m_iChildAlign;
		UINT m_iChildVAlign;

		RECT m_rcInset;

		CStdPtrArray m_items;
	};


}

