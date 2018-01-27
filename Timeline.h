#ifndef GGJ_2018_TIMELINE_H
#define GGJ_2018_TIMELINE_H
#pragma once

#include "Flick.h"
#include "Range.h"

#include <cstddef>
#include <vector>

namespace GGJ
{
    using keystroke_t = int;

    class Timeline final
    {
        using iterator = detail::Range<std::vector<keystroke_t>::const_iterator>;

    public:
        Timeline() noexcept;

        void     AddKeystroke        (flick_t timestamp, keystroke_t keystroke) noexcept;
        void     EraseAfter          (flick_t timestamp)                        noexcept;
        iterator GetKeystrokesInRange(flick_t begin, flick_t end)               noexcept; //The range is like (begin, end]

    private:
        std::vector<flick_t>     timestamps;
        std::vector<keystroke_t> keystrokes;
    };
};

#endif
