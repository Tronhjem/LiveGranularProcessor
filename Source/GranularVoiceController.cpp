//
//  GranularSnH.cpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 31.03.21.
//

#include "GranularVoiceController.hpp"

GranularVoiceController::GranularVoiceController(int maxLengthInSamples, int numberOfVoices)
{
    for (USHORT i = 0; i < mVoices.size(); i++)
        mVoices[i] = new GranularVoice(VoiceGrainWindowSize, mVoiceFadeSize);
    
    NumberOfCurrentVoices = numberOfVoices;
     
    if (NumberOfCurrentVoices > mVoices.size())
        NumberOfCurrentVoices = (int)mVoices.size();
    
    mRingBuffer = new Granulizer::AudioBuffer(2, maxLengthInSamples);
}

GranularVoiceController::~GranularVoiceController()
{
    delete mRingBuffer;
}

void GranularVoiceController::Process(AudioBuffer<float>& inBuffer)
{
    int inBufferSize = inBuffer.getNumSamples();
    int channels = inBuffer.getNumChannels();
    int ringBufferSize = mRingBuffer->GetNumOfSamples();
    
    for (int sample=0; sample < inBufferSize; sample++)
    {
        if (mChannelBufferPosition >= ringBufferSize)
            mChannelBufferPosition = 0;
        
        for (USHORT channel=0; channel < channels; channel++)
        {
            mRingBuffer->SetSample(channel, mChannelBufferPosition, inBuffer.getSample(channel, sample));
            inBuffer.setSample(channel, sample, mBufferChannels[channel]);
            mBufferChannels[channel] = 0;
        }
        
        mChannelBufferPosition++;
        
        for (USHORT i = 0; i < NumberOfCurrentVoices; i++)
        {
            // Set voice params
            mVoices[i]->GrainWindowSize = VoiceGrainWindowSize;
            mVoices[i]->GraindWindowRandomSpread = VoiceGrainWindowSizeRange;
            mVoices[i]->MaxGrainIterations = VoiceMaxRepition;

            // Process voice
            mVoices[i]->ProcessSample(*mRingBuffer, mChannelBufferPosition, mBufferChannels, channels);
        }
    }    
}
