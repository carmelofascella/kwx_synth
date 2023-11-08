/*
  ==============================================================================

    FilterComponent.h
    Created: 3 Nov 2023 8:46:14pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterCutoffId, juce::String filterResonanceID);
    
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox filterTypeSelector {"Filter Type"};
    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId,std::unique_ptr<SliderAttachment>& attachment);
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectorAttachment;
    
    std::unique_ptr<SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<SliderAttachment> filterResonanceAttachment;
    
    juce::Label filterSelectorLabel {"Filter Type", "Filter Type"};
    juce::Label filterCutoffLabel {"Cutoff", "Cutoff"};
    juce::Label filterResonanceLabel {"Resonance", "Resonance"};

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
    
};
