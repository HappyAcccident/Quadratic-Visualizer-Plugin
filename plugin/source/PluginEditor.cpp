#include "VisualizerPlugin/PluginProcessor.h"
#include "VisualizerPlugin/PluginEditor.h"
#include "VisualizerPlugin/Animation.h"
#include "VisualizerPlugin/Shape.h"
#include "VisualizerPlugin/Display.h"

//==============================================================================
VisualizerPluginAudioProcessorEditor::VisualizerPluginAudioProcessorEditor (VisualizerPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1920/2, 1080/2);

    display.setResolution(256);
    int res = display.getResolution();
    A = new Shape(3, (13.0/3.0), M_PI/8.0 + 0.2, 3, res, "Regular"); //8 pointed star with steepness 3
    B = new Shape(1, res, M_PI/res, -2, res, "Regular"); //circle
    C = new Shape(1, 4, M_PI/4, 1, res, "Regular"); //square
    Animation* ARotate = new RotateAnimation(144*4);
    Animation* CScale = new ScaleAnimation(144*1, 1, 2);
    C->addAnimation(CScale, 0);
    A->addAnimation(ARotate, 0);

    quadraticRoot = Shape::quadratic(*A, *B, *C);
    pos = new Shape(quadraticRoot.first);
    neg = new Shape(quadraticRoot.second);
    testCounter++;
    display.clearShapes();
    display.addShape(pos);
    display.addShape(neg);
    // display.addShape(A);

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
    // g.drawFittedText (std::to_string(getFrameCounter()), getLocalBounds(), juce::Justification::centred, 1);
    // g.drawFittedText (std::to_string(display.getNumShapes()), getLocalBounds(), juce::Justification::centred, 1);
    g.drawFittedText (juce::String(testCounter) + ", " + juce::String(display.getNumShapes()), getLocalBounds(), juce::Justification::centred, 1);
    float radius = 2.0f;

    float scale = display.getMaxRadius();

    for (auto shape : display.getShapes())
    {
        juce::Path shapePath;
        auto shapePts = shape->getPts();
        int res = display.getResolution();
                
        float radius = 2.0f;
                
        for (int i = 0; i < shapePts.size(); i++) {
            float x = (250 / scale) * shapePts[i].real();
            float y = (250 / scale) * shapePts[i].imag();
        
            float xPrint = getWidth() / 2.f + x;
            float yPrint = getHeight() / 2.f - y;

            shapePath.addTriangle(xPrint - radius, yPrint - radius, xPrint + radius, yPrint - radius, xPrint, yPrint + radius);
        }

        g.strokePath (shapePath, juce::PathStrokeType (1.0f));
    }
}

void VisualizerPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void VisualizerPluginAudioProcessorEditor::update()
{
    A->updateAnimations(getFrameCounter());
    B->updateAnimations(getFrameCounter());
    C->updateAnimations(getFrameCounter());

    quadraticRoot = Shape::quadratic(*A, *B, *C);
    *pos = quadraticRoot.first;
    *neg = quadraticRoot.second;
}

void VisualizerPluginAudioProcessorEditor::timerCallback()
{
    incrementFrameCounter();
    update();
    repaint();
}