#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Player.h"

#define WINDOW_MARGIN 10


// This focus on the GUI of the Plugin
//==============================================================================
AdaptiveMetronomeAudioProcessorEditor::AdaptiveMetronomeAudioProcessorEditor(AdaptiveMetronomeAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Fix the size of the plugin GUI and make it unresizable
    setSize(1250, 650);
    setResizable(false, false);

    // Adding MIDI, Config, and Reset Buttons
    addAndMakeVisible(loadMidiBtn);
    loadMidiBtn.setButtonText("Load MIDI");
    loadMidiBtn.onClick = [this] {
        DBG("Load MIDI button has been pressed");
        };

    // Config and Reset should initially be set to disabled until a MIDI is loaded
    addAndMakeVisible(loadCongifBtn);
    loadCongifBtn.setButtonText("Load XML Config");
    loadCongifBtn.setEnabled(false);

    addAndMakeVisible(resetBtn);
    resetBtn.setButtonText("Reset");
    resetBtn.setEnabled(false);

    // Adding Combo Box for Number of Players and Label to Indicate what the box is
    addAndMakeVisible(noPlayerCB);
    noPlayerCB.addItem("0", 1);
    noPlayerCB.addItem("1", 2);
    noPlayerCB.addItem("2", 3);
    noPlayerCB.addItem("3", 4);
    noPlayerCB.addItem("4", 5);
    noPlayerCB.setSelectedItemIndex(1);
    noPlayerCB.onChange = [this]
        {
            int numPlayers = noPlayerCB.getSelectedItemIndex();
            playersSection.updatePlayerSetup(numPlayers);
            alphasAndBetas.updatePlayerSetup(numPlayers);
        };

    addAndMakeVisible(noPlayerLB);
    noPlayerLB.setText("Number of User Players", juce::dontSendNotification);
    noPlayerLB.setFont(juce::Font(25.0f));
    noPlayerLB.attachToComponent(&noPlayerCB, true);

    // Adding Section for the Players and Alphas/Betas
    addAndMakeVisible(playersSection);
    addAndMakeVisible(alphasAndBetas);

    // Adding Status Message
    addAndMakeVisible(statusLB);
    statusLB.setFont(juce::Font(25.0f));
    statusLB.setText("Status Text Here", juce::dontSendNotification);

    // Debug mode
#if JUCE_DEBUG
    // Add button for saving parameters to CSV only in 
    addAndMakeVisible(saveToCSVBtn);
    saveToCSVBtn.setButtonText("Save Player Params to CSV");
    saveToCSVBtn.onClick = [this] {
        savePlayerParametersToCSV();
        };

    // Add "Load Parameters" button 
    addAndMakeVisible(loadParamsBtn);
    loadParamsBtn.setButtonText("Load Parameters");
    loadParamsBtn.onClick = [this] {
        UpdateModel(); // Call the processor's UpdatePlayers() method
        DBG("Load Parameters button has been pressed. Players updated.");
        };
#endif

    
}

AdaptiveMetronomeAudioProcessorEditor::~AdaptiveMetronomeAudioProcessorEditor() = default;

//==============================================================================
void AdaptiveMetronomeAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black.brighter(0.1f));
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(36.0f));

    g.drawFittedText("Adaptive Metronome V2", getLocalBounds().reduced(WINDOW_MARGIN), juce::Justification::topLeft, 1);
}

void AdaptiveMetronomeAudioProcessorEditor::resized()
{
    int componentHeight = 50;
    int buttonWidth = 150;
    int gap = 10;

#pragma region Buttons
    // Calculate the positions for the buttons
    int loadMidiBtnX = getWidth() - buttonWidth - WINDOW_MARGIN;
    int loadConfigBtnX = loadMidiBtnX - buttonWidth - gap;
    int resetBtnX = loadConfigBtnX - buttonWidth - gap;
    int buttonY = getHeight() - componentHeight - WINDOW_MARGIN;

    // Set the bounds for each button
    loadMidiBtn.setBounds(loadMidiBtnX, buttonY, buttonWidth, componentHeight);
    loadCongifBtn.setBounds(loadConfigBtnX, buttonY, buttonWidth, componentHeight);
    resetBtn.setBounds(resetBtnX, buttonY, buttonWidth, componentHeight);
#pragma endregion Setting the Position of the Buttons (MIDI, Config, and Reset)

#pragma region ComboBox - Number of Players
    int comboBoxWidth = 100;
    int comboBoxHeight = 50;
    noPlayerCB.setBounds(resetBtnX - comboBoxWidth - gap, buttonY + (componentHeight - comboBoxHeight) / 2, comboBoxWidth, comboBoxHeight);
#pragma endregion Setting Position of ComboBox

#pragma region Player Parameters
    int playerSectionY = 60; // Start of the Player Parameters section
    int playerSectionWidth = getWidth() / 2 - WINDOW_MARGIN; // Half of the window width
    int playerSectionHeight = getHeight() - playerSectionY - componentHeight - gap; // Remaining height after buttons
    playersSection.setBounds(WINDOW_MARGIN, playerSectionY, playerSectionWidth, playerSectionHeight);
#pragma endregion Setting Position of Player Parameters

#pragma region AlphasAndBetas Section
    int alphasAndBetasSectionWidth = getWidth() / 2 - WINDOW_MARGIN; // Other half of the window width
    int alphasAndBetasSectionHeight = getHeight() - playerSectionY - componentHeight - gap;
    alphasAndBetas.setBounds(playerSectionWidth + WINDOW_MARGIN, playerSectionY, alphasAndBetasSectionWidth, alphasAndBetasSectionHeight);
#pragma endregion Setting Position of AlphasAndBetas Section

#pragma region Status Label
    int statusLabelWidth = 300;
    int statusLabelHeight = 30;
    statusLB.setBounds(getWidth() - statusLabelWidth - WINDOW_MARGIN, WINDOW_MARGIN, statusLabelWidth, statusLabelHeight);
    statusLB.setJustificationType(juce::Justification::centredRight); //Aligns the text on the right
#pragma endregion Setting Position of Status Label

#if JUCE_DEBUG
#pragma region Save to CSV and Load Paramaters Button
    int checkboxWidth = 100;
    int checkboxHeight = 30;
    saveToCSVBtn.setBounds(WINDOW_MARGIN, getHeight() - checkboxHeight - WINDOW_MARGIN, checkboxWidth, checkboxHeight);
    loadParamsBtn.setBounds(WINDOW_MARGIN + checkboxWidth + gap, getHeight() - checkboxHeight - WINDOW_MARGIN, checkboxWidth, checkboxHeight);
#pragma endregion Setting Position of Save to CSV and Load Parameters
#endif
}

void AdaptiveMetronomeAudioProcessorEditor::updateStatusLabel(const juce::String& message)
{
    statusLB.setText(message, juce::dontSendNotification);
}

PlayerStruct AdaptiveMetronomeAudioProcessorEditor::GetPlayerParameters(int playerIndex)
{
    PlayerStruct player;

    // Get the parameters from playersSection
    auto playerParams = playersSection.getPlayerParameters(playerIndex);
    player.isUser = playerParams.isUser;
    player.id = playerParams.id;
    player.midiChannel = playerParams.midiChannel;
    player.volume = playerParams.volume;
    player.delay = playerParams.delay;
    player.motorNoiseSTD = playerParams.motorNoiseSTD;
    player.timeKeeperNoiseSTD = playerParams.timeKeeperNoiseSTD;

    // Get the alpha and beta values from alphasAndBetas
    auto alphaAndBetaParams = alphasAndBetas.getPlayerParameters(playerIndex);
    for (int i = 0; i < 4; ++i)
    {
        player.alphas[i] = alphaAndBetaParams.alphas[i];
        player.betas[i] = alphaAndBetaParams.betas[i];
    }

    return player;
}

void AdaptiveMetronomeAudioProcessorEditor::savePlayerParametersToCSV()
{
    juce::File file("D:/player_parameters.csv");

    // Delete the file if it already exists (recreate it)
    if (file.existsAsFile())
    {
        file.deleteFile(); // Deletes the file
    }

    // Open the file for writing
    juce::FileOutputStream outputStream(file);
    if (outputStream.openedOk())
    {
        // Write headers to the CSV file
        outputStream << "Player ID, Is User, MIDI Channel,Volume,Delay,Motor Noise STD,Time Keeper Noise STD,Alpha 1,Alpha 2,Alpha 3,Alpha 4,Beta 1,Beta 2,Beta 3,Beta 4\n";

        // Iterate through players and write their parameters
        for (int i = 0; i < 4; ++i) // Number of players selected
        {
            PlayerStruct player = GetPlayerParameters(i);

            // Write player parameters to the CSV file
            outputStream << player.id << ","
                << player.isUser << ","
                << player.midiChannel << ","
                << player.volume << ","
                << player.delay << ","
                << player.motorNoiseSTD << ","
                << player.timeKeeperNoiseSTD << ","
                << player.alphas[0] << ","
                << player.betas[0] << ","
                << player.alphas[1] << ","
                << player.betas[1] << ","
                << player.alphas[2] << ","
                << player.betas[2] << ","
                << player.alphas[3] << ","
                << player.betas[3] << "\n";
        }

        DBG("Player parameters have been saved to player_parameters.csv");
    }
    else
    {
        DBG("Failed to open CSV file for writing");
    }

}

void AdaptiveMetronomeAudioProcessorEditor::UpdateModel()
{
    // Create a JUCE array to store Player objects
    juce::Array<Player> players;

    // Iterate over the number of players selected
    for (int i = 0; i < 4; ++i)
    {
        // Get player parameters from the GUI
        PlayerStruct playerParams = GetPlayerParameters(i);

        // Create Player object using the parameters
        Player player(
            playerParams.id,
            playerParams.isUser,
            playerParams.midiChannel,
            playerParams.volume,
            playerParams.delay,
            playerParams.motorNoiseSTD,
            playerParams.timeKeeperNoiseSTD,
            { playerParams.alphas[0], playerParams.alphas[1], playerParams.alphas[2], playerParams.alphas[3] },
            { playerParams.betas[0], playerParams.betas[1], playerParams.betas[2], playerParams.betas[3] }
        );

        players.add(player);
        audioProcessor.UpdatePlayers(players);
    }
    DBG("All players have been updated.");
}

