/*
  ==============================================================================

    OscData.cpp
    Created: 29 Oct 2023 6:16:58pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include "OscData.h"

void OscData::selectWaveType(const int choice)
{
    
    switch (choice) {
        case 0:
            //Sine
            initialise([](float x) {return std::sin(x); } );
            break;
        case 1:
            // Saw wave
            initialise([](float x) {return x / juce::MathConstants<float>::pi; } );
            break;
            
        case 2:
            //Square wave
            initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f;} );
            break;
            
        default:
            jassertfalse;   //trigger jassert cause not supposed to be here.
            break;
    }
}


void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    prepare(spec);
    fmOsc.prepare(spec);
    
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    //Process the frequency of each sample of the main oscillator with the the other wave (fmOsc)
    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            fmMod = fmOsc.processSample(block.getSample(ch, sample)) * fmDepth;
        }
    }
    
    process(juce::dsp::ProcessContextReplacing<float> (block)) ;        //repplace what it s inside the buffer with the new osc content.
}


void OscData::setWaveFrequency(const int midiNoteNumber)
{
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    lastMidiNote = midiNoteNumber;
}

void OscData::setFmParams (const float depth, const float freq)
{
    fmOsc.setFrequency(freq);
    fmDepth = depth;
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;
    setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);

}
