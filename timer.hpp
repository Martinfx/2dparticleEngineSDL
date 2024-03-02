#pragma once
#include <cstdint>


class Timer
{
public:

    Timer();

    void Start();
    std::uint32_t Read() const;
    float ReadSec() const;

private:
    std::uint32_t	started_at;
};
