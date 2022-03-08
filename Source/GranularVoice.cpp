//
//  GranularVoice.cpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 31.03.21.
//

#include "GranularVoice.hpp"
#include "GranularVoiceController.hpp"
#include <random>
using namespace juce;


GranularVoice::GranularVoice(int windowSize, int fadeSize, int numOfChannels, GranularVoiceController* const controller)
{
    GrainWindowSize = windowSize;
    mWindowSize = fadeSize;
    mFadeGainCoef = static_cast<double>(1.f/fadeSize);
    mRandomGen = new Random();
    mController = controller;
}

GranularVoice::~GranularVoice()
{
    delete mRandomGen;
}

float GranularVoice::ProcessSample(CTDSP::AudioBuffer<float>& inBuffer, USHORT channel)
{
    UINT bufferSize = inBuffer.GetSizeInSamples();
    
    if (mCurrentSampleCount > GrainWindowSize)
    {
        mCurrentSampleCount = 0;
        
        if (mCurrentIteration > MaxGrainIterations)
        {
            int randomRange = mRandomGen->nextInt((mController->VoiceGrainWindowSizeRange + 1));
            int windowSize = mController->VoiceGrainWindowSize;
            
            GrainWindowSize = (randomRange - (randomRange * 0.5)) + windowSize;
            if (GrainWindowSize <= 5)
                GrainWindowSize = 5;
                                                  
            MaxGrainIterations = mController->VoiceMaxRepition + mRandomGen->nextInt(5);

            mReadStartPosition = mController->GetCurrentPosition(channel) - GrainWindowSize;
            mCurrentIteration = 0;
            
            // wrap around.
            if (mReadStartPosition < 0)
                mReadStartPosition += bufferSize;
        }
        else
            mCurrentIteration++;
    }
    
    double sampleGainFactor = mController->mEnvelope->GetEnvelope(0.5);
    
//    // fade in
//    if (mCurrentSampleCount <= mWindowSize)
//        sampleGainFactor = mFadeGainCoef * mCurrentSampleCount;
//
//    // fade out
//    else if (mCurrentSampleCount >= GrainWindowSize - mWindowSize)
//        sampleGainFactor = mFadeGainCoef * (GrainWindowSize - mCurrentSampleCount);
//
//    // squared gain
    sampleGainFactor *= sampleGainFactor;
    
    int readPosition = mReadStartPosition + mCurrentSampleCount;
    
    if (readPosition >= bufferSize)
        readPosition = readPosition - bufferSize;
    
    mCurrentSampleCount++;
    
    return inBuffer.GetSample(channel, readPosition) * sampleGainFactor;
}
