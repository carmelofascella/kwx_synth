/*
  ==============================================================================

    SynthSound.h
    Created: 24 Oct 2023 8:10:43pm
    Author:  Carmelo Fascella

  ==============================================================================
*/
//Sound: something produced by the voice.

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel (int midiChannel) override { return true;}
};
