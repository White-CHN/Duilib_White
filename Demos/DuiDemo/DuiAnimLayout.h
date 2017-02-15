#pragma once


#define DUI_CTR_ANIMLAYOUT		(_T("AnimLayout"))

class CDuiAnimLayout
    : public CDuiVerticalLayout
    , public IUIEffectCallBack
{
public:
    CDuiAnimLayout(void);
    virtual ~CDuiAnimLayout(void);
public:
    CDuiString GetClass() const OVERRIDE;
    LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

    void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;

    // 每一 个 动画开始时回调
    void OnUiEffectBegin(WPARAM effectKey, DWORD animaType) OVERRIDE;
    // 每一 个 动画结束时回调
    void OnUiEffectEnd(WPARAM effectKey, DWORD animaType) OVERRIDE;
    // 每一 帧 动画绘制时回调
    void OnUiEffectDraw() OVERRIDE;

    BOOL StartEffect();
private:
    BOOL				m_bPlaying;
    DWORD				m_dwEffectNum;
    HDC					m_hMemDc;
    HBITMAP				m_hOldBitmap;
    HBITMAP				m_hTransBitmap;
    IUIEffect*			m_pEffect;
    CDuiRect			m_rcCustomShadow;		//保存窗体反转前的阴影九宫格

};

