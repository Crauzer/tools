/*
 * AIM obj_extractor
 * Copyright (C) 2015 lzwdgc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>

#include <buffer.h>
#include <types.h>

struct MechGroup
{
    char name[0x20];
    char org[0x20];
    uint32_t type1 = 0;
    uint32_t len1 = 0;
    char name1[0x70];
    //{3,4
        uint32_t unk30 = 0;
    //}
    //{2
        uint32_t len = 0;
        std::vector<uint32_t> unk11;
    //}
    //{1,0
        uint32_t unk20 = 0;
        uint32_t unk21 = 0;
    //}
    std::vector<std::string> configs;
    char unk100;

    void load(buffer &b)
    {
        READ(b, name);
        READ(b, org);
        READ(b, type1);
        READ(b, len1);
        READ(b, name1);
        if (type1 == 3 || type1 == 4)
        {
            READ(b, unk30);
        }
        else if (type1 == 2)
        {
            READ(b, len);
            unk11.resize(len);
            for (int i = 0; i < len; i++)
                READ(b, unk11[i]);
        }
        else if (type1 == 1 || type1 == 0)
        {
            READ(b, unk20);
            READ(b, unk21);
        }
        else
            assert(false);
        configs.resize(len1, std::string(0x20, 0));
        for (int i = 0; i < len1; i++)
            READ_N(b, configs[i][0], 0x20);
        READ(b, unk100);
    }
};

struct MechGroups
{
    uint32_t length = 0;
    uint32_t n = 0;
    char prefix[0x30];

    std::vector<MechGroup> mgs;

    void load(buffer &b)
    {
        if (gameType == GameType::Aim2)
            READ(b, length);
        READ(b, n);
        READ(b, prefix);

        for (int s = 0; s < n; s++)
        {
            MechGroup mg;
            mg.load(b);
            mgs.push_back(mg);
        }
    }
};

struct MapGoods
{
    uint32_t length = 0;
    uint32_t unk2 = 0;
    uint32_t unk3 = 0;
    uint32_t n = 0;

    std::vector<BuildingGoods> bgs;

    void load(buffer &b)
    {
        READ(b, length);
        READ(b, unk2);
        if (gameType != GameType::Aim2)
            READ(b, unk3);
        READ(b, n);

        for (int i = 0; i < n; i++)
        {
            BuildingGoods bg;
            bg.load(b);
            bgs.push_back(bg);
            if (gameType == GameType::Aim2)
                READ(b, unk2);
        }
    }
};

struct MapSound
{
    char name[0x20];
    float unk1[4];
    uint32_t unk2 = 0;
    float unk3[4];

    void load(buffer &b)
    {
        READ(b, name);
        READ(b, unk1);
        READ(b, unk2);
        READ(b, unk3);
    }
};

struct MapSounds
{
    uint32_t n = 0;
    std::vector<MapSound> sounds;

    void load(buffer &b)
    {
        READ(b, n);
        for (int i = 0; i < n; i++)
        {
            MapSound s;
            s.load(b);
            sounds.push_back(s);
        }
    }
};

struct Price
{
    char tov_name[0x20];
    uint32_t unk0 = 0;
    uint32_t unk1 = 0;
    float unk2[3];

    void load(buffer &b)
    {
        READ(b, tov_name);
        READ(b, unk0);
        READ(b, unk1);
        READ(b, unk2);
    }
};

struct BuildingPrice
{
    char name[0x20];
    uint32_t n_tov = 0;
    std::vector<Price> prices;

    void load(buffer &b)
    {
        READ(b, name);
        READ(b, n_tov);
        for (int i = 0; i < n_tov; i++)
        {
            Price s;
            s.load(b);
            prices.push_back(s);
        }
    }
};

struct BuildingPrices
{
    uint32_t n_tov = 0;
    std::vector<Price> prices;
    uint32_t n_bases = 0;
    std::vector<BuildingPrice> buildingPrices;

    void load(buffer &b)
    {
        READ(b, n_tov);
        for (int i = 0; i < n_tov; i++)
        {
            Price s;
            s.load(b);
            prices.push_back(s);
        }
        READ(b, n_bases);
        for (int i = 0; i < n_bases; i++)
        {
            BuildingPrice s;
            s.load(b);
            buildingPrices.push_back(s);
        }
    }
};

struct Prices
{
    uint32_t len = 0;
    uint32_t unk0 = 0;
    BuildingPrices buildingPrices;

    void load(buffer &b)
    {
        READ(b, len);
        READ(b, unk0);
        buildingPrices.load(b);
    }
};
