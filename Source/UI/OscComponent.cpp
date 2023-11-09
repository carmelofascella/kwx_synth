/*
  ==============================================================================

    OscComponent.cpp
    Created: 29 Oct 2023 6:40:58pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::String name,juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId, juce::String gainValueId, juce::String isOscActiveBtnId)
{
    componentName = name;
    
    juce::StringArray choices {"Sine",  "Saw", "Square"};
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);
    
    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);
    

    setSliderWithLabel(gainSlider, gainLabel, apvts, gainValueId, gainAttachment);
    setSliderWithLabel(fmFreqSlider, fmFreqLabel, apvts, fmFreqId, fmFreqAttachment);
    setSliderWithLabel(fmDepthSlider, fmDepthLabel, apvts, fmDepthId, fmDepthAttachment);
    
    
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, isOscActiveBtnId, isOscActiveBtn);
    addAndMakeVisible(isOscActiveBtn);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced (5);
    auto labelSpace = bounds.removeFromTop (25.0f);
    
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText (componentName, labelSpace.withX (5), juce::Justification::left);
    g.drawRoundedRectangle (bounds.toFloat(), 5.0f, 2.0f);


}

void OscComponent::resized()
{
//    oscWaveSelector.setBounds(0, 0, 90, 20);
//    fmFreqSlider.setBounds(0, 80, 100, 90);
//    fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY()-20, fmFreqSlider.getWidth(), 20);
//
//    fmDepthSlider.setBounds(fmFreqSlider.getRight() - 10, 80, 100, 90);
//    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY()-20, fmDepthSlider.getWidth(), 20);
    
    const auto startY = 55;
    const auto sliderWidth = 100;
    const auto sliderHeight = 95;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;
    
    
    isOscActiveBtn.setBounds(10, 30, 30, 30);
    oscWaveSelector.setBounds (10, startY + 5, 90, 30);
    
    //oscWaveSelector.setBounds (10, startY - labelYOffset, 90, labelHeight);
    
    gainSlider.setBounds (oscWaveSelector.getRight(), startY, sliderWidth, sliderHeight);
    gainLabel.setBounds (gainSlider.getX(), gainSlider.getY() - labelYOffset, gainSlider.getWidth(), labelHeight);
    
    fmFreqSlider.setBounds (gainSlider.getRight(), startY, sliderWidth, sliderHeight);
    fmFreqLabel.setBounds (fmFreqSlider.getX(), fmFreqSlider.getY() - labelYOffset, fmFreqSlider.getWidth(), labelHeight);
    
    fmDepthSlider.setBounds (fmFreqSlider.getRight(), startY, sliderWidth, sliderHeight);
    fmDepthLabel.setBounds (fmDepthSlider.getX(), fmDepthSlider.getY() - labelYOffset, fmDepthSlider.getWidth(), labelHeight);
    
//    fmFreqSlider.setBounds (oscWaveSelector.getRight(), startY, sliderWidth, sliderHeight);
//    fmFreqLabel.setBounds (fmFreqSlider.getX(), fmFreqSlider.getY() - labelYOffset, fmFreqSlider.getWidth(), labelHeight);

//    fmDepthSlider.setBounds (fmFreqSlider.getRight(), startY, sliderWidth, sliderHeight);
//    fmDepthLabel.setBounds (fmDepthSlider.getX(), fmDepthSlider.getY() - labelYOffset, fmDepthSlider.getWidth(), labelHeight);
}


using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
void OscComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId,std::unique_ptr<SliderAttachment>& attachment)
{

    attachment = std::make_unique<SliderAttachment>(apvts, paramId, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
    

    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
    
    
}
