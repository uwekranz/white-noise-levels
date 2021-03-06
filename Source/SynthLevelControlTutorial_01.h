/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2017 - ROLI Ltd.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             SynthLevelControlTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Synthesiser with level control.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
class MainContentComponent   : public AudioAppComponent
{
public:
    MainContentComponent()
    {
        levelSlider.setRange (0.0, 0.25);
        levelSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
        levelLabel.setText ("Noise Level", dontSendNotification);

        addAndMakeVisible (levelSlider);
        addAndMakeVisible (levelLabel);

        levelSlider1.setRange(0.0, 0.25);
        levelSlider1.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
        levelLabel1.setText("Noise Level 1", dontSendNotification);

        addAndMakeVisible(levelSlider1);
        addAndMakeVisible(levelLabel1);

        setSize (600, 100);
        setAudioChannels (0, 2);
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay (int, double) override {}

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            auto level = 0.0;
            auto levelScale = 0.0;

            if (channel == 0)
            {
                level = (float)levelSlider.getValue();
                levelScale = level * 2.0f;
            }
            if (channel == 1)
            {
                level = (float)levelSlider1.getValue();
                levelScale = level * 2.0f;
            }

            auto* buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);

            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                auto noise = random.nextFloat() - 1.0f;
                buffer[sample] = noise * levelScale;
            }
        }
    }

    void releaseResources() override {}

    void resized() override
    {
        levelLabel .setBounds (10, 10, 90, 20);
        levelSlider.setBounds (100, 10, getWidth() - 110, 20);

        levelLabel1.setBounds(10, 40, 90, 20);
        levelSlider1.setBounds(100, 40, getWidth() - 110, 20);
    }

private:
    Random random;
    Slider levelSlider;
    Label  levelLabel;

    Slider levelSlider1;
    Label  levelLabel1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
