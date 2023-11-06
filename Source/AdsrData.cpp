/*
  ==============================================================================

    AdsrData.cpp
    Created: 29 Oct 2023 5:55:24pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include "AdsrData.h"


void AdsrData::update(const float attack, const float decay, const float sustain, const float release)
{
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    
    setParameters(adsrParams);  //native method for juce::ADSR
}
