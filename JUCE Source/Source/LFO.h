/*
  ==============================================================================

    LFO.h
    Created: 10 Jun 2025 12:52:10pm
    Author:  admin

  ==============================================================================
*/

#pragma once
#include <cmath>

class LFO
{
public:

    LFO(float rate, float depth)
    {r = rate/24000.0;
    d = depth;};

    uint8_t process(){
        p += r;
        if(p > 1) p -= 2;
        uint8_t out = floorf(4*p*(1-fabs(p))*d);
        return floorf(out);
    }


private:
    float r;
    float d;

    float p = 0.0;

};


