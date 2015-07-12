/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "SHA1.h"

#ifndef _WARDEN_KEY_GENERATION_H
#define _WARDEN_KEY_GENERATION_H

class SHA1Randx {
public:
    SHA1Randx(uint8 *buff, uint32 size) {
        uint32 taken = size/2;

        sh.Initialize();
        sh.UpdateData(buff,taken);
        sh.Finalize();

        memcpy(o1,sh.GetDigest(), 20);

        sh.Initialize();
        sh.UpdateData(buff+taken,size-taken);
        sh.Finalize();

        memcpy(o2,sh.GetDigest(), 20);

        memset(o0, 0x00, 20);

        fillUp();
    }

    void generate(uint8 *buf, uint32 sz) {
        for(uint32 i = 0;i<sz;i++) {
            if(taken == 20) {
                fillUp();
            }

            buf[i] = o0[taken];
            taken++;
        }
    }
private:
    void fillUp() {
        sh.Initialize();
        sh.UpdateData(o1, 20);
        sh.UpdateData(o0, 20);
        sh.UpdateData(o2, 20);
        sh.Finalize();

        memcpy(o0,sh.GetDigest(), 20);

        taken = 0;
    }
    SHA1Hash sh;
    uint32 taken;
    uint8 o0[20],o1[20],o2[20];
};

#endif