/*
  ==============================================================================

    Reverb.h
    Created: 10 Jun 2025 12:52:03pm
    Author:  admin

  ==============================================================================
*/


#include "LFO.h"
#include "filter_svf_biquad.h"
#include "EQ.h"

class reverb{

    public:
    
    reverb()
    {
        mid_cut_fb.notch_init();
    }

    void set_decay(float decay_val)
    {
        decay = decay_val * 0.5 + 0.499;
    }
    


    void set_damp(float damp_val)
    {
        damp_factor = (damp_val * -0.75) + 0.999;
    }

    void set_mod(float mod_val)
    {
        mod = (mod_val * 16);

    }

        void process(float input_L, float input_R)

        {
            // APF1 (nodes 23-24) --------------------------------------------

            float tank_1_in = lpf_1.process_lpf(hpf_1.process_hpf(input_R) + delay_line_4_out); // Inject input and feedback signal into tank

            int delay_1 = static_cast<int>(apf1_delay);

            int read_idx_1 = apf1_write_idx - delay_1 + lfo1.process(); // Ensure write index does not go below 0
            if (read_idx_1 < 0){
                read_idx_1 += apf1_size;
            }

            float delayed_1 = apf1[read_idx_1];

            float apf1_out = input_diffuse_fb_1 * tank_1_in + delayed_1; // Positive feedback

            apf1[apf1_write_idx] = tank_1_in - input_diffuse_fb_1 * apf1_out; // Negative feed-forward

            apf1_write_idx = (apf1_write_idx + 1) % apf1_size; // Modulo increment (avoids wrap around)

            //return apf1_out;

            // DELAY LINE 1 (nodes 24-30) --------------------------------------------

            int delay_2 = static_cast<int>(delay_line_1_delay);

            int read_idx_2 = delay_line_1_write_idx - delay_2;
            if (read_idx_2 < 0){
                read_idx_2 += delay_line_1_size;
            }

            float delayed_2 = delay_line_1[read_idx_2];

            float delay_line_1_out = delayed_2;

            delay_line_1[delay_line_1_write_idx] =  apf1_out;

            delay_line_1_write_idx = (delay_line_1_write_idx + 1) % delay_line_1_size;

            //return delay_line_1_out;

            // DAMPING FILTER (nodes 30-31) ----------------------------

            float sum_in = damp_factor * damp_filt1_delay + (delay_line_1_out * (1.0 - damp_factor)); // prevout = out

            damp_filt1_delay = sum_in;

            // DECAY CONTROL 1 -------------------------------

            float damp_1_out = sum_in * decay;

            // APF2 ---------------------------------------------

            int delay_3 = static_cast<int>(apf2_delay);

            int read_idx_3 = apf2_write_idx - delay_3 + lfo2.process();
            if (read_idx_3 < 0){
                read_idx_3 += apf2_size;
            }

            float delayed_3 = apf2[read_idx_3];

            float apf3_out = -input_diffuse_fb_2 * damp_1_out + delayed_3;

            apf2[apf2_write_idx] = damp_1_out + input_diffuse_fb_2 * apf3_out;

            apf2_write_idx = (apf2_write_idx + 1) % apf2_size;

            //return apf3_out;

            // DELAY LINE 2 --------------------------------------------

            int delay_4 = static_cast<int>(delay_line_2_delay);

            int read_idx_4 = delay_line_2_write_idx - delay_4;
            if (read_idx_4 < 0){
                read_idx_4 += delay_line_2_size;
            }

            float delayed_4 = delay_line_2[read_idx_4];

            float delay_line_2_out = delayed_4;

            delay_line_2[delay_line_2_write_idx] = apf3_out;

            delay_line_2_write_idx = (delay_line_2_write_idx + 1) % delay_line_2_size;

            delay_line_2_out *= decay;

            // APF3 --------------------------------------------

            // Tank 2 refers to the right side of the loop

            float tank_2_input = lpf_2.process_lpf(delay_line_2_out + hpf_2.process_hpf(input_L)); // Inject input signal into tank

            int delay_5 = static_cast<int>(apf3_delay);

            int read_idx_5 = apf3_write_idx - delay_5 + lfo3.process();
            if (read_idx_5 < 0){
                read_idx_5 += apf3_size;
            }

            float delayed_5 = apf3[read_idx_5];

            float apf_3_out = input_diffuse_fb_1 * tank_2_input + delayed_5;

            apf3[apf3_write_idx] = tank_2_input - input_diffuse_fb_1 * apf_3_out;

            apf3_write_idx = (apf3_write_idx + 1) % apf3_size;

            //return apf3_out;

            // DELAY LINE 3 --------------------------------------------

            int delay_6 = static_cast<int>(delay_line_3_delay);

            int read_idx_6 = delay_line_3_write_idx - delay_6;
            if (read_idx_6 < 0){
                read_idx_6 += delay_line_3_size;
            }

            float delayed_6 = delay_line_3[read_idx_6];

            float delay_line_3_out = delayed_6;

            delay_line_3[delay_line_3_write_idx] =  apf_3_out;

            delay_line_3_write_idx = (delay_line_3_write_idx + 1) % delay_line_3_size;

            //return delay_line_3_out;

            // DAMPING FILTER 2 ----------------------------

            float sum_in_2 = damp_factor * damp_filt2_delay + (delay_line_3_out * (1.0 - damp_factor)); // prevout = out

            damp_filt2_delay = sum_in_2;

            // DECAY CONTROL 3 -------------------------------

            float damp_2_out = sum_in_2 * decay;

            // APF4 ---------------------------------------------

            int delay_7 = static_cast<int>(apf4_delay);

            int read_idx_7 = apf4_write_idx - delay_7 + lfo4.process();
            if (read_idx_7 < 0){
                read_idx_7 += apf4_size;
            }

            float delayed_7 = apf4[read_idx_7];

            float apf_4_out = -input_diffuse_fb_2 * damp_2_out + delayed_7;

            apf4[apf4_write_idx] = damp_2_out + input_diffuse_fb_2 * apf_4_out;

            apf4_write_idx = (apf4_write_idx + 1) % apf4_size;

            //return apf3_out;

            // DELAY LINE 4 --------------------------------------------

            int delay_8 = static_cast<int>(delay_line_4_delay);

            int read_idx_8 = delay_line_4_write_idx - delay_8;
            if (read_idx_8 < 0){
                read_idx_8 += delay_line_4_size;
            }

            float delayed_8 = delay_line_4[read_idx_8];

            delay_line_4_out = delayed_8;

            delay_line_4[delay_line_4_write_idx] = apf_4_out;

            delay_line_4_write_idx = (delay_line_4_write_idx + 1) % delay_line_4_size;

            //DECAY CONTROL 4 ----------------------------------------------------------------

            delay_line_4_out *= decay; // This sample gets fed-back into the tank at the start

            // OUTPUT SECTION ================================================================
            // Sum all output taps into L/R wet signals

            int tap_point = 0; // buffer read index for grabbing outs

            // LEFT (all values taken from Dattoro's paper) ----------------------------------

            tap_point = delay_line_3_write_idx - 266;
            if (tap_point < 0) tap_point += delay_line_3_size;
            wet_L = 0.6 * delay_line_3[tap_point];

            tap_point = delay_line_3_write_idx - 2974;
            if (tap_point < 0) tap_point += delay_line_3_size;
            wet_L += 0.6 * delay_line_3[tap_point];

            tap_point = apf4_write_idx - 1913;
            if (tap_point < 0) tap_point += apf4_size;
            wet_L -= 0.6 * apf4[tap_point];

            tap_point = delay_line_4_write_idx - 1996;
            if (tap_point < 0) tap_point += delay_line_4_size;
            wet_L += 0.6 * delay_line_4[tap_point];

            tap_point = delay_line_1_write_idx - 1990;
            if (tap_point < 0) tap_point += delay_line_1_size;
            wet_L -= 0.6 * delay_line_1[tap_point];

            tap_point = apf2_write_idx - 187;
            if (tap_point < 0) tap_point += apf2_size;
            wet_L -= 0.6 * apf2[tap_point];

            tap_point = delay_line_2_write_idx - 1066;
            if (tap_point < 0) tap_point += delay_line_2_size;
            wet_L -= 0.6 * delay_line_2[tap_point];

            // RIGHT (all values taken from Dattoro's paper) ----------------------------------

            tap_point = delay_line_1_write_idx - 353;
            if (tap_point < 0) tap_point += delay_line_1_size;
            wet_R = 0.6 * delay_line_1[tap_point];

            tap_point = delay_line_1_write_idx - 3627;
            if (tap_point < 0) tap_point += delay_line_1_size;
            wet_R += 0.6 * delay_line_1[tap_point];

            tap_point = apf2_write_idx - 1228;
            if (tap_point < 0) tap_point += apf2_size;
            wet_R -= 0.6 * apf2[tap_point];

            tap_point = delay_line_2_write_idx - 2673;
            if (tap_point < 0) tap_point += delay_line_2_size;
            wet_R += 0.6 * delay_line_2[tap_point];

            tap_point = delay_line_3_write_idx - 2111;
            if (tap_point < 0) tap_point += delay_line_3_size;
            wet_R -= 0.6 * delay_line_3[tap_point];

            tap_point = apf4_write_idx - 335;
            if (tap_point < 0) tap_point += apf4_size;
            wet_R -= 0.6 * apf4[tap_point];

            tap_point = delay_line_4_write_idx - 121;
            if (tap_point < 0) tap_point += delay_line_4_size;
            wet_R -= 0.6 * delay_line_4[tap_point];

        }

        // Send summed taps out (call after process)

        float out_L(){return wet_L;}
        float out_R(){return wet_R;}

    private:

        // Buffers (all-pass and delay lines)

        float apf1[673+16] = {0.0};
        uint16_t apf1_size = 673+16;
        float apf1_delay = 672.0 + 16.0;
        uint16_t apf1_write_idx = 0;

        float delay_line_1[4454] = {0.0};
        uint16_t delay_line_1_size = 4454;
        float delay_line_1_delay = 4453;
        uint16_t delay_line_1_write_idx = 0;

        float apf2[1801+16] = {0.0};
        uint16_t apf2_size = 1801+16;
        float apf2_delay = 1800.0+ 16.0;
        uint16_t apf2_write_idx = 0;

        float delay_line_2[3721] = {0.0};
        uint16_t delay_line_2_size = 3721;
        float delay_line_2_delay = 3720.0;
        uint16_t delay_line_2_write_idx = 0;

        float apf3[909+16] = {0.0};
        uint16_t apf3_size = 909+16;
        float apf3_delay = 908+ 16.0;
        uint16_t apf3_write_idx = 0;

        float delay_line_3[4213] = {0.0};
        uint16_t delay_line_3_size = 4213;
        float delay_line_3_delay = 4212.0;
        uint16_t delay_line_3_write_idx = 0;

        float apf4[2657+16] = {0.0};
        uint16_t apf4_size = 2657+16;
        float apf4_delay = 2656.0+ 16.0;
        uint16_t apf4_write_idx = 0;

        float delay_line_4[3164] = {0.0};
        uint16_t delay_line_4_size = 3164;
        float delay_line_4_delay = 3163.0;
        uint16_t delay_line_4_write_idx = 0;

        // Assorted coeffs (taken from datorros paper)

        float input_diffuse_fb_1 = 0.700;
        float input_diffuse_fb_2 = 0.625;

        float damp_factor = 0.0005;

        float decay = 0.4;

        // Initialisation values

        float delay_line_4_out = 0.0;

        float damp_filt1_delay = 0.0;
        float damp_filt2_delay = 0.0;

        float wet_L = 0.0;
        float wet_R = 0.0;

        float mod = 40.0;

        LFO lfo1{0.13,mod};
        LFO lfo2{0.27,mod};
        LFO lfo3{0.67,mod};
        LFO lfo4{0.06,mod};

    svf hpf_1{80.0f, 0.707f, 24000.0f};
    svf hpf_2{80.0f, 0.707f, 24000.0f};
    svf lpf_1{5000.0f, 0.707f, 24000.0f};
    svf lpf_2{5000.0f, 0.707f, 24000.0f};

    corrective_eq mid_cut_fb;



    };
