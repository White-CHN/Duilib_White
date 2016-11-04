#pragma once
class CDemoFrame : public CDuiDlgImplBase
{
public:
	CDemoFrame(void);
	virtual ~CDemoFrame(void);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName() const;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage(HWND hWnd);

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;

	virtual CDuiControl* CreateControl(LPCTSTR pstrClass);

	virtual void Notify(TNotifyUI& msg) ;
};

