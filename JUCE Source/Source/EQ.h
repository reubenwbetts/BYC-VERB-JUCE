/*
  ==============================================================================

    EQ.h
    Created: 18 Jun 2025 11:49:02pm
    Author:  admin

    *FIXED FREQUENCY CURVE FOR INPUT/LOOP SHAPING*
 
  ==============================================================================
*/

#pragma once
#include "filter_svf_biquad.h"

class corrective_eq
{
    public:

    float process_24k(float input);
        
    void notch_init();
    
    float notch_process(float input);
    
    
    private:


        // biquads would have made a much better choice as they're static but ah well

        svf filter_hs {1500.37, 0.707, 24000.0};
        svf filter_hs2 {2000.0, 0.303, 24000.0};


        svf filter_hmb {2103.0, 0.4, 24000.0};
        svf filter_hmb2 {2103.0, 0.4, 24000.0};

        svf filter_lmb {180.0, 0.4, 24000.0};
        svf filter_lmb2 {180.0, 0.4, 24000.0};


        svf filter_ls {150.0, 0.707, 24000.0};
        svf filter_ls2 {300.0, 0.303, 24000.0};
    
    
    
        biquad peak_notch_lm;
        biquad peak_notch_hm;

        float g_ls = 2.0;
        float g_hs = -1.0;
        float g_lmb = -0.1;
        float g_hmb = -1.0;
};
