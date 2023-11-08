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
osc(audioProcessor.apvts, "OSC1WAVETYPE", "OSC1FMFREQ", "OSC1FMDEPTH"),
adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE"),
filter(audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES"),
modAdsr("Mod Enveloper", audioProcessor.apvts, "MODATTACK", "MODDECAY", "MODSUSTAIN", "MODRELEASE"),
effectsBox(audioProcessor.apvts, audioProcessor.variableTree, *audioProcessor.synthVoice, audioProcessor.savedFile, audioProcessor.root)
{
    
    setSize (620, 800);
    addAndMakeVisible(adsr);
    addAndMakeVisible(osc);
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
    const auto paddingY = 35;
    const auto paddingY2 = 235;
    const auto width = 300;
    const auto height = 200;
    const auto paddingY3 = 435;
    const auto paddingY4 = 535;

    
    osc.setBounds (paddingX, paddingY, width, height);
    adsr.setBounds (osc.getRight(), paddingY, width, height);
    filter.setBounds(paddingX, paddingY2, width, height);
    modAdsr.setBounds(filter.getRight(), paddingY2, width, height);
    
    effectsBox.setBounds(paddingX, paddingY3, modAdsr.getRight(), 100);
    
    oscilloscope.setBounds(paddingX, paddingY4, modAdsr.getRight(), height);

    audioProcessor.audioViewer.setBounds(oscilloscope.getBoundsInParent().withSizeKeepingCentre(oscilloscope.getWidth()*0.9f, oscilloscope.getHeight()*0.6f));
    
}



