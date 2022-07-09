//
//  GranularSnH.cpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 31.03.21.
//

#include "GranularVoiceController.hpp"
#include "GranularVoice.hpp"


GranularVoiceController::GranularVoiceController(int maxLengthInSamples, int numberOfVoices) 
{
    for (USHORT i = 0; i < mVoices.size(); i++)
        mVoices[i] = new GranularVoice(VoiceGrainWindowSize, 2, this);
    
    NumberOfCurrentVoices = numberOfVoices;
    
    mRingBuffer = new CTDSP::AudioBuffer<float>(2, maxLengthInSamples);
}

GranularVoiceController::~GranularVoiceController()
{
    delete mRingBuffer;
}

void GranularVoiceController::Process(AudioBuffer<float>& inBuffer)
{
    int inBufferSize = inBuffer.getNumSamples();
    int numOfChannels = inBuffer.getNumChannels();
    
    // ======================================================================
    // CHANNELS
    // ======================================================================
    for (int channel=0; channel < numOfChannels; ++channel)
    {
        float* inBufferPointer = inBuffer.getWritePointer(channel);
        for (int i=0; i < inBufferSize; ++i)
        {
            mRingBuffer->WriteNextSample(channel, inBufferPointer[i]);
        }
        
        int voiceStartIndex = NumberOfCurrentVoices * channel;
        int maxVoiceForChannel = voiceStartIndex + NumberOfCurrentVoices;
        
        // ======================================================================
        // Sample buffer loop
        // ======================================================================
        for (int j=0; j < inBufferSize; ++j)
        {
            double sum = 0;
          
            for (int voice = voiceStartIndex; voice < maxVoiceForChannel; ++voice)
            {
                sum += static_cast<double>(mVoices[voice]->ProcessSample(*mRingBuffer, channel));
            }
            sum /= NumberOfCurrentVoices;
            float tempDry = inBufferPointer[j];
            inBufferPointer[j] = tempDry * DryGain + static_cast<float>(sum * WetGain);
        }
    }
    ParamsDirtied = false; // clear diry flag.
}

// ======================================================================
int GranularVoiceController::GetCurrentPosition(int channel) const
{
    return mRingBuffer->GetWriteIndex(channel);
}
