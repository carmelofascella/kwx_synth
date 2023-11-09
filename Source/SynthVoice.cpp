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
    for (int i=0; i<numChannelsToProcess; i++)
    {
        osc1[i].setWaveFrequency(midiNoteNumber);
        osc2[i].setWaveFrequency(midiNoteNumber);
    }

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
    
    for (int ch = 0; ch < numChannelsToProcess; ch++)
    {
        osc1[ch].prepareToPlay(spec);
        osc2[ch].prepareToPlay(spec);
    }

    filterAdsr.setSampleRate(sampleRate);
    filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
    adsr.setSampleRate(sampleRate);
    gain.prepare(spec);
    
    gain.setGainLinear(0.3f);
    
    irLoader.reset();
    irLoader.prepare(spec);
    
//    irLoader.loadImpulseResponse(irPath,
//                                 juce::dsp::Convolution::Stereo::yes,
//                                 juce::dsp::Convolution::Trim::yes,
//                                 0);

    isPrepared = true;
}

//void SynthVoice::updateAdsr(const float attack, const float decay, const float sustain, const float release)
//{
//    adsr.updateADSR(attack, decay, sustain, release);
//}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared);
    
    if (! isVoiceActive())
        return;
    
    synthBuffer.setSize (outputBuffer.getNumChannels(), numSamples, false, false, true);

    filterAdsr.applyEnvelopeToBuffer (synthBuffer, 0, numSamples); //each voice has it s own mod filter. It processes each samples with the chosen envelope.
    synthBuffer.clear();
    
    //osc1.getNextAudioBlock (audioBlock);
    //osc2.getNextAudioBlock(audioBlock);
    
    //Sum osc values 
    for (int ch = 0; ch < synthBuffer.getNumChannels(); ++ch)
    {
        auto* buffer = synthBuffer.getWritePointer (ch, 0);
        
        for (int s = 0; s < synthBuffer.getNumSamples(); ++s)
        {
            if(isActiveBtnOsc1 and isActiveBtnOsc2)  {buffer[s] = (osc1[ch].processNextSample (buffer[s]) + osc2[ch].processNextSample (buffer[s]))/2;}
            else if ((isActiveBtnOsc1 and !isActiveBtnOsc2) ) {buffer[s] = osc1[ch].processNextSample (buffer[s]);}
            else if ((!isActiveBtnOsc1 and isActiveBtnOsc2) ) {buffer[s] = osc2[ch].processNextSample (buffer[s]);}
            //buffer[s] = osc1[ch].processNextSample (buffer[s]);
        }
    }
    
    juce::dsp::AudioBlock<float> audioBlock { synthBuffer }; //Creates an AudioBlock that points to the data in an AudioBuffer.
    
    
    adsr.applyEnvelopeToBuffer (synthBuffer, 0, synthBuffer.getNumSamples()); //audioBlock and outputBuffer are aliases, so the same thing (putting audio into one means putting stuff into the other)
    filter.process (synthBuffer);
    
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    //Convolution with IR.
    if(irLoader.getCurrentIRSize() > 0 and isConvolutionActive)
    {
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

void SynthVoice::setConvolutionFlag(bool convolFlag)
{
    isConvolutionActive = convolFlag;
}

void SynthVoice::setOscillatorActivity(bool isActiveBtnOsc1, bool isActiveBtnOsc2)
{
    this->isActiveBtnOsc1=isActiveBtnOsc1;
    this->isActiveBtnOsc2=isActiveBtnOsc2;
}
