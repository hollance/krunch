#pragma once

#include <JuceHeader.h>

class Oversampler
{
public:
    void prepareToPlay(double newSampleRate, int samplesPerBlock) noexcept
    {
        size_t factor = 0;
        if (newSampleRate <= 48000.0) {  // 8x
            factor = 3;
        } else if (newSampleRate <= 96000.0) {  // 4x
            factor = 2;
        } else if (newSampleRate <= 192000.0) {  // 2x
            factor = 1;
        }

        originalSampleRate = newSampleRate;
        sampleRate = newSampleRate * double(1 << factor);

        oversampling.reset(new juce::dsp::Oversampling<float>(
            2, factor,
            juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR,
            true, true));

        oversampling->initProcessing(size_t(samplesPerBlock));

        //DBG("oversampling factor: " << oversampling->getOversamplingFactor() <<
        //    ", sampleRate: " << sampleRate <<
        //    ", latency: " << oversampling->getLatencyInSamples());
    }

    void reset() noexcept
    {
        oversampling->reset();
    }

    int getLatencyInSamples() const noexcept
    {
        return int(oversampling->getLatencyInSamples());
    }

    juce::dsp::AudioBlock<float> processSamplesUp(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> inputBlock(buffer);
        return oversampling->processSamplesUp(inputBlock);
    }

    void processSamplesDown(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> outputBlock(buffer);
        oversampling->processSamplesDown(outputBlock);
    }

    double getOversampledRate() const noexcept
    {
        return sampleRate;
    }

    double getOriginalSampleRate() const noexcept
    {
        return originalSampleRate;
    }

private:
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling = nullptr;
    double sampleRate;
    double originalSampleRate;
};
