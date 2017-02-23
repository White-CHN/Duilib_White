#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiRing
        : public CDuiLabel
    {
        DECLARE_DUICONTROL(CDuiRing)
    public:
        CDuiRing(void);
        virtual ~CDuiRing(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        void SetBkImage(LPCTSTR pStrImage) OVERRIDE;

        void PaintBkImage(HDC hDC) OVERRIDE;
        void SetVisible(BOOL bVisible = TRUE) OVERRIDE;
        void SetInternVisible(BOOL bVisible = TRUE) OVERRIDE;
        void DoInit() OVERRIDE;
        void DoEvent(CDuiEvent& event) OVERRIDE;
    private:
        void PlayImage();
        void StopImage();
        void InitImage();
        void DeleteImage();
    private:
        float m_fCurAngle;
        Gdiplus::Image* m_pBkimage;
    };

}