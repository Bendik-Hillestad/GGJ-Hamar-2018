#include "InputManager.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdio>

namespace GGJ
{
    struct input_event_t
    {
        WPARAM wParam;
        LPARAM repeatCount     : 16;
        LPARAM scanCode        :  8;
        LPARAM extendedKey     :  1;
        LPARAM reserved        :  4;
        LPARAM contextCode     :  1;
        LPARAM previousState   :  1;
        LPARAM transitionState :  1;
        bool   keyDown;
    };

    InputManager* InputManager::GetInputManager() noexcept
    {
        static InputManager inputManager{};

        //Return instance
        return &inputManager;
    }

    Key InputManager::Poll() noexcept
    {
        //Check if it's empty
        if (this->inputQueue.empty()) return Key::NONE;

        //Pop an item off of the queue and return it
        auto item = this->inputQueue.front();
        this->inputQueue.pop();
        return item;
    }

    void InputManager::PushEvent(input_event_t* inputEvent) noexcept
    {
        //Switch on the scan code
        switch (inputEvent->scanCode)
        {
            //1
            case 2:
            {
                //Put the input into the queue
                this->inputQueue.push(static_cast<Key>((inputEvent->keyDown ? Key::Pressed : Key::Released) | Key::One));
            } break;

            //2
            case 3:
            {
                //Put the input into the queue
                this->inputQueue.push(static_cast<Key>((inputEvent->keyDown ? Key::Pressed : Key::Released) | Key::Two));
            } break;

            //3
            case 4:
            {
                //Put the input into the queue
                this->inputQueue.push(static_cast<Key>((inputEvent->keyDown ? Key::Pressed : Key::Released) | Key::Three));
            } break;

            //W
            case 17:
            {
                //Put the input into the queue
                this->inputQueue.push(static_cast<Key>((inputEvent->keyDown ? Key::Pressed : Key::Released) | Key::W));
            } break;

            //A
            case 30:
            {
                //Put the input into the queue
                this->inputQueue.push(static_cast<Key>((inputEvent->keyDown ? Key::Pressed : Key::Released) | Key::A));
            } break;

            //S
            case 31:
            {
                //Put the input into the queue
                this->inputQueue.push(static_cast<Key>((inputEvent->keyDown ? Key::Pressed : Key::Released) | Key::S));
            } break;

            //D
            case 32:
            {
                //Put the input into the queue
                this->inputQueue.push(static_cast<Key>((inputEvent->keyDown ? Key::Pressed : Key::Released) | Key::D));
            } break;

            //Z
            case 44:
            {
                //Put the input into the queue
                this->inputQueue.push(static_cast<Key>((inputEvent->keyDown ? Key::Pressed : Key::Released) | Key::Z));
            } break;
        }
    }
};
