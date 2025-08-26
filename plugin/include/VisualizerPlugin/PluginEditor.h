#pragma once

#define _USE_MATH_DEFINES
#define M_PI 3.14159265359

#include <cmath>
#include <array>
#include "PluginProcessor.h"
#include "Shape.h"
#include "Display.h"

//==============================================================================
class VisualizerPluginAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                                   public juce::Timer, 
                                                   public juce::Slider::Listener,
                                                   public juce::ComboBox::Listener
{
public:
    explicit VisualizerPluginAudioProcessorEditor (VisualizerPluginAudioProcessor&);
    ~VisualizerPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

    void update();

    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    void comboBoxChanged(juce::ComboBox* comboBox) override;

    void hideAllShapeSliders();

    void drawShape(juce::Graphics& g, const Shape* shape, int x, int y, int radius, float scale);
    void drawShape(juce::Graphics& g, const Shape* shape, juce::Rectangle<int> bounds, int radius, float scale);
    void drawShape(juce::Graphics& g, const Shape* shape, juce::Rectangle<int> bounds, int radius);
    void drawShape(juce::Graphics& g, const Shape* shape, juce::Rectangle<int> bounds);


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VisualizerPluginAudioProcessor& processorRef;

    juce::Slider resolutionSlider;
    juce::Label resolutionLabel;

    juce::ComboBox shapeSelectorComboBox;
    juce::Label shapeSelectorLabel;

    std::array<juce::Slider, 3> inputStepSliders;
    std::array<juce::Label, 3> inputStepLabels;

    std::array<juce::Slider, 3> numeratorSliders;
    std::array<juce::Label, 3> numeratorLabels;

    std::array<juce::Slider, 3> denominatorSliders;
    std::array<juce::Label, 3> denominatorLabels;

    std::array<juce::Slider, 3> outputStepSliders;
    std::array<juce::Label, 3> outputStepLabels;


    Display& display = Display::getInstance();

    State& currentState = State::getInstance();

    Shape* A;
    Shape* B;
    Shape* C;

    std::array<Shape*, 3> mainShapes;

    std::pair<Shape, Shape> quadraticRoot;
    Shape* pos;
    Shape* neg;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualizerPluginAudioProcessorEditor)
};
