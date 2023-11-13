/*
  ==============================================================================

    Oscilloscope.h
    Created: 6 Nov 2023 5:04:44pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WavePlotComponent  : public juce::Component
{
public:
    WavePlotComponent(juce::AudioProcessorValueTreeState& apvts, juce::String masterId, juce::String panId);
    ~WavePlotComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::Slider masterSlider;
    juce::Label  masterLabel { "Master", "Master" };
    
    juce::Slider panSlider;
    juce::Label  panLabel { "Pan", "Pan" };
    
    using SliderAttachment =juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> masterAttachment;
    std::unique_ptr<SliderAttachment> panAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavePlotComponent)

};
