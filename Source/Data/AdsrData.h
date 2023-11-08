/*
  ==============================================================================

    AdsrData.h
    Created: 29 Oct 2023 5:55:24pm
    Author:  Carmelo Fascella

  ==============================================================================
*/
//Data DSP of the ADSR object

#pragma once
#include <JuceHeader.h>

class AdsrData : public juce::ADSR
{
public:
    void update(const float attack, const float decay, const float sustain, const float release);
    
private:

    juce::ADSR::Parameters adsrParams;
    
};


