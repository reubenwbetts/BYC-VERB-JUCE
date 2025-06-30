/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeelCustom.h"

//==============================================================================
/**
*/
class BYC_VRBAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BYC_VRBAudioProcessorEditor (BYC_VRBAudioProcessor&);
    ~BYC_VRBAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    LookAndFeelCustom LookAndFeelCustom;
    
    juce::Slider decayKnob;
    juce::Slider gainKnob;
    juce::Slider dampKnob;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffKnobAttatchment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainKnobAttatchment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampKnobAttatchment;
    
    BYC_VRBAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BYC_VRBAudioProcessorEditor)
};
