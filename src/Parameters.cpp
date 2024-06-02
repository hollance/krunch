#include <JuceHeader.h>
#include "Parameters.h"

template<typename T>
inline void castParameter(juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);  // parameter does not exist or wrong type
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + " dB";
}

static juce::String stringFromPercent(float value, int)
{
    return juce::String(int(value)) + " %";
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts_) : apvts(apvts_)
{
    castParameter(apvts, ParameterID::bypass, bypassParam);
    castParameter(apvts, ParameterID::oversample, oversampleParam);
    castParameter(apvts, ParameterID::krunch, krunchParam);
    castParameter(apvts, ParameterID::mix, mixParam);
    castParameter(apvts, ParameterID::outputLevel, outputLevelParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterID::bypass,
        "Bypass",
        false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterID::oversample,
        "Oversample",
        false));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::krunch,
        "Krunch",
        juce::NormalisableRange<float>(0.0f, 100.0f),
        50.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction(stringFromPercent)));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::mix,
        "Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f),
        100.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction(stringFromPercent)));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::outputLevel,
        "Output",
        juce::NormalisableRange<float>(-24.0f, 12.0f),
        0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction(stringFromDecibels)));

    return layout;
}

void Parameters::prepare(double sampleRate) noexcept
{
    double smoothTime = 0.02;
    krunchSmoother.reset(sampleRate, smoothTime);
    mixSmoother.reset(sampleRate, smoothTime);
    outputLevelSmoother.reset(sampleRate, smoothTime);
}

void Parameters::reset() noexcept
{
    krunchSmoother.setCurrentAndTargetValue(krunchParam->get() * 0.01f);
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);
    outputLevelSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(outputLevelParam->get()));
}

void Parameters::update() noexcept
{
    bypassed = bypassParam->get();
    oversample = oversampleParam->get();
    krunchSmoother.setTargetValue(krunchParam->get() * 0.01f);
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    outputLevelSmoother.setTargetValue(juce::Decibels::decibelsToGain(outputLevelParam->get()));
}

void Parameters::smoothen() noexcept
{
    krunch = 1.0f - krunchSmoother.getNextValue();
    mix = mixSmoother.getNextValue();
    outputLevel = outputLevelSmoother.getNextValue();
}
