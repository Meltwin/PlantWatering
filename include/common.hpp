//
// Created by meltwin on 18/12/24.
//

#ifndef COMMON_HPP
#define COMMON_HPP

// Custom types
#define constchar static constexpr const char*
typedef uint16_t PWMValue;
typedef uint32_t TimeValue;

// Time utils
constexpr TimeValue S_2US{1000000};
constexpr TimeValue S_2MS{1000};
constexpr TimeValue M_2_US{60 * S_2US};
constexpr TimeValue H_2_US{60 * M_2_US};

#endif // COMMON_HPP
