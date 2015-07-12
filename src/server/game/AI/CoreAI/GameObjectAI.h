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

#ifndef TRINITY_GAMEOBJECTAI_H
#define TRINITY_GAMEOBJECTAI_H

#include "Define.h"
#include <list>
#include "Object.h"
#include "GameObject.h"
#include "CreatureAI.h"

class GameObjectAI
{
protected:
    GameObject* const go;
public:
    explicit GameObjectAI(GameObject* g) : go(g) { }
    virtual ~GameObjectAI() { }

    virtual void UpdateAI(const uint32 /*diff*/) { }

    virtual void InitializeAI() { Reset(); }

    virtual void Reset() { };

    static int Permissible(const GameObject* go);

    virtual bool GossipHello(Player* /*pPlayer*/) {return false;}
    virtual bool GossipSelect(Player* /*pPlayer*/, uint32 /*sender*/, uint32 /*action*/) {return false;}
    virtual bool GossipSelectCode(Player* /*pPlayer*/, uint32 /*sender*/, uint32 /*action*/, const char* /*code*/) {return false;}
    virtual bool QuestAccept(Player* /*pPlayer*/, Quest const* /*quest*/) {return false;}
    virtual bool QuestReward(Player* /*pPlayer*/, Quest const* /*quest*/, uint32 /*opt*/) {return false;}
    virtual uint32 GetDialogStatus(Player* /*pPlayer*/) {return 100;}
    virtual void Destroyed(Player* /*pPlayer*/, uint32 /*eventId*/) { }
    virtual void SetData(uint32 /*id*/, uint32 /*value*/) { }
};

class NullGameObjectAI : public GameObjectAI
{
    public:
        explicit NullGameObjectAI(GameObject* g);

        void UpdateAI(const uint32 /*diff*/) { }

        static int Permissible(const GameObject* /*pGo*/) { return PERMIT_BASE_IDLE; }
};

#endif
