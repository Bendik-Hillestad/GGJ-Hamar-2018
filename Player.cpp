#include "Player.h"
#include "Game.h"

#include <cmath>

namespace GGJ
{
    static constexpr float PLAYER_ACC       = 5000;
    static constexpr float PLAYER_MAX_SPEED =  500;

    Player::Player(int x, int y, int width, int height, float invMass) noexcept : Block{x, y, width, height, invMass},
        up{ false }, left{ false }, down{ false }, right{ false }, timeline{} {}

    void Player::Think(float dt) noexcept
    {
        this->xvel *= (1.0f - 2.0f * dt);
        this->yvel *= (1.0f - 2.0f * dt);

        if (this->up)    this->AddAccel(            0.0f,  PLAYER_ACC * dt);
        if (this->left)  this->AddAccel(-PLAYER_ACC * dt,             0.0f);
        if (this->down)  this->AddAccel(            0.0f, -PLAYER_ACC * dt);
        if (this->right) this->AddAccel( PLAYER_ACC * dt,             0.0f);

        float vel = this->xvel*this->xvel + this->yvel*this->yvel;
        if (vel > PLAYER_MAX_SPEED*PLAYER_MAX_SPEED)
        {
            float div   = std::sqrt(vel) / PLAYER_MAX_SPEED;
            this->xvel /= div;
            this->yvel /= div;
        }
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

                //Store it in the timeline
                this->timeline.AddKeystroke(Game::GetGame()->GetWorldTime(), key);
            } break;

            //A
            case Key::A:
            {
                //Check if it's being pressed or released
                if ((key & 0b11) == Key::Pressed) this->left =  true;
                else                              this->left = false;

                //Store it in the timeline
                this->timeline.AddKeystroke(Game::GetGame()->GetWorldTime(), key);
            } break;

            //S
            case Key::S:
            {
                //Check if it's being pressed or released
                if ((key & 0b11) == Key::Pressed) this->down =  true;
                else                              this->down = false;

                //Store it in the timeline
                this->timeline.AddKeystroke(Game::GetGame()->GetWorldTime(), key);
            } break;

            //W
            case Key::D:
            {
                //Check if it's being pressed or released
                if ((key & 0b11) == Key::Pressed) this->right =  true;
                else                              this->right = false;

                //Store it in the timeline
                this->timeline.AddKeystroke(Game::GetGame()->GetWorldTime(), key);
            } break;
        }
    }
};
