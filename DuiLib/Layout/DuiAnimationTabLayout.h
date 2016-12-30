#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiAnimationTabLayout
        : public CDuiTabLayout
        , public IAnimation
    {
        DECLARE_DUICONTROL(CDuiAnimationTabLayout)
    public:
        CDuiAnimationTabLayout(void);
        virtual ~CDuiAnimationTabLayout(void);
    public:
        virtual CDuiString GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);

        void AnimationSwitch();
        virtual BOOL SelectItem(int iIndex);

        virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop);
        virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
        virtual void OnAnimationStop(INT nAnimationID);

        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        virtual void DoEvent(TEventUI& event);
    private:
        enum
        {
            TAB_ANIMATION_ELLAPSE = 10,
            TAB_ANIMATION_FRAME_COUNT = 15,
        };
    private:
        BOOL m_bIsVerticalDirection;
        BOOL m_bControlVisibleFlag;
        int m_nPositiveDirection;
        CDuiControl* m_pCurrentControl;
        CDuiRect m_rcCurPos;
        CDuiRect m_rcItemOld;

    };


}
