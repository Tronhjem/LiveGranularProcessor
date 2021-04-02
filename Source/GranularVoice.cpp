//
//  GranularVoice.cpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 31.03.21.
//

#include "GranularVoice.hpp"
using namespace juce;

GranularVoice::GranularVoice(int maxSize, float fadeSize, int channels)
{
    mBuffer = new AudioBuffer<float>(channels, maxSize);
    mFadeGainCoef = 1/fadeSize;
    mFadeSize = fadeSize;
    mCurrentSampleCount = new int[channels]{0};
}

void GranularVoice::Process(AudioBuffer<float>& buffer)
{
    // process channels
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        // Process samples
        if (mCurrentSampleCount[channel] >= mMaxSamplesCount)
            mCurrentSampleCount = 0;
        int bufferSize = buffer.getNumSamples();
        
        for (int i=0; i < bufferSize; i++)
        {
            int channelSampleCount = mCurrentSampleCount[channel];
            
            //########################################################################
            // sample a window
            if (channelSampleCount < mGrainWindowSize)
            {
                float sampleGainFactor = 1.f;
                
                // fade in
                if ( channelSampleCount < mFadeSize)
                    sampleGainFactor = mFadeGainCoef * i;
                
                // fade out
                else if (channelSampleCount > mGrainWindowSize - mFadeSize)
                    sampleGainFactor = mFadeGainCoef * (mGrainWindowSize - i);
                
                float processedSample =  buffer.getSample(channel, i) * sampleGainFactor;
                
                mBuffer -> setSample(channel, i, processedSample);
            }
            //########################################################################
            // repeat that window.
            else if (channelSampleCount < mMaxSamplesCount)
            {
                float processedSample = mBuffer->getSample(channel, channelSampleCount % mGrainWindowSize);
                buffer.setSample(channel, i, processedSample);
            }
            //########################################################################
            mCurrentSampleCount[channel]++;
        }
    }
}

void GranularVoice::SetSize(int sizeInSamples)
{

}

void GranularVoice::SetPan(float pan)
{
    
}

void GranularVoice::SetStartPosition(int startPosInSamples)
{
    
}

void GranularVoice::SetRandomRange(int randomRange)
{
 
}

void GranularVoice::SetVoiceGainAttenuation(float attenuation)
{
    
}
