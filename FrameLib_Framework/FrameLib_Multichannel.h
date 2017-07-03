
#ifndef FRAMELIB_MULTICHANNEL_H
#define FRAMELIB_MULTICHANNEL_H

#include "FrameLib_Context.h"
#include "FrameLib_Block.h"
#include "FrameLib_DSP.h"
#include "FrameLib_ConnectionQueue.h"
#include <algorithm>
#include <vector>

// FrameLib_MultiChannel

// This abstract class allows mulitchannel connnections and the means to update the network according to the number of channels

class FrameLib_MultiChannel : private FrameLib_ConnectionQueue::Item
{
    // Connection and Other Structures
    
protected:

    struct ConnectionInfo
    {
        ConnectionInfo(FrameLib_Block *object = NULL, unsigned long idx = 0) : mObject(object), mIndex(idx) {}
        
        FrameLib_Block *mObject;
        unsigned long mIndex;
    };

private:
    
    struct MultiChannelInput
    {
        FrameLib_MultiChannel *mObject;
        unsigned long mIndex;
    };
    
    struct MultiChannelOutput
    {
        std::vector <ConnectionInfo> mConnections;
    };
    
public:
    
    // Constructors

    FrameLib_MultiChannel(FrameLib_Context context, unsigned long nIns, unsigned long nOuts)
    : mContext(context), mQueue(context.getConnectionQueue())
    { setIO(nIns, nOuts); }
    
    FrameLib_MultiChannel(FrameLib_Context context) : mContext(context), mQueue(context.getConnectionQueue()) {}
    
    // Destructor (virtual) - inheriting classes MUST call clearConnections(), before deleting internal objects
    
    virtual ~FrameLib_MultiChannel()
    {
        mContext.releaseConnectionQueue();
    }
    
    // Basic Setup / IO Queries (override the audio methods if handling audio)

    virtual void setSamplingRate(double samplingRate) {};

    unsigned long getNumIns()                   { return mInputs.size(); }
    unsigned long getNumOuts()                  { return mOutputs.size(); }
    virtual unsigned long getNumAudioIns()      { return 0; }
    virtual unsigned long getNumAudioOuts()     { return 0; }

    // Set Fixed Inputs
    
    virtual void setFixedInput(unsigned long idx, double *input, unsigned long size) {};

    // Audio processing
    
    // Override to handle audio at the block level (objects with block-based audio must overload this)

    virtual void blockProcess(double **ins, double **outs, unsigned long vecSize) {}
    
    static bool handlesAudio() { return false; }
    
    virtual void reset() {}
    
    // Multichannel updates
    
    // N.B. - No sanity checks here to maximise speed and help debugging (better for it to crash if a mistake is made)
    
    void deleteConnection(unsigned long inIdx);
    void addConnection(FrameLib_MultiChannel *object, unsigned long outIdx, unsigned long inIdx);
    void clearConnections();
    bool isConnected(unsigned long inIdx);

protected:
    
    // IO Utilities
    
    // Call this in derived class constructors if the IO size is not static
    
    void setIO(unsigned long nIns, unsigned long nOuts)
    {
        mInputs.resize(nIns);
        mOutputs.resize(nOuts);
    }
    
    void outputUpdate();
    
    unsigned long getNumChans(unsigned long inIdx);
    ConnectionInfo getChan(unsigned long inIdx, unsigned long chan);

    // ************************************************************************************** //

    // Connection methods (private)

private:

    // Dependency updating

    std::vector <FrameLib_MultiChannel *>::iterator removeOutputDependency(FrameLib_MultiChannel *object);
    void addOutputDependency(FrameLib_MultiChannel *object);

    // Removal of one connection to this object (before replacement / deletion)
    
    void removeConnection(unsigned long inIdx);

    // Removal of all connections from one object to this object
    
    std::vector <FrameLib_MultiChannel *>::iterator removeConnections(FrameLib_MultiChannel *object);

    // Member variables
    
protected:

    // Context
    
    FrameLib_Context mContext;
    
    // Outputs
    
    std::vector <MultiChannelOutput> mOutputs;

private:
    
    // Queue
    
    FrameLib_ConnectionQueue *mQueue;
    
    // Connection Info
    
    std::vector <FrameLib_MultiChannel *> mOutputDependencies;
    std::vector <MultiChannelInput> mInputs;
};


// ************************************************************************************** //

// FrameLib_Pack - Pack single multichannel signals

class FrameLib_Pack : public FrameLib_MultiChannel
{
    enum AtrributeList {kInputs};

public:
    
    FrameLib_Pack(FrameLib_Context context, FrameLib_Attributes::Serial *serialAttributes, void *owner);
    ~FrameLib_Pack();
    
private:
    
    virtual void inputUpdate();
    
    FrameLib_Attributes mAttributes;
};

// ************************************************************************************** //

// FrameLib_Unpack - Unpack multichannel signals

class FrameLib_Unpack : public FrameLib_MultiChannel
{
    enum AtrributeList {kOutputs};
    
public:

    FrameLib_Unpack(FrameLib_Context context, FrameLib_Attributes::Serial *serialAttributes, void *owner);
    ~FrameLib_Unpack();
        
private:

    // Update (unpack)
    
    virtual void inputUpdate();
        
    FrameLib_Attributes mAttributes;
};

// ************************************************************************************** //

// FrameLib_Expand - MultiChannel expansion for FrameLib_Block objects

template <class T> class FrameLib_Expand : public FrameLib_MultiChannel
{

public:
    
    FrameLib_Expand(FrameLib_Context context, FrameLib_Attributes::Serial *serialisedAttributes, void *owner)
    : FrameLib_MultiChannel(context), mAllocator(context.getAllocator()), mOwner(owner)
    {
        // Make first block
        
        mBlocks.push_back(new T(context, serialisedAttributes, owner));
        
        // Copy serialised attributes for later instantiations
        
        mSerialisedAttributes = new FrameLib_Attributes::Serial(serialisedAttributes->size());
        mSerialisedAttributes->write(serialisedAttributes);
        
        // Set up IO
        
        setIO(mBlocks[0]->getNumIns(), mBlocks[0]->getNumOuts());
        
        // Set up Fixed Inputs
        
        mFixedInputs.resize(getNumIns());
        
        // Make initial output connections
        
        for (unsigned long i = 0; i < getNumOuts(); i++)
            mOutputs[i].mConnections.push_back(ConnectionInfo(mBlocks[0], i));
        
        setSamplingRate(0.0);
    }
    
    ~FrameLib_Expand()
    {
        // Clear connections before deletion
        
        clearConnections();
        
        // Delete blocks
        
        for (std::vector <FrameLib_Block *> :: iterator it = mBlocks.begin(); it != mBlocks.end(); it++)
            delete (*it);
        
        delete mSerialisedAttributes;
        
        mContext.releaseAllocator();
    }
    
    // Fixed Inputs
    
    virtual void setFixedInput(unsigned long idx, double *input, unsigned long size)
    {
        if (idx < mFixedInputs.size())
        {
            mFixedInputs[idx].assign(input, input + size);
            updateFixedInput(idx);
        }
    }
    
    // Sampling Rate

    virtual void setSamplingRate(double samplingRate)
    {
        mSamplingRate = samplingRate;
        
        for (std::vector <FrameLib_Block *> :: iterator it = mBlocks.begin(); it != mBlocks.end(); it++)
            (*it)->setSamplingRate(samplingRate);
    }
    
    // Reset
    
    void reset()
    {
        for (std::vector <FrameLib_Block *> :: iterator it = mBlocks.begin(); it != mBlocks.end(); it++)
            (*it)->reset();
    }
    
    // Handles Audio
    
    static bool handlesAudio() { return T::handlesAudio(); }
    
    // IO
    
    virtual unsigned long getNumAudioIns()  { return mBlocks[0]->getNumAudioIns(); }
    virtual unsigned long getNumAudioOuts() { return mBlocks[0]->getNumAudioOuts(); }
    
    // ************************************************************************************** //
    
    // Audio Processing
    
    virtual void blockProcess(double **ins, double **outs, unsigned long vecSize)
    {
        // FIX - this won't fly on windows...
        
        double *temps[getNumAudioOuts()];
        
        for (unsigned long i = 0; i < getNumAudioOuts(); i++)
        {
            if (i == 0)
                temps[0] = (double *) mAllocator->alloc(sizeof(double) * vecSize * getNumAudioOuts());
            else
                temps[i] = temps[0] + (i * vecSize);

            std::fill(outs[i], outs[i] + vecSize, 0.0);
        }
        
        for (std::vector <FrameLib_Block *> :: iterator it = mBlocks.begin(); it != mBlocks.end(); it++)
        {
            // Process then sum to output
            
            (*it)->blockUpdate(ins, temps, vecSize);
            
            for (unsigned long i = 0; i < getNumAudioOuts(); i++)
                for (unsigned long j = 0; j < vecSize; j++)
                    outs[i][j] += temps[i][j];
        }

        if (getNumAudioOuts())
            mAllocator->dealloc(temps[0]);
                
        mAllocator->clear();
    }
    
    // ************************************************************************************** //

private:

    // Update Fixed Inputs
    
    void updateFixedInput(unsigned long idx)
    {
        for (unsigned long i = 0; i < mBlocks.size(); i++)
            mBlocks[i]->setFixedInput(idx, &mFixedInputs[idx][0], mFixedInputs[idx].size());
    }
    
    // Update (expand)
    
    void inputUpdate()
    {        
        // Find number of channels
        
        unsigned long nChannels = 0;
        unsigned long cChannels = mBlocks.size();
        
        for (unsigned long i = 0; i < getNumIns(); i++)
            if (getNumChans(i) > nChannels)
                nChannels = getNumChans(i);
        
        // Always maintain at least one object
        
        nChannels = nChannels ? nChannels : 1;
        
        // Resize if necessary
        
        if (nChannels != cChannels)
        {
            if (nChannels > cChannels)
            {
                mBlocks.resize(nChannels);
                
                for (unsigned long i = cChannels; i < nChannels; i++)
                {
                    mBlocks[i] = new T(mContext, mSerialisedAttributes, mOwner);
                    mBlocks[i]->setSamplingRate(mSamplingRate);
                }
            }
            else
            {
                for (unsigned long i = nChannels; i < cChannels; i++)
                    delete mBlocks[i];
                
                mBlocks.resize(nChannels);
            }
            
            // Redo output connection lists
            
            for (unsigned long i = 0; i < getNumOuts(); i++)
                mOutputs[i].mConnections.clear();
            
            for (unsigned long i = 0; i < getNumOuts(); i++)
                for (unsigned long j = 0; j < nChannels; j++)
                    mOutputs[i].mConnections.push_back(ConnectionInfo(mBlocks[j], i));
            
            // Update Fixed Inputs
            
            for (unsigned long i = 0; i < getNumIns(); i++)
                updateFixedInput(i);
            
            // Update output dependencies
            
            outputUpdate();
        }
        
        // Make input connections

        for (unsigned long i = 0; i < getNumIns(); i++)
        {
            if (getNumChans(i))
            {
                for (unsigned long j = 0; j < nChannels; j++)
                {
                    ConnectionInfo connection = getChan(i, j % getNumChans(i));
                    mBlocks[j]->addConnection(connection.mObject, connection.mIndex, i);
                }
            }
            else
            {
                for (unsigned long j = 0; j < nChannels; j++)
                    mBlocks[j]->deleteConnection(i);
            }
        }
    }

    FrameLib_LocalAllocator *mAllocator;
    FrameLib_Attributes::Serial *mSerialisedAttributes;

    std::vector <FrameLib_Block *> mBlocks;
    std::vector <std::vector <double> > mFixedInputs;
    
    void *mOwner;
    
    double mSamplingRate;
};

#endif
