#ifndef GGJ_2018_FLICK_H
#define GGJ_2018_FLICK_H
#pragma once

#include <chrono>
#include <ratio>

namespace GGJ
{
    using flick_t = std::chrono::duration<std::chrono::nanoseconds::rep, std::ratio<1, 705600000>>;
    using clock_t = std::chrono::high_resolution_clock;
};

#endif
