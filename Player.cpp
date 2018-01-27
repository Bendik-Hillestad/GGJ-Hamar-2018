#include "Player.h"

namespace GGJ
{
    static constexpr float PLAYER_ACC       = 150;
    static constexpr float PLAYER_MAX_SPEED = 200;

    Player::Player(int x, int y, int width, int height, float invMass) noexcept : Block{x, y, width, height, invMass},
        up{ false }, left{ false }, down{false}, right{false} {}

    void Player::Think(float dt) noexcept
    {
        if (this->up)    this->AddAccel(            0.0f,  PLAYER_ACC * dt);
        if (this->left)  this->AddAccel(-PLAYER_ACC * dt,             0.0f);
        if (this->down)  this->AddAccel(            0.0f, -PLAYER_ACC * dt);
        if (this->right) this->AddAccel( PLAYER_ACC * dt,             0.0f);

        if (this->xvel >  PLAYER_MAX_SPEED) this->xvel =  PLAYER_MAX_SPEED;
        if (this->xvel < -PLAYER_MAX_SPEED) this->xvel = -PLAYER_MAX_SPEED;
        if (this->yvel >  PLAYER_MAX_SPEED) this->yvel =  PLAYER_MAX_SPEED;
        if (this->yvel < -PLAYER_MAX_SPEED) this->yvel = -PLAYER_MAX_SPEED;
    }

    void Player::HandleKeyEvent(Key key) noexcept
    {
        //Switch on the key
        switch (key & ~0b11)
        {
            //W
            case Key::W:
            {
                //Check if it's being pressed or released
                if ((key & 0b11) == Key::Pressed) this->up   =  true;
                else                              this->up   = false;
            } break;

            //A
            case Key::A:
            {
                //Check if it's being pressed or released
                if ((key & 0b11) == Key::Pressed) this->left =  true;
                else                              this->left = false;
            } break;

            //S
            case Key::S:
            {
                //Check if it's being pressed or released
                if ((key & 0b11) == Key::Pressed) this->down =  true;
                else                              this->down = false;
            } break;

            //W
            case Key::D:
            {
                //Check if it's being pressed or released
                if ((key & 0b11) == Key::Pressed) this->right =  true;
                else                              this->right = false;
            } break;
        }
    }
};
