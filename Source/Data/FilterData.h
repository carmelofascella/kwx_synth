/*
  ==============================================================================

    FilterData.h
    Created: 1 Nov 2023 10:09:52pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class FilterData
{
public:
    void prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(const float modulator, const int filterType, const float frequency, const float resonance);
    
    void reset();
private:
    juce::dsp::StateVariableTPTFilter<float> filter;
    bool isPrepared = false;
    
};
