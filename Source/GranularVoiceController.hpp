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
#include "AudioBuffer.hpp"

#define MAX_VOICES 10
typedef unsigned short int USHORT;

class GranularVoiceController {

public:
    GranularVoiceController(int maxLengthInSamples, int numberOfVoices);
    ~GranularVoiceController();
    
    void    Process(juce::AudioBuffer<float>& inBuffer);
    USHORT  VoiceGrainWindowSize = 9000;
    UINT    VoiceGrainWindowSizeRange = 1000;
    USHORT  NumberOfCurrentVoices = 6;
    USHORT  VoiceMaxRepition = 2;
    
private:
    USHORT  mVoiceFadeSize = 50;
    int     mChannelBufferPosition = 0;
    Granulizer::AudioBuffer* mRingBuffer;
    std::array<GranularVoice*, MAX_VOICES> mVoices;
    std::array<double, 4> mBufferChannels;
};
 
#endif /* GranularVoiceConroller_hpp */
