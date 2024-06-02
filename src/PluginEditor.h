#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel.h"

class AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    AudioProcessorEditor(AudioProcessor&);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AudioProcessor& audioProcessor;

    LookAndFeel lookAndFeel;

    juce::Slider krunchSlider;
    juce::Slider mixSlider;
    juce::Slider outputLevelSlider;
    juce::ImageButton oversampleButton;
    juce::ImageButton bypassButton;

    juce::AudioProcessorValueTreeState::SliderAttachment krunchAttachment {
        audioProcessor.apvts, ParameterID::krunch.getParamID(), krunchSlider
    };
    juce::AudioProcessorValueTreeState::SliderAttachment mixAttachment {
        audioProcessor.apvts, ParameterID::mix.getParamID(), mixSlider
    };
    juce::AudioProcessorValueTreeState::SliderAttachment outputLevelAttachment {
        audioProcessor.apvts, ParameterID::outputLevel.getParamID(), outputLevelSlider
    };
    juce::AudioProcessorValueTreeState::ButtonAttachment oversampleAttachment {
        audioProcessor.apvts, ParameterID::oversample.getParamID(), oversampleButton
    };
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment {
        audioProcessor.apvts, ParameterID::bypass.getParamID(), bypassButton
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioProcessorEditor)
};
