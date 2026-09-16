#ifndef RANDOM_H
#define RANDOM_H

#include <chrono>
#include <random>
#include <algorithm>
#include <limits>

namespace Random {
    inline std::mt19937 generate()
    {
        std::random_device rd{};
        
        std::seed_seq ss{
            static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
                rd()};

        return std::mt19937{ ss };
    }


    inline thread_local std::mt19937 mt{ generate() };

    template <typename T>
    T getRandUnit(){
        constexpr int bits = std::min(std::numeric_limits<T>::digits, 32);
        constexpr T scale = T(1) / static_cast<T>(1ULL << bits);
        constexpr int shift = (32 - bits);
        
        return (mt() >> shift) * scale;
    }

    template <typename T>
    T getRand(T min, T max)
    {
        return min + (max - min) * getRandUnit<T>();
    }
}

#endif