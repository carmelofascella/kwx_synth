/*
  ==============================================================================

    Oscilloscope.cpp
    Created: 6 Nov 2023 5:04:44pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WavePlotComponent.h"

//==============================================================================
WavePlotComponent::WavePlotComponent(juce::AudioProcessorValueTreeState& apvts, juce::String masterId)
{
    addAndMakeVisible(masterSlider);
    addAndMakeVisible(masterLabel);
    
    
    masterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, masterId, masterSlider);
    
    masterSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    masterSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 25);
    
    masterLabel.setColour (juce::Label::ColourIds::textColourId, juce::Colours::white);
    masterLabel.setFont (15.0f);
    masterLabel.setJustificationType (juce::Justification::centred);

}

WavePlotComponent::~WavePlotComponent()
{
}

void WavePlotComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced (5);
    auto labelSpace = bounds.removeFromTop (25.0f);

    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText ("Waveform", labelSpace.withX (5), juce::Justification::left);
    g.drawRoundedRectangle (bounds.toFloat(), 5.0f, 2.0f);
}

void WavePlotComponent::resized()
{
    const int sliderWidth = 50;
    const int sliderHeight = 120;
    
    masterSlider.setBounds(getWidth()-70, 60, sliderWidth, sliderHeight);
    masterLabel.setBounds (masterSlider.getX(), 25, masterSlider.getWidth(), 50);
}
