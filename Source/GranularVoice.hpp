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
#include <array>
#include "AudioBuffer.h"
#include "Envelope.h"

typedef short unsigned int USHORT;
using namespace juce;

class GranularVoiceController;

class GranularVoice {

public:
    GranularVoice(int windowSize, int numOfChannels, GranularVoiceController* const controller);
    ~GranularVoice();
    
    float ProcessSample(CTDSP::AudioBuffer<float>& inBuffer, USHORT channel);
    
private:
    USHORT                          mGraindWindowRandomSpread = 200;
    USHORT                          mMaxGrainIterations = 4;
    bool                            mWasDirtied = false;
    int                             mGrainWindowSize = 400;
    int                             mReadStartPosition = 0;
    int                             mCurrentIteration = 0;
    int                             mCurrentSampleCount = 0;
    const GranularVoiceController*  mController;
    Random*                         mRandomGen;
};

#endif /* GranularVoice_hpp */
