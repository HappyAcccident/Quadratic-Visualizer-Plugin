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
    
    addAndMakeVisible(resolutionLabel);
    resolutionLabel.setText("Resolution", juce::dontSendNotification);
    resolutionLabel.attachToComponent(&resolutionSlider, true);

    addAndMakeVisible(shapeSelectorComboBox);
    shapeSelectorComboBox.addItem("A", 1);
    shapeSelectorComboBox.addItem("B", 2);
    shapeSelectorComboBox.addItem("C", 3);
    shapeSelectorComboBox.setSelectedId(1, true);
    shapeSelectorComboBox.addListener(this);

    addAndMakeVisible(shapeSelectorLabel);
    shapeSelectorLabel.setText("Shape", juce::dontSendNotification);
    shapeSelectorLabel.attachToComponent(&shapeSelectorComboBox, true);

    for (int i = 0; i < 3; i++)
    {
        addChildComponent(inputStepSliders[i]);
        inputStepSliders[i].setRange(-10, 10, 1);
        inputStepSliders[i].addListener(this);

        addChildComponent(inputStepLabels[i]);
        inputStepLabels[i].setText("Input Step", juce::dontSendNotification);
        inputStepLabels[i].attachToComponent(&inputStepSliders[i], true);


        addChildComponent(numeratorSliders[i]);
        numeratorSliders[i].setRange(1, 512, 1);
        numeratorSliders[i].setSkewFactor(0.2, false);
        numeratorSliders[i].addListener(this);

        addChildComponent(numeratorLabels[i]);
        numeratorLabels[i].setText("Numerator", juce::dontSendNotification);
        numeratorLabels[i].attachToComponent(&numeratorSliders[i], true);


        addChildComponent(denominatorSliders[i]);
        denominatorSliders[i].setRange(1, 512, 1);
        denominatorSliders[i].setSkewFactor(0.2, false);
        denominatorSliders[i].addListener(this);

        addChildComponent(denominatorLabels[i]);
        denominatorLabels[i].setText("Denominator", juce::dontSendNotification);
        denominatorLabels[i].attachToComponent(&denominatorSliders[i], true);


        addChildComponent(outputStepSliders[i]);
        outputStepSliders[i].setRange(-10, 10, 1);
        outputStepSliders[i].addListener(this);

        addChildComponent(outputStepLabels[i]);
        outputStepLabels[i].setText("Output Step", juce::dontSendNotification);
        outputStepLabels[i].attachToComponent(&outputStepSliders[i], true);
    }

    inputStepSliders[0].setValue(3);
    numeratorSliders[0].setValue(4);
    denominatorSliders[0].setValue(1);
    outputStepSliders[0].setValue(-1);

    inputStepSliders[1].setValue(1);
    numeratorSliders[1].setValue(512);
    denominatorSliders[1].setValue(1);
    outputStepSliders[1].setValue(-4);

    inputStepSliders[2].setValue(1);
    numeratorSliders[2].setValue(4);
    denominatorSliders[2].setValue(1);
    outputStepSliders[2].setValue(1);

    inputStepSliders[0].setVisible(true);
    numeratorSliders[0].setVisible(true);
    denominatorSliders[0].setVisible(true);
    outputStepSliders[0].setVisible(true);

    int res = display.getResolution();
    // A = new Shape(3, (13.0/3.0), M_PI/8.0 + 0.2, 3, res, "Regular"); //8 pointed star with steepness 3
    // B = new Shape(1, res, M_PI/res, -2, res, "Regular"); //circle
    // C = new Shape(1, 4, M_PI/4, 1, res, "Regular"); //square
    A = new Shape(3, 4, 0, -1, res, "Regular"); 
    B = new Shape(1, res, 0, -4, res, "Regular"); //circle
    C = new Shape(1, 4, M_PI/4, 1, res, "Regular"); //square

    mainShapes[0] = A;
    mainShapes[1] = B;
    mainShapes[2] = C;

    auto ABassVolumeLambda = [](float x) -> float {return 0.667*x-0.6f;};
    auto BBassVolumeLambda = [](float x) -> float {return 1.5*x-0.6f;};

    Animation* AVolume = new VolumeScaleAnimation(Band::Bass, ABassVolumeLambda);
    Animation* BVolume = new VolumeScaleAnimation(Band::Bass, BBassVolumeLambda);
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
    // display.addShape(C);


    //return 3
    // std::cout << mainShapes[0]->getInputStep() << std::endl;

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

    // g.drawFittedText(juce::String(pos->getMaxRadius()) + " " + juce::String(neg->getMaxRadius()), getLocalBounds(), juce::Justification::centred, 1);


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

    for (int i = 0; i < 3; i++)
    {
        inputStepSliders[i].setBounds(sliderBounds.getCentreX(),
                                   startY + (sliderHeight + sliderSpacing) * 2,
                                   sliderBounds.getWidth(),
                                   sliderHeight);

        numeratorSliders[i].setBounds(sliderBounds.getCentreX(),
                                   startY + (sliderHeight + sliderSpacing) * 3,
                                   sliderBounds.getWidth(),
                                   sliderHeight);

        denominatorSliders[i].setBounds(sliderBounds.getCentreX(),
                                     startY + (sliderHeight + sliderSpacing) * 4,
                                     sliderBounds.getWidth(),
                                     sliderHeight);

        outputStepSliders[i].setBounds(sliderBounds.getCentreX(),
                                    startY + (sliderHeight + sliderSpacing) * 5,
                                    sliderBounds.getWidth(),
                                    sliderHeight);
    }
}

void VisualizerPluginAudioProcessorEditor::drawShape(juce::Graphics& g, const Shape* shape, int x, int y, int radius, float scale)
{
    juce::Path shapePath;
    auto shapePts = shape->getPts();
    int res = display.getResolution();
            
    float thickness = 1.5 * sqrt(getShortestSide(getLocalBounds()) * 0.0075);
    float a = 0.25;
    auto ease = [a](float x) -> float {return a * (cbrt((1/a)*x - 1) + 1);};

    scale = (scale < 0.01) ? 0.01 : scale;

    g.setColour(juce::Colour::fromHSV(ease(currentState.getMeanBandVolume(Band::Mid)+0.3), 1.0f, 1.0f, 1.0f));
    for (int i = 0; i < shapePts.size(); i++) {
        float real = (radius / scale) * shapePts[i].real();
        float imag = (radius / scale) * shapePts[i].imag();

        float xPrint = real + x;
        float yPrint = y - imag;
        g.fillRect(xPrint, yPrint, thickness, thickness);
        // shapePath.addTriangle(xPrint - thickness, yPrint - thickness, xPrint + thickness, yPrint - thickness, xPrint, yPrint + thickness);
    }
    // g.strokePath (shapePath, juce::PathStrokeType (1.0f));
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

    int i = shapeSelectorComboBox.getSelectedId() - 1;

    if (slider == &inputStepSliders[i])
    {
        mainShapes[i]->setInputStep(slider->getValue());
    }
    else if (slider == &numeratorSliders[i])
    {
        float q = (slider->getValue()/denominatorSliders[i].getValue());
        if (q != 2)
        {
            mainShapes[i]->setN(q);
        }
    }
    else if (slider == &denominatorSliders[i])
    {
        float q = (numeratorSliders[i].getValue())/slider->getValue();
        if (q != 2)
        {
            mainShapes[i]->setN(q);
        }
    }
    else if (slider == &outputStepSliders[i])
    {
        mainShapes[i]->setOutputStep(slider->getValue());
    }
    

    A->regenerate();
    B->regenerate();
    C->regenerate();
}

void VisualizerPluginAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &shapeSelectorComboBox)
    {
        int i = comboBox->getSelectedId() - 1;

        hideAllShapeSliders();

        inputStepSliders[i].setVisible(true);
        numeratorSliders[i].setVisible(true);
        denominatorSliders[i].setVisible(true);
        outputStepSliders[i].setVisible(true);
    }
}

void VisualizerPluginAudioProcessorEditor::hideAllShapeSliders()
{
    for (int i = 0; i < 3; i++)
    {
        inputStepSliders[i].setVisible(false);
        numeratorSliders[i].setVisible(false);
        denominatorSliders[i].setVisible(false);
        outputStepSliders[i].setVisible(false);
    }
}
