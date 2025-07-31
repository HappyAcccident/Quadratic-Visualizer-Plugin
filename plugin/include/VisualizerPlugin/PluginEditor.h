#pragma once

#define _USE_MATH_DEFINES
#define M_PI 3.14159265359

#include <cmath>
#include "PluginProcessor.h"
#include "Shape.h"
#include "Display.h"

//==============================================================================
class VisualizerPluginAudioProcessorEditor final : public juce::AudioProcessorEditor,
juce::Timer
{
public:
    explicit VisualizerPluginAudioProcessorEditor (VisualizerPluginAudioProcessor&);
    ~VisualizerPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

    void update();

    void drawShape(const Shape& shape, juce::Graphics& g);


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VisualizerPluginAudioProcessor& processorRef;

    Display& display = Display::getInstance();

    State& currentState = State::getInstance();

    Shape* A;
    Shape* B;
    Shape* C;

    std::pair<Shape, Shape> quadraticRoot;
    Shape* pos;
    Shape* neg;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualizerPluginAudioProcessorEditor)
};
