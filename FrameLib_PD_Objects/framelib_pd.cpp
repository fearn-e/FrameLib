
#include "FrameLib_PDClass.h"

// Filters

#include "FrameLib_0dfSVF.h"
#include "FrameLib_OnePole.h"
#include "FrameLib_OnePoleZero.h"
#include "FrameLib_Resonant.h"
#include "FrameLib_SallenAndKey.h"

// Generators

#include "FrameLib_Gaussian.h"
#include "FrameLib_Ramp.h"
#include "FrameLib_Random.h"
#include "FrameLib_Uniform.h"

// IO

#include "FrameLib_Source.h"
#include "FrameLib_Sink.h"
#include "FrameLib_Trace.h"

// Mapping

#include "FrameLib_Constant.h"
#include "FrameLib_Convert.h"
#include "FrameLib_Lookup.h"
#include "FrameLib_Map.h"
#include "FrameLib_SampleRate.h"

// Parameters

#include "FrameLib_CombineTags.h"
#include "FrameLib_FilterTags.h"
#include "FrameLib_GetParam.h"
#include "FrameLib_Tag.h"
#include "FrameLib_Untag.h"

// Routing

#include "FrameLib_Dispatch.h"
#include "FrameLib_Route.h"
#include "FrameLib_Select.h"

// Schedulers

#include "FrameLib_AudioTrigger.h"
#include "FrameLib_Future.h"
#include "FrameLib_Interval.h"
#include "FrameLib_Once.h"
#include "FrameLib_PerBlock.h"

// Spatial

#include "FrameLib_CoordinateSystem.h"
#include "FrameLib_Spatial.h"

// Spectral

#include "FrameLib_FFT.h"
#include "FrameLib_iFFT.h"
#include "FrameLib_Correlate.h"
#include "FrameLib_Convolve.h"
#include "FrameLib_Multitaper.h"
#include "FrameLib_Window.h"

// Storage

#include "FrameLib_Recall.h"
#include "FrameLib_Register.h"
#include "FrameLib_Store.h"

// Streaming

#include "FrameLib_StreamID.h"

// Time Smoothing

#include "FrameLib_EWMA.h"
#include "FrameLib_EWMSD.h"
#include "FrameLib_FrameDelta.h"
#include "FrameLib_Lag.h"
#include "FrameLib_TimeMean.h"
#include "FrameLib_TimeMedian.h"
#include "FrameLib_TimeStdDev.h"

// Timing

#include "FrameLib_Now.h"
#include "FrameLib_Ticks.h"
#include "FrameLib_TimeDelta.h"
#include "FrameLib_Timer.h"

// Vector

#include "FrameLib_AccumPoint.h"
#include "FrameLib_Chop.h"
#include "FrameLib_Join.h"
#include "FrameLib_MedianFilter.h"
#include "FrameLib_NanFilter.h"
#include "FrameLib_NonZero.h"
#include "FrameLib_Pad.h"
#include "FrameLib_Peaks.h"
#include "FrameLib_Percentile.h"
#include "FrameLib_Reverse.h"
#include "FrameLib_Shift.h"
#include "FrameLib_Sort.h"
#include "FrameLib_Split.h"
#include "FrameLib_Subframe.h"
#include "FrameLib_Vector_Objects.h"

// Operators

#include "FrameLib_Unary_Objects.h"
#include "FrameLib_Binary_Objects.h"
#include "FrameLib_Ternary_Objects.h"
#include "FrameLib_ExpressionGraph.h"

// Complex Operators

#include "FrameLib_Complex_Unary_Objects.h"
#include "FrameLib_Cartopol.h"
#include "FrameLib_Poltocar.h"
#include "FrameLib_Complex_Binary_Objects.h"

// Buffer

#include "FrameLib_Read.h"
#include "PD_Specific/pd_buffer.h"

// PD Read Class

class FrameLib_PDClass_Read : public FrameLib_PDClass<FrameLib_Expand<FrameLib_Read> >
{
    struct ReadProxy : public FrameLib_Read::Proxy, public FrameLib_PDProxy
    {
        virtual void update(const char *name)
        {
            mBufferName = gensym(name);
        }
        
        virtual void acquire(unsigned long& length, double& samplingRate)
        {
            mBuffer = pd_buffer(mBufferName);
            length = mBuffer.get_length();
            samplingRate = 0.0;
        }
        
        virtual void release()
        {
            mBuffer = pd_buffer();
        };
        
        virtual void read(double *output, const double *positions, unsigned long size, long chan, InterpType interpType)
        {
            mBuffer.read(output, positions, size, 1.0, interpType);
        }
        
    private:
        
        pd_buffer mBuffer;
        t_symbol *mBufferName;
    };
    
public:
    
    // Constructor
    
    FrameLib_PDClass_Read(t_symbol *s, long argc, t_atom *argv) : FrameLib_PDClass(s, argc, argv, new ReadProxy()) {}
};

// PD Expression Class

// The expression object parses it's arguments differently to normal, which is handled by pre-parsing the atoms into a different format

class ArgumentParser
{
    
public:
    
    ArgumentParser(t_symbol *s, long argc, t_atom *argv) : mSymbol(s)
    {
        concatenate(argc, argv);
        
        while (argc--)
        {
            if (atom_getsymbol(argv) == gensym("/expr"))
                concatenate(argc, ++argv);
            else
                mArgs.push_back(*argv++);
        }
    }
    
    t_symbol *symbol() const    { return mSymbol;}
    long count() const          { return static_cast<long>(mArgs.size());}
    t_atom* args() const        { return const_cast<t_atom*>(mArgs.data()); }
    
private:
    
    // Tag Detection
    
    bool isValidTag(t_atom *a)
    {
        t_symbol *sym = atom_getsymbol(a);
        size_t len = strlen(sym->s_name);
        
        // Input tag
        
        if (len > 2 && sym->s_name[0] == '[' && sym->s_name[len - 1] == ']')
            return true;
        
        // Basic parameter tag test
        
        if (len <= 1 || sym->s_name[0] != '/')
            return false;
        
        // Escape division by known constants
        
        if (sym == gensym("/pi") || sym == gensym("/epsilon") || sym == gensym("/e") || sym == gensym("/inf"))
            return false;
        
        // Require a parameter tag to have only letters after the slash
        
        for (const char *c = sym->s_name + 1; *c; c++)
            if (!((*c >= 'a') && (*c <= 'z')) || ((*c >= 'A') && (*c <= 'Z')))
                return false;
        
        return true;
    }
    
    // Concatenator
    
    void concatenate(long& argc, t_atom*& argv)
    {
        std::string concatenated;
        
        for (; argc && !isValidTag(argv); argc--, argv++)
        {
            if (argv->a_type == A_SYMBOL)
                concatenated += atom_getsymbol(argv)->s_name;
            else
                concatenated += std::to_string(atom_getfloat(argv));
            
            // Add whitespace between symbols
            
            concatenated += " ";
        }
        
        if (concatenated.length())
        {
            t_symbol *arg = gensym(concatenated.c_str());
            mArgs.push_back(t_atom());
            SETSYMBOL(&mArgs.back(), arg);
        }
    }
    
    t_symbol *mSymbol;
    std::vector<t_atom> mArgs;
};

// This class is a wrapper that allows the parsing to happen correctly

struct FrameLib_PDClass_Expression_Parsed : public FrameLib_PDClass<FrameLib_Expand<FrameLib_Expression> >
{
    FrameLib_PDClass_Expression_Parsed(const ArgumentParser &parsed) :
    FrameLib_PDClass(parsed.symbol(), parsed.count(), parsed.args(), new FrameLib_PDProxy()) {}
};

// PD Class (inherits from the parsed version which inherits the standard pd class

struct FrameLib_PDClass_Expression : public FrameLib_PDClass_Expression_Parsed
{
    // Constructor
    
    FrameLib_PDClass_Expression(t_symbol *s, long argc, t_atom *argv) :
    FrameLib_PDClass_Expression_Parsed(ArgumentParser(s, argc, argv)) {}
};

extern "C" void framelib_pd_setup(void)
{
    // Filters
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_0dfSVF> >::makeClass("fl.0dfsvf~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_OnePole> >::makeClass("fl.onepole~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_OnePoleZero> >::makeClass("fl.onepolezero~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Resonant> >::makeClass("fl.resonant~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_SallenAndKey> >::makeClass("fl.sallenkey~");
  
    // Generators
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Gaussian> >::makeClass("fl.gaussian~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Ramp> >::makeClass("fl.ramp~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Random> >::makeClass("fl.random~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Uniform> >::makeClass("fl.uniform~");

    // IO
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Sink> >::makeClass("fl.sink~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Source> >::makeClass("fl.source~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Trace> >::makeClass("fl.trace~");

    // Mapping
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Constant> >::makeClass("fl.constant~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Convert> >::makeClass("fl.convert~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Lookup> >::makeClass("fl.lookup~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Map> >::makeClass("fl.map~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_SampleRate> >::makeClass("fl.samplerate~");
    
    // Parameters
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_CombineTags> >::makeClass("fl.combinetags~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_FilterTags> >::makeClass("fl.filtertags~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_GetParam> >::makeClass("fl.getparam~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Tag> >::makeClass("fl.tag~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Untag> >::makeClass("fl.untag~");
    
    // Routing
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Dispatch> >::makeClass("fl.dispatch~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Route> >::makeClass("fl.route~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Select> >::makeClass("fl.select~");
    
    // Schedulers
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_AudioTrigger> >::makeClass("fl.audiotrigger~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Future> >::makeClass("fl.future~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Interval> >::makeClass("fl.interval~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Once> >::makeClass("fl.once~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_PerBlock> >::makeClass("fl.perblock~");
    
    // Spatial
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_CoordinateSystem> >::makeClass("fl.coordinatesystem~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Spatial> >::makeClass("fl.spatial~");
    
    // Spectral
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_FFT> >::makeClass("fl.fft~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_iFFT> >::makeClass("fl.ifft~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Correlate> >::makeClass("fl.correlate~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Convolve> >::makeClass("fl.convolve~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Multitaper> >::makeClass("fl.multitaper~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Window> >::makeClass("fl.window~");
    
    // Storage
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Recall> >::makeClass("fl.recall~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Register> >::makeClass("fl.register~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Store> >::makeClass("fl.store~");
    
    // Streaming
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_StreamID> >::makeClass("fl.streamid~");
    
    // Time Smoothing
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_EWMA> >::makeClass("fl.ewma~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_EWMSD> >::makeClass("fl.ewmsd~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_FrameDelta> >::makeClass("fl.framedelta~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Lag> >::makeClass("fl.lag~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_TimeMean> >::makeClass("fl.timemean~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_TimeMedian> >::makeClass("fl.timemedian~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_TimeStdDev> >::makeClass("fl.timestddev~");
    
    // Timing
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Now> >::makeClass("fl.now~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Ticks> >::makeClass("fl.ticks~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_TimeDelta> >::makeClass("fl.timedelta~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Timer> >::makeClass("fl.timer~");
    
    // Vector
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_AccumPoint> >::makeClass("fl.accumpoint~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Chop> >::makeClass("fl.chop~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Join> >::makeClass("fl.join~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_MedianFilter> >::makeClass("fl.medianfilter~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_NonZero> >::makeClass("fl.nonzero~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Pad> >::makeClass("fl.pad~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Peaks> >::makeClass("fl.peaks~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Percentile> >::makeClass("fl.percentile~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Reverse> >::makeClass("fl.reverse~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Shift> >::makeClass("fl.shift~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Sort> >::makeClass("fl.sort~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Split> >::makeClass("fl.split~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Subframe> >::makeClass("fl.subframe~");

    FrameLib_PDClass<FrameLib_Expand<FrameLib_Length> >::makeClass("fl.length~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_VectorMin> >::makeClass("fl.vmin~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_VectorMax> >::makeClass("fl.vmax~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Sum> >::makeClass("fl.sum~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Product> >::makeClass("fl.product~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Mean> >::makeClass("fl.mean~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_GeometricMean> >::makeClass("fl.geometricmean~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_StandardDeviation> >::makeClass("fl.standarddeviation~");

    FrameLib_PDClass<FrameLib_Expand<FrameLib_Centroid> >::makeClass("fl.centroid~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Spread> >::makeClass("fl.spread~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Skewness> >::makeClass("fl.skewness~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Kurtosis> >::makeClass("fl.kurtosis~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Flatness> >::makeClass("fl.flatness~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_RMS> >::makeClass("fl.rms~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Crest> >::makeClass("fl.crest~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_VectorArgMin> >::makeClass("fl.argmin~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_VectorArgMax> >::makeClass("fl.argmax~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_NanFilter> >::makeClass("fl.nanfilter~");

    
    // Unary Operators
    
    FrameLib_PDClass_Expand<FrameLib_LogicalNot>::makeClass("fl.not~");
    
    FrameLib_PDClass_Expand<FrameLib_Acos>::makeClass("fl.acos~");
    FrameLib_PDClass_Expand<FrameLib_Asin>::makeClass("fl.asin~");
    FrameLib_PDClass_Expand<FrameLib_Atan>::makeClass("fl.atan~");
    FrameLib_PDClass_Expand<FrameLib_Cos>::makeClass("fl.cos~");
    FrameLib_PDClass_Expand<FrameLib_Sin>::makeClass("fl.sin~");
    FrameLib_PDClass_Expand<FrameLib_Tan>::makeClass("fl.tan~");
    FrameLib_PDClass_Expand<FrameLib_Acosh>::makeClass("fl.acosh~");
    FrameLib_PDClass_Expand<FrameLib_Asinh>::makeClass("fl.asinh~");
    FrameLib_PDClass_Expand<FrameLib_Atanh>::makeClass("fl.atanh~");
    FrameLib_PDClass_Expand<FrameLib_Cosh>::makeClass("fl.cosh~");
    FrameLib_PDClass_Expand<FrameLib_Sinh>::makeClass("fl.sinh~");
    FrameLib_PDClass_Expand<FrameLib_Tanh>::makeClass("fl.tanh~");
    
    FrameLib_PDClass_Expand<FrameLib_Log>::makeClass("fl.log~");
    FrameLib_PDClass_Expand<FrameLib_Log2>::makeClass("fl.log2~");
    FrameLib_PDClass_Expand<FrameLib_Log10>::makeClass("fl.log10~");
    FrameLib_PDClass_Expand<FrameLib_Exp>::makeClass("fl.exp~");
    FrameLib_PDClass_Expand<FrameLib_Exp2>::makeClass("fl.exp2~");
    
    FrameLib_PDClass_Expand<FrameLib_Abs>::makeClass("fl.abs~");
    FrameLib_PDClass_Expand<FrameLib_Ceil>::makeClass("fl.ceil~");
    FrameLib_PDClass_Expand<FrameLib_Floor>::makeClass("fl.floor~");
    FrameLib_PDClass_Expand<FrameLib_Round>::makeClass("fl.round~");
    FrameLib_PDClass_Expand<FrameLib_Trunc>::makeClass("fl.trunc~");
    
    FrameLib_PDClass_Expand<FrameLib_Sqrt>::makeClass("fl.sqrt~");
    FrameLib_PDClass_Expand<FrameLib_Cbrt>::makeClass("fl.cbrt~");
    FrameLib_PDClass_Expand<FrameLib_Erf>::makeClass("fl.erf~");
    FrameLib_PDClass_Expand<FrameLib_Erfc>::makeClass("fl.erfc~");
    
    // Binary  Operators
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Plus>, kAllInputs>::makeClass("fl.+~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Minus>, kAllInputs>::makeClass("fl.-~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Multiply>, kAllInputs>::makeClass("fl.*~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Divide>, kAllInputs>::makeClass("fl./~");
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Equal>, kAllInputs>::makeClass("fl.==~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_NotEqual>, kAllInputs>::makeClass("fl.!=~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_GreaterThan>, kAllInputs>::makeClass("fl.>~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_LessThan>, kAllInputs>::makeClass("fl.<~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_GreaterThanEqual>, kAllInputs>::makeClass("fl.>=~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_LessThanEqual>, kAllInputs>::makeClass("fl.<=~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_LogicalAnd>, kAllInputs>::makeClass("fl.and~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_LogicalOr>, kAllInputs>::makeClass("fl.or~");
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Pow>, kAllInputs>::makeClass("fl.pow~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Atan2>, kAllInputs>::makeClass("fl.atan2~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Hypot>, kAllInputs>::makeClass("fl.hypot~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_CopySign>, kAllInputs>::makeClass("fl.copysign~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Min>, kAllInputs>::makeClass("fl.min~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Max>, kAllInputs>::makeClass("fl.max~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Diff>, kAllInputs>::makeClass("fl.diff~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Modulo>, kAllInputs>::makeClass("fl.%~");

    // Ternary  Operators
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Clip>, kDistribute>::makeClass("fl.clip~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Fold>, kDistribute>::makeClass("fl.fold~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Wrap>, kDistribute>::makeClass("fl.wrap~");
    
    // Complex Unary Operators
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Cos>, kAllInputs>::makeClass("fl.complexcos~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Sin>, kAllInputs>::makeClass("fl.complexsin~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Tan>, kAllInputs>::makeClass("fl.complextan~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Cosh>, kAllInputs>::makeClass("fl.complexcosh~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Sinh>, kAllInputs>::makeClass("fl.complexsinh~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Tanh>, kAllInputs>::makeClass("fl.complextanh~");

    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Log>, kAllInputs>::makeClass("fl.complexlog~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Log10>, kAllInputs>::makeClass("fl.complexlog10~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Exp>, kAllInputs>::makeClass("fl.complexexp~");

    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Sqrt>, kAllInputs>::makeClass("fl.complexsqrt~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Conj>, kAllInputs>::makeClass("fl.complexconj~");
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Cartopol>, kAllInputs>::makeClass("fl.cartopol~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Poltocar>, kAllInputs>::makeClass("fl.poltocar~");
    
    // Complex Binary Operators
    
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Plus>, kAllInputs>::makeClass("fl.complexplus~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Minus>, kAllInputs>::makeClass("fl.complexminus~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Multiply>, kAllInputs>::makeClass("fl.complexmultiply~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Divide>, kAllInputs>::makeClass("fl.complexdivide~");
    FrameLib_PDClass<FrameLib_Expand<FrameLib_Complex_Pow>, kAllInputs>::makeClass("fl.complexpow~");

    // Buffer
    
    FrameLib_PDClass_Read::makeClass<FrameLib_PDClass_Read>("fl.read~");
}
