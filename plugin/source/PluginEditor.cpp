#include "VisualizerPlugin/PluginProcessor.h"
#include "VisualizerPlugin/PluginEditor.h"

//==============================================================================
VisualizerPluginAudioProcessorEditor::VisualizerPluginAudioProcessorEditor (VisualizerPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 600);
    startTimerHz(60);
}

VisualizerPluginAudioProcessorEditor::~VisualizerPluginAudioProcessorEditor()
{
}

//==============================================================================
void VisualizerPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (getLookAndFeel().findColour (juce::Slider::thumbColourId));
    auto numberOfDots = getFrameCounter(); // [1]
    juce::Path spinePath; // [2]
    for (auto i = 0; i < numberOfDots; ++i) // [3]
    {
        int radius = 150;
        juce::Point<float> p ((float) getWidth() / 2.0f + 1.0f * (float) radius * std::sin ((float) getFrameCounter() * 0.04f + (float) i * 0.12f),
            (float) getHeight() / 2.0f + 1.0f * (float) radius * std::cos ((float) getFrameCounter() * 0.04f + (float) i * 0.12f));
        if (i == 0)
            spinePath.startNewSubPath (p); // if this is the first point, start a new path..
        else
            spinePath.lineTo (p); // ...otherwise add the next point
    }
    // draw an outline around the path that we have created
    g.strokePath (spinePath, juce::PathStrokeType (4.0f)); // [4]
}

void VisualizerPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void VisualizerPluginAudioProcessorEditor::timerCallback()
{
    incrementFrameCounter();
    repaint();
}