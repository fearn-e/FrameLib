
#include "FrameLib_DSP.h"

class FrameLib_Interval : public FrameLib_Scheduler
{
    enum AttributeList {kArg0, kInterval};

public:
    
    FrameLib_Interval(DSPQueue *queue, FrameLib_Attributes::Serial *serialisedAttributes) : FrameLib_Scheduler(queue, 0, 1)
    {
        mAttributes.addDouble(kArg0, "0", 1);
        mAttributes.addDouble(kInterval, "interval", 16);
        
        mAttributes.set(serialisedAttributes);
        
        if (!mAttributes.changed(kInterval))
            mAttributes.set(kInterval, mAttributes.getValue(kArg0));
        
        mInterval = mAttributes.getValue(kInterval);
    }
    
protected:
    
    SchedulerInfo schedule(bool newFrame, bool noOutput)
    {
        return SchedulerInfo(mInterval, TRUE, TRUE);
    }
    
private:
    
    FrameLib_TimeFormat mInterval;
};

#define OBJECT_CLASS FrameLib_Expand <FrameLib_Interval>
#define OBJECT_NAME "fl.interval~"

#include "Framelib_Max.h"
