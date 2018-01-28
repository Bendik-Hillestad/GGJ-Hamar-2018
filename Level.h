#ifndef GGJ_2018_LEVEL_H
#define GGJ_2018_LEVEL_H
#pragma once

#include "Block.h"
#include "Player.h"

#include <vector>

namespace GGJ
{
    int GetLevelWidth () noexcept;
    int GetLevelHeight() noexcept;

    void LoadLevel(Player* outPlayer, std::vector<Block>* outMap) noexcept;
};

#endif
