/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GranulizerThingyAudioProcessorEditor::GranulizerThingyAudioProcessorEditor (GranulizerThingyAudioProcessor& p)
    :   AudioProcessorEditor (&p),
        audioProcessor (p)
{
      
    auto& params = processor.getParameters();
    
    int mSliderWidthIncrement = mSliderWidthOffset;
    
    // Number of voices slider
    mNumOfVoicesSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mNumOfVoicesSlider.setBounds(mSliderWidthIncrement, mSliderHeightOffset, mSliderWidth, mSliderHeight);
    mNumOfVoicesSlider.setRange(1, 6, 1);
    mNumOfVoicesSlider.setValue(3);
    mNumOfVoicesSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, mSliderWidth, mTextBoxHeight);
    addAndMakeVisible(mNumOfVoicesSlider);
    mParamListenerVoices = new ParameterListenerInt(static_cast<AudioParameterInt*>(params.getUnchecked(0)));
    mNumOfVoicesSlider.addListener(mParamListenerVoices);
    
    mNumOfVoicesLabel.attachToComponent(&mNumOfVoicesSlider, false);
    mNumOfVoicesLabel.setText("Voices", juce::dontSendNotification);
    mNumOfVoicesLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mNumOfVoicesLabel);
   
    // ===========================================================================================
    mSliderWidthIncrement += mSliderWidth + mSliderMargin;
    // WindowSize Slider ====
    mWindowSizeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mWindowSizeSlider.setBounds(mSliderWidthIncrement, mSliderHeightOffset, mSliderWidth, mSliderHeight);
    mWindowSizeSlider.setRange(1.f, 1000.f, 0.1f);
    mWindowSizeSlider.setValue(30.f);
    mWindowSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, mSliderWidth, mTextBoxHeight);
    
    mParamListenerWindowSize = new ParameterListenerFloat(static_cast<AudioParameterFloat*>(params.getUnchecked(1)));
    mWindowSizeSlider.addListener(mParamListenerWindowSize);
    addAndMakeVisible(mWindowSizeSlider);
    
    mWindowSizeLabel.attachToComponent(&mWindowSizeSlider, false);
    mWindowSizeLabel.setText("Grain Size", juce::dontSendNotification);
    mWindowSizeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mWindowSizeLabel);
    
    // ===========================================================================================
    
    mSliderWidthIncrement += mSliderWidth + mSliderMargin;
    // WindowSize Random range Slider
    mWindowSizeRandomRangeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mWindowSizeRandomRangeSlider.setBounds(mSliderWidthIncrement, mSliderHeightOffset, mSliderWidth, mSliderHeight);
    mWindowSizeRandomRangeSlider.setRange(0.f, 1000.f, 0.1f);
    mWindowSizeRandomRangeSlider.setValue(0.f);
    mWindowSizeRandomRangeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, mSliderWidth, mTextBoxHeight);
    
    mParamListenerWindowSizeRandom = new ParameterListenerFloat(static_cast<AudioParameterFloat*>(params.getUnchecked(2)));
    mWindowSizeRandomRangeSlider.addListener(mParamListenerWindowSizeRandom);
    addAndMakeVisible(mWindowSizeRandomRangeSlider);
    
    mWinodowSizeRandomLabel.attachToComponent(&mWindowSizeRandomRangeSlider, false);
    mWinodowSizeRandomLabel.setText("Random Range", juce::dontSendNotification);
    mWinodowSizeRandomLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mWinodowSizeRandomLabel);
    
    // ===========================================================================================
    mSliderWidthIncrement += mSliderWidth + mSliderMargin;
    // Repetition slider
    mGrainRepetitionSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mGrainRepetitionSlider.setBounds(mSliderWidthIncrement, mSliderHeightOffset, mSliderWidth, mSliderHeight);
    mGrainRepetitionSlider.setRange(0, 50, 1);
    mGrainRepetitionSlider.setValue(3);
    mGrainRepetitionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, mSliderWidth, mTextBoxHeight);
    
    mParamListenerReptition = new ParameterListenerInt(static_cast<AudioParameterInt*>(params.getUnchecked(3)));
    mGrainRepetitionSlider.addListener(mParamListenerReptition);
    addAndMakeVisible(mGrainRepetitionSlider);
    
    mRepetitionLabel.attachToComponent(&mGrainRepetitionSlider, false);
    mRepetitionLabel.setText("Repetitions", juce::dontSendNotification);
    mRepetitionLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mRepetitionLabel);
    
    // ===========================================================================================
    
    mSliderWidthIncrement += mSliderWidth + mSliderMargin;
    // DRY slider
    mDryLevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mDryLevelSlider.setBounds(mSliderWidthIncrement, mSliderHeightOffset, mSliderWidth, mSliderHeight);
    mDryLevelSlider.setRange(0.f, 1.f, 0.001f);
    mDryLevelSlider.setValue(1.0f);
    mDryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, mSliderWidth, mTextBoxHeight);
    
    mParamListenerDryLevel = new ParameterListenerFloat(static_cast<AudioParameterFloat*>(params.getUnchecked(4)));
    mDryLevelSlider.addListener(mParamListenerDryLevel);
    addAndMakeVisible(mDryLevelSlider);
    
    mDryLevelLabel.attachToComponent(&mDryLevelSlider, false);
    mDryLevelLabel.setText("Dry", juce::dontSendNotification);
    mDryLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mDryLevelLabel);
    
    // ===========================================================================================
    mSliderWidthIncrement += mSliderWidth + mSliderMargin;
    // Wet slider
    mWetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mWetLevelSlider.setBounds(mSliderWidthIncrement, mSliderHeightOffset, mSliderWidth, mSliderHeight);
    mWetLevelSlider.setRange(0.f, 1.f, 0.001f);
    mWetLevelSlider.setValue(0.5f);
    mWetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, mSliderWidth, mTextBoxHeight);
    mParamListenerWetLevel = new ParameterListenerFloat(static_cast<AudioParameterFloat*>(params.getUnchecked(5)));
    mWetLevelSlider.addListener(mParamListenerWetLevel);
    addAndMakeVisible(mWetLevelSlider);
    
    mWetLevelLabel.attachToComponent(&mWetLevelSlider, false);
    mWetLevelLabel.setText("Wet", juce::dontSendNotification);
    mWetLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mWetLevelLabel);
    
    // ===========================================================================================
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (mTotalWidth, mTotalHeight);
}

GranulizerThingyAudioProcessorEditor::~GranulizerThingyAudioProcessorEditor()
{
    processor.getParameters();
    delete mParamListenerVoices;
    delete mParamListenerWindowSize;
    delete mParamListenerWindowSizeRandom;
    delete mParamListenerReptition;
    delete mParamListenerDryLevel;
    delete mParamListenerWetLevel;
}

//==============================================================================
void GranulizerThingyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (0));

    g.setColour (juce::Colours::white);
    g.setFont (12.0f);
    
}

void GranulizerThingyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

