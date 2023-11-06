/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 29 Oct 2023 5:12:21pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::String name, juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, juce::String sustainId, juce::String releaseId)
{
    
    componentName = name;
    
//    attackAttachment = std::make_unique<SliderAttachment>(apvts, attackId, attackSlider);
//    decayAttachment = std::make_unique<SliderAttachment>(apvts, decayId, decaySlider);
//    sustainAttachment = std::make_unique<SliderAttachment>(apvts, sustainId, sustainSlider);
//    releaseAttachment = std::make_unique<SliderAttachment>(apvts, releaseId, releaseSlider);
//
//    setSliderParams(attackSlider);
//    setSliderParams(decaySlider);
//    setSliderParams(sustainSlider);
//    setSliderParams(releaseSlider);
    
    setSliderWithLabel (attackSlider, attackLabel, apvts, attackId, attackAttachment);
    setSliderWithLabel (decaySlider, decayLabel, apvts, decayId, decayAttachment);
    setSliderWithLabel (sustainSlider, sustainLabel, apvts, sustainId, sustainAttachment);
    setSliderWithLabel (releaseSlider, releaseLabel, apvts, releaseId, releaseAttachment);

}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced (5);
    auto labelSpace = bounds.removeFromTop (25.0f);
    
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText (componentName, labelSpace.withX (5), juce::Justification::left);
    g.drawRoundedRectangle (bounds.toFloat(), 5.0f, 2.0f);
}

void AdsrComponent::resized()
{
    const auto bounds = getLocalBounds().reduced (10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getHeight() - 45;
    const auto sliderStartX = padding + 5;
    const auto sliderStartY = 55;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;
    const auto labelStart = sliderStartY - labelYOffset;
    
    attackSlider.setBounds (sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    attackLabel.setBounds (attackSlider.getX(), labelStart, sliderWidth, labelHeight);
    
    decaySlider.setBounds (attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    decayLabel.setBounds (decaySlider.getX(), labelStart, sliderWidth, labelHeight);
    
    sustainSlider.setBounds (decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainLabel.setBounds (sustainSlider.getX(), labelStart, sliderWidth, labelHeight);
    
    releaseSlider.setBounds (sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseLabel.setBounds (releaseSlider.getX(), labelStart, sliderWidth, labelHeight);

}

//void AdsrComponent::setSliderParams(juce::Slider& slider)    //takes as input the reference to the slider to make sure we are working on that and not on a copy.
//{
//    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
//    addAndMakeVisible(slider);
//    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
//}

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
void AdsrComponent::setSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment)
{
    slider.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible (slider);
    
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramId, slider);
    
    label.setColour (juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont (15.0f);
    label.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (label);
}
