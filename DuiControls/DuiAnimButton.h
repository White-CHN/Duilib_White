#pragma once

#define  DUI_CTR_ANIMBUTTON                          (_T("AnimButton"))
#define TIMER_ID_ANIMBUTTON_IN		TIMER_ID_USER + 1
#define TIMER_ID_ANIMBUTTON_OUT		TIMER_ID_USER + 2

class CDuiAnimButton
    : public CDuiButton
    , public IAnimation
{
    DECLARE_DUICONTROL(CDuiAnimButton)
public:
    CDuiAnimButton(void);
    virtual ~CDuiAnimButton(void);
public:
    CDuiString GetClass() const OVERRIDE;
    LPVOID GetInterface(CDuiString strName) OVERRIDE;

    int GetEllapse() const;
    void SetEllapse(int nEllapse);

    BOOL AddImage(CDuiString strImage);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
    void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) OVERRIDE;

    void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;
    void DoEvent(CDuiEvent& event) OVERRIDE;
private:
    enum
    {
        ANIMATION_ELLAPSE		= 60,
    };
private:
    int m_nFramePosition;			// 当前放到第几帧
    int m_nEllapse;
    vector<CDuiString> m_vtImages;
};
