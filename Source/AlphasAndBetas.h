#pragma once

#include <JuceHeader.h>
struct PlayerAlphaAndBeta
{
    double alphas[4]; // Array for alpha values
    double betas[4];  // Array for beta values
};

//==============================================================================
// AlphasAndBetas class definition
class AlphasAndBetas : public juce::Component
{
public:
    AlphasAndBetas()
    {
        // Column labels for "Alphas and Betas"
        const juce::StringArray columnNames = { "1", "2", "3", "4" };

        // Initialize column labels
        for (int i = 0; i < columnNames.size(); ++i)
        {
            auto* label = new juce::Label();
            label->setText("\n\n\n\nPlayer " + columnNames[i] + "\nAlpha       Beta", juce::dontSendNotification);
            label->setJustificationType(juce::Justification::centred);
            addAndMakeVisible(label);
            columnLabels.add(label);
        }

        // Initialize alpha and beta sliders
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                // Alpha slider
                auto* alphaSlider = new juce::Slider();
                alphaSlider->setSliderStyle(juce::Slider::Rotary);
                alphaSlider->setRange(0.0, 1.0, 0.01);
                alphaSlider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
                addAndMakeVisible(alphaSlider);
                alphaSliders.add(alphaSlider);

                // Beta slider
                auto* betaSlider = new juce::Slider();
                betaSlider->setSliderStyle(juce::Slider::Rotary);
                betaSlider->setRange(0.0, 1.0, 0.01);
                betaSlider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
                addAndMakeVisible(betaSlider);
                betaSliders.add(betaSlider);
            }
        }
    }

    ~AlphasAndBetas() override
    {
        columnLabels.clear(true);
        alphaSliders.clear(true);
        betaSliders.clear(true);
    }

    void paint(juce::Graphics& g) override
    {
        // Fill the background with a slightly lighter black color
        g.fillAll(juce::Colours::black.brighter(0.12f));

        // Draw the "Alphas and Betas" text at the top (overlapping the table)
        g.setFont(juce::Font(30.0f));
        g.setColour(juce::Colours::white);
        g.drawText("Alphas and Betas", getLocalBounds().removeFromTop(70), juce::Justification::centred);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10); // Padding around the table
        int cellHeight = area.getHeight() / 5;    // Four rows + header row
        int columnWidth = area.getWidth() / 4;    // Four columns

        // Set bounds for column labels (header row)
        for (int i = 0; i < columnLabels.size(); ++i)
        {
            columnLabels[i]->setBounds(
                i * columnWidth,                  // X position
                0,                                // Y position for header
                columnWidth,                      // Width of the column
                cellHeight                        // Height of the header
            );
        }

        // Set bounds for alpha and beta sliders in the cells
        int sliderWidth = 80;  // Width for each slider
        int sliderHeight = 80; // Height for each slider
        int sliderMargin = -10;

        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                // Calculate cell's top-left position
                int cellX = col * columnWidth;
                int cellY = (row + 1) * cellHeight; // +1 to skip the header row

                // Calculate positions for the sliders in the cell
                int centerX = cellX + columnWidth / 2; // Center of the cell horizontally
                int centerY = cellY + cellHeight / 2; // Center of the cell vertically

                // Alpha slider bounds (positioned slightly left of center)
                auto alphaBounds = juce::Rectangle<int>(
                    centerX - sliderWidth - sliderMargin,
                    centerY - sliderHeight / 2,
                    sliderWidth,
                    sliderHeight
                );

                // Beta slider bounds (positioned slightly right of center)
                auto betaBounds = juce::Rectangle<int>(
                    centerX + sliderMargin,
                    centerY - sliderHeight / 2,
                    sliderWidth,
                    sliderHeight
                );

                // Assign bounds to sliders
                int sliderIndex = row * 4 + col;
                alphaSliders[sliderIndex]->setBounds(alphaBounds);
                alphaSliders[sliderIndex]->setColour(juce::Slider::thumbColourId, juce::Colours::red);
                alphaSliders[sliderIndex]->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);

                betaSliders[sliderIndex]->setBounds(betaBounds);
                betaSliders[sliderIndex]->setColour(juce::Slider::thumbColourId, juce::Colours::orange);
                betaSliders[sliderIndex]->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
            }
        }
    }

    // Function to get alpha and beta values for a specific player
    PlayerAlphaAndBeta getPlayerParameters(int playerRow) const
    {
        PlayerAlphaAndBeta params = {};

        // Ensure the row is within valid range
        if (playerRow < 0 || playerRow >= 4)
            return params;

        for (int col = 0; col < 4; ++col)
        {
            int sliderIndex = playerRow * 4 + col;
            params.alphas[col] = alphaSliders[sliderIndex]->getValue();
            params.betas[col] = betaSliders[sliderIndex]->getValue();
        }

        return params;
    }

    // New method to update player setup and disable/enable sliders
    void updatePlayerSetup(int numPlayers)
    {
        // Ensure only the first 'numPlayers' rows are active, others are disabled
        for (int row = 0; row < 4; ++row)
        {
            bool isUserPlayer = row < numPlayers; // Enable rows up to the selected number of players

            // Disable/Enable alpha and beta sliders based on 'isUserPlayer'
            for (int col = 0; col < 4; ++col)
            {
                int sliderIndex = row * 4 + col;
                alphaSliders[sliderIndex]->setVisible(!isUserPlayer);
                betaSliders[sliderIndex]->setVisible(!isUserPlayer);
            }
        }
    }

private:
    juce::OwnedArray<juce::Label> columnLabels;     // Column labels
    juce::OwnedArray<juce::Slider> alphaSliders;    // Alpha sliders (one per cell)
    juce::OwnedArray<juce::Slider> betaSliders;     // Beta sliders (one per cell)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AlphasAndBetas)
};
