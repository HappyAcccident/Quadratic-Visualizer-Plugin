#include "VisualizerPlugin/PluginProcessor.h"
#include "VisualizerPlugin/PluginEditor.h"

//==============================================================================
VisualizerPluginAudioProcessorEditor::VisualizerPluginAudioProcessorEditor (VisualizerPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 600);
}

VisualizerPluginAudioProcessorEditor::~VisualizerPluginAudioProcessorEditor()
{
}

//==============================================================================
void VisualizerPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.setFont (15.0f);
    
}

void VisualizerPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
