/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BYC_VRBAudioProcessor::BYC_VRBAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
    .withInput  ("Input", juce::AudioChannelSet::stereo(), true)
    .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
), apvts (*this, nullptr, "Parameters", createParameters())
#endif
{
}


BYC_VRBAudioProcessor::~BYC_VRBAudioProcessor()
{
}

//==============================================================================
const juce::String BYC_VRBAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BYC_VRBAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BYC_VRBAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BYC_VRBAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BYC_VRBAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BYC_VRBAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BYC_VRBAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BYC_VRBAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BYC_VRBAudioProcessor::getProgramName (int index)
{
    return {};
}

void BYC_VRBAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BYC_VRBAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mid_cut_l.notch_init();
    mid_cut_r.notch_init();
}

void BYC_VRBAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BYC_VRBAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Only allow stereo in and stereo out
    const auto& mainIn = layouts.getMainInputChannelSet();
    const auto& mainOut = layouts.getMainOutputChannelSet();
    
    return mainIn == juce::AudioChannelSet::stereo()
        && mainOut == juce::AudioChannelSet::stereo();
}
#endif

void BYC_VRBAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++)
    {
        buffer.clear(i,0,buffer.getNumSamples());
    }
    float sr = getSampleRate();
    
    //-----------------------------------
    // read params
    std::atomic<float>* decay_value_ptr = apvts.getRawParameterValue("DECAY");
    
    if (decay_value_ptr != nullptr)
    {
        float decay_value = decay_value_ptr->load();
        reverb_tank.set_decay(decay_value);

          
    }
    
    std::atomic<float>* damp_value_ptr = apvts.getRawParameterValue("DAMP");

    if (damp_value_ptr != nullptr)
    {
        float damp_value = damp_value_ptr->load();
        reverb_tank.set_damp(damp_value);
        early_diffusion_L.set_damping(damp_value);
        early_diffusion_R.set_damping(damp_value);
       
          
    }
    
    std::atomic<float>* blend_value_ptr = apvts.getRawParameterValue("BLEND");
    float blend_value = 0.0;
    
    if (blend_value_ptr != nullptr)
    {
        blend_value = blend_value_ptr->load();
    }

    float blend_value_dry = blend_value * -1.0 + 1.0;
    
    // ---------------------------------------
    
    // audio processing
    auto* leftData  = buffer.getWritePointer (0);
    auto* rightData = buffer.getWritePointer (1);

    auto num_samples = buffer.getNumSamples();
    
    std::vector<float> out_l(buffer.getNumSamples());
    std::vector<float> out_r(buffer.getNumSamples());
    
    float* out_l_ptr = out_l.data();
    float* out_r_ptr = out_r.data();
    
    for (int i = 0; i < num_samples; i++)
    {
        out_l_ptr[i] = delay_comp_r.delay(leftData[i]);
        out_r_ptr[i] = delay_comp_l.delay(rightData[i]);
        
    }
 
    
    std::vector<float> down_sampled_buffer_l(buffer.getNumSamples()/2);
    std::vector<float> down_sampled_buffer_r(buffer.getNumSamples()/2);
    
    float* down_sampled_buffer_ptr_l = down_sampled_buffer_l.data();
    float* down_sampled_buffer_ptr_r = down_sampled_buffer_r.data();
    
    down_sampler_l.process_down_sample(leftData, num_samples, down_sampled_buffer_ptr_l);
    down_sampler_r.process_down_sample(rightData, num_samples, down_sampled_buffer_ptr_r);
    
    for (int i = 0; i < num_samples/2; ++i)
    {
        float left_channel = down_sampled_buffer_l[i]*2.0f;
        float right_channel = down_sampled_buffer_r[i]*2.0f;
        left_channel = early_diffusion_L.process(left_channel);
        right_channel = early_diffusion_R.process(right_channel);
        float early_diffuse_L = left_channel;
        float early_diffuse_R = right_channel;
        
        reverb_tank.process(left_channel, right_channel);
        down_sampled_buffer_l[i] = reverb_tank.out_L()*0.5+ early_diffuse_L * 0.5;
        down_sampled_buffer_r[i] = reverb_tank.out_R()*0.5 + early_diffuse_R * 0.5;
        
        down_sampled_buffer_l[i] = mid_cut_l.notch_process(down_sampled_buffer_l[i])*2.5f;
        down_sampled_buffer_r[i] = mid_cut_r.notch_process(down_sampled_buffer_r[i])*2.5f;
        
    }
    
    down_sampler_l.process_up_sample(leftData, num_samples, down_sampled_buffer_ptr_l);
    down_sampler_r.process_up_sample(rightData, num_samples, down_sampled_buffer_ptr_r);
    
    for (int i = 0; i < num_samples; i++)
    {
        leftData[i] = leftData[i] * blend_value + out_l_ptr[i] * blend_value_dry;
        rightData[i] = rightData[i] * blend_value + out_r_ptr[i] * blend_value_dry;
    }
 
    
}



//==============================================================================
bool BYC_VRBAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BYC_VRBAudioProcessor::createEditor()
{
    return new BYC_VRBAudioProcessorEditor (*this);
}

//==============================================================================
void BYC_VRBAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BYC_VRBAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BYC_VRBAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout BYC_VRBAudioProcessor::createParameters()
{
    //for a commercial release would implement seperate versions for each param.
    constexpr int paramVersion = 2;
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // ParameterID{ID, version} lets you build for .au
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"DECAY", paramVersion}, "Decay", 0.0f, 0.99f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"BLEND", paramVersion}, "Blend", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"DAMP", paramVersion}, "Damp", 0.35f, 0.85f, 0.45f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"PDELAY", paramVersion}, "Pre Delay", 0.01f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"INPUTLPF", paramVersion}, "Input LPF", 0.25f, 0.5f, 0.25f));
    
    return {params.begin(), params.end()};
}

