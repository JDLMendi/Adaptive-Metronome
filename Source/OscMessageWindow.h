#pragma once
#include <JuceHeader.h>

class OscMessageWindow : public juce::DialogWindow
{
public:
    OscMessageWindow()
        : juce::DialogWindow("OSC Messages", juce::Colours::lightgrey, true)
    {
        setSize(400, 300);

        // Adding the TextEditor directly as a visible component
        addAndMakeVisible(static_cast<Component*>(&oscMessageEditor));


        // Configuring the TextEditor to be multi-line and non-editable
        oscMessageEditor.setMultiLine(true);
        oscMessageEditor.setReturnKeyStartsNewLine(true);
        oscMessageEditor.setReadOnly(true);
        oscMessageEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
        oscMessageEditor.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    }

    void addOscMessage(const juce::String& message)
    {
        // Append the new message to the accumulated messages
        accumulatedMessages += message + "\n";

        // Update the TextEditor with the full record of messages
        oscMessageEditor.setText(accumulatedMessages);
    }

private:
    juce::TextEditor oscMessageEditor;  // TextEditor for displaying messages
    juce::String accumulatedMessages;   // This will hold all OSC messages

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscMessageWindow)
};
