/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AdsrComponent.h"
#include "OscComponent.h"
#include "FilterComponent.h"

//==============================================================================
/**
*/
class TapSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TapSynthAudioProcessorEditor (TapSynthAudioProcessor&);
    ~TapSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    void setSliderParams(juce::Slider& slider);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TapSynthAudioProcessor& audioProcessor;
    
    OscComponent osc;
    AdsrComponent adsr;
    FilterComponent filter;
    AdsrComponent modAdsr;
    
    

    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapSynthAudioProcessorEditor)
};