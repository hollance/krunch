#pragma once

namespace ParameterID
{
    const juce::ParameterID bypass("bypass", 1);
    const juce::ParameterID oversample("oversample", 1);
    const juce::ParameterID krunch("krunch", 1);
    const juce::ParameterID mix("mix", 1);
    const juce::ParameterID outputLevel("outputLevel", 1);
}

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void prepare(double sampleRate) noexcept;
    void reset() noexcept;
    void update() noexcept;
    void smoothen() noexcept;

    bool bypassed = false;
    bool oversample = false;
    float krunch = 0.5f;
    float mix = 0.5f;
    float outputLevel = 1.0f;

    juce::AudioParameterBool* bypassParam;
    juce::AudioParameterBool* oversampleParam;
    juce::AudioParameterFloat* krunchParam;
    juce::AudioParameterFloat* mixParam;
    juce::AudioParameterFloat* outputLevelParam;

private:
    juce::AudioProcessorValueTreeState& apvts;

    juce::LinearSmoothedValue<float> krunchSmoother;
    juce::LinearSmoothedValue<float> mixSmoother;
    juce::LinearSmoothedValue<float> outputLevelSmoother;
};
