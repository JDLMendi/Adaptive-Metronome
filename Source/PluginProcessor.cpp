#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
#pragma region Main Functions

#pragma endregion Creation of Metronome and Block Handling

// Constructor
AdaptiveMetronomeAudioProcessor::AdaptiveMetronomeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

// Destructor
AdaptiveMetronomeAudioProcessor::~AdaptiveMetronomeAudioProcessor() = default;

// Called for Audio Playback - Things to be done before audio is played
void AdaptiveMetronomeAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    
}

// Main Function - Samples inputs through here as this is called continuously throughout playback, 
void AdaptiveMetronomeAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

}

// Called after Audio Playback 
void AdaptiveMetronomeAudioProcessor::releaseResources()
{
    
}

//==============================================================================
#pragma region State Handling


// Calls when a project is saved
void AdaptiveMetronomeAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

// Used to load saved information in the state
void AdaptiveMetronomeAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}
#pragma endregion Functions Related to handling closeing and opening projects with the plugin
//==============================================================================
#pragma region Helper Functions
const juce::String AdaptiveMetronomeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AdaptiveMetronomeAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AdaptiveMetronomeAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AdaptiveMetronomeAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AdaptiveMetronomeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AdaptiveMetronomeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int AdaptiveMetronomeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AdaptiveMetronomeAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String AdaptiveMetronomeAudioProcessor::getProgramName(int index)
{
    return {};
}

void AdaptiveMetronomeAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}
#pragma endregion Functions provided by the template that might not have purpose for the Metronome
//==============================================================================
#pragma region Plugin Editor/Configurations
#ifndef JucePlugin_PreferredChannelConfigurations
bool AdaptiveMetronomeAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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


//==============================================================================
bool AdaptiveMetronomeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AdaptiveMetronomeAudioProcessor::createEditor()
{
    return new AdaptiveMetronomeAudioProcessorEditor(*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AdaptiveMetronomeAudioProcessor();
}
#pragma endregion Functions related to the Plugin Editor and Confirguration
//==============================================================================



