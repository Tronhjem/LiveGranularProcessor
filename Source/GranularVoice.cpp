//
//  GranularVoice.cpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 31.03.21.
//

#include "GranularVoice.hpp"
using namespace juce;

GranularVoice::GranularVoice(int windowSize, int fadeSize)
{
    GrainWindowSize = windowSize;
    mWindowSize = fadeSize;
    mFadeGainCoef = 1/fadeSize;
    mRandomGen = new Random();
}

GranularVoice::~GranularVoice()
{
    delete mRandomGen;
}

void GranularVoice::ProcessSample(AudioBuffer<float>& inBuffer,
                                  int currentReadPosition,
                                  std::array<double, 4>& outBufferChannels,
                                  int bufferChannelsLength)
{
    int bufferSize = inBuffer.getNumSamples();
    

    if (mCurrentSampleCount > GrainWindowSize)
    {
        mCurrentSampleCount = 0;
        
        if (mCurrentIteration > MaxGrainIterations)
        {
            mReadStartPosition = currentReadPosition - GrainWindowSize + mRandomGen->nextInt(Range<int>(-4000,0));
            MaxGrainIterations = mRandomGen->nextInt(Range<int>(2,100));
            mCurrentIteration = 0;
            
            GrainWindowSize = mRandomGen->nextInt(Range<int>(260,900));
            // wrap around.
            if (mReadStartPosition < 0)
                mReadStartPosition += bufferSize;
        }
        else
            mCurrentIteration++;
    }
    
    double sampleGainFactor = 1.f;
    
    // fade in
    if (mCurrentSampleCount <= mWindowSize)
        sampleGainFactor = mFadeGainCoef * mCurrentSampleCount;

    // fade out
    else if (mCurrentSampleCount >= GrainWindowSize - mWindowSize)
        sampleGainFactor = mFadeGainCoef * (GrainWindowSize - mCurrentSampleCount);
    
    // squared gain
    sampleGainFactor *= sampleGainFactor;
    
    int readPosition = mReadStartPosition + mCurrentSampleCount;
    
    if (readPosition >= bufferSize)
        readPosition = readPosition - bufferSize;
    
    for (int channel = 0; channel < bufferChannelsLength; channel++)
        outBufferChannels[channel] += inBuffer.getSample(channel, readPosition) * sampleGainFactor * 0.25f;
    
    mCurrentSampleCount++;
}

