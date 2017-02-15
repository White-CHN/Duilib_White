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

    // ÿһ �� ������ʼʱ�ص�
    void OnUiEffectBegin(WPARAM effectKey, DWORD animaType) OVERRIDE;
    // ÿһ �� ��������ʱ�ص�
    void OnUiEffectEnd(WPARAM effectKey, DWORD animaType) OVERRIDE;
    // ÿһ ֡ ��������ʱ�ص�
    void OnUiEffectDraw() OVERRIDE;

    BOOL StartEffect();
private:
    BOOL				m_bPlaying;
    DWORD				m_dwEffectNum;
    HDC					m_hMemDc;
    HBITMAP				m_hOldBitmap;
    HBITMAP				m_hTransBitmap;
    IUIEffect*			m_pEffect;
    CDuiRect			m_rcCustomShadow;		//���洰�巴תǰ����Ӱ�Ź���

};

