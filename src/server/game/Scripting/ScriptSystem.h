/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#ifndef SC_SYSTEM_H
#define SC_SYSTEM_H
#include <ace/Singleton.h>

#define TEXT_SOURCE_RANGE -1000000                          //the amount of entries each text source has available

//TODO: find better namings and definitions.
//N=Neutral, A=Alliance, H=Horde.
//NEUTRAL or FRIEND = Hostility to player surroundings (not a good definition)
//ACTIVE or PASSIVE = Hostility to environment surroundings.
enum eEscortFaction
{
    FACTION_ESCORT_A_NEUTRAL_PASSIVE    = 10,
    FACTION_ESCORT_H_NEUTRAL_PASSIVE    = 33,
    FACTION_ESCORT_N_NEUTRAL_PASSIVE    = 113,

    FACTION_ESCORT_A_NEUTRAL_ACTIVE     = 231,
    FACTION_ESCORT_H_NEUTRAL_ACTIVE     = 232,
    FACTION_ESCORT_N_NEUTRAL_ACTIVE     = 250,

    FACTION_ESCORT_N_FRIEND_PASSIVE     = 290,
    FACTION_ESCORT_N_FRIEND_ACTIVE      = 495,

    FACTION_ESCORT_A_PASSIVE            = 774,
    FACTION_ESCORT_H_PASSIVE            = 775,

    FACTION_ESCORT_N_ACTIVE             = 1986,
    FACTION_ESCORT_H_ACTIVE             = 2046
};

struct ScriptPointMove
{
    uint32 uiCreatureEntry;
    uint32 uiPointId;
    float  fX;
    float  fY;
    float  fZ;
    uint32 uiWaitTime;
};

typedef std::vector<ScriptPointMove> ScriptPointVector;

struct StringTextData
{
    uint32 uiSoundId;
    uint8  uiType;
    uint32 uiLanguage;
    uint32 uiEmote;
};

class SystemMgr
{
        friend class ACE_Singleton<SystemMgr, ACE_Null_Mutex>;
        SystemMgr() { }
        ~SystemMgr() { }

    public:
        //Maps and lists
        typedef UNORDERED_MAP<int32, StringTextData> TextDataMap;
        typedef UNORDERED_MAP<uint32, ScriptPointVector> PointMoveMap;

        //Database
        void LoadVersion();
        void LoadScriptTexts();
        void LoadScriptWaypoints();

        //Retrive from storage
        StringTextData const* GetTextData(int32 textId) const
        {
            TextDataMap::const_iterator itr = m_mTextDataMap.find(textId);

            if(itr == m_mTextDataMap.end())
                return NULL;

            return &itr->second;
        }

        ScriptPointVector const& GetPointMoveList(uint32 creatureEntry) const
        {
            PointMoveMap::const_iterator itr = m_mPointMoveMap.find(creatureEntry);

            if(itr == m_mPointMoveMap.end())
                return _empty;

            return itr->second;
        }

    protected:
        TextDataMap     m_mTextDataMap;                     //additional data for text strings
        PointMoveMap    m_mPointMoveMap;                    //coordinates for waypoints

    private:
        static ScriptPointVector const _empty;
};

#define sScriptSystemMgr ACE_Singleton<SystemMgr, ACE_Null_Mutex>::instance()

#endif
