/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"
#include "UI/WavePlotComponent.h"
#include "UI/EffectsComponent.h"

//==============================================================================
/**
*/
class TapSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
//private juce::Timer

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
    
    OscComponent osc1;
    OscComponent osc2;
    
    AdsrComponent adsr;
    FilterComponent filter;
    AdsrComponent modAdsr;
    WavePlotComponent oscilloscope;
    EffectsComponent effectsBox;
    
    
    
    int i = 0;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapSynthAudioProcessorEditor)
};
