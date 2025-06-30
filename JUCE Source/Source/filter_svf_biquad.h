/*
  ==============================================================================

    filter_svf_bquad.h
    Created: 18 Jun 2025 11:49:48pm
    Author:  admin

  ==============================================================================
*/

#pragma once
#include <cmath>
const float pi = 3.14159715;

class biquad
{
    public:
    
    void setPeakNotch(double freq, double gdb, double bw) {
        double SR = 24000.0;
        double k = std::pow(10.0, gdb / 20.0);
        double w = 2.0 * pi * freq / SR;
        double bwr = 2.0 * pi * bw / SR;
        double tan_bwr_2 = std::tan(bwr / 2.0);
        double abw = (1.0 - tan_bwr_2) / (1.0 + tan_bwr_2);
        double gain = 0.5 * (1.0 + k + abw - k * abw);

        b0 = gain;
        b1 = gain * (-2.0 * std::cos(w) * (1.0 + abw)) / (1.0 + k + abw - k * abw);
        b2 = gain * (abw + k * abw + 1.0 - k) / (abw - k * abw + 1.0 + k);

        a1 = -2.0 * std::cos(w) / (1.0 + tan_bwr_2);
        a2 = abw;

        x1 = x2 = y1 = y2 = 0.0;
    }

    double processSample(float x) {
        float y = b0 * x + b1 * x1 + b2 * x2
        - a1 * y1 - a2 * y2;

        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;

        return y;
    }

private:

    double b0 = 0, b1 = 0, b2 = 0;
    double a1 = 0, a2 = 0;

    double x1 = 0, x2 = 0;
    double y1 = 0, y2 = 0;
};


class svf
{
    public:
    svf(float cutoff,float res, float sample_rate)
    {
        fs = sample_rate;
            f = cutoff/fs;
            q = res;
            w = 2.0 * fs * tan(pi * f / fs);

            a = w / res;
            b = w * w;

            c1 = (a + b) / (1.0 + a / 2.0 + b / 4.0);
            c2 = b / (a + b);

            d0lpf = c1 * c2 / 4.0;
            d0hpf = 1.0 -d0lpf;
            d0bpf = (1.0-c2) * c1/2.0;
            d1bpf = 1 -c2;
    };

    void coeff_gen(float cutoff, float res, float sample_rate)
    {
        fs = sample_rate;
        f = cutoff/fs;
        q = res;
        w = 2.0 * fs * tan(pi * f / fs);

        a = w / res;
        b = w * w;

        c1 = (a + b) / (1.0 + a / 2.0 + b / 4.0);
        c2 = b / (a + b);

        d0lpf = c1 * c2 / 4.0;
        d0hpf = 1.0 -d0lpf;
        d0bpf = (1.0-c2) * c1/2.0;
        d1bpf = 1 -c2;

    }

    float process_lpf(float input)
    {
        float x = input - z1_lpf - z2_lpf;
        z2_lpf += c2 * z1_lpf;
        float y = d0lpf * x + z2_lpf;
        z1_lpf += c1 * x;
        return y + 1e-20f - 1e-20f;
    }

    float process_hpf(float input)
    {
        float x = input - z1_hpf - z2_hpf;
        float y = d0hpf * x;
        z2_hpf += c2 * z1_hpf;
        z1_hpf += c1 * x;
        return y + 1e-20f - 1e-20f;
    }

     float process_bpf(float input)
    {
        float x = input - z1_bpf - z2_bpf;
        float y = d0bpf * x + d1 * z1_bpf;
        z2_bpf += c2 * z1_bpf;
        z1_bpf += c1 * x;
        return y + 1e-20f - 1e-20f;
    }

    private:
    
    float fs = 0.0;
    float q = 0.0;
    float f = 0.0;
    float w = 0.0;
    float a = 0.0;
    float b = 0.0;
    float c1 = 0.0;
    float c2 = 0.0;
    float z1_lpf = 0.0;
    float z2_lpf = 0.0;
    float z1_hpf = 0.0;
    float z2_hpf = 0.0;
    float z1_bpf = 0.0;
    float z2_bpf = 0.0;

    float d0lpf = 0.0;
    float d0hpf = 0.0;
    float d0bpf = 0.0;
    float d1bpf = 0.0;
    float d1 = 0.0;
    float d2 = 0.0;
    
};
