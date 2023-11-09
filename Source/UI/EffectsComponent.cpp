/*
  ==============================================================================

    EffectsComponent.cpp
    Created: 7 Nov 2023 5:08:18pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EffectsComponent.h"

//==============================================================================
EffectsComponent::EffectsComponent(juce::AudioProcessorValueTreeState& apvts, juce::ValueTree& variableTree, SynthVoice& synthVoice, juce::File& savedFile, juce::File& root)
{
    addAndMakeVisible(loadBtn);
    addAndMakeVisible(irName);
    addAndMakeVisible(convolBtn);
    
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "CONVFLAG", convolBtn);
    
    loadBtn.setButtonText("Load Impulse Response");
    loadBtn.onClick = [this, &variableTree, &synthVoice, &savedFile, &root]()
    {
        fileChooser = std::make_unique<juce::FileChooser>("Choose File",
                                                          root,
                                                          "*");
        const auto fileChooserFlags = juce::FileBrowserComponent::openMode |
        juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectDirectories;
        
        fileChooser->launchAsync(fileChooserFlags, [this, &variableTree, &synthVoice, &savedFile, &root](const juce::FileChooser& chooser)
                                 {
                                     juce::File result (chooser.getResult());
                                     
                                     if (result.getFileExtension() == ".wav" | result.getFileExtension() == ".mp3")
                                     {
                                         savedFile = result;
                                         variableTree.setProperty("file1", savedFile.getFullPathName(), nullptr);
                                         variableTree.setProperty("root", savedFile.getParentDirectory().getFullPathName(), nullptr);
                                         root = savedFile.getParentDirectory().getFullPathName();
                                         synthVoice.getIrLoader().loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
                                         irName.setText(result.getFileName(), juce::dontSendNotification);
                                         
                                         if(!convolBtn.getToggleState())
                                         {
                                             convolBtn.triggerClick();
                                         }
                                         
                                     }
                                 });

    };
    
//    convolBtn.onClick = [this]()
//    {
//        std::cout << (bool)convolBtn.getToggleState() << std::endl;
//    };
}

EffectsComponent::~EffectsComponent()
{
}

void EffectsComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced (5);
    auto labelSpace = bounds.removeFromTop (25.0f);
    
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText ("Convolution", labelSpace.withX (5), juce::Justification::left);
    g.drawRoundedRectangle (bounds.toFloat(), 5.0f, 2.0f);
}

void EffectsComponent::resized()
{
    //const auto btnX = getWidth() * JUCE_LIVE_CONSTANT(0.25);
    convolBtn.setBounds(20, 50, 30, 30);
    loadBtn.setBounds(convolBtn.getRight(), 50, 150, 30);
    irName.setBounds(loadBtn.getRight(), loadBtn.getY(), getWidth()/3, loadBtn.getHeight());
    

}
