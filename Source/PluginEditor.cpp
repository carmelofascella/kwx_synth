/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapSynthAudioProcessorEditor::TapSynthAudioProcessorEditor (TapSynthAudioProcessor& p)
: AudioProcessorEditor (&p),
audioProcessor (p),
osc1(audioProcessor.apvts, "OSC1WAVETYPE", "OSC1FMFREQ", "OSC1FMDEPTH", "OSC1GAIN"),
osc2(audioProcessor.apvts, "OSC2WAVETYPE", "OSC2FMFREQ", "OSC2FMDEPTH", "OSC2GAIN"),
adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE"),
filter(audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES"),
modAdsr("Mod Enveloper", audioProcessor.apvts, "MODATTACK", "MODDECAY", "MODSUSTAIN", "MODRELEASE"),
effectsBox(audioProcessor.apvts, audioProcessor.variableTree, *audioProcessor.synthVoice, audioProcessor.savedFile, audioProcessor.root)

{
    
    setSize (815, 920);
    addAndMakeVisible(adsr);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);
    addAndMakeVisible(filter);
    addAndMakeVisible(modAdsr);
    addAndMakeVisible(effectsBox);
    addAndMakeVisible(oscilloscope);
    addAndMakeVisible(audioProcessor.audioViewer);
    
    
    
    audioProcessor.audioViewer.setColours(juce::Colours::black, juce::Colours::white.withAlpha(0.8f));
}

TapSynthAudioProcessorEditor::~TapSynthAudioProcessorEditor()
{
 
}

//==============================================================================
void TapSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void TapSynthAudioProcessorEditor::resized()
{
//    adsr.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
//    osc.setBounds(10, 10, 175, 200);
    //fm.setBounds(10, 40, getWidth() / 2, 100);
    
    const auto paddingX = 5;
    const auto paddingY = 10;
    const auto paddingY2 = 210;
    const auto width = 400;
    const auto height = 200;
    const auto paddingY3 = 410;
    const auto paddingY4 = 610;
    const auto paddingY5 = 710;

    
    osc1.setBounds (paddingX, paddingY, width, height);
    osc2.setBounds(osc1.getX(), paddingY2, width, osc1.getHeight());
    adsr.setBounds (osc1.getRight(), paddingY, width, height);
    filter.setBounds(paddingX, paddingY3, width, height);
    modAdsr.setBounds(filter.getRight(), paddingY3, width, height);
    

    effectsBox.setBounds(paddingX, paddingY4, modAdsr.getRight(), 100);
    
    oscilloscope.setBounds(paddingX, paddingY5, modAdsr.getRight(), height);

    audioProcessor.audioViewer.setBounds(oscilloscope.getBoundsInParent().withSizeKeepingCentre(oscilloscope.getWidth()*0.9f, oscilloscope.getHeight()*0.6f));
    
}



