//
//  ParameterListener.h
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 10.03.22.
//

#ifndef ParameterListener_h
#define ParameterListener_h
#include <JuceHeader.h>

//TODO: make these two into a template class with T audioParamter.

class ParameterListenerFloat : public juce::Slider::Listener
{
public:
    AudioParameterFloat* mParam;
    
    ParameterListenerFloat(AudioParameterFloat* param) : mParam(param) {}
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        *mParam = slider->getValue();
    }
};


class ParameterListenerInt : public juce::Slider::Listener
{
public:
    AudioParameterInt* mParam;
    
    ParameterListenerInt(AudioParameterInt* param) : mParam(param) {}
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        *mParam = slider->getValue();
    }
};

#endif /* ParameterListener_h */
