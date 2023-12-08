/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapSynthAudioProcessor::TapSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
apvts (*this, nullptr, "Parameters", createParams()),
audioViewer(1)

#endif
{

    
    //Add voices
    for (int i=0; i<NUM_VOICES; i++){
        SynthVoice* synthVoice = new SynthVoice();
        synthVoiceVector.push_back(synthVoice);
        synth.addVoice(synthVoice);
    }

    //Add sound
    synthSound = new SynthSound();
    synth.addSound(synthSound);
    
    //Audio viewer settings
    audioViewer.setBufferSize(1024);
    audioViewer.setRepaintRate(30);
    audioViewer.setNumChannels(2);
    
    //Set variable Tree
    variableTree = {
            
            "Variables", {},
            {
              { "Group", {{ "name", "IR Vars" }},
                {
                  { "Parameter", {{ "id", "file1" }, { "value", "/" }}},
                    { "Parameter", {{ "id", "root" }, { "value", "/" }}}
                }
              }
            }
          };

}

TapSynthAudioProcessor::~TapSynthAudioProcessor()
{
}

//==============================================================================
const juce::String TapSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TapSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TapSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TapSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TapSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TapSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TapSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TapSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TapSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void TapSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TapSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for(int i=0; i<synth.getNumVoices(); i++)
    {
        if(auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))  //dynamic cast to get our custom function and not the parent.
        {
            DBG(i);
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
    
    //Convolution (use one single spec for synthvoice and for this).
    

    
}

void TapSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TapSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TapSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //Access each voice of the synthesiser and updating them
    for(int i = 0; i<synth.getNumVoices(); ++i)
    {
        juce::String oscId = std::to_string(i+1);
        
        if(auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            /* Get current apvts parameters*/
            //OSC1
            auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
            auto& fmDepth = *apvts.getRawParameterValue("OSC1FMDEPTH");
            auto& fmFreq = *apvts.getRawParameterValue("OSC1FMFREQ");
            auto& isActiveBtnOsc1 = *apvts.getRawParameterValue("OSC1BTN");
            auto& gainOsc1 = *apvts.getRawParameterValue("OSC1GAIN");
            
            //OSC2
            auto& oscWaveChoice2 = *apvts.getRawParameterValue("OSC2WAVETYPE");
            auto& fmDepth2 = *apvts.getRawParameterValue("OSC2FMDEPTH");
            auto& fmFreq2 = *apvts.getRawParameterValue("OSC2FMFREQ");
            auto& isActiveBtnOsc2 = *apvts.getRawParameterValue("OSC2BTN");
            auto& gainOsc2 = *apvts.getRawParameterValue("OSC2GAIN");

            // Amp Adsr
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");
            
            auto& attack2 = *apvts.getRawParameterValue("ATTACK2");
            auto& decay2 = *apvts.getRawParameterValue("DECAY2");
            auto& sustain2 = *apvts.getRawParameterValue("SUSTAIN2");
            auto& release2 = *apvts.getRawParameterValue("RELEASE2");
            
            // Filter
            auto& filterType = *apvts.getRawParameterValue("FILTERTYPE");
            auto& cutoff = *apvts.getRawParameterValue("FILTERCUTOFF");
            auto& resonance = *apvts.getRawParameterValue("FILTERRES");
            
            // Mod Adsr
            auto& modAttack = *apvts.getRawParameterValue("MODATTACK");
            auto& modDecay = *apvts.getRawParameterValue("MODDECAY");
            auto& modSustain = *apvts.getRawParameterValue("MODSUSTAIN");
            auto& modRelease = *apvts.getRawParameterValue("MODRELEASE");
            
            //Convolution Button
            auto& convFlag = *apvts.getRawParameterValue("CONVFLAG");
            
            //Pan
            auto& panValue = *apvts.getRawParameterValue("PAN");
            
            //Master
            auto& master = *apvts.getRawParameterValue("MASTER");
        
            /* Set oscillators parameters*/
            auto& osc1 = voice->getOscillator1();
            auto& osc2 = voice->getOscillator2();
            

            for (int i = 0; i < getTotalNumOutputChannels(); i++)
            {
                osc1[i].selectWaveType(oscWaveChoice);
                osc1[i].setFmParams(fmDepth, fmFreq);
                osc1[i].setGain(gainOsc1);
                
                osc2[i].selectWaveType(oscWaveChoice2);
                osc2[i].setFmParams(fmDepth2, fmFreq2);
                osc2[i].setGain(gainOsc2);
                
            }
            voice->setOscillatorActiveState(isActiveBtnOsc1.load(), isActiveBtnOsc2.load());
            
            voice->getAdsr1().update(attack.load(), decay.load(), sustain.load(), release.load());
            voice->getAdsr2().update(attack2.load(), decay2.load(), sustain2.load(), release2.load()); //load because they are atomic float and not
            
            voice->getFilterAdsr().update(modAttack.load(), modDecay.load(), modSustain.load(), modRelease.load());
            voice->updateFilter(filterType, cutoff, resonance);
            voice->setConvolutionFlag(convFlag.load());
            
            voice->setPan(panValue);
            
            voice->setMaster(master.load());
        
            
        }
    }
    
    //This calls the render in the synth voices.
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    audioViewer.pushBuffer(buffer);
    
    

    
}

//==============================================================================
bool TapSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TapSynthAudioProcessor::createEditor()
{
    return new TapSynthAudioProcessorEditor (*this);
}

//==============================================================================
void TapSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    apvts.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    apvts.state.writeToStream (stream);
}

void TapSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("Variables");

    if (tree.isValid())
    {
        apvts.state = tree;

        savedFile = juce::File(variableTree.getProperty("file1"));
        root = juce::File(variableTree.getProperty("root"));

//        synthVoice->getIrLoader().loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);

    }

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TapSynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout TapSynthAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    /* Parameters that the users will be able to modify */
    
    // First Oscillator parameters
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc1 Wave Type", juce::StringArray {"Sin", "Saw", "Square"}, 0)) ;
    // FM Freq (frequency of the modulation wave)
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("OSC1FMFREQ", "Osc1 FM Frequency", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.2f }, 0.0f));
    // FM Depth (how big we want this wave to be)
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("OSC1FMDEPTH", "Osc1 FM Depth", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC1GAIN", "Oscillator 1 Gain", juce::NormalisableRange<float> { -40.0f, 1.0f, 0.1f }, 0.1f, "dB"));
    params.push_back (std::make_unique<juce::AudioParameterBool>("OSC1BTN", "Osc1 Active Button", true));

    
    // Second Oscillator parameters
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2WAVETYPE", "Osc2 Wave Type", juce::StringArray {"Sin", "Saw", "Square"}, 0)) ;
    // FM Freq (frequency of the modulation wave)
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("OSC2FMFREQ", "Osc2 FM Frequency", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.2f }, 0.0f));
    // FM Depth (how big we want this wave to be)
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("OSC2FMDEPTH", "Osc2 FM Depth", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC2GAIN", "Oscillator 2 Gain", juce::NormalisableRange<float> { -40.0f, 1.0f, 0.1f }, 0.1f, "dB"));
    params.push_back (std::make_unique<juce::AudioParameterBool>("OSC2BTN", "Osc1 Active Button", true));
    
    
    
    // ADSR Osc1
    params.push_back (std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> { 0.1f, 5.0f, 0.01f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> { 0.1f, 3.0f, 0.01f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> { 0.1f, 3.0f, 0.01f }, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> { 0.1f, 5.0f, 0.01f }, 0.4f));
    
    // ADSR Osc2
    params.push_back (std::make_unique<juce::AudioParameterFloat>("ATTACK2", "Attack Osc2", juce::NormalisableRange<float> { 0.1f, 5.0f, 0.01f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("DECAY2", "Decay Osc2", juce::NormalisableRange<float> { 0.1f, 3.0f, 0.01f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("SUSTAIN2", "Sustain Osc2", juce::NormalisableRange<float> { 0.1f, 3.0f, 0.01f }, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("RELEASE2", "Release Osc2", juce::NormalisableRange<float> { 0.1f, 5.0f, 0.01f }, 0.4f));
    
    // Filter ADSR
    params.push_back (std::make_unique<juce::AudioParameterFloat>("MODATTACK", "Mod Attack", juce::NormalisableRange<float> { 0.1f, 5.0f, 0.01f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("MODDECAY", "Mod Decay", juce::NormalisableRange<float> { 0.1f, 3.0f, 0.01f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("MODSUSTAIN", "Mod Sustain", juce::NormalisableRange<float> { 0.1f, 3.0f, 0.01f }, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("MODRELEASE", "Mod Release", juce::NormalisableRange<float> { 0.1f, 5.0f, 0.01f }, 0.4f));
    
    //Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray {"Low-pass", "Band-pass", "High-pass"}, 0)) ;
    params.push_back (std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float> { 20.0f, 20000.0f, 0.1f, 0.6f }, 200.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float> { 1.0f, 10.0f, 0.1f}, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterBool>("CONVFLAG", "Convolution Flag", false));
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>("PAN", "Stereo Pan", juce::NormalisableRange<float> { -1.0f, 1.0f, 0.1f }, 0.0f));
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>("MASTER", "Master Volume", juce::NormalisableRange<float> { -70.0f, 2.0f, 0.1f }, 0.0f, "dB"));
    
    return { params.begin(), params.end() };
}
