//
//  AudioBuffer.hpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 01.05.21.
//

#ifndef AudioBuffer_hpp
#define AudioBuffer_hpp

#include <stdio.h>
typedef unsigned short int USHORT;
typedef unsigned int UINT;

namespace Granulizer {
    
class AudioBuffer
{
public:
    AudioBuffer(USHORT numOfChannels, UINT sizeInSamples) :
        mNumOfChannels(numOfChannels),
        mNumOfSamples(sizeInSamples)
    {
        mBuffer = new float[sizeInSamples * numOfChannels] { 0 };
    }
    
    ~AudioBuffer()
    {
        delete[] mBuffer;
    }
    
    UINT GetNumOfSamples()
    {
        return mNumOfSamples;
    }
    
    USHORT GetNumOfChannels()
    {
        return mNumOfChannels;
    }
    
    float GetSample(USHORT channel, UINT index)
    {
        UINT channelIndex = GetChannelIndex(channel, index);
        return mBuffer[channelIndex];
    }
    
    void SetSample(USHORT channel, UINT index, float inSample)
    {
        UINT channelIndex = GetChannelIndex(channel, index);
        mBuffer[channelIndex] = inSample;
    }
    
    float* const GetReadPointer(USHORT channel, UINT index)
    {
        return mBuffer + GetChannelIndex(channel, index);
    }
    
private:
    UINT GetChannelIndex(USHORT channel, UINT index)
    {
        return (channel * mNumOfSamples) + index;
    }
    
    USHORT mNumOfChannels;
    UINT mNumOfSamples;
    float* mBuffer;
};

}
#endif /* AudioBuffer_hpp */


