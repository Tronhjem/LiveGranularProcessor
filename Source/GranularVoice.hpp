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
    GranularVoice(int windowSize, int fadeSize, int numOfChannels, GranularVoiceController* const controller);
    ~GranularVoice();
    
    float ProcessSample(CTDSP::AudioBuffer<float>& inBuffer, USHORT channel);
    
    USHORT                          GrainWindowSize = 400;
    USHORT                          GraindWindowRandomSpread = 200;
    USHORT                          MaxGrainIterations = 4;
    
private:
    const GranularVoiceController*  mController;
    USHORT                          mWindowSize = 40;
    int                             mReadStartPosition = 0;
    double                          mFadeGainCoef = 0;
    int                             mCurrentIteration;
    int                             mCurrentSampleCount;
    Random*                         mRandomGen;
    
};

#endif /* GranularVoice_hpp */
