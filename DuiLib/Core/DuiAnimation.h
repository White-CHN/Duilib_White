#pragma once
namespace DuiLib
{
    class DUILIB_API CAnimationData
    {
    public:
        CAnimationData(int nElipse, int nFrame, int nID, BOOL bLoop);
    public:
        int m_nAnimationID;
        int m_nElapse;

        int m_nTotalFrame;
        int m_nCurFrame;

        BOOL m_bLoop;
        BOOL m_bFirstLoop;
    };

    class DUILIB_API IAnimation
    {
    public:
        IAnimation(CDuiControl* pOwner);
        virtual ~IAnimation(void);
    public:
        BOOL StartAnimation(int nElapse, int nTotalFrame, int nAnimationID = 0, BOOL bLoop = FALSE);
        void StopAnimation(int nAnimationID = 0);
        BOOL IsAnimationRunning(int nAnimationID);
        int GetCurrentFrame(int nAnimationID = 0);
        BOOL SetCurrentFrame(int nFrame, int nAnimationID = 0);

        virtual void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID);
        virtual void OnAnimationStart(int nAnimationID, BOOL bFirstLoop);
        virtual void OnAnimationStop(int nAnimationID);

        void OnAnimationElapse(int nAnimationID);
    protected:
        CAnimationData* GetAnimationDataByID(int nAnimationID);
    private:
        CDuiControl* m_pControl;
        vector<CAnimationData*> m_arAnimations;
    };

}

