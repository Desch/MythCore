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

#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "UpdateData.h"
#include "Item.h"
#include "Map.h"
#include "Transport.h"
#include "ObjectAccessor.h"
#include "CellImpl.h"

using namespace Trinity;

void VisibleNotifier::SendToSelf()
{
    // at this moment i_clientGUIDs have guids that not iterate at grid level checks
    // but exist one case when this possible and object not out of range: transports
    if(Transport* transport = i_player.GetTransport())
        for(Transport::PlayerSet::const_iterator itr = transport->GetPassengers().begin();itr != transport->GetPassengers().end();++itr)
        {
            if(vis_guids.find((*itr)->GetGUID()) != vis_guids.end())
            {
                vis_guids.erase((*itr)->GetGUID());

                i_player.UpdateVisibilityOf((*itr), i_data, i_visibleNow);

                if(!(*itr)->isNeedNotify(NOTIFY_VISIBILITY_CHANGED))
                    (*itr)->UpdateVisibilityOf(&i_player);
            }
        }

    for(Player::ClientGUIDs::const_iterator it = vis_guids.begin();it != vis_guids.end(); ++it)
    {
        i_player.m_clientGUIDs.erase(*it);
        i_data.AddOutOfRangeGUID(*it);

        if(IS_PLAYER_GUID(*it))
        {
            Player* plr = ObjectAccessor::FindPlayer(*it);
            if(plr && plr->IsInWorld() && !plr->isNeedNotify(NOTIFY_VISIBILITY_CHANGED))
                plr->UpdateVisibilityOf(&i_player);
        }
    }

    if(!i_data.HasData())
        return;

    WorldPacket packet;
    i_data.BuildPacket(&packet);
    i_player.GetSession()->SendPacket(&packet);

    for(std::set<Unit*>::const_iterator it = i_visibleNow.begin(); it != i_visibleNow.end(); ++it)
        i_player.SendInitialVisiblePackets(*it);
}

void VisibleChangesNotifier::Visit(PlayerMapType &m)
{
    for(PlayerMapType::iterator iter=m.begin(); iter != m.end(); ++iter)
    {
        if(iter->getSource() == &i_object)
            continue;

        iter->getSource()->UpdateVisibilityOf(&i_object);

        if(!iter->getSource()->GetSharedVisionList().empty())
            for(SharedVisionList::const_iterator i = iter->getSource()->GetSharedVisionList().begin();
                i != iter->getSource()->GetSharedVisionList().end(); ++i)
                if((*i)->m_seer == iter->getSource())
                    (*i)->UpdateVisibilityOf(&i_object);
    }
}

void VisibleChangesNotifier::Visit(CreatureMapType &m)
{
    for(CreatureMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        if(!iter->getSource()->GetSharedVisionList().empty())
            for(SharedVisionList::const_iterator i = iter->getSource()->GetSharedVisionList().begin();
                i != iter->getSource()->GetSharedVisionList().end(); ++i)
                if((*i)->m_seer == iter->getSource())
                    (*i)->UpdateVisibilityOf(&i_object);
}

void VisibleChangesNotifier::Visit(DynamicObjectMapType &m)
{
    for(DynamicObjectMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        if(IS_PLAYER_GUID(iter->getSource()->GetCasterGUID()))
            if(Player* caster = (Player*)iter->getSource()->GetCaster())
                if(caster->m_seer == iter->getSource())
                    caster->UpdateVisibilityOf(&i_object);
}

inline void CreatureUnitRelocationWorker(Creature* c, Unit* u)
{
    if(!c || !u || !u->isAlive() || !c->isAlive() || c == u || u->isInFlight())
        return;

    if(c->HasReactState(REACT_AGGRESSIVE) && !c->HasUnitState(UNIT_STAT_SIGHTLESS))
        if(c->IsAIEnabled && c->canSeeOrDetect(u, false, true))
            c->AI()->MoveInLineOfSight_Safe(u);
}

void PlayerRelocationNotifier::Visit(PlayerMapType &m)
{
    for(PlayerMapType::iterator iter=m.begin(); iter != m.end(); ++iter)
    {
        Player* pPlayer = iter->getSource();
        vis_guids.erase(pPlayer->GetGUID());

        i_player.UpdateVisibilityOf(pPlayer, i_data, i_visibleNow);
        if(pPlayer->m_seer->isNeedNotify(NOTIFY_VISIBILITY_CHANGED))
            continue;

        pPlayer->UpdateVisibilityOf(&i_player);
    }
}

void PlayerRelocationNotifier::Visit(CreatureMapType &m)
{
    bool relocated_for_ai = (&i_player == i_player.m_seer);

    for(CreatureMapType::iterator iter=m.begin(); iter != m.end(); ++iter)
    {
        Creature* pCreature = iter->getSource();

        vis_guids.erase(pCreature->GetGUID());

        i_player.UpdateVisibilityOf(pCreature, i_data, i_visibleNow);

        if(relocated_for_ai && !pCreature->isNeedNotify(NOTIFY_VISIBILITY_CHANGED))
            CreatureUnitRelocationWorker(pCreature, &i_player);
    }
}

void CreatureRelocationNotifier::Visit(PlayerMapType &m)
{
    for(PlayerMapType::iterator iter=m.begin(); iter != m.end(); ++iter)
    {
        Player* pPlayer = iter->getSource();

        if(pPlayer && !pPlayer->m_seer->isNeedNotify(NOTIFY_VISIBILITY_CHANGED))
            pPlayer->UpdateVisibilityOf(&i_creature);

        CreatureUnitRelocationWorker(&i_creature, pPlayer);
    }
}

void CreatureRelocationNotifier::Visit(CreatureMapType &m)
{
    if(!i_creature.isAlive())
        return;

    for(CreatureMapType::iterator iter=m.begin(); iter != m.end(); ++iter)
    {
        Creature* pCreature = iter->getSource();
        CreatureUnitRelocationWorker(&i_creature, pCreature);

        if(!pCreature->isNeedNotify(NOTIFY_VISIBILITY_CHANGED))
            CreatureUnitRelocationWorker(pCreature, &i_creature);
    }
}

void DelayedUnitRelocation::Visit(CreatureMapType &m)
{
    for(CreatureMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        Creature* pUnit = iter->getSource();
        if(!pUnit || !pUnit->isNeedNotify(NOTIFY_VISIBILITY_CHANGED))
            continue;

        CreatureRelocationNotifier relocate(*pUnit);

        TypeContainerVisitor<CreatureRelocationNotifier, WorldTypeMapContainer > c2world_relocation(relocate);
        TypeContainerVisitor<CreatureRelocationNotifier, GridTypeMapContainer >  c2grid_relocation(relocate);

        cell.Visit(p, c2world_relocation, i_map, *pUnit, i_radius);
        cell.Visit(p, c2grid_relocation, i_map, *pUnit, i_radius);
    }
}

void DelayedUnitRelocation::Visit(PlayerMapType &m)
{
    for(PlayerMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        Player* pPlayer = iter->getSource();
        if(!pPlayer)
            continue;

        WorldObject const* viewPoint = pPlayer->m_seer;

        if(!viewPoint || !viewPoint->isNeedNotify(NOTIFY_VISIBILITY_CHANGED))
            continue;

        if(!pPlayer || (pPlayer != viewPoint && !viewPoint->IsPositionValid()))
            continue;

        CellPair pair2(Trinity::ComputeCellPair(viewPoint->GetPositionX(), viewPoint->GetPositionY()));
        Cell cell2(pair2);
        PlayerRelocationNotifier relocate(*pPlayer);
        TypeContainerVisitor<PlayerRelocationNotifier, WorldTypeMapContainer > c2world_relocation(relocate);
        TypeContainerVisitor<PlayerRelocationNotifier, GridTypeMapContainer >  c2grid_relocation(relocate);

        cell2.Visit(pair2, c2world_relocation, i_map, *viewPoint, i_radius);
        cell2.Visit(pair2, c2grid_relocation, i_map, *viewPoint, i_radius);

        relocate.SendToSelf();
    }
}

void AIRelocationNotifier::Visit(CreatureMapType &m)
{
    for(CreatureMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        Creature* pCreature = iter->getSource();
        CreatureUnitRelocationWorker(pCreature, &i_unit);
        if(isCreature)
            CreatureUnitRelocationWorker((Creature*)&i_unit, pCreature);
    }
}

void MessageDistDeliverer::Visit(PlayerMapType &m)
{
    for(PlayerMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        Player* pTarget = iter->getSource();
        if(!pTarget || !pTarget->InSamePhase(i_phaseMask))
            continue;

        if(pTarget->GetExactDist2dSq(i_source) > i_distSq)
            continue;

        // Send packet to all who are sharing the player's vision
        if(!pTarget->GetSharedVisionList().empty())
        {
            SharedVisionList::const_iterator i = pTarget->GetSharedVisionList().begin();
            for(; i != pTarget->GetSharedVisionList().end(); ++i)
                if((*i)->m_seer == pTarget)
                    SendPacket(*i);
        }

        if(pTarget->m_seer == pTarget || pTarget->GetVehicle())
            SendPacket(pTarget);
    }
}

void MessageDistDeliverer::Visit(CreatureMapType &m)
{
    for(CreatureMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        if(!iter->getSource()->InSamePhase(i_phaseMask))
            continue;

        if(iter->getSource()->GetExactDist2dSq(i_source) > i_distSq)
            continue;

        // Send packet to all who are sharing the creature's vision
        if(!iter->getSource()->GetSharedVisionList().empty())
        {
            SharedVisionList::const_iterator i = iter->getSource()->GetSharedVisionList().begin();
            for(; i != iter->getSource()->GetSharedVisionList().end(); ++i)
                if((*i)->m_seer == iter->getSource())
                    SendPacket(*i);
        }
    }
}

void MessageDistDeliverer::Visit(DynamicObjectMapType &m)
{
    for(DynamicObjectMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        if(!iter->getSource()->InSamePhase(i_phaseMask))
            continue;

        if(iter->getSource()->GetExactDist2dSq(i_source) > i_distSq)
            continue;

        if(IS_PLAYER_GUID(iter->getSource()->GetCasterGUID()))
        {
            // Send packet back to the caster if the caster has vision of dynamic object
            Player* caster = (Player*)iter->getSource()->GetCaster();
            if(caster && caster->m_seer == iter->getSource())
                SendPacket(caster);
        }
    }
}

template<class T>
void ObjectUpdater::Visit(GridRefManager<T> &m)
{
    for(typename GridRefManager<T>::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        if(iter->getSource()->IsInWorld())
            iter->getSource()->Update(i_timeDiff);
    }
}

bool CannibalizeObjectCheck::operator()(Corpse* u)
{
    // ignore bones
    if(u->GetType() == CORPSE_BONES)
        return false;

    Player* owner = ObjectAccessor::FindPlayer(u->GetOwnerGUID());

    if(!owner || i_funit->IsFriendlyTo(owner))
        return false;

    if(i_funit->IsWithinDistInMap(u, i_range))
        return true;

    return false;
}

bool CarrionFeederObjectCheck::operator()(Corpse* u)
{
    // ignore bones
    if(u->GetType() == CORPSE_BONES)
        return false;

    Player* owner = ObjectAccessor::FindPlayer(u->GetOwnerGUID());

    if(!owner || i_funit->IsFriendlyTo(owner))
        return false;

    if(i_funit->IsWithinDistInMap(u, i_range))
        return true;

    return false;
}

template void ObjectUpdater::Visit<GameObject>(GameObjectMapType &);
template void ObjectUpdater::Visit<DynamicObject>(DynamicObjectMapType &);
