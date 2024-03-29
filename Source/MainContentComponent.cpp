#include <JuceHeader.h>
#include "MainContentComponent.h"
#include "DecibelConfig.h"

MainContentComponent::MainContentComponent() {
    setSize(600, 200);
    setAudioChannels(0, 2);

    addSliders();
    synchronizeSliders();
    initializeSliderValues();
}

void MainContentComponent::initializeSliderValues() {
    channel0DecibelSlider.setValue (DecibelConfig::minusInfinityDb, sendNotification);
    channel1DecibelSlider.setValue (DecibelConfig::minusInfinityDb, sendNotification);
}

void MainContentComponent::synchronizeSliders() {
    channel0DecibelSlider.onValueChange = [this] {
        channel0Level = Decibels::decibelsToGain ((float) channel0DecibelSlider.getValue(), DecibelConfig::minusInfinityDb);
        channel0GainSlider.setValue(channel0Level, dontSendNotification);
    };

    channel0GainSlider.onValueChange = [this] {
        channel0Level = (float) channel0GainSlider.getValue();
        channel0DecibelSlider.setValue(Decibels::gainToDecibels(channel0Level, DecibelConfig::minusInfinityDb), dontSendNotification);
    };

    channel1DecibelSlider.onValueChange = [this] {
        channel1Level = Decibels::decibelsToGain ((float) channel1DecibelSlider.getValue(), DecibelConfig::minusInfinityDb);
        channel1GainSlider.setValue(channel1Level, dontSendNotification);
    };

    channel1GainSlider.onValueChange = [this] {
        channel1Level = (float) channel1GainSlider.getValue();
        channel1DecibelSlider.setValue(Decibels::gainToDecibels(channel1Level, DecibelConfig::minusInfinityDb), dontSendNotification);
    };
}

MainContentComponent::~MainContentComponent() {
    shutdownAudio();
}

void MainContentComponent::addSliders() {
    addGainSlider("Channel 0 Gain", channel0GainSlider, channel0GainLabel);
    addGainSlider("Channel 1 Gain", channel1GainSlider, channel1GainLabel);

    addDecibelSlider("Channel 0 dB", channel0DecibelSlider, channel0DecibelLabel);
    addDecibelSlider("Channel 1 dB", channel1DecibelSlider, channel1DecibelLabel);
}

void MainContentComponent::addGainSlider(const String &labelText, Slider &slider, Label &label) {
    slider.setRange(0.0, 0.25);
    addSlider(labelText, slider, label);
}

void MainContentComponent::addDecibelSlider(const String &labelText, DecibelSlider &slider, Label &label) {
    slider.setRange(DecibelConfig::minusInfinityDb, -12);
    addSlider(labelText, slider, label);
}

void MainContentComponent::addSlider(const String &labelText, Slider &slider, Label &label) {
    slider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
    label.setText(labelText, dontSendNotification);

    addAndMakeVisible(slider);
    addAndMakeVisible(label);
}

void MainContentComponent::prepareToPlay(int, double) {}

void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) {
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float levelScale = 0;

        float level;
        if (channel == 0) {
            level = channel0Level;
            levelScale = channel0Level * 2;
        }
        if (channel == 1) {
            level = channel1Level;
            levelScale = channel1Level * 2;
        }

        auto *buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {
            buffer[sample] = random.nextFloat() * levelScale - level;
        }
    }
}

void MainContentComponent::releaseResources() {}

void MainContentComponent::resized() {
    channel0GainLabel.setBounds(10, 10, 90, 20);
    channel0GainSlider.setBounds(100, 10, getWidth() - 110, 20);
    channel0DecibelLabel.setBounds(10, 40, 90, 20);
    channel0DecibelSlider.setBounds(100, 40, getWidth() - 110, 20);

    channel1GainLabel.setBounds(10, 100, 90, 20);
    channel1GainSlider.setBounds(100, 100, getWidth() - 110, 20);
    channel1DecibelLabel.setBounds(10, 130, 90, 20);
    channel1DecibelSlider.setBounds(100, 130, getWidth() - 110, 20);
}
