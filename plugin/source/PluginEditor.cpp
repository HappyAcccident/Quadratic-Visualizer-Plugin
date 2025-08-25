#include <array>

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
    setSize(600, 400);
    setResizable(true, false);

    addAndMakeVisible(resolutionSlider);
    resolutionSlider.setRange(0, 2048, 1);
    resolutionSlider.setValue(512);
    resolutionSlider.addListener(this);

    addAndMakeVisible(AInputStepSlider);
    AInputStepSlider.setRange(-10, 10, 1);
    AInputStepSlider.setValue(3);
    AInputStepSlider.addListener(this);

    addAndMakeVisible(ANumeratorSlider);
    ANumeratorSlider.setRange(1, 23, 1);
    ANumeratorSlider.setValue(4);
    ANumeratorSlider.addListener(this);

    addAndMakeVisible(ADenominatorSlider);
    ADenominatorSlider.setRange(1, 21, 1);
    ADenominatorSlider.setValue(1);
    ADenominatorSlider.addListener(this);

    addAndMakeVisible(AOutputStepSlider);
    AOutputStepSlider.setRange(-10, 10, 1);
    AOutputStepSlider.setValue(-1);
    AOutputStepSlider.addListener(this);

    addAndMakeVisible(shapeSelectorComboBox);
    shapeSelectorComboBox.addItem("A", 1);
    shapeSelectorComboBox.addItem("B", 2);
    shapeSelectorComboBox.addItem("C", 3);
    shapeSelectorComboBox.setSelectedId(1, true);
    shapeSelectorComboBox.addListener(this);

    addAndMakeVisible(resolutionLabel);
    resolutionLabel.setText("Resolution", juce::dontSendNotification);
    resolutionLabel.attachToComponent(&resolutionSlider, true);

    addAndMakeVisible(shapeSelectorLabel);
    shapeSelectorLabel.setText("Shape", juce::dontSendNotification);
    shapeSelectorLabel.attachToComponent(&shapeSelectorComboBox, true);

    addAndMakeVisible(AInputStepLabel);
    AInputStepLabel.setText("Input Step", juce::dontSendNotification);
    AInputStepLabel.attachToComponent(&AInputStepSlider, true);

    addAndMakeVisible(ANumeratorLabel);
    ANumeratorLabel.setText("Numerator", juce::dontSendNotification);
    ANumeratorLabel.attachToComponent(&ANumeratorSlider, true);

    addAndMakeVisible(ADenominatorLabel);
    ADenominatorLabel.setText("Denominator", juce::dontSendNotification);
    ADenominatorLabel.attachToComponent(&ADenominatorSlider, true);

    addAndMakeVisible(AOutputStepLabel);
    AOutputStepLabel.setText("Output Step", juce::dontSendNotification);
    AOutputStepLabel.attachToComponent(&AOutputStepSlider, true);

    int res = display.getResolution();
    // A = new Shape(3, (13.0/3.0), M_PI/8.0 + 0.2, 3, res, "Regular"); //8 pointed star with steepness 3
    // B = new Shape(1, res, M_PI/res, -2, res, "Regular"); //circle
    // C = new Shape(1, 4, M_PI/4, 1, res, "Regular"); //square
    A = new Shape(3, 4, 0, -1, res, "Regular"); 
    B = new Shape(1, res, 0, -4, res, "Regular"); //circle
    C = new Shape(1, 4, M_PI/4, 1, res, "Regular"); //square

    auto ABassVolumeLambda = [](float x) -> float {return x-0.6f;};
    auto BBassVolumeLabda = [](float x) -> float {return x-0.6f;};

    Animation* AVolume = new VolumeScaleAnimation(Band::Bass, ABassVolumeLambda);
    Animation* BVolume = new VolumeScaleAnimation(Band::Bass, BBassVolumeLabda);
    Animation* CVolumeRotate = new BetterVolumeRotateAnimation(Band::Treble);

    A->addAnimation(AVolume);
    B->addAnimation(BVolume);
    C->addAnimation(CVolumeRotate);

    quadraticRoot = Shape::quadratic(*A, *B, *C);
    pos = new Shape(quadraticRoot.first);
    pos->setResolution(display.getResolution());
    neg = new Shape(quadraticRoot.second);
    neg->setResolution(display.getResolution());
    display.clearShapes();
    display.addShape(pos);
    display.addShape(neg);
    // display.addShape(A);

    startTimerHz(60);
}

VisualizerPluginAudioProcessorEditor::~VisualizerPluginAudioProcessorEditor()
{
}

int getShortestSide(const juce::Rectangle<int>& r)
{
    return std::min(r.getWidth(), r.getHeight());
}


//==============================================================================
void VisualizerPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    g.setColour (getLookAndFeel().findColour (juce::Slider::thumbColourId));
    // g.drawFittedText (std::to_string(getFrameCounter()), getLocalBounds(), juce::Justification::centred, 1);
    // g.drawFittedText (std::to_string(display.getNumShapes()), getLocalBounds(), juce::Justification::centred, 1);
    // g.drawFittedText (juce::String(testCounter) + ", " + juce::String(display.getNumShapes()), getLocalBounds(), juce::Justification::centred, 1);
    // g.drawFittedText (juce::String(currentState.getMeanBandVolume(Band::Bass), 4) + " / " + juce::String(currentState.getMeanBandVolume(Band::Mid), 4) + " / " + juce::String(currentState.getMeanBandVolume(Band::Treble), 4), getLocalBounds(), juce::Justification::centred, 1);

    auto bounds = getLocalBounds();
    // g.drawFittedText (juce::String(bounds.getWidth()) + " " + juce::String(bounds.getHeight()), getLocalBounds(), juce::Justification::centred, 1);
    // g.drawFittedText (juce::String(bounds.getCentreX()) + " " + juce::String(bounds.getCentreY()), getLocalBounds(), juce::Justification::centred, 1);
    // g.drawFittedText (juce::String(getShortestSide(bounds)), getLocalBounds(), juce::Justification::centred, 1);
    // g.drawFittedText (juce::String(pos->getMaxRadius()) + ", " + juce::String(neg->getMaxRadius()), getLocalBounds(), juce::Justification::centred, 1);

    // g.drawFittedText(juce::String(A->getMaxRadius()), getLocalBounds(), juce::Justification::centred, 1);


    float scale = display.getMaxRadius();

    for (auto shape : display.getShapes())
    {
        drawShape(g, shape, bounds, 0.5 * getShortestSide(bounds) * 0.667, scale);
    }
}

void VisualizerPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();
    auto sliderBounds = bounds.removeFromLeft(bounds.getWidth() * 0.167);

    int sliderHeight = bounds.getHeight() * 0.1;
    int sliderSpacing = bounds.getHeight() * 0.05;
    int startY = bounds.getHeight() * 0.1;       

    resolutionSlider.setBounds(sliderBounds.getCentreX(),
                               startY,
                               sliderBounds.getWidth(),
                               sliderHeight);

    shapeSelectorComboBox.setBounds(sliderBounds.getCentreX(),
                                    startY + (sliderHeight + sliderSpacing) * 1,
                                    sliderBounds.getWidth(),
                                    sliderHeight);

    AInputStepSlider.setBounds(sliderBounds.getCentreX(),
                               startY + (sliderHeight + sliderSpacing) * 2,
                               sliderBounds.getWidth(),
                               sliderHeight);

    ANumeratorSlider.setBounds(sliderBounds.getCentreX(),
                               startY + (sliderHeight + sliderSpacing) * 3,
                               sliderBounds.getWidth(),
                               sliderHeight);

    ADenominatorSlider.setBounds(sliderBounds.getCentreX(),
                                 startY + (sliderHeight + sliderSpacing) * 4,
                                 sliderBounds.getWidth(),
                                 sliderHeight);

    AOutputStepSlider.setBounds(sliderBounds.getCentreX(),
                                startY + (sliderHeight + sliderSpacing) * 5,
                                sliderBounds.getWidth(),
                                sliderHeight);
}

void VisualizerPluginAudioProcessorEditor::drawShape(juce::Graphics& g, const Shape* shape, int x, int y, int radius, float scale)
{
    juce::Path shapePath;
    auto shapePts = shape->getPts();
    int res = display.getResolution();
            
    float thickness = 2.0f;
    float a = 0.25;
    auto ease = [a](float x) -> float {return a * (cbrt((1/a)*x - 1) + 1);};

    scale = (scale < 0.01) ? 0.01 : scale;

    g.setColour(juce::Colour::fromHSV(ease(currentState.getMeanBandVolume(Band::Mid)+0.3), 1.0f, 1.0f, 1.0f));
    for (int i = 0; i < shapePts.size(); i++) {
        float real = (radius / scale) * shapePts[i].real();
        float imag = (radius / scale) * shapePts[i].imag();

        float xPrint = real + x;
        float yPrint = y - imag;
        shapePath.addTriangle(xPrint - thickness, yPrint - thickness, xPrint + thickness, yPrint - thickness, xPrint, yPrint + thickness);
    }
    g.strokePath (shapePath, juce::PathStrokeType (1.0f));
}

void VisualizerPluginAudioProcessorEditor::drawShape(juce::Graphics& g, const Shape* shape, juce::Rectangle<int> bounds, int radius, float scale)
{
    drawShape(g, shape, bounds.getCentreX(), bounds.getCentreY(), radius, scale);
}

void VisualizerPluginAudioProcessorEditor::drawShape(juce::Graphics& g, const Shape* shape, juce::Rectangle<int> bounds, int radius)
{
    drawShape(g, shape, bounds, radius, shape->getMaxRadius());
}

void VisualizerPluginAudioProcessorEditor::drawShape(juce::Graphics& g, const Shape* shape, juce::Rectangle<int> bounds)
{
    drawShape(g, shape, bounds, getShortestSide(bounds), shape->getMaxRadius());
}

void VisualizerPluginAudioProcessorEditor::update()
{
    currentState.setMeanBandVolume(Band::Bass, (processorRef.getBassBuffer()));
    currentState.setMeanBandVolume(Band::Mid, (processorRef.getMidBuffer()));
    currentState.setMeanBandVolume(Band::Treble, (processorRef.getTrebleBuffer()));;

    A->updateAnimations(currentState);
    B->updateAnimations(currentState);
    C->updateAnimations(currentState);

    quadraticRoot = Shape::quadratic(*A, *B, *C);
    *pos = quadraticRoot.first;
    *neg = quadraticRoot.second;
}

void VisualizerPluginAudioProcessorEditor::timerCallback()
{
    currentState.incrementCurrentFrame();
    update();
    repaint();
}

void VisualizerPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    // if (slider == &resolutionSlider)
    // {
    //     display.setResolution(slider->getValue());

    //     A->setResolution(display.getResolution());
    //     B->setResolution(display.getResolution());
    //     C->setResolution(display.getResolution());
    // }
    // else if (slider == &AInputStepSlider)
    // {
    //     A->setInputStep(slider->getValue());
    // }
    // else if (slider == &ANumeratorSlider)
    // {
    //     A->setN(slider->getValue()/ADenominatorSlider.getValue());
    // }
    // else if (slider == &ADenominatorSlider)
    // {
    //     A->setN(ANumeratorSlider.getValue()/slider->getValue());
    // }
    // else if (slider == &AOutputStepSlider)
    // {
    //     A->setOutputStep(slider->getValue());
    // }

    // A->regenerate();
    // B->regenerate();
    // C->regenerate();
}

void VisualizerPluginAudioProcessorEditor::sliderDragEnded(juce::Slider* slider)
{
    if (slider == &resolutionSlider)
    {
        display.setResolution(slider->getValue());

        A->setResolution(display.getResolution());
        B->setResolution(display.getResolution());
        C->setResolution(display.getResolution());
    }
    else if (slider == &AInputStepSlider)
    {
        A->setInputStep(slider->getValue());
    }
    else if (slider == &ANumeratorSlider)
    {
        float n = (slider->getValue())/ADenominatorSlider.getValue();
        if (n != 2)
        {
            A->setN(n);
        }
    }
    else if (slider == &ADenominatorSlider)
    {
        float n = (ANumeratorSlider.getValue())/slider->getValue();
        if (n != 2)
        {
            A->setN(n);
        }
    }
    else if (slider == &AOutputStepSlider)
    {
        A->setOutputStep(slider->getValue());
    }

    A->regenerate();
    B->regenerate();
    C->regenerate();
}
