#include "Timeline.h"

namespace GGJ
{
    Timeline::Timeline() noexcept
    {
        this->timestamps = std::vector<flick_t>{};
        this->keystrokes = std::vector<keystroke_t>{};
    }

    void Timeline::AddKeystroke(flick_t timestamp, keystroke_t keystroke) noexcept
    {

    }

    void Timeline::EraseAfter(flick_t timestamp) noexcept
    {

    }

    Timeline::iterator Timeline::GetKeystrokesInRange(flick_t begin, flick_t end) noexcept
    {

    }
};