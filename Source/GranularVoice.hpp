//
//  GranularVoice.hpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 31.03.21.
//

#ifndef GranularVoice_hpp
#define GranularVoice_hpp

#include <stdio.h>
#include <JuceHeader.h>
using namespace juce;

class GranularVoice {

public:
    
    GranularVoice(int maxSize, float fadeSize, int channels);
    
    void SetSize(int sizeInSamples);
    void SetPan(float pan);
    void SetStartPosition(int startPosInSamples);
    void SetRandomRange(int randomRange);
    void Process(AudioBuffer<float>& buffer);
    void SetVoiceGainAttenuation(float attenuation);
    
private:
    
    int mGrainWindowSize = 8800;
    int mMaxSamplesCount = 35200;
    
    int* mCurrentSampleCount;
    
    int mMaxRepetition = 0;
    int mCurrentRepetition = 0;
    
    float mFadeGainCoef = 0;
    int mFadeSize = 100;
    
    juce::AudioBuffer<float>* mBuffer;
    
};

#endif /* GranularVoice_hpp */
