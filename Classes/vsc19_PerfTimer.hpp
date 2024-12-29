
#pragma once

// C++ 17 chrono
#include <chrono>

namespace vsc19
{
    /**@brief Measuring function execution time with a standard clock*/
    template<typename Time = std::chrono::microseconds,
    typename Clock = std::chrono::high_resolution_clock>
    struct perfTime {
        template<typename F, typename... Args>
        static Time duration(F&& f, Args.. args)
        {
            auto start = Clock::now();

            std::invoke(std::forward<F>(f), std::forward<Args>(args)...);

            auto end = Clock::now();

            return std::chrono::duration_cast<Time>(end-start);
        }
    };
} // End of namespace