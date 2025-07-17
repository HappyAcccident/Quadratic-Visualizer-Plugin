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
protected:
    Display& display = Display::getInstance();

    Shape* A;
    Shape* B;
    Shape* C;

    std::pair<Shape, Shape> quadraticRoot;
    Shape* pos;
    Shape* neg;

    int testCounter = 0;

    float bassCoefficient;
    
    float meanBassVolume = 0;
    float meanMidVolume = 0;
    float meanTrebleVolume = 0;
public:
    explicit VisualizerPluginAudioProcessorEditor (VisualizerPluginAudioProcessor&);
    ~VisualizerPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

    int getFrameCounter() {return frameCounter;}
    void incrementFrameCounter() {frameCounter++;}

    void update();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VisualizerPluginAudioProcessor& processorRef;
    int frameCounter = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualizerPluginAudioProcessorEditor)
};
