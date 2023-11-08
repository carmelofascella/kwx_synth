/*
  ==============================================================================

    Oscilloscope.h
    Created: 6 Nov 2023 5:04:44pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WavePlotComponent  : public juce::Component
{
public:
    WavePlotComponent();
    ~WavePlotComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavePlotComponent)
};
