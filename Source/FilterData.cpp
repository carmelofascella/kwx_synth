/*
  ==============================================================================

    FilterData.cpp
    Created: 1 Nov 2023 10:09:52pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include "FilterData.h"

    
void FilterData::prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels)
{
    filter.reset();
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;
    
    filter.prepare(spec);
    
    isPrepared = true;
}
void FilterData::process(juce::AudioBuffer<float>& buffer)
{
    jassert (isPrepared);
    
    juce::dsp::AudioBlock<float> block { buffer };
    filter.process(juce::dsp::ProcessContextReplacing<float> {block});
    
    
}
void FilterData::updateParameters(const float modulator, const int filterType, const float frequency, const float resonance)
{
    switch(filterType)
    {
        case 0:
            filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            
        case 1:
            filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            
        case 2:
            filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            
        default:
            break;
    }
    
    
    float modFreq = frequency * modulator;  //modulate frequency.
    modFreq = std::fmax(std::fmin(modFreq, 20000.0f),20.0f);    //to be sure value is in the range.

    
    filter.setCutoffFrequency(modFreq);
    filter.setResonance(resonance);
}

void FilterData::reset()
{
    filter.reset();
}
