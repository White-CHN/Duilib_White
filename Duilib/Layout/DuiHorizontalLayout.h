#pragma once

namespace DuiLib {

	class DUILIB_API CDuiHorizontalLayout
		: public CDuiContainer
	{
		DECLARE_DUICONTROL(CDuiHorizontalLayout)
	public:
		CDuiHorizontalLayout(void);
		virtual ~CDuiHorizontalLayout(void);

	public:
		virtual LPCTSTR GetClass() const;
		virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);
	};

}