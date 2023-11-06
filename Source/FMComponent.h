/*
  ==============================================================================

    FMComponent.h
    Created: 31 Oct 2023 4:16:29pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FMComponent  : public juce::Component
{
public:
    FMComponent(juce::AudioProcessorValueTreeState& apvts);
    ~FMComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMComponent)
    
    juce::Slider fmFreqSlider;
    juce::Slider fmDepthSlider;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> fmFreqAttachment;
    std::unique_ptr<SliderAttachment> fmDepthAttachment;
    
    juce::Label fmFreqLabel {"FM Freq", "FM Freq"};
    juce::Label fmDepthLabel {"FM Depth", "FM Depth"};
    
    void setSliderStyle(juce::Slider& slider);
};
