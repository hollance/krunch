#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessor::AudioProcessor() :
    juce::AudioProcessor(
        BusesProperties()
            .withInput("Input", juce::AudioChannelSet::stereo(), true)
            .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ),
    apvts(*this, nullptr, "Parameters", Parameters::createParameterLayout()),
    params(apvts)
{
    // do nothing
}

void AudioProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    oversampler.prepareToPlay(newSampleRate, samplesPerBlock);
    setLatencySamples(oversampler.getLatencyInSamples());

    float sampleRate = float(oversampler.getOversampledRate());

    params.prepare(sampleRate);
    filterL.prepare(sampleRate);
    filterR.prepare(sampleRate);

    bypassCoeff = 1.0f - std::exp(-1.0f / (sampleRate * 0.01f));

    reset();
}

void AudioProcessor::releaseResources()
{
    // do nothing
}

void AudioProcessor::reset()
{
    oversampler.reset();

    params.reset();
    filterL.reset();
    filterR.reset();

    bypassFade = 1.0f;
    bypassTarget = 1.0f;
}

bool AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void AudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto numInputChannels = getTotalNumInputChannels();
    auto numOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    // Clear any output channels that don't contain input data.
    for (auto i = numInputChannels; i < numOutputChannels; ++i) {
        buffer.clear(i, 0, numSamples);
    }

    params.update();

    bypassTarget = params.bypassed ? 0.0f : 1.0f;

    juce::dsp::AudioBlock<float> renderBlock = oversampler.processSamplesUp(buffer);

    float* channelL = renderBlock.getChannelPointer(0);
    float* channelR = renderBlock.getChannelPointer(1);

    for (size_t sample = 0; sample < renderBlock.getNumSamples(); ++sample) {
        params.smoothen();

        filterL.setKrunch(params.krunch);
        filterR.setKrunch(params.krunch);

        float dryL = channelL[sample];
        float dryR = channelR[sample];

        float wetL = filterL(dryL);
        float wetR = filterR(dryR);

        float mixL = params.mix * wetL + (1.0f - params.mix) * dryL;
        float mixR = params.mix * wetR + (1.0f - params.mix) * dryR;

        float outL = mixL * params.outputLevel;
        float outR = mixR * params.outputLevel;

        // Crossfade for bypass
        bypassFade += bypassCoeff * (bypassTarget - bypassFade);
        outL = bypassFade * outL + (1.0f - bypassFade) * dryL;
        outR = bypassFade * outR + (1.0f - bypassFade) * dryR;

        channelL[sample] = outL;
        channelR[sample] = outR;
    }

    oversampler.processSamplesDown(buffer);
}

void AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

juce::AudioProcessorEditor* AudioProcessor::createEditor()
{
    return new AudioProcessorEditor(*this);
}

juce::AudioProcessorParameter* AudioProcessor::getBypassParameter() const
{
    return params.bypassParam;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioProcessor();
}
