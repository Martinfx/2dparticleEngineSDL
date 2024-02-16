#pragma once
#include "stddef.h"


class Timer
{
public:

    Timer();

    void Start();
    uint32_t Read() const;
    float ReadSec() const;

private:
    uint32_t	started_at;
};
