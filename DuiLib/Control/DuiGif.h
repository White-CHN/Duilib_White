#pragma once

namespace DuiLib
{
    class DUILIB_API CDuiGif
        : public CDuiControl
    {
        DECLARE_DUICONTROL(CDuiGif)
    public:
        CDuiGif(void);
        virtual ~CDuiGif(void);
    public:
        LPCTSTR	GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        void PlayGif();
        void PauseGif();
        void StopGif();

        BOOL IsAutoPlay() const;
        void SetAutoPlay(BOOL bIsAuto = TRUE);

        BOOL IsAutoSize() const;
        void SetAutoSize(BOOL bIsAuto = TRUE);

        void SetBkImage(LPCTSTR pStrImage) OVERRIDE;
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        void SetVisible(BOOL bVisible = TRUE) OVERRIDE;
        void SetInternVisible(BOOL bVisible = TRUE) OVERRIDE;

        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;

        void DoInit() OVERRIDE;

        void DoEvent(TEventUI& event) OVERRIDE;
    private:
        void DrawFrame(HDC hDC);		// ����GIFÿ֡
        void InitGifImage();
        void DeleteGif();
    private:
        BOOL m_bIsPlaying;
        BOOL m_bIsAutoPlay;				// �Ƿ��Զ�����gif
        BOOL m_bIsAutoSize;				// �Ƿ��Զ�����ͼƬ���ô�С
        UINT m_nFrameCount;				// gifͼƬ��֡��
        UINT m_nFramePosition;			// ��ǰ�ŵ��ڼ�֡
        Gdiplus::PropertyItem* m_pPropertyItem;	// ֡��֮֡����ʱ��
        Gdiplus::Image*	m_pGifImage;
    };

}