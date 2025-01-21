#pragma once

#include <array>

struct PlayerStruct {
    int id;
    bool isUser;
    int midiChannel;
    float volume;
    float delay;
    float motorNoiseSTD;
    float timeKeeperNoiseSTD;
    double alphas[4];
    double betas[4];
};

// Player class definition
class Player
{
public:
    // Constructor
    Player(int id, bool isUser, int midiChannel, float volume, float delay, float motorNoiseSTD, float timeKeeperNoiseSTD,
        const std::array<double, 4>& alphas, const std::array<double, 4>& betas)
        : id(id), midiChannel(midiChannel), volume(volume), delay(delay),
        motorNoiseSTD(motorNoiseSTD), timeKeeperNoiseSTD(timeKeeperNoiseSTD),
        alphas(alphas), betas(betas) {}

    // Default constructor
    Player() : id(0), isUser(false), midiChannel(0), volume(0.0f), delay(0.0f), motorNoiseSTD(0.0f), timeKeeperNoiseSTD(0.0f) {}

    // Getters
    int getId() const { return id; }
    bool getIsUser() const { return isUser; }
    int getMidiChannel() const { return midiChannel; }
    float getVolume() const { return volume; }
    float getDelay() const { return delay; }
    float getMotorNoiseSTD() const { return motorNoiseSTD; }
    float getTimeKeeperNoiseSTD() const { return timeKeeperNoiseSTD; }
    const std::array<double, 4>& getAlphas() const { return alphas; }
    const std::array<double, 4>& getBetas() const { return betas; }

    // Setters
    void setId(int newId) { id = newId; }
    void setIsUser(bool newUser) { isUser = newUser; }
    void setMidiChannel(int newMidiChannel) { midiChannel = newMidiChannel; }
    void setVolume(float newVolume) { volume = newVolume; }
    void setDelay(float newDelay) { delay = newDelay; }
    void setMotorNoiseSTD(float newMotorNoiseSTD) { motorNoiseSTD = newMotorNoiseSTD; }
    void setTimeKeeperNoiseSTD(float newTimeKeeperNoiseSTD) { timeKeeperNoiseSTD = newTimeKeeperNoiseSTD; }
    void setAlphas(const std::array<double, 4>& newAlphas) { alphas = newAlphas; }
    void setBetas(const std::array<double, 4>& newBetas) { betas = newBetas; }

private:
    int id;
    bool isUser;
    int midiChannel;
    float volume;
    float delay;
    float motorNoiseSTD;
    float timeKeeperNoiseSTD;
    std::array<double, 4> alphas;
    std::array<double, 4> betas;
};