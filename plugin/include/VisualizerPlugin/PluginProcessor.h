#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

using Filter = juce::dsp::IIR::Filter<float>;

using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;

struct ChainSettings
{

};

//==============================================================================
class VisualizerPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    VisualizerPluginAudioProcessor();
    ~VisualizerPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
private:
    juce::dsp::ProcessorChain<CutFilter> leftBassFilterChain;
    juce::dsp::ProcessorChain<CutFilter> rightBassFilterChain;
    juce::dsp::ProcessorChain<CutFilter, CutFilter> leftMidFilterChain;
    juce::dsp::ProcessorChain<CutFilter, CutFilter> rightMidFilterChain;
    juce::dsp::ProcessorChain<CutFilter> leftTrebleFilterChain;
    juce::dsp::ProcessorChain<CutFilter> rightTrebleFilterChain;

    juce::AudioBuffer<float> bassBuffer;
    juce::AudioBuffer<float> midBuffer;
    juce::AudioBuffer<float> trebleBuffer;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualizerPluginAudioProcessor)
};
