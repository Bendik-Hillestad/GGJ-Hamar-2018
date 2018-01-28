#ifndef GGJ_2018_PLAYER_H
#define GGJ_2018_PLAYER_H
#pragma once

#include "Block.h"
#include "InputManager.h"
#include "Timeline.h"

namespace GGJ
{
    class Player final : public Block
    {
    public:
        Player(int x, int y, int width, int height, float invMass) noexcept;

        void Think         (float dt) noexcept override;
        void HandleKeyEvent(Key key)  noexcept;

    private:
        bool up, left, down, right;

        bool isReplaying = false;
        Timeline timeline;
    };
};

#endif
