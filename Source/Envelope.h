//
//  Envelope.h
//  Created by Christian Tronhjem on 03.03.22.
//

#ifndef Envelope_h
#define Envelope_h
#include "UtilityFunctions.h"
#include <math.h>

namespace CTDSP
{

enum EnvelopeType
{
    Linear = 0,
    Exponential,
};

class Envelope
{
public:
    Envelope(int size = 2048, float fadePercentage = 0.125f, EnvelopeType eType = EnvelopeType::Linear) :
        mLength(size),
        mEnvelopeType(eType)
    {
        mEnvelope = new double[mLength]{0};
        
        // with Length of 2048, half size is 1024
        // with 0.125f fade size this is a fade of 128 samples.
        int halfSize = static_cast<int>(mLength / 2.0);
        mFadeSizeInSamples = halfSize * fadePercentage;
        
        switch (mEnvelopeType)
        {
            case EnvelopeType::Linear:
                GenerateLinearEnv();
                break;
            case EnvelopeType::Exponential:
                GenerateExponentialEnv();
                break;
            default:
                break;
        }
    }
    
    ~Envelope()
    {
        delete[] mEnvelope;
    }
    
    /// Get the envelope at a specific index.
    double GetEnvelopeAtIndex(const int index) const
    {
#if DEBUG
        assert(index < mLength);
        assert(index >= 0);
#endif
        return mEnvelope[index];
    }
   
    /// Takes a percentage of the buffer, to seek for the envelope at that time.
    /// Percentage is between 0 and 1
    double GetEnvelope(const double t) const
    {
#if DEBUG
        assert(t >= 0);
        assert(t <= 1);
#endif
        const double pureIndex = t * mLength;
        const int indexLow = static_cast<int>(floor(pureIndex));
        const int indexHigh = static_cast<int>(ceil(pureIndex));
        const double lerpT = static_cast<double>(pureIndex - indexLow);
        
        return Lerp(mEnvelope[indexLow], mEnvelope[indexHigh], lerpT);
    }
    
    int GetLength() const
    {
        return mLength;
    }
    
private:
    void inline GenerateLinearEnv()
    {
#if DEBUG
        assert(mLength > 0);
#endif
        
        double const fadeCoeff = 1/mFadeSizeInSamples;
        int length = mLength - 1;
        for (int i=0; i < mLength; ++i)
        {
            double gainFactor = 1;
            if (i <= mFadeSizeInSamples)
            {
                gainFactor = i * fadeCoeff;
            }
            else if (i >= length - mFadeSizeInSamples)
            {
                gainFactor = (length - i) * fadeCoeff;
            }
            mEnvelope[i] = gainFactor;
        }
    }
    
    void inline GenerateExponentialEnv()
    {
        double const fadeCoeff = 1/mFadeSizeInSamples;
        int length = mLength - 1;
        for (int i=0; i < mLength; ++i)
        {
            double gainFactor = 1;
            if (i <= mFadeSizeInSamples)
            {
                gainFactor = i * fadeCoeff * fadeCoeff;
            }
            else if (i >= length - mFadeSizeInSamples)
            {
                gainFactor = (length - i) * fadeCoeff * fadeCoeff;
            }
            mEnvelope[i] = gainFactor;
        }
    }
    
    int mLength;
    EnvelopeType mEnvelopeType;
    float mFadeSizeInSamples;
    double* mEnvelope;
};
} // namespace end
#endif /* Envelope_h */
