/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GranularVoiceController.hpp"
#include <memory>

//==============================================================================
/**
*/
class GranulizerThingyAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    GranulizerThingyAudioProcessor();
    ~GranulizerThingyAudioProcessor() override;

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
  
    
private:
    //==============================================================================
    
    AudioParameterInt*      mCurrentNumberOfVoices;
    AudioParameterInt*      mGrainWindowSize;
    AudioParameterInt*      mGrainWidowRanSpread;
    AudioParameterInt*      mGrainRepetition;
    AudioParameterFloat*    mDryLevel;
    AudioParameterFloat*    mWetLevel;
    
    std::unique_ptr<GranularVoiceController> mGrainVoiceController;
    int mSampleRate = 44100;
    float mSampleRateMiliseconds = 44.1f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranulizerThingyAudioProcessor)
};
