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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Spell.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"
#include "Util.h"
#include "Pet.h"
#include "World.h"
#include "Group.h"

void WorldSession::HandleDismissCritter(WorldPacket &recv_data)
{
    uint64 guid;
    recv_data >> guid;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_DISMISS_CRITTER for GUID " UI64FMTD, guid);

    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);

    if(!pet)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Vanitypet (guid: %u) does not exist - player '%s' (guid: %u / account: %u) attempted to dismiss it (possibly lagged out)",
                uint32(GUID_LOPART(guid)), GetPlayer()->GetName(), GetPlayer()->GetGUIDLow(), GetAccountId());
        return;
    }

    if(_player->GetCritterGUID() == pet->GetGUID())
    {
         if(pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->isSummon())
             pet->ToTempSummon()->UnSummon();
    }
}

void WorldSession::HandlePetAction(WorldPacket &recv_data)
{
    uint64 guid1;
    uint32 data;
    uint64 guid2;
    recv_data >> guid1;                                     //pPet guid
    recv_data >> data;
    recv_data >> guid2;                                     //tag guid

    uint32 spellid = UNIT_ACTION_BUTTON_ACTION(data);
    uint8 flag = UNIT_ACTION_BUTTON_TYPE(data);             //delete = 0x07 CastSpell = C1

    // used also for charmed creature
    Unit* pPet= ObjectAccessor::GetUnit(*_player, guid1);
    sLog->outDetail("HandlePetAction: Pet %u - flag: %u, spellid: %u, target: %u.", uint32(GUID_LOPART(guid1)), uint32(flag), spellid, uint32(GUID_LOPART(guid2)));

    if(!pPet)
    {
        sLog->outError("HandlePetAction: Pet (GUID: %u) doesn't exist for player '%s'", uint32(GUID_LOPART(guid1)), GetPlayer()->GetName());
        return;
    }

    if(pPet != GetPlayer()->GetFirstControlled())
    {
        sLog->outError("HandlePetAction: Pet (GUID: %u) does not belong to player '%s'", uint32(GUID_LOPART(guid1)), GetPlayer()->GetName());
        return;
    }

    if(!pPet->isAlive())
    {
        SpellEntry const* spell = (flag == ACT_ENABLED || flag == ACT_PASSIVE) ? sSpellStore.LookupEntry(spellid) : NULL;
        if(!spell)
            return;
        if(!(spell->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD))
            return;
    }

    //TODO: allow control charmed player?
    if(pPet->GetTypeId() == TYPEID_PLAYER && !(flag == ACT_COMMAND && spellid == COMMAND_ATTACK))
        return;

    if(GetPlayer()->m_Controlled.size() == 1)
        HandlePetActionHelper(pPet, guid1, spellid, flag, guid2);
    else
    {
        //If a pPet is dismissed, m_Controlled will change
        std::vector<Unit*> controlled;
        for(Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
            if((*itr)->GetEntry() == pPet->GetEntry() && (*itr)->isAlive())
                controlled.push_back(*itr);
        for(std::vector<Unit*>::iterator itr = controlled.begin(); itr != controlled.end(); ++itr)
            HandlePetActionHelper(*itr, guid1, spellid, flag, guid2);
    }
}

void WorldSession::HandlePetStopAttack(WorldPacket &recv_data)
{
    uint64 guid;
    recv_data >> guid;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_PET_STOP_ATTACK for GUID " UI64FMTD "", guid);

    Unit* pPet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);

    if(!pPet)
    {
        sLog->outError("HandlePetStopAttack: Pet %u does not exist", uint32(GUID_LOPART(guid)));
        return;
    }

    if(pPet != GetPlayer()->GetPet() && pPet != GetPlayer()->GetCharm())
    {
        sLog->outError("HandlePetStopAttack: Pet GUID %u isn't a pPet or charmed creature of player %s", uint32(GUID_LOPART(guid)), GetPlayer()->GetName());
        return;
    }

    if(!pPet->isAlive())
        return;

    pPet->AttackStop();
}

void WorldSession::HandlePetActionHelper(Unit* pPet, uint64 guid1, uint16 spellid, uint16 flag, uint64 guid2)
{
    if(!pPet)
        return;

    CharmInfo *pCharmInfo = pPet->GetCharmInfo(); // DEBUG HERE.

    if(!pCharmInfo)
    {
        sLog->outError("WorldSession::HandlePetAction(pPetGuid: " UI64FMTD ", tagGuid: " UI64FMTD ", spellId: %u, flag: %u): object (entry: %u TypeId: %u) is considered pPet-like but doesn't have a charminfo!",
            guid1, guid2, spellid, flag, pPet->GetGUIDLow(), pPet->GetTypeId());
        return;
    }

    switch(flag)
    {
        case ACT_COMMAND:                                   //0x07
            switch(spellid)
            {
                case COMMAND_STAY:                          //flat=1792  //STAY
                    pPet->StopMoving();
                    pPet->GetMotionMaster()->Clear(false);
                    pPet->GetMotionMaster()->MoveIdle();
                    pCharmInfo->SetCommandState(COMMAND_STAY);

                    pCharmInfo->SetIsCommandAttack(false);
                    pCharmInfo->SetIsAtStay(true);
                    pCharmInfo->SetIsFollowing(false);
                    pCharmInfo->SetIsReturning(false);
                    pCharmInfo->SaveStayPosition();
                    break;
                case COMMAND_FOLLOW:                        //spellid=1792  //FOLLOW
                    pPet->AttackStop();
                    pPet->InterruptNonMeleeSpells(false);
                    pPet->GetMotionMaster()->MoveFollow(_player, PET_FOLLOW_DIST, pPet->GetFollowAngle());
                    pCharmInfo->SetCommandState(COMMAND_FOLLOW);

                    pCharmInfo->SetIsCommandAttack(false);
                    pCharmInfo->SetIsAtStay(false);
                    pCharmInfo->SetIsReturning(true);
                    pCharmInfo->SetIsFollowing(false);
                    break;
                case COMMAND_ATTACK:                        //spellid=1792  //ATTACK
                {
                    // Can't attack if owner is pacified
                    if(!_player || _player->HasAuraType(SPELL_AURA_MOD_PACIFY))
                    {
                        pPet->SendPetCastFail(spellid, SPELL_FAILED_PACIFIED);
                        //TODO: Send proper error message to client
                        return;
                    }

                    // only place where pPet can be player
                    Unit* TargetUnit = ObjectAccessor::GetUnit(*_player, guid2);
                    if(!TargetUnit)
                        return;

                    if(Unit* owner = pPet->GetOwner())
                        if(!owner || !owner->canAttack(TargetUnit))
                            return;

                    // Not let attack through obstructions
                    if(sWorld->getBoolConfig(CONFIG_VMAP_ENABLED))
                    {
                        if(!pPet->IsWithinLOSInMap(TargetUnit))
                            return;
                    }

                    pPet->ClearUnitState(UNIT_STAT_FOLLOW);
                    // This is true if pPet has no target or has target but targets differs.
                    if(pPet->getVictim() != TargetUnit || (pPet->getVictim() == TargetUnit && !pPet->GetCharmInfo()->IsCommandAttack()))
                    {
                        if(pPet->getVictim())
                            pPet->AttackStop();

                        if(pPet->GetTypeId() != TYPEID_PLAYER && pPet->ToCreature()->IsAIEnabled)
                        {
                            pCharmInfo->SetIsCommandAttack(true);
                            pCharmInfo->SetIsAtStay(false);
                            pCharmInfo->SetIsFollowing(false);
                            pCharmInfo->SetIsReturning(false);

                            pPet->ToCreature()->AI()->AttackStart(TargetUnit);

                            //10% chance to play special pPet attack talk, else growl
                            if(pPet->ToCreature()->isPet() && ((Pet*)pPet)->getPetType() == SUMMON_PET && pPet != TargetUnit && urand(0, 100) < 10)
                                pPet->SendPetTalk((uint32)PET_TALK_ATTACK);
                            else
                            {
                                // 90% chance for pPet and 100% chance for charmed creature
                                pPet->SendPetAIReaction(guid1);
                            }
                        }
                        else                                // charmed player
                        {
                            if(pPet->getVictim() && pPet->getVictim() != TargetUnit)
                                pPet->AttackStop();

                            pCharmInfo->SetIsCommandAttack(true);
                            pCharmInfo->SetIsAtStay(false);
                            pCharmInfo->SetIsFollowing(false);
                            pCharmInfo->SetIsReturning(false);

                            pPet->Attack(TargetUnit, true);
                            pPet->SendPetAIReaction(guid1);
                        }
                    }
                    break;
                }
                case COMMAND_ABANDON:                       // abandon (hunter pPet) or dismiss (summoned pPet)
                    if(pPet->GetCharmerGUID() == GetPlayer()->GetGUID())
                        _player->StopCastingCharm();
                    else if(pPet->GetOwnerGUID() == GetPlayer()->GetGUID())
                    {
                        ASSERT(pPet->GetTypeId() == TYPEID_UNIT);
                        if(pPet->isPet())
                        {
                            if(((Pet*)pPet)->getPetType() == HUNTER_PET)
                                GetPlayer()->RemovePet((Pet*)pPet, PET_SAVE_AS_DELETED);
                            else
                                //dismissing a summoned pPet is like killing them (this prevents returning a soulshard...)
                                pPet->setDeathState(CORPSE);
                        }
                        else if(pPet->HasUnitTypeMask(UNIT_MASK_MINION))
                        {
                            ((Minion*)pPet)->UnSummon();
                        }
                    }
                    break;
                default:
                    sLog->outError("WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
            }
            break;
        case ACT_REACTION:                                  // 0x6
            switch(spellid)
            {
                case REACT_PASSIVE:                         //passive
                    pPet->AttackStop();

                case REACT_DEFENSIVE:                       //recovery
                case REACT_AGGRESSIVE:                      //activete
                    if(pPet->GetTypeId() == TYPEID_UNIT)
                        pPet->ToCreature()->SetReactState(ReactStates(spellid));
                    break;
            }
            break;
        case ACT_DISABLED:                                  // 0x81    spell (disabled), ignore
        case ACT_PASSIVE:                                   // 0x01
        case ACT_ENABLED:                                   // 0xC1    spell
        {
            Unit* unit_target = NULL;

            if(guid2)
                unit_target = ObjectAccessor::GetUnit(*_player, guid2);

            // do not cast unknown spells
            SpellEntry const *spellInfo = sSpellStore.LookupEntry(spellid);
            if(!spellInfo)
            {
                sLog->outError("WORLD: unknown PET spell id %i", spellid);
                return;
            }

            if(spellInfo->StartRecoveryCategory > 0)
                if(pPet->GetCharmInfo() && pPet->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
                    return;

            for(uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if(spellInfo->EffectImplicitTargetA[i] == TARGET_UNIT_AREA_ENEMY_SRC || spellInfo->EffectImplicitTargetA[i] == TARGET_UNIT_AREA_ENEMY_DST || spellInfo->EffectImplicitTargetA[i] == TARGET_DEST_DYNOBJ_ENEMY)
                    return;
            }

            // do not cast not learned spells
            if(!pPet->HasSpell(spellid) || IsPassiveSpell(spellid))
                return;

            //  Clear the flags as if owner clicked 'attack'. AI will reset them
            //  after AttackStart, even if spell failed
            if(pPet->GetCharmInfo())
            {
                pPet->GetCharmInfo()->SetIsAtStay(false);
                pPet->GetCharmInfo()->SetIsCommandAttack(true);
                pPet->GetCharmInfo()->SetIsReturning(false);
                pPet->GetCharmInfo()->SetIsFollowing(false);
            }

            Spell* spell = new Spell(pPet, spellInfo, false);

            SpellCastResult result = spell->CheckPetCast(unit_target);

            //auto turn to target unless possessed
            if(result == SPELL_FAILED_UNIT_NOT_INFRONT && !pPet->isPossessed() && !pPet->IsVehicle())
            {
                if(unit_target)
                {
                    pPet->SetInFront(unit_target);
                    if(unit_target->GetTypeId() == TYPEID_PLAYER)
                        pPet->SendUpdateToPlayer((Player*)unit_target);
                }
                else if(Unit* unit_target2 = spell->m_targets.GetUnitTarget())
                {
                    pPet->SetInFront(unit_target2);
                    if(unit_target2->GetTypeId() == TYPEID_PLAYER)
                        pPet->SendUpdateToPlayer((Player*)unit_target2);
                }
                if(Unit* powner = pPet->GetCharmerOrOwner())
                    if(powner->GetTypeId() == TYPEID_PLAYER)
              pPet->SendUpdateToPlayer(powner->ToPlayer());
                result = SPELL_CAST_OK;
            }

            if(result == SPELL_CAST_OK)
            {
                pPet->ToCreature()->AddCreatureSpellCooldown(spellid);

                unit_target = spell->m_targets.GetUnitTarget();

                //10% chance to play special pPet attack talk, else growl
                //actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if(pPet->ToCreature()->isPet() && (((Pet*)pPet)->getPetType() == SUMMON_PET) && (pPet != unit_target) && (urand(0, 100) < 10))
                    pPet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
                else
                {
                    pPet->SendPetAIReaction(guid1);
                }

                if(unit_target && !GetPlayer()->IsFriendlyTo(unit_target) && !pPet->isPossessed() && !pPet->IsVehicle())
                {
                    // This is true if pPet has no target or has target but targets differs.
                    if(pPet->getVictim() != unit_target)
                    {
                        if(pPet->getVictim())
                            pPet->AttackStop();
                        pPet->GetMotionMaster()->Clear();
                        if(pPet->ToCreature()->IsAIEnabled)
                            pPet->ToCreature()->AI()->AttackStart(unit_target);
                    }
                }

                spell->prepare(&(spell->m_targets));
            }
            else
            {
                if(pPet->isPossessed() || pPet->IsVehicle())
                    Spell::SendCastResult(GetPlayer(), spellInfo, 0, result);
                else
                    pPet->SendPetCastFail(spellid, result);

                if(!pPet->ToCreature()->HasSpellCooldown(spellid))
                    GetPlayer()->SendClearCooldown(spellid, pPet);

                spell->finish(false);
                delete spell;

                // reset specific flags in case of spell fail. AI will reset other flags
                if(pPet->GetCharmInfo())
                    pPet->GetCharmInfo()->SetIsCommandAttack(false);
            }
            break;
        }
        default:
            sLog->outError("WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
    }
}

void WorldSession::HandlePetNameQuery(WorldPacket &recv_data)
{
    sLog->outDetail("HandlePetNameQuery. CMSG_PET_NAME_QUERY");

    uint32 petnumber;
    uint64 petguid;

    recv_data >> petnumber;
    recv_data >> petguid;

    SendPetNameQuery(petguid, petnumber);
}

void WorldSession::SendPetNameQuery(uint64 petguid, uint32 petnumber)
{
    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, petguid);
    if(!pet)
    {
        WorldPacket data(SMSG_PET_NAME_QUERY_RESPONSE, (4+1+4+1));
        data << uint32(petnumber);
        data << uint8(0);
        data << uint32(0);
        data << uint8(0);
        _player->GetSession()->SendPacket(&data);
        return;
    }

    std::string name = pet->GetName();

    WorldPacket data(SMSG_PET_NAME_QUERY_RESPONSE, (4+4+name.size()+1));
    data << uint32(petnumber);
    data << name.c_str();
    data << uint32(pet->GetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP));

    if(pet->isPet() && ((Pet*)pet)->GetDeclinedNames())
    {
        data << uint8(1);
        for(uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data << ((Pet*)pet)->GetDeclinedNames()->name[i];
    }
    else
        data << uint8(0);

    _player->GetSession()->SendPacket(&data);
}

bool WorldSession::CheckStableMaster(uint64 guid)
{
    // spell case or GM
    if(guid == GetPlayer()->GetGUID())
    {
        if(!GetPlayer()->isGameMaster() && !GetPlayer()->HasAuraType(SPELL_AURA_OPEN_STABLE))
        {
            sLog->outDebug(LOG_FILTER_UNITS, "Player (GUID:%u) attempt open stable in cheating way.", GUID_LOPART(guid));
            return false;
        }
    }
    // stable master case
    else
    {
        if(!GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_STABLEMASTER))
        {
            sLog->outDebug(LOG_FILTER_UNITS, "Stablemaster (GUID:%u) not found or you can't interact with him.", GUID_LOPART(guid));
            return false;
        }
    }
    return true;
}

void WorldSession::HandlePetSetAction(WorldPacket &recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "HandlePetSetAction. CMSG_PET_SET_ACTION");

    uint64 petguid;
    uint8  count;

    recv_data >> petguid;

    Unit* pet = ObjectAccessor::GetUnit(*_player, petguid);

    if(!pet || pet != _player->GetFirstControlled())
    {
        sLog->outError("HandlePetSetAction: Unknown pet (GUID: %u) or pet owner (GUID: %u)", GUID_LOPART(petguid), _player->GetGUIDLow());
        return;
    }

    CharmInfo *charmInfo = pet->GetCharmInfo();
    if(!charmInfo)
    {
        sLog->outError("WorldSession::HandlePetSetAction: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    count = (recv_data.size() == 24) ? 2 : 1;

    uint32 position[2];
    uint32 data[2];
    bool move_command = false;

    for(uint8 i = 0; i < count; ++i)
    {
        recv_data >> position[i];
        recv_data >> data[i];

        uint8 act_state = UNIT_ACTION_BUTTON_TYPE(data[i]);

        //ignore invalid position
        if(position[i] >= MAX_UNIT_ACTION_BAR_INDEX)
            return;

        // in the normal case, command and reaction buttons can only be moved, not removed
        // at moving count == 2, at removing count == 1
        // ignore attempt to remove command|reaction buttons (not possible at normal case)
        if(act_state == ACT_COMMAND || act_state == ACT_REACTION)
        {
            if(count == 1)
                return;

            move_command = true;
        }
    }

    // check swap (at command->spell swap client remove spell first in another packet, so check only command move correctness)
    if(move_command)
    {
        uint8 act_state_0 = UNIT_ACTION_BUTTON_TYPE(data[0]);
        if(act_state_0 == ACT_COMMAND || act_state_0 == ACT_REACTION)
        {
            uint32 spell_id_0 = UNIT_ACTION_BUTTON_ACTION(data[0]);
            UnitActionBarEntry const* actionEntry_1 = charmInfo->GetActionBarEntry(position[1]);
            if(!actionEntry_1 || spell_id_0 != actionEntry_1->GetAction() ||
                act_state_0 != actionEntry_1->GetType())
                return;
        }

        uint8 act_state_1 = UNIT_ACTION_BUTTON_TYPE(data[1]);
        if(act_state_1 == ACT_COMMAND || act_state_1 == ACT_REACTION)
        {
            uint32 spell_id_1 = UNIT_ACTION_BUTTON_ACTION(data[1]);
            UnitActionBarEntry const* actionEntry_0 = charmInfo->GetActionBarEntry(position[0]);
            if(!actionEntry_0 || spell_id_1 != actionEntry_0->GetAction() ||
                act_state_1 != actionEntry_0->GetType())
                return;
        }
    }

    for(uint8 i = 0; i < count; ++i)
    {
        uint32 spell_id = UNIT_ACTION_BUTTON_ACTION(data[i]);
        uint8 act_state = UNIT_ACTION_BUTTON_TYPE(data[i]);

        sLog->outDetail("Player %s has changed pet spell action. Position: %u, Spell: %u, State: 0x%X", _player->GetName(), position[i], spell_id, uint32(act_state));

        //if it's act for spell (en/disable/cast) and there is a spell given (0 = remove spell) which pet doesn't know, don't add
        if(!((act_state == ACT_ENABLED || act_state == ACT_DISABLED || act_state == ACT_PASSIVE) && spell_id && !pet->HasSpell(spell_id)))
        {
            //sign for autocast
            if(act_state == ACT_ENABLED && spell_id)
            {
                if(pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->isPet())
                    ((Pet*)pet)->ToggleAutocast(spell_id, true);
                else
                    for(Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
                        if((*itr)->GetEntry() == pet->GetEntry())
                            (*itr)->GetCharmInfo()->ToggleCreatureAutocast(spell_id, true);
            }
            //sign for no/turn off autocast
            else if(act_state == ACT_DISABLED && spell_id)
            {
                if(pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->isPet())
                    ((Pet*)pet)->ToggleAutocast(spell_id, false);
                else
                    for(Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
                        if((*itr)->GetEntry() == pet->GetEntry())
                            (*itr)->GetCharmInfo()->ToggleCreatureAutocast(spell_id, false);

            }

            charmInfo->SetActionBar(position[i], spell_id, ActiveStates(act_state));
        }
    }
}

void WorldSession::HandlePetRename(WorldPacket &recv_data)
{
    sLog->outDetail("HandlePetRename. CMSG_PET_RENAME");

    uint64 petguid;
    uint8 isdeclined;

    std::string name;
    DeclinedName declinedname;

    recv_data >> petguid;
    recv_data >> name;
    recv_data >> isdeclined;

    Pet* pet = ObjectAccessor::FindPet(petguid);
                                                            // check it!
    if(!pet || !pet->isPet() || ((Pet*)pet)->getPetType()!= HUNTER_PET ||
        !pet->HasByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED) ||
        pet->GetOwnerGUID() != _player->GetGUID() || !pet->GetCharmInfo())
        return;

    PetNameInvalidReason res = ObjectMgr::CheckPetName(name);
    if(res != PET_NAME_SUCCESS)
    {
        SendPetNameInvalid(res, name, NULL);
        return;
    }

    if(sObjectMgr->IsReservedName(name))
    {
        SendPetNameInvalid(PET_NAME_RESERVED, name, NULL);
        return;
    }

    pet->SetName(name);

    Unit* owner = pet->GetOwner();
    if(owner && (owner->GetTypeId() == TYPEID_PLAYER) && owner->ToPlayer()->GetGroup())
        owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_NAME);

    pet->RemoveByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED);

    if(isdeclined)
    {
        for(uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        {
            recv_data >> declinedname.name[i];
        }

        std::wstring wname;
        Utf8toWStr(name, wname);
        if(!ObjectMgr::CheckDeclinedNames(wname, declinedname))
        {
            SendPetNameInvalid(PET_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME, name, &declinedname);
            return;
        }
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    if(isdeclined)
    {
        for(uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            CharacterDatabase.EscapeString(declinedname.name[i]);
        trans->PAppend("DELETE FROM character_pet_declinedname WHERE owner = '%u' AND id = '%u'", _player->GetGUIDLow(), pet->GetCharmInfo()->GetPetNumber());
        trans->PAppend("INSERT INTO character_pet_declinedname (id, owner, genitive, dative, accusative, instrumental, prepositional) VALUES ('%u', '%u', '%s', '%s', '%s', '%s', '%s')",
            pet->GetCharmInfo()->GetPetNumber(), _player->GetGUIDLow(), declinedname.name[0].c_str(), declinedname.name[1].c_str(), declinedname.name[2].c_str(), declinedname.name[3].c_str(), declinedname.name[4].c_str());
    }

    CharacterDatabase.EscapeString(name);
    trans->PAppend("UPDATE character_pet SET name = '%s', renamed = '1' WHERE owner = '%u' AND id = '%u'", name.c_str(), _player->GetGUIDLow(), pet->GetCharmInfo()->GetPetNumber());
    CharacterDatabase.CommitTransaction(trans);

    pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL))); // cast can't be helped
}

void WorldSession::HandlePetAbandon(WorldPacket &recv_data)
{
    uint64 guid;
    recv_data >> guid;                                      //pet guid
    sLog->outDetail("HandlePetAbandon. CMSG_PET_ABANDON pet guid is %u", GUID_LOPART(guid));

    if(!_player->IsInWorld())
        return;

    // pet/charmed
    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);
    if(pet)
    {
        if(pet->isPet())
        {
            if(pet->GetGUID() == _player->GetPetGUID())
            {
                uint32 feelty = pet->GetPower(POWER_HAPPINESS);
                pet->SetPower(POWER_HAPPINESS, (feelty-50000) > 0 ?(feelty-50000) : 0);
            }

            _player->RemovePet((Pet*)pet, PET_SAVE_AS_DELETED);
        }
        else if(pet->GetGUID() == _player->GetCharmGUID())
            _player->StopCastingCharm();
    }
}

void WorldSession::HandlePetSpellAutocastOpcode(WorldPacket& recvPacket)
{
    sLog->outDetail("CMSG_PET_SPELL_AUTOCAST");
    uint64 guid;
    uint32 spellid;
    uint8  state;                                           //1 for on, 0 for off
    recvPacket >> guid >> spellid >> state;

    if(!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    if(ObjectAccessor::FindPlayer(guid))
        return;

    Creature* pet=ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);

    if(!pet || (pet != _player->GetGuardianPet() && pet != _player->GetCharm()))
    {
        sLog->outError("HandlePetSpellAutocastOpcode.Pet %u isn't pet of player %s .", uint32(GUID_LOPART(guid)), GetPlayer()->GetName());
        return;
    }

    // do not add not learned spells/ passive spells
    if(!pet->HasSpell(spellid) || !IsAutocastableSpell(spellid))
        return;

    CharmInfo *charmInfo = pet->GetCharmInfo();
    if(!charmInfo)
    {
        sLog->outError("WorldSession::HandlePetSpellAutocastOpcod: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    if(pet->isPet())
        ((Pet*)pet)->ToggleAutocast(spellid, state);
    else
        pet->GetCharmInfo()->ToggleCreatureAutocast(spellid, state);

    charmInfo->SetSpellAutocast(spellid, state);
}

void WorldSession::HandlePetCastSpellOpcode(WorldPacket& recvPacket)
{
    sLog->outDetail("WORLD: CMSG_PET_CAST_SPELL");

    uint64 guid;
    uint8  castCount;
    uint32 spellId;
    uint8  castFlags;

    recvPacket >> guid >> castCount >> spellId >> castFlags;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_PET_CAST_SPELL, guid: " UI64FMTD ", castCount: %u, spellId %u, castFlags %u", guid, castCount, spellId, castFlags);

    // This opcode is also sent from charmed and possessed units (players and creatures)
    if(!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    Unit* caster = ObjectAccessor::GetUnit(*_player, guid);

    if(!caster || (caster != _player->GetGuardianPet() && caster != _player->GetCharm()))
    {
        sLog->outError("HandlePetCastSpellOpcode: Pet %u isn't pet of player %s .", uint32(GUID_LOPART(guid)), GetPlayer()->GetName());
        return;
    }

    SpellEntry const *spellInfo = sSpellStore.LookupEntry(spellId);
    if(!spellInfo)
    {
        sLog->outError("WORLD: unknown PET spell id %i", spellId);
        return;
    }

    if(spellInfo->StartRecoveryCategory > 0) // Check if spell is affected by GCD
        if(caster->GetTypeId() == TYPEID_UNIT && caster->GetCharmInfo() && caster->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
        {
            caster->SendPetCastFail(spellId, SPELL_FAILED_NOT_READY);
            return;
        }

    // do not cast not learned spells
    if(!caster->HasSpell(spellId) || IsPassiveSpell(spellId))
        return;

    SpellCastTargets targets;
    targets.Read(recvPacket, caster);
    HandleClientCastFlags(recvPacket, castFlags, targets);

    caster->ClearUnitState(UNIT_STAT_FOLLOW);

    Spell* spell = new Spell(caster, spellInfo, false);
    spell->m_cast_count = castCount;                    // probably pending spell cast
    spell->m_targets = targets;

    // TODO: need to check victim?
    SpellCastResult result;
    if(caster->m_movedPlayer)
        result = spell->CheckPetCast(caster->m_movedPlayer->GetSelectedUnit());
    else
        result = spell->CheckPetCast(NULL);
    if(result == SPELL_CAST_OK)
    {
        if(caster->GetTypeId() == TYPEID_UNIT)
        {
            Creature* pet = caster->ToCreature();
            pet->AddCreatureSpellCooldown(spellId);
            if(pet->isPet())
            {
                Pet* p = (Pet*)pet;
                // 10% chance to play special pet attack talk, else growl
                // actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if(p->getPetType() == SUMMON_PET && (urand(0, 100) < 10))
                    pet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
                else
                    pet->SendPetAIReaction(guid);
            }
        }

        spell->prepare(&(spell->m_targets));
    }
    else
    {
        caster->SendPetCastFail(spellId, result);
        if(caster->GetTypeId() == TYPEID_PLAYER)
        {
            if(!caster->ToPlayer()->HasSpellCooldown(spellId))
                GetPlayer()->SendClearCooldown(spellId, caster);
        }
        else
        {
            if(!caster->ToCreature()->HasSpellCooldown(spellId))
                GetPlayer()->SendClearCooldown(spellId, caster);
        }

        spell->finish(false);
        delete spell;
    }
}

void WorldSession::SendPetNameInvalid(uint32 error, const std::string& name, DeclinedName *declinedName)
{
    WorldPacket data(SMSG_PET_NAME_INVALID, 4 + name.size() + 1 + 1);
    data << uint32(error);
    data << name;
    if(declinedName)
    {
        data << uint8(1);
        for(uint32 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data << declinedName->name[i];
    }
    else
        data << uint8(0);
    SendPacket(&data);
}

void WorldSession::HandlePetLearnTalent(WorldPacket &recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_PET_LEARN_TALENT");

    uint64 guid;
    uint32 talent_id, requested_rank;
    recv_data >> guid >> talent_id >> requested_rank;

    _player->LearnPetTalent(guid, talent_id, requested_rank);
    _player->SendTalentsInfoData(true);
}

void WorldSession::HandleLearnPreviewTalentsPet(WorldPacket &recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LEARN_PREVIEW_TALENTS_PET");

    uint64 guid;
    recv_data >> guid;

    uint32 talentsCount;
    recv_data >> talentsCount;

    uint32 talentId, talentRank;

    for(uint32 i = 0; i < talentsCount; ++i)
    {
        recv_data >> talentId >> talentRank;

        _player->LearnPetTalent(guid, talentId, talentRank);
    }

    _player->SendTalentsInfoData(true);
}
