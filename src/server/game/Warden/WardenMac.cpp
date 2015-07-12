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

#include "WardenKeyGeneration.h"
#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "Opcodes.h"
#include "ByteBuffer.h"
#include <openssl/md5.h>
#include "World.h"
#include "Config.h"
#include "Player.h"
#include "Util.h"
#include "WardenMac.h"
#include "WardenModuleMac.h"

WardenMac::WardenMac() { }

WardenMac::~WardenMac() { }

void WardenMac::Init(WorldSession* pClient, BigNumber* K)
{
    Client = pClient;
    // Generate Warden Key
    SHA1Randx WK(K->AsByteArray(), K->GetNumBytes());
    WK.generate(InputKey, 16);
    WK.generate(OutputKey, 16);

    uint8 mod_seed[16] = { 0x4D, 0x80, 0x8D, 0x2C, 0x77, 0xD9, 0x05, 0xC4, 0x1A, 0x63, 0x80, 0xEC, 0x08, 0x58, 0x6A, 0xFE };

    memcpy(Seed, mod_seed, 16);

    iCrypto.Init(InputKey);
    oCrypto.Init(OutputKey);
    sLog->outDebug(LOG_FILTER_UNITS, "Server side warden for client %u initializing...", pClient->GetAccountId());
    //PrintHexArray("  C->S Key: ", InputKey, 16, true);
    //PrintHexArray("  S->C Key: ", OutputKey, 16, true);
    //PrintHexArray("  Seed: ", Seed, 16, true);
    sLog->outDebug(LOG_FILTER_UNITS, "Loading Module...");

    Module = GetModuleForClient();

    PrintHexArray("  Module Key: ", Module->Key, 16, true);
    PrintHexArray("  Module ID: ", Module->ID, 16, true);
    RequestModule();
}

ClientWardenModule* WardenMac::GetModuleForClient()
{
    ClientWardenModule* mod = new ClientWardenModule;

    uint32 len = sizeof(Module_0DBBF209A27B1E279A9FEC5C168A15F7_Data);

    // data assign
    mod->CompressedSize = len;
    mod->CompressedData = new uint8[len];
    memcpy(mod->CompressedData, Module_0DBBF209A27B1E279A9FEC5C168A15F7_Data, len);
    memcpy(mod->Key, Module_0DBBF209A27B1E279A9FEC5C168A15F7_Key, 16);

    // md5 hash
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, mod->CompressedData, len);
    MD5_Final((uint8*)&mod->ID, &ctx);

    return mod;
}

void WardenMac::InitializeModule()
{
    sLog->outDebug(LOG_FILTER_UNITS, "Initialize module");
}

void WardenMac::RequestHash()
{
    sLog->outDebug(LOG_FILTER_UNITS, "Request hash");

    // Create packet structure
    WardenHashRequest Request;
    Request.Command = WARDEN_SMSG_HASH_REQUEST;
    memcpy(Request.Seed, Seed, 16);

    // Encrypt with warden RC4 key.
    EncryptData((uint8*)&Request, sizeof(WardenHashRequest));

    WorldPacket pkt(SMSG_WARDEN_DATA, sizeof(WardenHashRequest));
    pkt.append((uint8*)&Request, sizeof(WardenHashRequest));
    Client->SendPacket(&pkt);
}

void WardenMac::HandleHashResult(ByteBuffer &buff)
{
    // test
    int keyIn[4];

    uint8 mod_seed[16] = { 0x4D, 0x80, 0x8D, 0x2C, 0x77, 0xD9, 0x05, 0xC4, 0x1A, 0x63, 0x80, 0xEC, 0x08, 0x58, 0x6A, 0xFE };

    for(int i = 0; i < 4; ++i)
    {
        keyIn[i] = *(int*)(&mod_seed[0] + i * 4);
    }

    int keyOut[4];
    int keyIn1, keyIn2;
    keyOut[0] = keyIn[0];
    keyIn[0] ^= 0xDEADBEEFu;
    keyIn1 = keyIn[1];
    keyIn[1] -= 0x35014542u;
    keyIn2 = keyIn[2];
    keyIn[2] += 0x5313F22u;
    keyIn[3] *= 0x1337F00Du;
    keyOut[1] = keyIn1 - 0x6A028A84;
    keyOut[2] = keyIn2 + 0xA627E44;
    keyOut[3] = 0x1337F00D * keyIn[3];
    // end test

    buff.rpos(buff.wpos());

    SHA1Hash sha1;
    sha1.UpdateData((uint8*)keyIn, 16);
    sha1.Finalize();

    // verify key not equal kick player
    if(memcmp(buff.contents() + 1, sha1.GetDigest(), 20) != 0)
    {
        sLog->outWarden("Request hash reply: failed");
        if(sWorld->getBoolConfig(CONFIG_BOOL_WARDEN_KICK))
            Client->KickPlayer();
        return;
    }

    sLog->outDebug(LOG_FILTER_UNITS, "Request hash reply: succeed");

    // change keys here
    memcpy(InputKey, keyIn, 16);
    memcpy(OutputKey, keyOut, 16);

    iCrypto.Init(InputKey);
    oCrypto.Init(OutputKey);

    m_initialized = true;

    m_WardenTimer = getMSTime();
}

void WardenMac::RequestData()
{
    sLog->outDebug(LOG_FILTER_UNITS, "Request data");

    ByteBuffer buff;
    buff << uint8(WARDEN_SMSG_CHEAT_CHECKS_REQUEST);

    std::string str = "Test string!";

    buff << uint8(str.size());
    buff.append(str.c_str(), str.size());

    buff.hexlike();

    // Encrypt with warden RC4 key.
    EncryptData(const_cast<uint8*>(buff.contents()), buff.size());

    WorldPacket pkt(SMSG_WARDEN_DATA, buff.size());
    pkt.append(buff);
    Client->SendPacket(&pkt);

    m_WardenDataSent = true;
}

void WardenMac::HandleData(ByteBuffer &buff)
{
    sLog->outDebug(LOG_FILTER_UNITS, "Handle data");

    m_WardenDataSent = false;
    m_WardenKickTimer = 0;

    bool found = false;
    bool needBan = true;

    std::string str = "Test string!";

    SHA1Hash sha1;
    sha1.UpdateData(str);
    uint32 magic = 0xFEEDFACE;                              // unsure
    sha1.UpdateData((uint8*)&magic, 4);
    sha1.Finalize();

    uint8 SHA1Hash[20];
    buff.read(SHA1Hash, 20);

    if(memcmp(SHA1Hash, sha1.GetDigest(), 20))
    {
        sLog->outWarden("Handle data failed: SHA1 hash is wrong!");
        found = true;
    }

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, str.c_str(), str.size());
    uint8 ourMD5Hash[16];
    MD5_Final(ourMD5Hash, &ctx);

    uint8 theirsMD5Hash[16];
    buff.read(theirsMD5Hash, 16);

    if(memcmp(ourMD5Hash, theirsMD5Hash, 16))
    {
        sLog->outDebug(LOG_FILTER_UNITS, "Handle data failed: MD5 hash is wrong!");
        found = true;
    }

    if(needBan &&  m_previousCheckFailed)
        needBan = false;

    m_previousCheckFailed = false;

    if(found && needBan && sWorld->getIntConfig(CONFIG_INT_WARDEN_BANDAY) > 0)
    {
        std::string sDuratuin = (std::string(sConfig->GetStringDefault("Warden.BanDay", "")) + "Jour");
        std::string sText = ("Le Joueur: " + std::string(Client->GetPlayerName()) + " a utilisé un logiciel de triche et est banni pour " + sDuratuin.c_str());
        sWorld->SendGMText(LANG_SYSTEMMESSAGE, sText.c_str());
        sWorld->BanAccount(BAN_CHARACTER, Client->GetPlayerName(), sDuratuin.c_str(), "Cheating software user", "Server Guard");
    } else if(found && sWorld->getBoolConfig(CONFIG_BOOL_WARDEN_KICK))
        Client->KickPlayer();
}
