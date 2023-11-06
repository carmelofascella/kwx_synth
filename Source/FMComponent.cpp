/*
  ==============================================================================

    FMComponent.cpp
    Created: 31 Oct 2023 4:16:29pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FMComponent.h"

//==============================================================================
FMComponent::FMComponent(juce::AudioProcessorValueTreeState& apvts)
{
    setSliderStyle(fmFreqSlider);
    setSliderStyle(fmDepthSlider);

    //fmFreqAttachment = std::make_unique<SliderAttachment>(apvts, "FMFREQ", fmFreqSlider);
    //fmDepthAttachment = std::make_unique<SliderAttachment>(apvts, "FMDEPTH", fmDepthSlider);
    

}

FMComponent::~FMComponent()
{
}

void FMComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::black);   // clear the background

}

void FMComponent::resized()
{
    fmFreqSlider.setBounds(0, 0, 75, 75);
    fmDepthSlider.setBounds(100, 0, 75, 75);

}

void FMComponent::setSliderStyle(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
    
}
