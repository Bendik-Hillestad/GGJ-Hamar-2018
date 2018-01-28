#include "Timeline.h"

#include <cstdio>

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

        std::printf("%lld: %d\n", timestamp.count(), keystroke);
    }

    void Timeline::EraseAfter(flick_t timestamp) noexcept
    {
        std::size_t idx = 0;

        //Iterate until we find the point to erase from
        for (std::size_t i = 0; i < this->timestamps.size(); i++)
        {
            //Check if it's greater or equal to the timestamp
            if (this->timestamps[i] >= timestamp)
            {
                //Erase from here
                idx = i;
                break;
            }
        }

        //Erase data from vectors
        this->timestamps.erase(this->timestamps.begin() + idx);
        this->keystrokes.erase(this->keystrokes.begin() + idx);
    }

    Timeline::iterator Timeline::GetKeystrokesInRange(flick_t begin, flick_t end) noexcept
    {
        std::size_t idxBegin = 0;
        std::size_t idxEnd   = 0;

        //Iterate until we find the beginning spot
        for (std::size_t i = 0; i < this->timestamps.size(); i++)
        {
            //Check if it's greater than the begin time
            if (this->timestamps[i] > begin)
            {
                //Start from here
                idxBegin = i;
                break;
            }
        }

        //Iterate until we find the end spot
        for (std::size_t i = idxBegin; i < this->timestamps.size(); i++)
        {
            //Check if it's less than or equal to the end time
            if (this->timestamps[i] <= end)
            {
                //End here
                idxEnd = i;
                break;
            }
        }

        //Return iterator
        return make_range(this->keystrokes, idxBegin, idxEnd - idxBegin);
    }
};