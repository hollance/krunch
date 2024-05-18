#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessorEditor::AudioProcessorEditor(AudioProcessor& p) :
    juce::AudioProcessorEditor(&p),
    audioProcessor(p)
{
    setOpaque(true);
    setSize(400, 400);
}

AudioProcessorEditor::~AudioProcessorEditor()
{
}

void AudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void AudioProcessorEditor::resized()
{
}
