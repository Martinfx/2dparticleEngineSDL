#pragma once

#include <cstdint>

class PerfTimer
{
public:

    PerfTimer();

    void Start();
    double ReadMs() const;
    std::uint64_t ReadTicks() const;

private:
    std::uint64_t	started_at;
    static std::uint64_t frequency;
};
