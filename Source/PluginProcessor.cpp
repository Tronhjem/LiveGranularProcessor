/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GranulizerThingyAudioProcessor::GranulizerThingyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    int ringBufferSize = 4 * 44100;
    int grainWindowSize = 1000.f;
    int spreadSize = 1000.f;
    
    mGrainVoiceController = std::make_unique<GranularVoiceController>(ringBufferSize, 3);
    
    mCurrentNumberOfVoices  =   new AudioParameterInt("numVoices",
                                                      "NumberOfActiveVoices",
                                                      1,
                                                      MAX_VOICES,
                                                      3);
    addParameter(mCurrentNumberOfVoices);
    
    mGrainWindowSize        =   new AudioParameterInt("windowSize",
                                                      "GrainWindowSize",
                                                      1,
                                                      grainWindowSize,
                                                      30);
    addParameter(mGrainWindowSize);
    
    mGrainWidowRanSpread    =   new AudioParameterInt("windowRanSpread",
                                                      "WindowRandomSpread",
                                                      0,
                                                      spreadSize,
                                                      0);
    
    addParameter(mGrainWidowRanSpread);
    mGrainRepetition        =   new AudioParameterInt("grainRepetition",
                                                      "Grain Repetition",
                                                      1,
                                                      50,
                                                      10);
    addParameter(mGrainRepetition);
    mDryLevel               =   new AudioParameterFloat("dryLevel",
                                                 "DryLevel",
                                                    0,
                                                    1.f,
                                                    1.f);
    addParameter(mDryLevel);
    mWetLevel               =   new AudioParameterFloat("wetLevel",
                                                 "wetLevel",
                                                    0,
                                                    1.f,
                                                    1.f);
    addParameter(mWetLevel);
    
}

GranulizerThingyAudioProcessor::~GranulizerThingyAudioProcessor()
{

}

//==============================================================================
const juce::String GranulizerThingyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GranulizerThingyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GranulizerThingyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GranulizerThingyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GranulizerThingyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GranulizerThingyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GranulizerThingyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GranulizerThingyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GranulizerThingyAudioProcessor::getProgramName (int index)
{
    return {};
}

void GranulizerThingyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GranulizerThingyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mSampleRate = sampleRate;
    mSampleRateMiliseconds = mSampleRate / 1000.f;
}

void GranulizerThingyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GranulizerThingyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GranulizerThingyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    mGrainVoiceController->NumberOfCurrentVoices = mCurrentNumberOfVoices->get();
    
    int windowSize = mGrainWindowSize->get() * mSampleRateMiliseconds;
    if (windowSize != mGrainVoiceController->VoiceGrainWindowSize)
        mGrainVoiceController->ParamsDirtied = true;
    mGrainVoiceController->VoiceGrainWindowSize = windowSize;
    
    int windowSpread = mGrainWidowRanSpread->get() * mSampleRateMiliseconds;
    if (windowSpread != mGrainVoiceController->VoiceGrainWindowSizeRange)
        mGrainVoiceController->ParamsDirtied = true;
    
    mGrainVoiceController->VoiceGrainWindowSizeRange = windowSpread;
    
    int repetition = mGrainRepetition->get();
    if (repetition != mGrainVoiceController->VoiceMaxRepition)
        mGrainVoiceController->ParamsDirtied = true;;
    
    mGrainVoiceController->VoiceMaxRepition = repetition;
    
    
    mGrainVoiceController->WetGain = mWetLevel->get();
    mGrainVoiceController->DryGain = mDryLevel->get();
    
    
    
    mGrainVoiceController->Process(buffer);
}

//==============================================================================
bool GranulizerThingyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

::AudioProcessorEditor* GranulizerThingyAudioProcessor::createEditor()
{
    return new GranulizerThingyAudioProcessorEditor (*this);
}

//==============================================================================
void GranulizerThingyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GranulizerThingyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GranulizerThingyAudioProcessor();
}
