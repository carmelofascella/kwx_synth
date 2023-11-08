/*
  ==============================================================================

    EffectsComponent.h
    Created: 7 Nov 2023 5:08:18pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../SynthVoice.h"

//==============================================================================
/*
*/
class EffectsComponent  : public juce::Component
{
public:
    EffectsComponent(juce::AudioProcessorValueTreeState& apvts, juce::ValueTree& variableTree, SynthVoice& synthVoice, juce::File& savedFile, juce::File& root);
    ~EffectsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::TextButton loadBtn;
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::Label irName;
    juce::ToggleButton convolBtn;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectsComponent)
    
};
