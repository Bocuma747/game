#pragma once

class CMomentumPlayer;

struct FrameOfTASData
{
    FrameOfTASData();

    FrameOfTASData(const QAngle &angViewAngles, const Vector &vecPosition, const Vector &vecViewOffset,
                   const Vector &vecAbsVelocity, int &iButtons);

    ~FrameOfTASData();

    // In reality there should be far more data to have in history:
    // Wich is basically everything that gamemovement is using, so whole player, but it takes too much bytes I guess.
    // But hopefully those should be accurate enough.
    // MOM_TODO: Take all the needed variables from player entity to make the exact same movement later so it can
    // re-simulate properly previous stuff. 10000 (10000~ bytes for only recording one frame of CMovementPlayer) * 100
    // (100 tickrate) * 60 (60 seconds) * 120 (2h hours) = 7.2gb for 2h of recording for one player. Multiply it by the
    // number of players (64) and bye game. We can't do that.

    QAngle m_angViewAngles;
    Vector m_vecPosition;
    Vector m_vecViewOffset;
    Vector m_vecAbsVelocity;
    // MOM_TODO: Buttons for stats
    int m_iButtons;
};

class CTASRecording
{
  public:
    CTASRecording(CMomentumPlayer *pPlayer);

    ~CTASRecording();

    void Reset();

    void Think();

    void Start();

    void Stop();

    void Erase(int iFrames);

    FrameOfTASData *GetFrame(int iFrame);

    FrameOfTASData *GetCurFrame();

    // MOM_TODO: Looks unsmooth for angles, to interpolate angles we could use InterpolateAngles with gpGlobals->interpolation_amount.
    template <bool bUseCurrentFrame = true, bool bSetOnlyOnceAnglesThisFrame = true> void SetFrame(int iFrame = 0)
    {
        if (bUseCurrentFrame)
            iFrame = m_iChosenFrame;

        auto chosenFrame = GetFrame(iFrame);
        m_pPlayer->SetAbsOrigin(chosenFrame->m_vecPosition);

        if (bSetOnlyOnceAnglesThisFrame)
        {
            if ( m_iSetFrame != iFrame )
            {
                m_pPlayer->SnapEyeAngles(chosenFrame->m_angViewAngles);
                m_pPlayer->SetLocalAngles(chosenFrame->m_angViewAngles);
            }
        }
        else
        {
            m_pPlayer->SnapEyeAngles(chosenFrame->m_angViewAngles);
            m_pPlayer->SetLocalAngles(chosenFrame->m_angViewAngles);
        }

        m_pPlayer->SetAbsVelocity(chosenFrame->m_vecAbsVelocity);
        m_pPlayer->SetViewOffset(chosenFrame->m_vecViewOffset);

        m_iSetFrame = iFrame;
    }

    int NumberOfFrames();

    CUtlVector<FrameOfTASData> m_vecTASData;
    CMomentumPlayer *m_pPlayer;
    eStatusOfTAS m_Status, m_OldStatus;
    int m_iChosenFrame, m_iPauseTickCount, m_iFramesElapsed, m_iSetFrame;
    float m_flTimeScale;
};
