/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BYC_VRBAudioProcessorEditor::BYC_VRBAudioProcessorEditor (BYC_VRBAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
   // auto futuraMedium = juce::Typeface::createSystemTypefaceFor (BinaryData::FuturaMedium01_ttf, BinaryData::FuturaMedium01_ttfSize);
    
    //juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface (futuraMedium);
    
    decayKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decayKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 50);
    decayKnob.setLookAndFeel(&LookAndFeelCustom);
    addAndMakeVisible(decayKnob);
    
    gainKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 50);
    gainKnob.setLookAndFeel(&LookAndFeelCustom);
    addAndMakeVisible(gainKnob);
    
    dampKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    dampKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 50);
    dampKnob.setLookAndFeel(&LookAndFeelCustom);
    addAndMakeVisible(dampKnob);
    
    cutoffKnobAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DECAY", decayKnob);
    gainKnobAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BLEND", gainKnob);
    dampKnobAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DAMP", dampKnob);
  
    setSize (450, 561/2);
}

BYC_VRBAudioProcessorEditor::~BYC_VRBAudioProcessorEditor()
{
}

//==============================================================================
void BYC_VRBAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::transparentBlack );
    
    auto new_background = juce::ImageCache::getFromMemory (BinaryData::new_bg_png, BinaryData::new_bg_pngSize);
    
    g.drawImage(new_background, 0, 0, 900/2, 561/2, 0, 0, 900, 561);

}

void BYC_VRBAudioProcessorEditor::resized()
{
    decayKnob.setBounds((654-209)/2, (318 - 206)/2,418/2, 418/2);
    
    gainKnob.setBounds(72/2, 130/2,120/2,360/2);
    
    dampKnob.setBounds(299/2, 130/2,120/2,360/2);

    }
