//
//  GranularSnH.hpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 31.03.21.
//
#ifndef GranularVoiceConroller_hpp
#define GranularVoiceConroller_hpp

#include <stdio.h>
#include <JuceHeader.h>
#include "GranularVoice.hpp"
#include <array>
#include <memory>
#include "AudioBuffer.h"
#include "CTTypes.h"
#include "Envelope.h"

#define MAX_VOICES 6
#define ENVELOPE_SIZE 2048
#define ENVELOPE_FADE_PERCENT 0.125

class GranularVoiceController {

public:
    GranularVoiceController(int maxLengthInSamples, int numberOfVoices);
    ~GranularVoiceController();
    
    void                                        Process(juce::AudioBuffer<float>& inBuffer);
    int                                         GetCurrentPosition(int channel) const;
    UINT                                        VoiceGrainWindowSizeRange = 1000;
    USHORT                                      VoiceGrainWindowSize = 500;
    USHORT                                      NumberOfCurrentVoices = 4;
    USHORT                                      VoiceMaxRepition = 4;
    float                                       WetGain = 1.f;
    float                                       DryGain = 0.5f;
    const CTDSP::Envelope                       mEnvelope {ENVELOPE_SIZE, ENVELOPE_FADE_PERCENT};
    
private:
    USHORT                                      mVoiceFadeSize = 50;
    CTDSP::AudioBuffer<float>*                  mRingBuffer;
    std::array<GranularVoice*, MAX_VOICES * 2>  mVoices;
    
};
 
#endif /* GranularVoiceConroller_hpp */
