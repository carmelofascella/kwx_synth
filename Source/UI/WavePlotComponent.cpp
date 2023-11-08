/*
  ==============================================================================

    Oscilloscope.cpp
    Created: 6 Nov 2023 5:04:44pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WavePlotComponent.h"

//==============================================================================
WavePlotComponent::WavePlotComponent()
{


}

WavePlotComponent::~WavePlotComponent()
{
}

void WavePlotComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced (5);
    auto labelSpace = bounds.removeFromTop (25.0f);

    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText ("Waveform", labelSpace.withX (5), juce::Justification::left);
    g.drawRoundedRectangle (bounds.toFloat(), 5.0f, 2.0f);
}

void WavePlotComponent::resized()
{

}
