//
//  GranularVoice.cpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 31.03.21.
//

#include "GranularVoice.hpp"
#include "GranularVoiceController.hpp"
#include <random>
#define MINIMUM_WINDOW_SIZE 30
using namespace juce;


GranularVoice::GranularVoice(int windowSize, int numOfChannels, GranularVoiceController* const controller)
{
    mGrainWindowSize = windowSize;
    mRandomGen = new Random();
    mController = controller;
}

GranularVoice::~GranularVoice()
{
    delete mRandomGen;
}

float GranularVoice::ProcessSample(CTDSP::AudioBuffer<float>& inBuffer, USHORT channel)
{
    const UINT bufferSize = inBuffer.GetSizeInSamples();
    
    mWasDirtied = mController->ParamsDirtied || mWasDirtied;
    
    if (mCurrentSampleCount > mGrainWindowSize)
    {
        mCurrentSampleCount = 0;
        
        if (mCurrentIteration > mMaxGrainIterations || mWasDirtied)
        {
            const int randomRange = mRandomGen->nextInt((mController->VoiceGrainWindowSizeRange + 1));
            const int windowSize = mController->VoiceGrainWindowSize;
            
            mGrainWindowSize = (randomRange - (randomRange * 0.5)) + windowSize;
            if (mGrainWindowSize <= MINIMUM_WINDOW_SIZE)
                mGrainWindowSize = MINIMUM_WINDOW_SIZE;
                                                  
            mMaxGrainIterations = mController->VoiceMaxRepition + mRandomGen->nextInt(5);
            mReadStartPosition = mController->GetCurrentPosition(channel) - mGrainWindowSize;
            mCurrentIteration = 0;
            
            // Wrap around.
            if (mReadStartPosition < 0)
                mReadStartPosition += bufferSize;
            mWasDirtied = false;
        }
        else
            mCurrentIteration++;
    }
    
    const double progression = static_cast<double>(mCurrentSampleCount) / static_cast<double>(mGrainWindowSize);
    double sampleGainFactor = mController->mEnvelope.GetEnvelope(progression);
    
    // squared gain
    sampleGainFactor *= sampleGainFactor;
    
    int readPosition = mReadStartPosition + mCurrentSampleCount;
    
    if (readPosition >= bufferSize)
        readPosition = readPosition - bufferSize;
    
    mCurrentSampleCount++;
    
    return inBuffer.GetSample(channel, readPosition) * sampleGainFactor;
}
