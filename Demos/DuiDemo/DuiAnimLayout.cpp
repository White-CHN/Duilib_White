#include "StdAfx.h"
#include "DuiAnimLayout.h"


CDuiAnimLayout::CDuiAnimLayout(void)
    : m_bPlaying(FALSE)
    , m_dwEffectNum(2)
    , m_hMemDc(NULL)
    , m_hOldBitmap(NULL)
    , m_hTransBitmap(NULL)
    , m_pEffect(NULL)
{
#ifdef _UNICODE
    m_pEffect = GetAnimation();
    ASSERT(m_pEffect);
#endif

}


CDuiAnimLayout::~CDuiAnimLayout(void)
{
    if(m_pEffect)
    {
        m_pEffect->ClearAllAnimation();
#ifdef _UNICODE
        ReleaseAnimation(m_pEffect);
#endif

    }
}

CDuiString CDuiAnimLayout::GetClass() const
{
    return DUI_CTR_ANIMLAYOUT;
}

LPVOID CDuiAnimLayout::GetInterface(CDuiString strName)
{
    if(strName == DUI_CTR_ANIMLAYOUT)
    {
        return static_cast<CDuiAnimLayout*>(this);
    }
    return CDuiVerticalLayout::GetInterface(strName);
}

void CDuiAnimLayout::DoPaint(HDC hDC, const RECT& rcPaint)
{
    if(!m_bPlaying)
    {
        CDuiVerticalLayout::DoPaint(hDC, rcPaint);
        return;
    }
    CDuiRect rc =  GetPos();
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    typedef BOOL(WINAPI * LPALPHABLEND)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
    static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(_T("msimg32.dll")), "AlphaBlend");
    if(lpAlphaBlend)
    {
        lpAlphaBlend(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, m_hMemDc,
                     0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
    }

}

void CDuiAnimLayout::OnUiEffectBegin(WPARAM effectKey, DWORD animaType)
{
    CDuiControl* pCtrl = (CDuiControl*)effectKey;
    if(pCtrl == NULL)
    {
        return;
    }
}

void CDuiAnimLayout::OnUiEffectEnd(WPARAM effectKey, DWORD animaType)
{
    CDuiControl* pCtrl = (CDuiControl*)effectKey;
    if(pCtrl == NULL)
    {
        return;
    }

    m_bPlaying = FALSE;
    SelectObject(m_hMemDc, m_hOldBitmap);
    DeleteObject(m_hTransBitmap);
    DeleteObject(m_hMemDc);

    Invalidate();
}

void CDuiAnimLayout::OnUiEffectDraw()
{
    // �������ǿ���ػ洰�壬�����޷���ʾ���������ܹؼ�
    RedrawWindow(GetManager()->GetPaintWindow(), NULL, NULL,  RDW_INVALIDATE | RDW_UPDATENOW);
}

void RestoreAlphaColor(LPBYTE pBits, int bitsWidth, PRECT rc)
{
    for(int i = rc->top; i < rc->bottom; ++i)
    {
        for(int j = rc->left; j < rc->right; ++j)
        {
            int x = (i * bitsWidth + j) * 4;
            if((pBits[x + 3] == 0) && (pBits[x + 0] != 0 || pBits[x + 1] != 0 || pBits[x + 2] != 0))
            {
                pBits[x + 3] = 255;
            }
        }
    }
}

BOOL CDuiAnimLayout::StartEffect()
{
    if(m_bPlaying || m_pEffect == NULL)
    {
        return FALSE;
    }
    if(m_dwEffectNum > 80)
    {
        m_dwEffectNum = 2;
        return FALSE;
    }
    m_hMemDc = ::CreateCompatibleDC(GetManager()->GetPaintDC());
    m_hTransBitmap = CRenderEngine::GenerateBitmap(GetManager(), this, GetPos());
    if(m_hTransBitmap == NULL)
    {
        return FALSE;
    }
    m_hOldBitmap = (HBITMAP) ::SelectObject(m_hMemDc, m_hTransBitmap);
    BITMAP bmDst;
    GetObject(m_hTransBitmap, sizeof(bmDst), &bmDst);
    SIZE szMemDc = { bmDst.bmWidth, bmDst.bmHeight };

    //�޲�һ��Alphaͨ��,һЩ�ؼ�(Richedit)����AlphaΪ0
    RECT rcRestore = GetPos();
    RestoreAlphaColor((LPBYTE)bmDst.bmBits, bmDst.bmWidth, &rcRestore);

    // ��䶯������
    AnimationParam animParam;
    animParam.effectKey = (WPARAM)this;				//�ؼ�ָ��
    animParam.animationEffect = m_dwEffectNum++;	//�������ͣ���2-80��1Ϊ�Զ��嶯������û����ֲ����
    animParam.animationFrequency = 20;				//�������
    animParam.bShow = TRUE;							//����˳��
    animParam.hBitmap = m_hTransBitmap;
    animParam.pBmpData = (BYTE*)bmDst.bmBits;
    animParam.bmpSize = szMemDc;
    animParam.hdc = m_hMemDc;

    BOOL bRet = m_pEffect->AppendAnimation(animParam);
    ASSERT(bRet);

    m_bPlaying = TRUE;

    // ������ͬ��ִ�еģ�Animation�����ڶ�����Ϻ󷵻�
    bRet = m_pEffect->Animation(dynamic_cast<IUIEffectCallBack*>(this), 0);
    ASSERT(bRet);

    // �ݹ���ʾ���ж���Ч��,��ֻ��Ϊ����ʾЧ��,ʵ�ʿ�����Ҫ������!
    //StartEffect();

    m_pEffect->DependAnimation(animParam.effectKey);

    return TRUE;
}
