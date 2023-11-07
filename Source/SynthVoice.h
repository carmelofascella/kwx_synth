/*
  ==============================================================================

    SynthVoice.h
    Created: 24 Oct 2023 8:10:26pm
    Author:  Carmelo Fascella

  ==============================================================================
*/
//Voice: something that produces sound

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "OscData.h"
#include "AdsrData.h"
#include "FilterData.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound *sound) override;
    
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    
    void stopNote (float velocity, bool allowTailOff) override;
    
    void pitchWheelMoved (int newPitchWheelValue) override;
    
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    
    //void updateAdsr(const float attack, const float decay, const float sustain, const float release);
    //void updateModAdsr(const float attack, const float decay, const float sustain, const float release);
    
    OscData& getOscillator() { return osc; }
    AdsrData& getAdsr() { return adsr; }
    AdsrData& getFilterAdsr() { return filterAdsr; }
    FilterData& getFilter() { return filter; }
    
    void updateFilter(const int filterType, const float cutoff, const float resonance);
    
    
    
private:
    juce::AudioBuffer<float> synthBuffer;
    
    //order of dsp
    OscData osc;
    AdsrData adsr;
    AdsrData filterAdsr;
    FilterData filter;
    juce::dsp::Convolution irLoader;
    juce::dsp::Gain<float> gain;
    
//    juce::String irPath = "/Users/carmelofascella/Documents/Programming/scripts/juce-audio-scripts/kwx_synth/Dataset/ir_huge_reverb.wav";
    
    juce::String irPath = "/Users/carmelofascella/Documents/Programming/scripts/juce-audio-scripts/kwx_synth/Dataset/Overdriven-E112-K100-G4-SSP2-v1.0/SSP2/DYN-US-6/OD-E112-K100-DYN-US-6-P14-10.wav";

    
    
    bool isPrepared {false};
    

};
