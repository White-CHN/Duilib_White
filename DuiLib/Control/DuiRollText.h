#pragma once
namespace DuiLib
{
#define ROLLTEXT_LEFT		0
#define ROLLTEXT_RIGHT		1
#define ROLLTEXT_UP 		2
#define ROLLTEXT_DOWN		3

    class DUILIB_API CDuiRollText
        : public CDuiLabel
    {
        DECLARE_DUICONTROL(CDuiRollText);
    public:
        CDuiRollText(void);
        virtual ~CDuiRollText(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        void SetVisible(BOOL bVisible = TRUE) OVERRIDE;
        void SetInternVisible(BOOL bVisible = TRUE) OVERRIDE;

        int GetSpeed() const;
        void SetSpeed(int nSpeed);

        int GetRollDirection() const;
        void SetRollDirection(int nDirection);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void DoInit() OVERRIDE;

        void PaintText(HDC hDC) OVERRIDE;
        void DoEvent(TEventUI& event) OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void SetText(LPCTSTR pstrText) OVERRIDE;
    private:
        int m_nStep;
        int m_nScrollPos;
        int m_nRollDirection;
        int m_nText_W_H;
        int m_nSpeed;
        CDuiRect  rcClient;
    };

}