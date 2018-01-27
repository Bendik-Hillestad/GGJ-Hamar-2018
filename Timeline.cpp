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
        //Push timestamp
        this->timestamps.push_back(timestamp);

        //Push keystroke
        this->keystrokes.push_back(keystroke);
    }

    void Timeline::EraseAfter(flick_t timestamp) noexcept
    {

    }

    Timeline::iterator Timeline::GetKeystrokesInRange(flick_t begin, flick_t end) noexcept
    {
        return this->keystrokes.begin();
    }
};