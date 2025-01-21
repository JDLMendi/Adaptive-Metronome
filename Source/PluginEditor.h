#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PlayersGUI.h"
#include "AlphasAndBetas.h"
#include "Player.h"

//==============================================================================
/**
*/

class AdaptiveMetronomeAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AdaptiveMetronomeAudioProcessorEditor (AdaptiveMetronomeAudioProcessor&);
    ~AdaptiveMetronomeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void updateStatusLabel(const juce::String&);
    PlayerStruct GetPlayerParameters(int);
    void savePlayerParametersToCSV();
    void UpdateModel();

private:
    AdaptiveMetronomeAudioProcessor& audioProcessor;

    juce::TextButton loadMidiBtn;
    juce::TextButton loadCongifBtn;
    juce::TextButton resetBtn;
    juce::TextButton oscMessageBtn;

#if JUCE_DEBUG
    juce::TextButton saveToCSVBtn;
    juce::TextButton loadParamsBtn;
#endif

    juce::ComboBox noPlayerCB;
    juce::Label noPlayerLB;

    PlayersGUI playersSection;
    AlphasAndBetas alphasAndBetas;

    juce::Label statusLB;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdaptiveMetronomeAudioProcessorEditor)
};
