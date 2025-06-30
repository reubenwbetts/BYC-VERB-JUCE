/*
  ==============================================================================

    delay_buffer.h
    Created: 18 Jun 2025 5:21:35pm
    Author:  admin

  ==============================================================================
*/

#pragma once

class delay_buffer
{
    public:
    
    float delay(float input_sample)
    {
        int delay_0 = static_cast<int>(pre_delay_length);
        
        int read_idx_0 = pre_delay_write_idx - delay_0;
        if (read_idx_0 < 0){
            read_idx_0 += pre_delay_size;
        }
        
        float delayed_0 = pre_delay_buff[read_idx_0];
        
        float pre_delay_out = delayed_0;
        
        pre_delay_buff[pre_delay_write_idx] = input_sample;
        
        pre_delay_write_idx = (pre_delay_write_idx + 1) % pre_delay_size;
        
        return pre_delay_out;
    }
    
    private:
    
    float pre_delay_buff[238] = {0.0};
    uint16_t pre_delay_size = 238;
    float pre_delay_length = 0.0;
    uint16_t pre_delay_write_idx = 0;

    
    };
