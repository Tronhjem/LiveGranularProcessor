//
//  GranularSnH.cpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 31.03.21.
//

#include "GranularVoiceController.hpp"
using namespace juce;

GranularVoiceController::GranularVoiceController(int maxLengthInSamples, int numberOfVoices)
{
    for (int i=0; i < mVoices.size(); i++) {
        mVoices[i] = std::make_unique<GranularVoice>(VoiceGrainWindowSize, mVoiceFadeSize);
    }
    
    NumberOfCurrentVoices = numberOfVoices;
     
    if (NumberOfCurrentVoices > mVoices.size())
        NumberOfCurrentVoices = (int)mVoices.size();
    
    mRingBuffer = std::make_unique<AudioBuffer<float>>(2, maxLengthInSamples);
}

GranularVoiceController::~GranularVoiceController()
{
    
}

void GranularVoiceController::Process(AudioBuffer<float>& inBuffer)
{
    int inBufferSize = inBuffer.getNumSamples();
    int channels = inBuffer.getNumChannels();
    int ringBufferSize = mRingBuffer->getNumSamples();
    
    for (int sample=0; sample < inBufferSize; sample++)
    {
        if (mChannelBufferPosition >= ringBufferSize)
        {
            mChannelBufferPosition = 0;
        }
        
        for (int channel=0; channel < channels; channel++)
        {
            mRingBuffer->setSample(channel,
                                   mChannelBufferPosition,
                                   inBuffer.getSample(channel, sample));
            
            inBuffer.setSample(channel,
                               sample,
                               mBufferChannels[channel]);
            
            mBufferChannels[channel] = 0;
        }
        
        mChannelBufferPosition++;
        
        for (int i = 0; i < NumberOfCurrentVoices; i++)
        {
            mVoices[i]->ProcessSample(*mRingBuffer,
                                      mChannelBufferPosition,
                                      mBufferChannels,
                                      channels);
        }
    }    
}