#ifndef RANDOM_H
#define RANDOM_H

#include <chrono>
#include <random>
namespace Random {
    inline std::mt19937 generate()
    {
        std::random_device rd{};
        
        std::seed_seq ss{
            static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
                rd()};

        return std::mt19937{ ss };
    }


    inline std::mt19937 mt{ generate() };

    template <typename T>
    T getRand(T min, T max)
    {
        return std::uniform_real_distribution<T>{min, max}(mt);
    }
}

#endif RANDOM_H