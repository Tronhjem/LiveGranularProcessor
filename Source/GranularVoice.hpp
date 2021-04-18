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

typedef short unsigned int USHORT;
using namespace juce;

class GranularVoice {

public:
    
    GranularVoice(int windowSize, int fadeSize);
    ~GranularVoice();
    void ProcessSample(AudioBuffer<float>& inBuffer,
                       int currentReadPosition,
                       std::array<double, 4>& outBufferChannels,
                       int bufferChannelsLenght);
    
    USHORT  GrainWindowSize = 400;
    USHORT  GraindWindowRandomSpread = 200;
    USHORT  MaxGrainIterations = 4;
    
private:
    USHORT  mWindowSize = 40;
    int     mReadStartPosition = 0;
    int     mCurrentIteration = 0;
    double  mFadeGainCoef = 0;
    int     mCurrentSampleCount = 0;
    Random* mRandomGen;
};

#endif /* GranularVoice_hpp */
