/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "late_diffusion.h"
#include "downsample.h"
#include "early_diffusion.h"
#include "delay_buffer.h"
#include "EQ.h"

//==============================================================================
/**
*/
class BYC_VRBAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BYC_VRBAudioProcessor();
    ~BYC_VRBAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState apvts;


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BYC_VRBAudioProcessor)
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    
    reverb reverb_tank;
    early_diffusion early_diffusion_L;
    early_diffusion early_diffusion_R;
    
    downsample down_sampler_l;
    downsample down_sampler_r;
    
    delay_buffer delay_comp_l;
    delay_buffer delay_comp_r;
    
    corrective_eq mid_cut_l;
    corrective_eq mid_cut_r;
    
    
};
