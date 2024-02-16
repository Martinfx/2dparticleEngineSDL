#pragma once

#include <stddef.h>

class PerfTimer
{
public:

    PerfTimer();

    void Start();
    double ReadMs() const;
    uint64_t ReadTicks() const;

private:
    uint64_t	started_at;
    static uint64_t frequency;
};
