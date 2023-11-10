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
#include "Data/OscData.h"
#include "Data/AdsrData.h"
#include "Data/FilterData.h"

static constexpr int numChannelsToProcess { 2 };

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
    
//    OscData& getOscillator() { return osc; }
//    OscData& getOscillator2() { return osc2; }
    std::array<OscData, numChannelsToProcess>& getOscillator1() { return osc1; }
    std::array<OscData, numChannelsToProcess>& getOscillator2() { return osc2; }
    
    AdsrData& getAdsr1() { return adsr1; }
    AdsrData& getAdsr2() { return adsr2; }
    
    AdsrData& getFilterAdsr() { return filterAdsr; }
    FilterData& getFilter() { return filter; }
    juce::dsp::Convolution& getIrLoader() {return irLoader;}
    
    void updateFilter(const int filterType, const float cutoff, const float resonance);
    
    void setConvolutionFlag(bool convolFlag);
    void setOscillatorActiveState(bool isActiveBtnOsc1, bool isActiveBtnOsc2);
    void setMaster(float masterValue);
    
    
    
private:
    juce::AudioBuffer<float> synthBuffer;
    
    //order of dsp
//    OscData osc;
//    OscData osc2;
    
    std::array<OscData, numChannelsToProcess> osc1;
    std::array<OscData, numChannelsToProcess> osc2;
    
    AdsrData adsr1;
    AdsrData adsr2;
    
    AdsrData filterAdsr;
    FilterData filter;
    juce::dsp::Convolution irLoader;
    juce::dsp::Gain<float> masterGain;
    
    bool isConvolutionActive = false;
    
    bool isPrepared {false};
    
    int isActiveBtnOsc1{1};
    int isActiveBtnOsc2{1};
    int numActiveOsc {2};
    

};
