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

void GranularVoice::ProcessSample(Granulizer::AudioBuffer& inBuffer,
                                  int currentReadPosition,
                                  std::array<double, 4>& outBufferChannels,
                                  int bufferChannelsLength)
{
    int bufferSize = inBuffer.GetNumOfSamples();
    
    if (mCurrentSampleCount > GrainWindowSize)
    {
        mCurrentSampleCount = 0;

        if (mCurrentIteration > MaxGrainIterations)
        {
            int randomRange = mRandomGen->nextInt(Range<int>(GraindWindowRandomSpread * -1, 0 ));
//            GrainWindowSize =
//              + randomRange;
//            MaxGrainIterations = mRandomGen->nextInt(Range<int>(2,100));
//            GrainWindowSize = mRandomGen->nextInt(Range<int>(260,900));

            mReadStartPosition = currentReadPosition - GrainWindowSize + randomRange;
            mCurrentIteration = 0;
            
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
    
    for (USHORT channel = 0; channel < bufferChannelsLength; channel++)
        outBufferChannels[channel] += inBuffer.GetSample(channel, readPosition) * sampleGainFactor * 0.25f;
    
    mCurrentSampleCount++;
}

