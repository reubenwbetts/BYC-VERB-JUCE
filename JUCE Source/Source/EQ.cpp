/*
  ==============================================================================

    EQ.cpp
    Created: 18 Jun 2025 11:49:02pm
    Author:  admin
 
    *FIXED FREQUENCY CURVE FOR INPUT/LOOP SHAPING*

  ==============================================================================
*/

#include "EQ.h"


float corrective_eq::process_24k(float input)
{
    
    {
        float audio_out = filter_ls.process_lpf(input * g_ls) + input;
        audio_out = filter_ls2.process_lpf(input * g_ls) + input;

        audio_out = filter_lmb.process_bpf(audio_out * g_lmb) + audio_out;
        audio_out = filter_lmb.process_lpf(filter_lmb.process_hpf(audio_out* g_lmb)) + audio_out;

        audio_out = filter_hmb.process_bpf(audio_out * g_hmb) + audio_out;
        audio_out = filter_hmb.process_lpf(filter_hmb.process_hpf(audio_out* g_hmb)) + audio_out;

        
        audio_out = tanh(audio_out);
        return audio_out;
    }
    return input;
};

void corrective_eq::notch_init()
{
    peak_notch_lm.setPeakNotch(60.0, -20.0, 400.0);
    peak_notch_hm.setPeakNotch(3400.0, -20.0, 2400.0);
    
};

float corrective_eq::notch_process(float input)
{
    float output = peak_notch_lm.processSample(input);
    output = peak_notch_hm.processSample(output);
    return output;
}

