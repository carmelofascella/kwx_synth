/*
  ==============================================================================

    SynthVoice.cpp
    Created: 24 Oct 2023 8:10:26pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include "SynthVoice.h"


bool SynthVoice::canPlaySound (juce::SynthesiserSound *sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr; //return true if it points to something
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{

    osc.setWaveFrequency(midiNoteNumber);
    adsr.noteOn();
    filterAdsr.noteOn();
    
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    filterAdsr.noteOff();
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
        
    osc.prepareToPlay(spec);
    filterAdsr.setSampleRate(sampleRate);
    filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
    adsr.setSampleRate(sampleRate);
    gain.prepare(spec);
    
    gain.setGainLinear(0.3f);
    
    irLoader.reset();
    irLoader.prepare(spec);
    
    irLoader.loadImpulseResponse(irPath,
                                 juce::dsp::Convolution::Stereo::yes,
                                 juce::dsp::Convolution::Trim::yes,
                                 0);

    isPrepared = true;
}

//void SynthVoice::updateAdsr(const float attack, const float decay, const float sustain, const float release)
//{
//    adsr.updateADSR(attack, decay, sustain, release);
//}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared); //check to run code only if prepareToPlay method has been called.
    
    if (! isVoiceActive())
        return;
    
    synthBuffer.setSize (outputBuffer.getNumChannels(), numSamples, false, false, true);
    filterAdsr.applyEnvelopeToBuffer (outputBuffer, 0, numSamples); //each voice has it s own mod filter.
    synthBuffer.clear();
    
        
    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
    osc.getNextAudioBlock (audioBlock);
    adsr.applyEnvelopeToBuffer (synthBuffer, 0, synthBuffer.getNumSamples()); //audioBlock and outputBuffer are aliases, so the same thing (putting audio into one means putting stuff into the other)
    filter.process (synthBuffer);
    
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    if(irLoader.getCurrentIRSize() >= 0)
    {
        std::cout<<"processing" << std::endl;
        irLoader.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    }
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom (channel, startSample, synthBuffer, channel, 0, numSamples);
        
        if (! adsr.isActive())
            clearCurrentNote();
    }

}

void SynthVoice::updateFilter(const int filterType, const float cutoff, const float resonance)
{
    auto modulator = filterAdsr.getNextSample();
    filter.updateParameters(modulator, filterType, cutoff, resonance);

}

//void SynthVoice::updateModAdsr(const float attack, const float decay, const float sustain, const float release)
//{
//    filterAdsr.updateADSR(attack, decay, sustain, release);
//}
