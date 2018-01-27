#ifndef GGJ_2018_RANGE_H
#define GGJ_2018_RANGE_H
#pragma once

#include <cstddef>

namespace GGJ
{
    namespace detail
    {
        template<typename T>
        class Range final
        {
        public:
            Range(T const &begin, T const &end) noexcept : b{ begin }, e{ end }
            {}

            T begin() const noexcept { return this->b; }
            T end  () const noexcept { return this->e;   }

        private:
            T const b;
            T const e;
        };
    };
    
    template<typename T>
    constexpr auto make_range(T const &container, std::size_t offset, std::size_t len)
    {
        return detail::Range<typename T::const_iterator>(container.begin() + offset, container.begin() + offset + len);
    }
};

#endif
