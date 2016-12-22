#pragma once
/*********************************************************************
*  ����Ϊduilib�����Ķ�̬��ɫ��ؼ�����ȫģ�¿ṷ��QQ�ĵ�ɫ�幦��
*  ʹ����HSB����ģʽ�����troy�о�����һ���㷨���ƽ���
*  �����ɫ����Դ���ģ�³��ṷ��ɫ�����ʽ�����ǻ��в��
*  �������λ����֪�����õ��㷨���������ƵĴ��룬���ߴ˴�����bug������ϵ��
*  By:Redrain  QQ��491646717   2014.8.19
*  ��������Ƶ����󣬿����޸������
*  thumbimage����ָ��ѡ����ɫ�Ĺ���زĵ�λ��
*  sample:<ColorPalette name="Pallet" width="506" height="220" palletheight="200" barheight="14" padding="8,5,0,0" bkcolor="#FFFFFFFF" thumbimage="UI\skin\cursor.png" />
*
*  ��ģ������troyԭ���Ĵ����������ѡ���֮�������±�д�ģ��޸����㷨����������˶�̬��ɫ������ܣ�֮��troy�ֽ�����һ��bug���ٴθ�л��֮����,troy�Ĵ���
*********************************************************************/
namespace DuiLib
{
    class DUILIB_API CDuiColorPalette
        : public CDuiControl
    {
        DECLARE_DUICONTROL(CDuiColorPalette)
    public:
        CDuiColorPalette(void);
        virtual ~CDuiColorPalette(void);
    public:
        LPCTSTR GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        //��ȡ���ձ�ѡ�����ɫ������ֱ����������duilib����ɫ
        DWORD GetSelectColor();
        void SetSelectColor(DWORD dwColor);

        //����/��ȡ Pallet����ɫ�������棩�ĸ߶�
        void SetPalletHeight(int nHeight);
        int	GetPalletHeight() const;

        //����/��ȡ �·�Bar������ѡ�������ĸ߶�
        void SetBarHeight(int nHeight);
        int GetBarHeight() const;
        //����/��ȡ ѡ��ͼ���·��
        void SetThumbImage(LPCTSTR pszImage);
        CDuiString GetThumbImage() const;

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        void DoInit() OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        virtual void PaintPallet(HDC hDC);
        void PaintBkColor(HDC hDC) OVERRIDE;
        void DoEvent(TEventUI& event) OVERRIDE;
    protected:
        //��������
        void UpdatePalletData();
        void UpdateBarData();
    private:
        HDC	m_MemDc;
        HBITMAP	m_hMemBitmap;
        BOOL m_bIsInBar;
        BOOL m_bIsInPallet;
        int	m_nPalletHeight;
        int	m_nBarHeight;
        int	m_nCurH;
        int	m_nCurS;
        int	m_nCurB;
        UINT m_uButtonState;

        BYTE* m_pBits;

        BITMAP m_bmInfo;

        CDuiPoint m_ptLastPalletMouse;
        CDuiPoint m_ptLastBarMouse;

        CDuiString m_strThumbImage;
    };

}