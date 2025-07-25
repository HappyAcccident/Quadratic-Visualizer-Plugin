#include "VisualizerPlugin/PluginProcessor.h"
#include "VisualizerPlugin/PluginEditor.h"

//==============================================================================
VisualizerPluginAudioProcessor::VisualizerPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
}

VisualizerPluginAudioProcessor::~VisualizerPluginAudioProcessor()
{
}

//==============================================================================
const juce::String VisualizerPluginAudioProcessor::getName() const
{
    return "Quadratic Shape Visualizer";
}

bool VisualizerPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VisualizerPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VisualizerPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VisualizerPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VisualizerPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VisualizerPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VisualizerPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String VisualizerPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void VisualizerPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void VisualizerPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    
    auto& leftBassLowPass = leftBassFilterChain.get<0>();
    leftBassLowPass.get<0>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 250.0f);
    leftBassLowPass.get<1>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 250.0f);
    leftBassLowPass.get<2>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 250.0f);
    leftBassLowPass.get<3>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 250.0f);

    auto& rightBassLowPass = rightBassFilterChain.get<0>();
    rightBassLowPass.get<0>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 250.0f);
    rightBassLowPass.get<1>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 250.0f);
    rightBassLowPass.get<2>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 250.0f);
    rightBassLowPass.get<3>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 250.0f);

    auto& leftMidHighPass = leftMidFilterChain.get<0>();
    leftMidHighPass.get<0>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 100.0f);
    leftMidHighPass.get<1>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 100.0f);
    leftMidHighPass.get<2>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 100.0f);
    leftMidHighPass.get<3>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 100.0f);

    auto& rightMidHighPass = rightMidFilterChain.get<0>();
    rightMidHighPass.get<0>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 100.0f);
    rightMidHighPass.get<1>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 100.0f);
    rightMidHighPass.get<2>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 100.0f);
    rightMidHighPass.get<3>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 100.0f);

    auto& leftMidLowPass = leftMidFilterChain.get<1>();
    leftMidLowPass.get<0>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 2500.0f);
    leftMidLowPass.get<1>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 2500.0f);
    leftMidLowPass.get<2>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 2500.0f);
    leftMidLowPass.get<3>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 2500.0f);

    auto& rightMidLowPass = rightMidFilterChain.get<1>();
    rightMidLowPass.get<0>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 2500.0f);
    rightMidLowPass.get<1>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 2500.0f);
    rightMidLowPass.get<2>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 2500.0f);
    rightMidLowPass.get<3>().coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 2500.0f);

    auto& leftTrebleHighPass = leftTrebleFilterChain.get<0>();
    leftTrebleHighPass.get<0>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 2000.0f);
    leftTrebleHighPass.get<1>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 2000.0f);
    leftTrebleHighPass.get<2>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 2000.0f);
    leftTrebleHighPass.get<3>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 2000.0f);

    auto& rightTrebleHighPass = rightTrebleFilterChain.get<0>();
    rightTrebleHighPass.get<0>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 2000.0f);
    rightTrebleHighPass.get<1>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 2000.0f);
    rightTrebleHighPass.get<2>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 2000.0f);
    rightTrebleHighPass.get<3>().coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 2000.0f);

    bassBuffer.setSize(2, samplesPerBlock);
    midBuffer.setSize(2, samplesPerBlock);
    trebleBuffer.setSize(2, samplesPerBlock);


    juce::dsp::ProcessSpec spec;

    spec.maximumBlockSize = samplesPerBlock;

    spec.numChannels = 1;

    spec.sampleRate = sampleRate;

    leftBassFilterChain.prepare(spec);
    rightBassFilterChain.prepare(spec);
    leftMidFilterChain.prepare(spec);
    rightMidFilterChain.prepare(spec);
    leftMidFilterChain.prepare(spec);
    rightMidFilterChain.prepare(spec);
}

void VisualizerPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool VisualizerPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void VisualizerPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    bassBuffer.makeCopyOf(buffer);
    juce::dsp::AudioBlock<float> bassBlock(bassBuffer);
    auto leftBassBlock = bassBlock.getSingleChannelBlock(0);
    auto rightBassBlock = bassBlock.getSingleChannelBlock(1);
    juce::dsp::ProcessContextReplacing<float> leftBassContext(leftBassBlock);
    juce::dsp::ProcessContextReplacing<float> rightBassContext(rightBassBlock);

    midBuffer.makeCopyOf(buffer);
    juce::dsp::AudioBlock<float> midBlock(midBuffer);
    auto leftMidBlock = midBlock.getSingleChannelBlock(0);
    auto rightMidBlock = midBlock.getSingleChannelBlock(1);
    juce::dsp::ProcessContextReplacing<float> leftMidContext(leftMidBlock);
    juce::dsp::ProcessContextReplacing<float> rightMidContext(rightMidBlock);

    trebleBuffer.makeCopyOf(buffer);
    juce::dsp::AudioBlock<float> trebleBlock(trebleBuffer);
    auto leftTrebleBlock = trebleBlock.getSingleChannelBlock(0);
    auto rightTrebleBlock = trebleBlock.getSingleChannelBlock(1);
    juce::dsp::ProcessContextReplacing<float> leftTrebleContext(leftTrebleBlock);
    juce::dsp::ProcessContextReplacing<float> rightTrebleContext(rightTrebleBlock);

    leftBassFilterChain.process(leftBassContext);
    rightBassFilterChain.process(rightBassContext);

    leftMidFilterChain.process(leftMidContext);
    rightMidFilterChain.process(rightMidContext);

    leftTrebleFilterChain.process(leftTrebleContext);
    rightTrebleFilterChain.process(rightTrebleContext);
}

//==============================================================================
bool VisualizerPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VisualizerPluginAudioProcessor::createEditor()
{
    return new VisualizerPluginAudioProcessorEditor (*this);
}

//==============================================================================
void VisualizerPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void VisualizerPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VisualizerPluginAudioProcessor();
}
