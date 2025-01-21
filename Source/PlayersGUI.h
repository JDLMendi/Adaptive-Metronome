#pragma once

#include <JuceHeader.h>
#include "Player.h"

// PlayerParameter Structure for handling retrieval of parameters
struct PlayerParameters
{
    bool isUser;
    int id;
    int midiChannel;
    double volume;
    double delay;
    double motorNoiseSTD;
    double timeKeeperNoiseSTD;
};

//==============================================================================
// PlayersGUI class definition
class PlayersGUI : public juce::Component
{
public:
    PlayersGUI()
    {
        // Column names excluding "Alpha and Betas"
        const juce::StringArray columnNames = { "Player", "MIDI Channel", "Volume", "Delay\n(ms)", "Motor Noise\nSTD\n(ms)", "Time Keeper \nNoise STD\n(ms)" };

        // Initialize column labels
        for (int i = 0; i < columnNames.size(); ++i)
        {
            auto* label = new juce::Label();
            label->setText(columnNames[i], juce::dontSendNotification);
            label->setJustificationType(juce::Justification::centred);
            addAndMakeVisible(label);
            columnLabels.add(label);
        }

        // Initialize cells
        for (int row = 0; row < 4; ++row)
        {
            // Player number label
            auto* playerLabel = new juce::Label();
            playerLabel->setText(juce::String(row + 1), juce::dontSendNotification);
            playerLabel->setJustificationType(juce::Justification::centred);
            playerLabel->setFont(juce::Font(20.0f));
            addAndMakeVisible(playerLabel);
            playerLabels.add(playerLabel);

            // MIDI Channel ComboBox
            auto* comboBox = new juce::ComboBox();
            for (int i = 1; i <= 15; ++i)
                comboBox->addItem(juce::String(i), i);
            comboBox->setSelectedId(row + 1);
            addAndMakeVisible(comboBox);
            midiChannelCombos.add(comboBox);

            // Sliders for each cell
            for (int col = 2; col <= 5; ++col)
            {
                auto* slider = new juce::Slider();
                slider->setSliderStyle(juce::Slider::Rotary);

                switch (col)
                {
                case 2: slider->setRange(0.0, 1.0, 0.01); break;              // Volume
                case 3: 
                    slider->setRange(0.0, 200, 0.5);
                    slider->setColour(juce::Slider::thumbColourId, juce::Colours::seagreen);
                    break;              // Delay
                case 4:
                    slider->setRange(0.0, 10, 0.01);
                    slider->setColour(juce::Slider::thumbColourId, juce::Colours::seagreen);
                    break;              // Motor Noise STD
                case 5: 
                    slider->setRange(0.0, 50, 0.01);
                    slider->setColour(juce::Slider::thumbColourId, juce::Colours::seagreen);
                    break;              // Time Keeper Noise STD
                default: break;
                }

                slider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
                slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
                addAndMakeVisible(slider);
                sliders.add(slider);
            }
        }
    }

    ~PlayersGUI() override
    {
        columnLabels.clear(true);
        playerLabels.clear(true);
        midiChannelCombos.clear(true);
        sliders.clear(true);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black.brighter(0.12f)); // Background color
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        int cellHeight = area.getHeight() / 5; // 4 rows + header
        int columnWidth = area.getWidth() / 6;

        // Increase the size of the rotary sliders by 20%
        int sliderWidth = static_cast<int>(columnWidth * 1.2);  // 20% larger
        int sliderHeight = static_cast<int>(cellHeight * 0.75 * 1.2);  // 20% larger

        // Position column labels
        for (int i = 0; i < columnLabels.size(); ++i)
            columnLabels[i]->setBounds(i * columnWidth, 0, columnWidth, cellHeight);

        // Position player labels, combo boxes, and sliders
        for (int row = 0; row < 4; ++row)
        {
            int y = (row + 1) * cellHeight;

            // Player number label
            playerLabels[row]->setBounds(0, y, columnWidth, cellHeight);

            // MIDI Channel ComboBox
            midiChannelCombos[row]->setBounds(columnWidth + columnWidth / 8, y + cellHeight / 4, columnWidth * 3 / 4, cellHeight / 2);

            // Sliders
            for (int col = 2; col <= 5; ++col)
            {
                // Set bounds for each slider with the new increased size
                sliders[row * 4 + (col - 2)]->setBounds(col * columnWidth, y + cellHeight / 8, sliderWidth, sliderHeight);
            }
        }
    }

    void PlayersGUI::updatePlayerSetup(int numPlayers)
    {
        // Ensure the userPlayers array has enough space for 4 players
        if (userPlayers.size() < 4)
            userPlayers.resize(4);

        // Update which players are user players and deactivate controls accordingly
        for (int row = 0; row < 4; ++row)
        {
            bool isUserPlayer = row < numPlayers; // Enable rows up to the selected number of players
            userPlayers.getReference(row) = isUserPlayer;  // Track if the player is a user player

            // Deactivate player parameters if it is user player
            for (int col = 3; col <= 5; ++col)
            {
                sliders[row * 4 + (col - 2)]->setVisible(!isUserPlayer);
            }
        }
    }


    // Method to get parameters for a specific player
    PlayerParameters getPlayerParameters(int playerIndex) const
    {
        jassert(playerIndex >= 0 && playerIndex < 4); // Ensure the index is valid

        PlayerParameters params;

        params.id = playerIndex + 1; // Assign the player ID (1-based index)
        params.isUser = userPlayers[playerIndex];
        params.midiChannel = midiChannelCombos[playerIndex]->getSelectedId();
        params.volume = sliders[playerIndex * 4 + 0]->getValue();          // Volume slider
        params.delay = sliders[playerIndex * 4 + 1]->getValue();           // Delay slider
        params.motorNoiseSTD = sliders[playerIndex * 4 + 2]->getValue();   // Motor Noise STD slider
        params.timeKeeperNoiseSTD = sliders[playerIndex * 4 + 3]->getValue(); // Time Keeper Noise STD slider

        return params;
    }

private:
    juce::OwnedArray<juce::Label> columnLabels;
    juce::OwnedArray<juce::Label> playerLabels;
    juce::OwnedArray<juce::ComboBox> midiChannelCombos;
    juce::OwnedArray<juce::Slider> sliders;
    juce::Array<bool> userPlayers;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayersGUI)
};
