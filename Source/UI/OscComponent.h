/*
  ==============================================================================

    OscComponent.h
    Created: 29 Oct 2023 6:40:58pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::String name, juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId, juce::String gainValueId, juce::String isOscActiveBtnId);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
    
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;
    
    juce::Slider gainSlider;
    juce::Slider fmFreqSlider;
    juce::Slider fmDepthSlider;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> gainAttachment;
    std::unique_ptr<SliderAttachment> fmFreqAttachment;
    std::unique_ptr<SliderAttachment> fmDepthAttachment;
    
    juce::ToggleButton isOscActiveBtn;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;

    juce::Label fmFreqLabel {"FM Freq", "FM Freq"};
    juce::Label fmDepthLabel {"FM Depth", "FM Depth"};
    juce::Label gainLabel {"Gain", "Gain"};
    
//    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId,std::unique_ptr<SliderAttachment>& attachment);
    
    juce::String componentName {""};
    
 };
