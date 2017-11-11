

#ifndef FRAMELIB_SOURCE_H
#define FRAMELIB_SOURCE_H

#include "FrameLib_DSP.h"
#include <vector>

class FrameLib_Source : public FrameLib_AudioInput
{
    // Parameter Enums and Info

    enum ParameterList { kBufferSize, kLength, kUnits, kDelay };
    enum Units { kSamples, kMS, kSeconds };
    
    struct ParameterInfo : public FrameLib_Parameters::Info { ParameterInfo(); };
    
public:
    
    // Constructor
    
    FrameLib_Source(FrameLib_Context context, FrameLib_Parameters::Serial *serialisedParameters, void *owner);
    
    // Info
    
    std::string objectInfo(bool verbose);
    std::string inputInfo(unsigned long idx, bool verbose);
    std::string outputInfo(unsigned long idx, bool verbose);
    std::string audioInfo(unsigned long idx, bool verbose);
    
private:
    
    // Helpers
    
    size_t bufferSize() const { return mBuffer.size(); }
    
    unsigned long convertTimeToSamples(double time);
    
    void copy(const double *input, unsigned long offset, unsigned long size);
    
    // Object Reset, Block Process, Update and Process 
    
    void objectReset();
    void blockProcess(const double * const *ins, double **outs, unsigned long blockSize);
    void update();
    void process();
    
    // Data
    
    std::vector<double> mBuffer;
    unsigned long mLength;
    unsigned long mCounter;
    
    static ParameterInfo sParamInfo;
};

#endif
