//
//  GranularSnH.hpp
//  GranulizerThingy
//
//  Created by Christian Tronhjem on 31.03.21.
//

#ifndef GranularSnH_hpp
#define GranularSnH_hpp

#include <stdio.h>
#include <JuceHeader.h>
using namespace juce;

class GranularSnH {

public:
    GranularSnH(int maxLengthInSamples);
    void Process(float* const bufferPointer);
    
    
private:
    float mWindowMS = 400;
    int mWindowSamples = 0;
    
    float mResamplingThresholdMs = 900;
    int mResamplingThresholdSamples = 0;
    
    
    
};

#endif /* GranularSnH_hpp */
