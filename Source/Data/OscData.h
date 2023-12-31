/*
  ==============================================================================

    OscData.h
    Created: 29 Oct 2023 6:16:58pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void selectWaveType(const int choice);
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
    void setWaveFrequency(const int midiNoteNumber);
    void setFmParams (const float depth, const float freq);
    float processNextSample (float input);
    void setGain (const float levelInDecibels);
    

    
private:
    juce::dsp::Oscillator<float> oscillator {[](float x) {return std::sin(x); } };
    juce::dsp::Gain<float> gain;
    float fmMod { 0.0f };
    float fmDepth { 0.0f };
    int lastMidiNote;
};
