/*
  ==============================================================================

    downsample.h
    Created: 15 Jun 2025 9:13:39pm
    Author:  admin

  ==============================================================================
*/

#include "fir_test.h"
#pragma once

class downsample
{
    public:
    
    void process_down_sample(float* input_buffer, int input_buffer_length, float* down_sampled_buffer)
    {

        for (int i = 0; i < input_buffer_length; ++i)
        {
            input_buffer[i] = fir_test_1.fir_test(input_buffer[i]);
        }

        int downsampled_index = 0;
        for (int i = 0; i < input_buffer_length; i += 2)
        {
            down_sampled_buffer[downsampled_index++] = input_buffer[i];
        }
    }
    
    void process_up_sample(float* output_buffer, int output_buffer_length, float* down_sampled_buffer)
    {
        
        for (int i = 0; i < output_buffer_length / 2; i++)
        {
            output_buffer[i*2] = down_sampled_buffer[i];
            
            output_buffer[i * 2 + 1] = 0.0f;
        }
        
        for (int i = 0; i < output_buffer_length; ++i)
        {
            output_buffer[i] = fir_test_2.fir_test(output_buffer[i]);
        }
    }
    
    private:
    
    fir_example_custom fir_test_1;
    fir_example_custom fir_test_2;
    
   

};
