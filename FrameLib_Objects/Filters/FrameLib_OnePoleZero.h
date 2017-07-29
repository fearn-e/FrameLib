
#ifndef FRAMELIB_ONEPOLEZERO_H
#define FRAMELIB_ONEPOLEZERO_H

#include "FrameLib_DSP.h"
#include "FrameLib_Filter_Constants.h"

// FIX - All filters to templates
// FIX - time varying params

class FrameLib_OnePoleZero : public FrameLib_Processor
{
    // Filter Class

    class OnePoleZero
    {
        
    public:
        
        OnePoleZero() : a0(0.0), a1(0.0), r1(0.0) {}
        
        // Reset

        void reset()                { r1 = 0.0; }
        
        // Filter Types

        double HPF(double x)        { return x - calculateFilter(x); }
        double LPF(double x)        { return calculateFilter(x); }

        // Set Parameters
        
        void setParams(double freq, double samplingRate);
        
    private:
        
        // Filter Calculation

        inline double calculateFilter(double x);
        
        // Coefficients and Memories

        double a0, a1, r1;
    };
    
    // Parameter Enums and Info

    enum ParameterList { kFreq, kMode };
    enum Modes { kLPF, kHPF };

public:
	
    // Constructor
    
    FrameLib_OnePoleZero(FrameLib_Context context, FrameLib_Parameters::Serial *serialisedParameters, void *owner);
    
private:
    
    // Update and Process

    void update();
    void process();
};

#endif
