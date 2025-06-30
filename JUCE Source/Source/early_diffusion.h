/*
  ==============================================================================

    Early_diffusion.h
    Created: 10 Jun 2025 2:11:05pm
    Author:  admin

  ==============================================================================
*/

#pragma once
#include "filter_svf_biquad.h"

class early_diffusion{

public:

    void set_bandwidth(float bw)
    {
        bandwidth = (bw * 0.75);
    }
    
    void set_damping(float damp_val)
    {
        damp_factor = (damp_val * -0.75) + 0.999;
    }
    void set_pre_delay(float delay, float sample_rate)
    {
        pre_delay_length = delay * sample_rate;
        if (pre_delay_length > 48000)
        {
            pre_delay_length = 48000;
        }
    }

    float process(float input)

    {
 
        // PRE DELAY -------------------------------------
        int delay_0 = static_cast<int>(pre_delay_length);

        int read_idx_0 = pre_delay_write_idx - delay_0;
        if (read_idx_0 < 0){
            read_idx_0 += pre_delay_size;
        }

        float delayed_0 = pre_delay_buff[read_idx_0];

        float pre_delay_out = delayed_0;

        pre_delay_buff[pre_delay_write_idx] = input;

        pre_delay_write_idx = (pre_delay_write_idx + 1) % pre_delay_size;
        
        // INPUT LPF (DAMPING FILTER)
        
        float lpf_out = damp_factor * damp_filt_2_delay + (pre_delay_out * (1.0 - damp_factor)); // prevout = out

        damp_filt_2_delay = lpf_out;


        // BANDWIDTH (DAMPING FILTER) ----------------------

        float sum_in = -bandwidth * damp_filt_delay + lpf_out; // prevout = out

        damp_filt_delay = sum_in;

        float damp_filt_out = lpf_out;

        // APF1 --------------------------------------------

        int delay_1 = static_cast<int>(apf1_delay);

        int read_idx_1 = apf1_write_idx - delay_1;
        if (read_idx_1 < 0){
            read_idx_1 += apf1_size;
        }

        float delayed_1 = apf1[read_idx_1];

        float apf1_out = -input_diffuse_fb_1 * damp_filt_out + delayed_1;

        apf1[apf1_write_idx] = damp_filt_out + input_diffuse_fb_1 * apf1_out;

        apf1_write_idx = (apf1_write_idx + 1) % apf1_size;

        //return apf1_out;

        // APF2 --------------------------------------------

        int delay_2 = static_cast<int>(apf2_delay);

        int read_idx_2 = apf2_write_idx - delay_2;
        if (read_idx_2 < 0){
            read_idx_2 += apf2_size;
        }

        float delayed_2 = apf2[read_idx_2];

        float apf2_out = -input_diffuse_fb_1 * apf1_out + delayed_2;

        apf2[apf2_write_idx] =  apf1_out + input_diffuse_fb_1 * apf2_out;

        apf2_write_idx = (apf2_write_idx + 1) % apf2_size;

        //return apf2_out;

        // APF3 ---------------------------------------------

        int delay_3 = static_cast<int>(apf3_delay);

        int read_idx_3 = apf3_write_idx - delay_3;
        if (read_idx_3 < 0){
            read_idx_3 += apf3_size;
        }

        float delayed_3 = apf3[read_idx_3];

        float apf3_out = -input_diffuse_fb_2 * apf2_out + delayed_3;

        apf3[apf3_write_idx] = apf2_out + input_diffuse_fb_2 * apf3_out;

        apf3_write_idx = (apf3_write_idx + 1) % apf3_size;

        //return apf3_out;

        // APF4 --------------------------------------------

        int delay_4 = static_cast<int>(apf4_delay);

        int read_idx_4 = apf4_write_idx - delay_4;
        if (read_idx_4 < 0){
            read_idx_4 += apf4_size;
        }

        float delayed_4 = apf4[read_idx_4];

        float apf4_out = -input_diffuse_fb_2 * apf3_out + delayed_4;

        apf4[apf4_write_idx] = apf3_out + input_diffuse_fb_2 * apf4_out;

        apf4_write_idx = (apf4_write_idx + 1) % apf4_size;

        return apf4_out;

    }

private:

    // all-pass filter buffers (taken from datorros paper)

    float pre_delay_buff[48000] = {0.0};
    uint16_t pre_delay_size = 48000;
    float pre_delay_length = 0.0;
    uint16_t pre_delay_write_idx = 0;

    float apf1[143] = {0.0};
    uint16_t apf1_size = 143;
    float apf1_delay = 142;
    uint16_t apf1_write_idx = 0;

    float apf2[108] = {0.0};
    uint16_t apf2_size = 108;
    float apf2_delay = 107;
    uint16_t apf2_write_idx = 0;

    float apf3[380] = {0.0};
    uint16_t apf3_size = 380;
    float apf3_delay = 379;
    uint16_t apf3_write_idx = 0;

    float apf4[278] = {0.0};
    uint16_t apf4_size = 278;
    float apf4_delay = 277;
    uint16_t apf4_write_idx = 0;

    // fb coeffs (taken from datorros paper)

    float input_diffuse_fb_1 = 0.700;
    float input_diffuse_fb_2 = 0.625;

    float bandwidth = 0.45;
    float damp_factor = 0.25;
    float damp_filt_delay = 0.0;
    float damp_filt_2_delay = 0.0;
    

    

};
