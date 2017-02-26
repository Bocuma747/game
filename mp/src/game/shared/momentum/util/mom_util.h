#ifndef MOM_UTIL_H
#define MOM_UTIL_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "KeyValues.h"
#include "UtlSortVector.h"
#include "filesystem.h"
#include <gason.h>
#include "run/run_compare.h"
#include "run/run_stats.h"
#include "steam/steam_api.h"
#include "run/mom_replay_base.h"

#ifdef CLIENT_DLL
#include "ChangelogPanel.h"
#endif

class MomentumUtil
{
  public:
    void DownloadCallback(HTTPRequestCompleted_t *, bool);

    void DownloadMap(const char *);

    template <class T>
    bool CreateAndSendHTTPReq(const char* szURL, CCallResult<T, HTTPRequestCompleted_t> *callback,
        typename CCallResult<T, HTTPRequestCompleted_t>::func_t func, T* pCaller,
        const EHTTPMethod kReqMethod = k_EHTTPMethodGET, KeyValues *pParams = nullptr)
    {
        bool bSuccess = false;
        if (steamapicontext && steamapicontext->SteamHTTP() && callback)
        {
            HTTPRequestHandle handle = steamapicontext->SteamHTTP()->CreateHTTPRequest(kReqMethod, szURL);

            if (pParams && (kReqMethod == k_EHTTPMethodPOST || kReqMethod == k_EHTTPMethodGET))
            {
                FOR_EACH_VALUE(pParams, p_value)
                {
                    steamapicontext->SteamHTTP()->SetHTTPRequestGetOrPostParameter(handle, p_value->GetName(),
                        p_value->GetString());
                }
            }

            SteamAPICall_t apiHandle;

            if (steamapicontext->SteamHTTP()->SendHTTPRequest(handle, &apiHandle))
            {
                callback->Set(apiHandle, pCaller, func);
            }
            else
            {
                Warning("Failed to send HTTP Request!\n");
                steamapicontext->SteamHTTP()->ReleaseHTTPRequest(handle); // GC
                bSuccess = true;
            }
        }
        else
        {
            Warning("Steampicontext failure.\nCould not find Steam Api Context active");
        }

        return bSuccess;
    }

    //CCallResult<MomentumUtil, HTTPRequestCompleted_t> cbDownloadCallback;

#ifdef CLIENT_DLL
    void GetRemoteChangelog();
    CCallResult<MomentumUtil, HTTPRequestCompleted_t> cbChangeLog;
    void ChangelogCallback(HTTPRequestCompleted_t *, bool);

    void GetRemoteRepoModVersion();
    CCallResult<MomentumUtil, HTTPRequestCompleted_t> cbVersionCallback;
    void VersionCallback(HTTPRequestCompleted_t *, bool);

    // For the ComparisonsSettingsPage
    void GenerateBogusRunStats(C_MomRunStats *pStatsOut);
#endif

    // Color GetColorFromVariation(float variation, float deadZone, Color normalcolor, Color increasecolor, Color
    // decreasecolor);
    Color *GetColorFromHex(const char *hexColor); // in hex color format RRGGBB
    Color m_newColor;

    Color GetColorFromVariation(const float variation, float deadZone, const Color &normalcolor, const Color &increasecolor,
                                const Color &decreasecolor) const;
    // Formats time in ticks by a given tickrate into time. Includes minutes if time > minutes, hours if time > hours,
    // etc
    // Precision is miliseconds by default
    void FormatTime(float seconds, char *pOut, const int precision = 3, const bool fileName = false, const bool negativeTime = false) const;


    CMomReplayBase *GetBestTime(const char *szMapName, float tickrate, uint32 flags = 0) const;
    bool GetRunComparison(const char *szMapName, const float tickRate, const int flags, RunCompare_t *into) const;
    void FillRunComparison(const char *compareName, CMomRunStats *kvBestRun, RunCompare_t *into) const;

    bool FloatEquals(const float a, const float b, const float epsilon = FLT_EPSILON) const { return fabs(a - b) < epsilon; }

    FORCEINLINE bool VectorEquals(const Vector &a, const Vector &b, const vec_t epsilon = FLT_EPSILON) const { return fabs(a.x - b.x) < epsilon && fabs(a.y - b.y) < epsilon && fabs(a.z - b.z) < epsilon; }
    // Checks if source is within a rectangle formed by leftCorner and rightCorner
    bool IsInBounds(const Vector2D &source, const Vector2D &bottomLeft, const Vector2D &topRight) const
    {
        return (source.x > bottomLeft.x && source.x < topRight.x) && (source.y > bottomLeft.y && source.y < topRight.y);
    }

    bool IsInBounds(const int x, const int y, const int rectX, const int rectY, const int rectW, const int rectH) const
    {
        return IsInBounds(Vector2D(x, y), Vector2D(rectX, rectY), Vector2D(rectX + rectW, rectY + rectH));
    }

    void KVSaveVector(KeyValues *kvInto, const char *pName, const Vector &toSave);
    void KVLoadVector(KeyValues *kvFrom, const char *pName, Vector &vecInto);

    void KVSaveQAngles(KeyValues *kvInto, const char *pName, const QAngle &toSave);
    void KVLoadQAngles(KeyValues *kvFrom, const char *pName, QAngle &angInto);

    bool MapThumbnailExists(const char *pMapName);
};


extern MomentumUtil *g_pMomentumUtil;

#endif // MOM_UTIL_H