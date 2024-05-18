#pragma once

namespace ParameterID
{
    #define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);

    PARAMETER_ID(bypass)
    PARAMETER_ID(quality)
    PARAMETER_ID(krunch)
    PARAMETER_ID(mix)
    PARAMETER_ID(outputLevel)

    #undef PARAMETER_ID
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
    bool quality = false;
    float krunch = 0.5f;
    float mix = 1.0f;
    float outputLevel = 1.0f;

    juce::AudioParameterBool* bypassParam;
    juce::AudioParameterBool* qualityParam;
    juce::AudioParameterFloat* krunchParam;
    juce::AudioParameterFloat* mixParam;
    juce::AudioParameterFloat* outputLevelParam;

private:
    juce::AudioProcessorValueTreeState& apvts;

    juce::LinearSmoothedValue<float> krunchSmoother;
    juce::LinearSmoothedValue<float> mixSmoother;
    juce::LinearSmoothedValue<float> outputLevelSmoother;
};
