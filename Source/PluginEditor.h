/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ParameterListener.h"

/**
 //==============================================================================
*/
class GranulizerThingyAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GranulizerThingyAudioProcessorEditor (GranulizerThingyAudioProcessor&);
    ~GranulizerThingyAudioProcessorEditor() override;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GranulizerThingyAudioProcessor& audioProcessor;

    const int mNumOfParams  =   6;
    const int mSliderWidth  =   80;
    
    const int mSliderWidthOffset = 50;
    const int mSliderMargin =   40;
    const int mSliderHeight =   200;
    const int mTextBoxHeight =  40;
    const int mSliderHeightOffset = 50;
   
    const int mTextOffset   =   20;
    const int mTextHeight   =   50;
    const int mTextWidth    =   100;
    const int mTotalHeight  =   mSliderHeight + (mTextHeight * 2);
    
    const int mTotalWidth   =   (mNumOfParams * mSliderWidth) +
                                (mSliderMargin * mNumOfParams) +
                                mSliderWidthOffset;
    
    juce::Slider                mNumOfVoicesSlider;
    juce::Label                 mNumOfVoicesLabel;
    ParameterListenerInt*       mParamListenerVoices;
    
    juce::Slider                mWindowSizeSlider;
    juce::Label                 mWindowSizeLabel;
    ParameterListenerFloat*     mParamListenerWindowSize;
    
    juce::Slider                mWindowSizeRandomRangeSlider;
    juce::Label                 mWinodowSizeRandomLabel;
    ParameterListenerFloat*     mParamListenerWindowSizeRandom;
    
    juce::Slider                mGrainRepetitionSlider;
    juce::Label                 mRepetitionLabel;
    ParameterListenerInt*       mParamListenerReptition;
    
    juce::Slider                mDryLevelSlider;
    juce::Label                 mDryLevelLabel;
    ParameterListenerFloat*     mParamListenerDryLevel;
    
    juce::Slider                mWetLevelSlider;
    juce::Label                 mWetLevelLabel;
    ParameterListenerFloat*     mParamListenerWetLevel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranulizerThingyAudioProcessorEditor)
};
