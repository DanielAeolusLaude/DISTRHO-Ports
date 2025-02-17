/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginEditor.h"


//==============================================================================
StereoSourceSeparationAudioProcessorEditor::StereoSourceSeparationAudioProcessorEditor (StereoSourceSeparationAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
    width_ = 400;
    height_ = 200;
    dirAngle = M_PI/2;
    widAngle = M_PI/16;
    radius = width_/2-5;
    arrowLine = Line<float>(width_/2+50, height_+100, 50+width_/2+radius*cos(dirAngle), 100+height_-radius*sin(dirAngle));
    paintColour = Colours::grey;
    
    addAndMakeVisible(widthSlider = new Slider());
    widthSlider->setTextBoxStyle(Slider::NoTextBox, true, 220, 30);
    widthSlider->setColour(Slider::thumbColourId, Colours::wheat);
    widthSlider->setColour(Slider::trackColourId, Colours::wheat);
    widthSlider->setRange(0, 100);
    widthSlider->setValue(12.5);
    widthSlider->addListener(this);
    
    addAndMakeVisible (modeLabel = new Label (String::empty, "Mode"));
    modeLabel->setFont (Font (16.00f, Font::bold));
    modeLabel->setJustificationType (Justification::centred);
    modeLabel->setEditable (false, false, false);
    modeLabel->setColour (Label::textColourId, Colours::wheat);
    modeLabel->setColour (TextEditor::textColourId, Colours::wheat);
    modeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (filterLabel = new Label (String::empty, "Filter Type"));
    filterLabel->setFont (Font (16.00f, Font::bold));
    filterLabel->setJustificationType (Justification::centred);
    filterLabel->setEditable (false, false, false);
    filterLabel->setColour (Label::textColourId, Colours::wheat);
    filterLabel->setColour (TextEditor::textColourId, Colours::wheat);
    filterLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (bypassToggle = new ToggleButton ("Bypass"));
    bypassToggle->setColour(TextButton::buttonColourId, Colours::lightgrey);
    bypassToggle->setColour(ToggleButton::textColourId, Colours::lightgrey);
    bypassToggle->setToggleState(true, juce::sendNotification);
    bypassToggle->addListener (this);
    
    addAndMakeVisible (soloToggle = new ToggleButton ("Solo"));
    soloToggle->setColour(TextButton::buttonColourId, Colours::lightblue);
    soloToggle->setColour(ToggleButton::textColourId, Colours::lightblue);
    soloToggle->addListener (this);
    
    addAndMakeVisible (muteToggle = new ToggleButton ("Mute"));
    muteToggle->setColour(TextButton::buttonColourId, Colours::pink);
    muteToggle->setColour(ToggleButton::textColourId, Colours::pink);
    muteToggle->addListener (this);
    
    addAndMakeVisible (allpassToggle = new ToggleButton ("No filter"));
    allpassToggle->setColour(TextButton::buttonColourId, Colours::lightgrey);
    allpassToggle->setColour(ToggleButton::textColourId, Colours::lightgrey);
    allpassToggle->setToggleState(true, juce::sendNotification);
    allpassToggle->setEnabled(false);
    allpassToggle->addListener (this);
    
    addAndMakeVisible (lowpassToggle = new ToggleButton ("Low pass"));
    lowpassToggle->setColour(TextButton::buttonColourId, Colours::lightblue);
    lowpassToggle->setColour(ToggleButton::textColourId, Colours::lightblue);
    lowpassToggle->setEnabled(false);
    lowpassToggle->addListener (this);
    
    addAndMakeVisible (highpassToggle = new ToggleButton ("High pass"));
    highpassToggle->setColour(TextButton::buttonColourId, Colours::pink);
    highpassToggle->setColour(ToggleButton::textColourId, Colours::pink);
    highpassToggle->setEnabled(false);
    highpassToggle->addListener (this);
    
    addAndMakeVisible(cutofffreqSlider = new Slider());
    cutofffreqSlider->setSliderStyle(juce::Slider::LinearBar);
    cutofffreqSlider->setTextBoxStyle(Slider::NoTextBox, true, 220, 30);
    cutofffreqSlider->setColour(Slider::thumbColourId, Colours::lightgrey);
    cutofffreqSlider->setColour(Slider::trackColourId, Colours::lightgrey);
    cutofffreqSlider->setRange(0, 89.45);
    cutofffreqSlider->setValue(20);
    cutofffreqSlider->addListener(this);
    
    addAndMakeVisible (freqLabel = new Label (String::empty,"Cutoff frequency :"));
    freqLabel->setFont (Font (18.00f, Font::bold));
    freqLabel->setJustificationType (Justification::centred);
    freqLabel->setEditable (false, false, false);
    freqLabel->setColour (Label::textColourId, Colours::wheat);
    freqLabel->setColour (TextEditor::textColourId, Colours::wheat);
    freqLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (freqVal = new Label (String::empty, "400 Hz"));
    freqVal->setFont (Font (16.00f, Font::bold));
    freqVal->setJustificationType (Justification::centred);
    freqVal->setEditable (false, false, false);
    freqVal->setColour (Label::textColourId, Colours::wheat);
    freqVal->setColour (TextEditor::textColourId, Colours::wheat);
    freqVal->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (lowboundLabel = new Label (String::empty, "0"));
    lowboundLabel->setFont (Font (16.00f, Font::bold));
    lowboundLabel->setJustificationType (Justification::centred);
    lowboundLabel->setEditable (false, false, false);
    lowboundLabel->setColour (Label::textColourId, Colours::wheat);
    lowboundLabel->setColour (TextEditor::textColourId, Colours::wheat);
    lowboundLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (highboundLabel = new Label (String::empty, "8000"));
    highboundLabel->setFont (Font (16.00f, Font::bold));
    highboundLabel->setJustificationType (Justification::centred);
    highboundLabel->setEditable (false, false, false);
    highboundLabel->setColour (Label::textColourId, Colours::wheat);
    highboundLabel->setColour (TextEditor::textColourId, Colours::wheat);
    highboundLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (resetButton = new TextButton ("C"));
    resetButton->setColour(TextButton::buttonColourId, Colours::wheat);
    resetButton->addListener (this);
    
    addAndMakeVisible (dirLabel = new Label (String::empty,"Direction : "));
    dirLabel->setFont (Font (20.00f, Font::bold));
    dirLabel->setJustificationType (Justification::centred);
    dirLabel->setEditable (false, false, false);
    dirLabel->setColour (Label::textColourId, Colours::wheat);
    dirLabel->setColour (TextEditor::textColourId, Colours::wheat);
    dirLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (widLabel = new Label (String::empty,"Width : +/-"));
    widLabel->setFont (Font (20.00f, Font::bold));
    widLabel->setJustificationType (Justification::centred);
    widLabel->setEditable (false, false, false);
    widLabel->setColour (Label::textColourId, Colours::wheat);
    widLabel->setColour (TextEditor::textColourId, Colours::wheat);
    widLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (dirVal = new Label (String::empty, "0"));
    dirVal->setFont (Font (20.00f, Font::bold));
    dirVal->setJustificationType (Justification::centred);
    dirVal->setEditable (false, false, false);
    dirVal->setColour (Label::textColourId, Colours::wheat);
    dirVal->setColour (TextEditor::textColourId, Colours::wheat);
    dirVal->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (widVal = new Label (String::empty, "22.5"));
    widVal->setFont (Font (20.00f, Font::bold));
    widVal->setJustificationType (Justification::centred);
    widVal->setEditable (false, false, false);
    widVal->setColour (Label::textColourId, Colours::wheat);
    widVal->setColour (TextEditor::textColourId, Colours::wheat);
    widVal->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (sideVal = new Label (String::empty, "M"));
    sideVal->setFont (Font (20.00f, Font::bold));
    sideVal->setJustificationType (Justification::centred);
    sideVal->setEditable (false, false, false);
    sideVal->setColour (Label::textColourId, Colours::wheat);
    sideVal->setColour (TextEditor::textColourId, Colours::wheat);
    sideVal->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    setSize (750, 400);
    
    getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kDirection, 50);
    getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kWidth, 25);
    getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kStatus, ADRess::kBypass);
}

StereoSourceSeparationAudioProcessorEditor::~StereoSourceSeparationAudioProcessorEditor()
{
    widthSlider = nullptr;
    soloToggle = nullptr;
    muteToggle = nullptr;
    bypassToggle = nullptr;
    allpassToggle = nullptr;
    lowpassToggle = nullptr;
    highpassToggle = nullptr;
    cutofffreqSlider = nullptr;
    resetButton = nullptr;
    dirLabel = nullptr;
    widLabel = nullptr;
    dirVal = nullptr;
    widVal = nullptr;
    sideVal = nullptr;
    freqLabel = nullptr;
    freqVal = nullptr;
    lowboundLabel = nullptr;
    highboundLabel = nullptr;
    modeLabel = nullptr;
    filterLabel = nullptr;
}

//==============================================================================
void StereoSourceSeparationAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::darkgrey);
    g.setColour (paintColour);
    g.fillPath (internalPath);
    g.strokePath (internalPath, PathStrokeType (1.200f));
    g.setColour(Colour (0xff3b3b3b));
    g.fillPath (arrowPath);
    g.strokePath (arrowPath, PathStrokeType (1.200f));
    g.setColour (Colours::wheat);
    g.drawEllipse(50+5, 100+5, width_-10, 2*height_-10, 8);
    g.setColour(Colours::lightgrey);
    g.drawLine(600, 90, 600, 190);
    g.setColour(Colours::darkgrey);
    g.fillRect(0, 100+height_, getWidth(), getHeight()-height_-100);
}

void StereoSourceSeparationAudioProcessorEditor::resized()
{
    internalPath.clear();
    internalPath.startNewSubPath (width_/2+50, height_+100);
    internalPath.addCentredArc(width_/2+50, height_+100, radius, radius, 0, (M_PI/2-dirAngle-widAngle+2*M_PI), (M_PI/2-dirAngle+widAngle+M_PI*2), false);
    internalPath.closeSubPath();
    arrowPath.clear();
    arrowPath.startNewSubPath (width_/2+50, height_+100);
    arrowPath.addArrow(arrowLine, 5, 20, 20);
    arrowPath.closeSubPath();
    
    widthSlider->setBounds(150, 330, 200, 40);
    modeLabel->setBounds(510, 80, 40, 30);
    filterLabel->setBounds(620, 80, 80, 30);
    bypassToggle->setBounds(500, 110, 80, 30);
    soloToggle->setBounds(500, 140, 80, 30);
    muteToggle->setBounds(500, 170, 80, 30);
    allpassToggle->setBounds(620, 110, 80, 30);
    lowpassToggle->setBounds(620, 140, 80, 30);
    highpassToggle->setBounds(620, 170, 80, 30);
    cutofffreqSlider->setBounds(500, 280, 200, 20);
    lowboundLabel->setBounds(500, 300, 20, 20);
    freqLabel->setBounds(500, 230, 120, 40);
    freqVal->setBounds(620, 230, 80, 40);
    highboundLabel->setBounds(660, 300, 40, 20);
    resetButton->setBounds(100, 340, 20, 20);
    dirLabel->setBounds(50, 50, 80, 25);
    widLabel->setBounds(300, 50, 100, 25);
    dirVal->setBounds(150, 50, 60, 25);
    widVal->setBounds(400, 50, 60, 25);
    sideVal->setBounds(130, 50, 30, 25);
}


void StereoSourceSeparationAudioProcessorEditor::mouseDrag (const juce::MouseEvent &e)
{
    if (e.getPosition().y > height_+98)
        return;
    dirAngle = atanf((height_+100-e.getPosition().y)*1.0/(e.getPosition().x-width_/2-100));
    if (dirAngle < 0)
        dirAngle += M_PI;

    int dirAngle_toPass = (M_PI-dirAngle)/M_PI*100;
    getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kDirection, dirAngle_toPass);
    
    if (dirAngle > M_PI/2)
        sideVal->setText("L", juce::sendNotification);
    else if (dirAngle == M_PI/2)
        sideVal->setText("M", juce::sendNotification);
    else
        sideVal->setText("R", juce::sendNotification);
    dirVal->setText(String(fabs((dirAngle-M_PI/2)*180/M_PI)), juce::sendNotification);
    arrowLine.setEnd(50+width_/2+radius*cos(dirAngle), 100+height_-radius*sin(dirAngle));
    resized();
    repaint();
}

void StereoSourceSeparationAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider == widthSlider){
        widAngle = slider->getValue()/100*M_PI/2;
        int widAngle_toPass = slider->getValue();
        getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kWidth, widAngle_toPass);
        widVal->setText(String(widAngle*180/M_PI), juce::sendNotification);
    }
    else if (slider == cutofffreqSlider){
        float val = slider->getValue();
        float freq_toPass = val*val;
        getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kCutOffFrequency, freq_toPass);
        freqVal->setText(String(freq_toPass)+" Hz", juce::sendNotification);
    }
    resized();
    repaint();
}

void StereoSourceSeparationAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == soloToggle && soloToggle->getToggleState())
    {
        muteToggle->setToggleState(false, juce::sendNotification);
        bypassToggle->setToggleState(false, juce::sendNotification);
        soloToggle->setEnabled(false);
        muteToggle->setEnabled(true);
        bypassToggle->setEnabled(true);
        if (!allpassToggle->getToggleState())
            allpassToggle->setEnabled(true);
        if (!lowpassToggle->getToggleState())
            lowpassToggle->setEnabled(true);
        if (!highpassToggle->getToggleState())
            highpassToggle->setEnabled(true);
        allpassToggle->setButtonText("No filter");
        paintColour = Colours::lightblue;
        getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kStatus, ADRess::kSolo);
    }
    else if (buttonThatWasClicked == muteToggle && muteToggle->getToggleState())
    {
        soloToggle->setToggleState(false, juce::sendNotification);
        bypassToggle->setToggleState(false, juce::sendNotification);
        soloToggle->setEnabled(true);
        muteToggle->setEnabled(false);
        bypassToggle->setEnabled(true);
        if (!allpassToggle->getToggleState())
            allpassToggle->setEnabled(true);
        if (!lowpassToggle->getToggleState())
            lowpassToggle->setEnabled(true);
        if (!highpassToggle->getToggleState())
            highpassToggle->setEnabled(true);
        allpassToggle->setButtonText("All reject");
        paintColour = Colour (0xfff08080);
        getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kStatus, ADRess::kMute);
    }
    else if (buttonThatWasClicked == bypassToggle && bypassToggle->getToggleState())
    {
        soloToggle->setToggleState(false, juce::sendNotification);
        muteToggle->setToggleState(false, juce::sendNotification);
        soloToggle->setEnabled(true);
        muteToggle->setEnabled(true);
        bypassToggle->setEnabled(false);
        allpassToggle->setEnabled(false);
        lowpassToggle->setEnabled(false);
        highpassToggle->setEnabled(false);
        allpassToggle->setButtonText("No filter");
        paintColour = Colours::grey;
        getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kStatus, ADRess::kBypass);
    }
    else if (buttonThatWasClicked == allpassToggle && allpassToggle->getToggleState())
    {
        lowpassToggle->setToggleState(false, juce::sendNotification);
        highpassToggle->setToggleState(false, juce::sendNotification);
        allpassToggle->setEnabled(false);
        lowpassToggle->setEnabled(true);
        highpassToggle->setEnabled(true);
        cutofffreqSlider->setColour(Slider::thumbColourId, Colours::lightgrey);
        getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kFilterType, ADRess::kAllPass);
    }
    else if (buttonThatWasClicked == lowpassToggle && lowpassToggle->getToggleState())
    {
        allpassToggle->setToggleState(false, juce::sendNotification);
        highpassToggle->setToggleState(false, juce::sendNotification);
        lowpassToggle->setEnabled(false);
        allpassToggle->setEnabled(true);
        highpassToggle->setEnabled(true);
        cutofffreqSlider->setColour(Slider::thumbColourId, Colours::lightblue);
        getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kFilterType, ADRess::kLowPass);
    }
    else if (buttonThatWasClicked == highpassToggle && highpassToggle->getToggleState())
    {
        allpassToggle->setToggleState(false, juce::sendNotification);
        lowpassToggle->setToggleState(false, juce::sendNotification);
        highpassToggle->setEnabled(false);
        allpassToggle->setEnabled(true);
        lowpassToggle->setEnabled(true);
        cutofffreqSlider->setColour(Slider::thumbColourId, Colour (0xfff08080));
        getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kFilterType, ADRess::kHighPass);
    }
    else if (buttonThatWasClicked == resetButton)
    {
        dirAngle = M_PI/2;
        sideVal->setText("M", juce::sendNotification);
        dirVal->setText(String(0), juce::sendNotification);
        arrowLine.setEnd(50+width_/2, 100+height_-radius);
        getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kDirection, 50);
        getProcessor()->setParameter(StereoSourceSeparationAudioProcessor::kWidth, widthSlider->getValue());
    }
    resized();
    repaint();
}