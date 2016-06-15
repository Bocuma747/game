#include "cbase.h"
#include "hud_mapfinished.h"

#include "tier0/memdbgon.h"

DECLARE_HUDELEMENT_DEPTH(CHudMapFinishedDialog, 70);

//NOTE: The "CHudMapFinishedDialog" (main panel) control settings are found in MapFinishedDialog.res
CHudMapFinishedDialog::CHudMapFinishedDialog(const char *pElementName) : 
CHudElement(pElementName), BaseClass(g_pClientMode->GetViewport(), "CHudMapFinishedDialog")
{
    SetProportional(true);
    SetKeyBoardInputEnabled(false);
    SetMouseInputEnabled(false);
    SetHiddenBits(HIDEHUD_WEAPONSELECTION);

    m_pRunStats = nullptr;
    m_iCurrentPage = 0;

    surface()->CreatePopup(GetVPanel(), false, false, false, false, false);
    
    LoadControlSettings("resource/UI/MapFinishedDialog.res");
    m_pNextZoneButton = FindControl<ImagePanel>("Next_Zone");
    m_pNextZoneButton->SetMouseInputEnabled(true);
    m_pNextZoneButton->InstallMouseHandler(this);
    m_pPrevZoneButton = FindControl<ImagePanel>("Prev_Zone");
    m_pPrevZoneButton->SetMouseInputEnabled(true);
    m_pPrevZoneButton->InstallMouseHandler(this);
    m_pPlayReplayButton = FindControl<ImagePanel>("Replay_Icon");
    m_pPlayReplayButton->SetMouseInputEnabled(true);
    m_pPlayReplayButton->InstallMouseHandler(this);
    m_pRepeatButton = FindControl<ImagePanel>("Repeat_Button");
    m_pRepeatButton->SetMouseInputEnabled(true);
    m_pRepeatButton->InstallMouseHandler(this);
    m_pClosePanelButton = FindControl<ImagePanel>("Close_Panel");
    m_pClosePanelButton->SetMouseInputEnabled(true);
    m_pClosePanelButton->InstallMouseHandler(this);
    m_pPlayReplayLabel = FindControl<Label>("Replay_Label");
    m_pDetachMouseLabel = FindControl<Label>("Detach_Mouse");
    m_pCurrentZoneLabel = FindControl<Label>("Current_Zone");
    m_pZoneOverallTime = FindControl<Label>("Zone_Overall_Time");
    m_pZoneEnterTime = FindControl<Label>("Zone_Enter_Time");
    m_pZoneJumps = FindControl<Label>("Zone_Jumps");
    m_pZoneStrafes = FindControl<Label>("Zone_Strafes");
    m_pZoneVelEnter = FindControl<Label>("Zone_Vel_Enter");
    m_pZoneVelExit = FindControl<Label>("Zone_Vel_Exit");
    m_pZoneVelAvg = FindControl<Label>("Zone_Vel_Avg");
    m_pZoneVelMax = FindControl<Label>("Zone_Vel_Max");
    m_pZoneSync1 = FindControl<Label>("Zone_Sync1");
    m_pZoneSync2 = FindControl<Label>("Zone_Sync2");
    m_pRunSaveStatus = FindControl<Label>("Run_Save_Status");
    m_pRunUploadStatus = FindControl<Label>("Run_Upload_Status");
}

CHudMapFinishedDialog::~CHudMapFinishedDialog()
{
}

bool CHudMapFinishedDialog::ShouldDraw()
{
    return CHudElement::ShouldDraw();
    
    bool shouldDrawLocal = false;
    C_MomentumPlayer *pPlayer = ToCMOMPlayer(CBasePlayer::GetLocalPlayer());
    if (pPlayer)
    {
        if (pPlayer->IsWatchingReplay())
        {
            C_MomentumReplayGhostEntity *pEnt = pPlayer->GetReplayEnt();
            shouldDrawLocal = pEnt && pEnt->m_RunData.m_bMapFinished;
        }
        else
        {
            shouldDrawLocal = pPlayer->m_RunData.m_bMapFinished;
        }
    }

    if (!shouldDrawLocal)
        SetMouseInputEnabled(false);

    return CHudElement::ShouldDraw() && shouldDrawLocal && m_pRunStats;
}


void CHudMapFinishedDialog::ApplySchemeSettings(IScheme *pScheme)
{
    BaseClass::ApplySchemeSettings(pScheme);
    SetBgColor(GetSchemeColor("MOM.Panel.Bg", pScheme));
    m_pDetachMouseLabel->SetFont(m_hTextFont);
    m_pPlayReplayLabel->SetFont(m_hTextFont);
    m_pCurrentZoneLabel->SetFont(m_hTextFont);
}

void CHudMapFinishedDialog::OnMousePressed(MouseCode code)
{
    if (code == MOUSE_LEFT)
    {
        VPANEL over = input()->GetMouseOver();
        if (over == m_pPlayReplayButton->GetVPanel())
        {
            DevLog("Clicked on the replay icon! Starting replay...\n");
            //MOM_TODO: Play the replay
            //engine->ServerCmd()
        }
        else if (over == m_pNextZoneButton->GetVPanel())
        {
            //MOM_TODO (beta+): Play animations?
            m_iCurrentPage = (m_iCurrentPage + 1) % 5;//(g_MOMEventListener->m_iMapZoneCount + 1);
        }
        else if (over == m_pPrevZoneButton->GetVPanel())
        {
            //MOM_TODO: (beta+) play animations?
            int newPage = m_iCurrentPage - 1;
            m_iCurrentPage = newPage < 0 ? /*g_MOMEventListener->m_iMapZoneCount*/4 : newPage;
        }
        else if (over == m_pRepeatButton->GetVPanel())
        {
            //The player either wants to repeat the replay (if spectating), or restart the map (not spec)
        }
        else if (over == m_pClosePanelButton->GetVPanel())
        {
            //This is where we unload comparisons, as well as the ghost if the player was speccing it
            SetMouseInputEnabled(false);
            IGameEvent *pClosePanel = gameeventmanager->CreateEvent("mapfinished_panel_closed");
            if (pClosePanel)
            {
                //Fire this event so other classes can get at this
                gameeventmanager->FireEvent(pClosePanel);
            }
        }

        //MOM_TODO: Other buttons here
    }
    else if (code == MOUSE_RIGHT)
    {
        SetMouseInputEnabled(false);//Lock mouse again
    }
}


void CHudMapFinishedDialog::Init()
{
    Reset();
    //cache localization files

    //Run saving/uploading
    FIND_LOCALIZATION(m_pwRunSavedLabel, "#MOM_MF_RunSaved");
    FIND_LOCALIZATION(m_pwRunNotSavedLabel, "#MOM_MF_RunNotSaved");
    FIND_LOCALIZATION(m_pwRunUploadedLabel, "#MOM_MF_RunUploaded");
    FIND_LOCALIZATION(m_pwRunNotUploadedLabel, "#MOM_MF_RunNotUploaded");

    // Stats
    FIND_LOCALIZATION(m_pwCurrentPageOverall, "#MOM_MF_OverallStats");
    FIND_LOCALIZATION(m_pwCurrentPageZoneNum, "#MOM_MF_ZoneNum");
    FIND_LOCALIZATION(m_pwOverallTime, "#MOM_MF_RunTime");
    FIND_LOCALIZATION(m_pwZoneEnterTime, "#MOM_MF_Zone_Enter");
    FIND_LOCALIZATION(m_pwZoneTime, "#MOM_MF_Time_Zone");
    FIND_LOCALIZATION(m_pwVelAvg, "#MOM_MF_Velocity_Avg");
    FIND_LOCALIZATION(m_pwVelMax, "#MOM_MF_Velocity_Max");
    FIND_LOCALIZATION(m_pwVelZoneEnter, "#MOM_MF_Velocity_Enter");
    FIND_LOCALIZATION(m_pwVelZoneExit, "#MOM_MF_Velocity_Exit");
    FIND_LOCALIZATION(m_pwJumpsOverall, "#MOM_MF_JumpCount");
    FIND_LOCALIZATION(m_pwJumpsZone, "#MOM_MF_Jumps");
    FIND_LOCALIZATION(m_pwStrafesOverall, "#MOM_MF_StrafeCount");
    FIND_LOCALIZATION(m_pwStrafesZone, "#MOM_MF_Strafes");
    FIND_LOCALIZATION(m_pwSync1Overall, "#MOM_MF_AvgSync");
    FIND_LOCALIZATION(m_pwSync1Zone, "#MOM_MF_Sync1");
    FIND_LOCALIZATION(m_pwSync2Overall, "#MOM_MF_AvgSync2");
    FIND_LOCALIZATION(m_pwSync2Zone, "#MOM_MF_Sync2");
}

void CHudMapFinishedDialog::Reset()
{
    //default values
    m_pRunStats = nullptr;
    strcpy(m_pszEndRunTime, "00:00:00.000"); 
}

#define MAKE_UNI_NUM(name, size, number, isInt) \
    wchar_t name[size]; \
    V_snwprintf(name, size, isInt ? L"%.4f" : L"%d", number)

inline void PaintLabel(Label *label, wchar_t *wFormat, float value, bool isInt)
{
    wchar_t temp[BUFSIZELOCL];
    MAKE_UNI_NUM(tempNum, BUFSIZESHORT, isInt ? int(value) : value, isInt);
    g_pVGuiLocalize->ConstructString(temp, sizeof(temp), wFormat, 1, tempNum);
    label->SetText(temp);
}

void CHudMapFinishedDialog::Paint()
{
    //text color
    surface()->DrawSetTextFont(m_hTextFont);
    surface()->DrawSetTextColor(GetFgColor());

    // --- CURRENT PAGE TITLE (ZONE) ---
    wchar_t currentPageTitle[BUFSIZELOCL];
    if (m_iCurrentPage == 0)
    {
        //MOM_TODO: Take this width, divide by 2, 
        V_wcscpy_safe(currentPageTitle, m_pwCurrentPageOverall);
    }
    else
    {
        MAKE_UNI_NUM(num, 3, m_iCurrentPage, true);
        g_pVGuiLocalize->ConstructString(currentPageTitle, sizeof(currentPageTitle), m_pwCurrentPageZoneNum, 1, num);
    }
    
    m_pCurrentZoneLabel->SetText(currentPageTitle);
    int currentPageTitleWidth = UTIL_ComputeStringWidth(m_hTextFont, currentPageTitle) + 2;
    m_pNextZoneButton->SetPos(m_pCurrentZoneLabel->GetXPos() + currentPageTitleWidth, m_pCurrentZoneLabel->GetYPos());

    //// --- RUN TIME ---
    wchar_t currentZoneOverall[BUFSIZELOCL];
    wchar_t unicodeTime[BUFSIZETIME];
    //"Time:" shows up when m_iCurrentPage  == 0
    if (m_iCurrentPage < 1)// == 0, but I'm lazy to do an else-if
    {
        g_pVGuiLocalize->ConvertANSIToUnicode(m_pszEndRunTime, unicodeTime, sizeof(unicodeTime));
        g_pVGuiLocalize->ConstructString(currentZoneOverall, sizeof(currentZoneOverall), m_pwOverallTime, 1, unicodeTime);

        m_pZoneOverallTime->SetText(currentZoneOverall);//"Time" (overall run time)

        m_pZoneEnterTime->SetVisible(false);
        m_pZoneEnterTime->SetEnabled(false);
    }
    else
    {
        //"Zone Time:" shows up when m_iCurrentPage > 0
        char ansiTime[BUFSIZETIME];
        mom_UTIL->FormatTime(m_pRunStats ? m_pRunStats->GetZoneTime(m_iCurrentPage) : 0.0f, ansiTime);
        ANSI_TO_UNICODE(ansiTime, unicodeTime);
        g_pVGuiLocalize->ConstructString(currentZoneOverall, sizeof(currentZoneOverall), m_pwZoneTime, 1, unicodeTime);
        m_pZoneOverallTime->SetText(currentZoneOverall);//"Zone time" (time for that zone)


        //"Zone Enter Time:" shows up when m_iCurrentPage > 1
        if (m_iCurrentPage > 1)
        {
            m_pZoneEnterTime->SetEnabled(true);
            m_pZoneEnterTime->SetVisible(true);
            wchar_t zoneEnterTime[BUFSIZELOCL];
            mom_UTIL->FormatTime(m_pRunStats ? m_pRunStats->GetZoneEnterTime(m_iCurrentPage) : 0.0f, ansiTime);
            ANSI_TO_UNICODE(ansiTime, unicodeTime);
            g_pVGuiLocalize->ConstructString(zoneEnterTime, sizeof(zoneEnterTime), m_pwZoneEnterTime, 1, unicodeTime);
            m_pZoneEnterTime->SetText(zoneEnterTime);//"Zone enter time:" (time entered that zone)
        }
        else
        {
            m_pZoneEnterTime->SetVisible(false);
            m_pZoneEnterTime->SetEnabled(false);
        }
    }
    //// ---------------------

    //MOM_TODO: Set every label's Y pos higher if there's no ZoneEnterTime visible

    //// --- JUMP COUNT ---
    PaintLabel(m_pZoneJumps, 
        m_iCurrentPage == 0 ? m_pwJumpsOverall : m_pwJumpsZone,
        m_pRunStats ? m_pRunStats->GetZoneJumps(m_iCurrentPage) : 0, 
        true);
    //// ---------------------

    //// --- STRAFE COUNT ---
    PaintLabel(m_pZoneStrafes,
        m_iCurrentPage == 0 ? m_pwStrafesOverall : m_pwStrafesZone,
        m_pRunStats ? m_pRunStats->GetZoneStrafes(m_iCurrentPage) : 0,
        true);
    //// ---------------------

    //// --- SYNC1 ---
    PaintLabel(m_pZoneSync1,
        m_iCurrentPage == 0 ? m_pwSync1Overall : m_pwSync1Zone,
        m_pRunStats ? m_pRunStats->GetZoneStrafeSyncAvg(m_iCurrentPage) : 0.0f,
        false);
    //// ---------------------

    //// --- SYNC2---
    PaintLabel(m_pZoneSync2,
        m_iCurrentPage == 0 ? m_pwSync2Overall : m_pwSync2Zone,
        m_pRunStats ? m_pRunStats->GetZoneStrafeSync2Avg(m_iCurrentPage) : 0.0f,
        false);
    //// ---------------------

    //// --- STARTING VELOCITY---
    PaintLabel(m_pZoneVelEnter,
        m_pwVelZoneEnter,
        m_pRunStats ? m_pRunStats->GetZoneEnterSpeed(m_iCurrentPage, m_iVelocityType) : 0.0f,
        false);
    //// ---------------------

    //// --- ENDING VELOCITY---
    PaintLabel(m_pZoneVelExit,
        m_pwVelZoneExit,
        m_pRunStats ? m_pRunStats->GetZoneExitSpeed(m_iCurrentPage, m_iVelocityType) : 0.0f,
        false);
    //// ---------------------

    //// --- AVG VELOCITY---
    PaintLabel(m_pZoneVelAvg,
        m_pwVelAvg,
        m_pRunStats ? m_pRunStats->GetZoneVelocityAvg(m_iCurrentPage, m_iVelocityType) : 0.0f,
        false);
    //// ---------------------

    //// --- MAX VELOCITY---
    PaintLabel(m_pZoneVelMax,
        m_pwVelMax,
        m_pRunStats ? m_pRunStats->GetZoneVelocityMax(m_iCurrentPage, m_iVelocityType) : 0.0f,
        false);
    //// ---------------------

    //// ---- RUN SAVING AND UPLOADING ----

    //// -- run save --
    m_pRunSaveStatus->SetText(m_bRunSaved ? m_pwRunSavedLabel : m_pwRunNotSavedLabel);
    m_pRunSaveStatus->SetFgColor(m_bRunSaved ? COLOR_GREEN : COLOR_RED);

    //// -- run upload --
    //MOM_TODO: Should we have custom error messages here? One for server not responding, one for failed accept, etc
    m_pRunUploadStatus->SetText(m_bRunUploaded ? m_pwRunUploadedLabel : m_pwRunNotUploadedLabel);
    m_pRunUploadStatus->SetFgColor(m_bRunUploaded ? COLOR_GREEN : COLOR_RED);
    // ----------------
    // ------------------------------
}

//MOM_TODO: Do we want this to be a think method or update it to a usermsg/event, so it only calls once, and not every frame?
void CHudMapFinishedDialog::OnThink()
{
    C_MomentumPlayer * pPlayer = ToCMOMPlayer(C_BasePlayer::GetLocalPlayer());
    if (g_MOMEventListener && pPlayer)
    {
        m_bRunSaved = g_MOMEventListener->m_bTimeDidSave;
        m_bRunUploaded = g_MOMEventListener->m_bTimeDidUpload;
        //MOM_TODO: g_MOMEventListener has a m_szMapUploadStatus, do we want it on this panel?
        //Is it going to be a localized string, except for errors that have to be specific?
        
        ConVarRef hvel("mom_speedometer_hvel");
        m_iVelocityType = hvel.GetBool();

        C_MomentumReplayGhostEntity *pGhost = pPlayer->GetReplayEnt();
        m_pRunStats = pGhost ? &pGhost->m_RunStats : &pPlayer->m_RunStats;
        float lastRunTime = pGhost ? pGhost->m_RunData.m_flRunTime : pPlayer->m_RunData.m_flRunTime;
        mom_UTIL->FormatTime(lastRunTime, m_pszEndRunTime);
    }
}