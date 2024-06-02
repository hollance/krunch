#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessorEditor::AudioProcessorEditor(AudioProcessor& p) :
    juce::AudioProcessorEditor(&p),
    audioProcessor(p)
{
    float pi = juce::MathConstants<float>::pi;
    krunchSlider.setRotaryParameters(255.0f * pi / 180.0f, 465.0f * pi / 180.0f, true);

    krunchSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    krunchSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    krunchSlider.setPopupDisplayEnabled(true, true, this, -1);
    krunchSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(krunchSlider);

    mixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mixSlider.setPopupDisplayEnabled(true, true, this, -1);
    mixSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(mixSlider);

    outputLevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    outputLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outputLevelSlider.setPopupDisplayEnabled(true, true, this, -1);
    outputLevelSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(outputLevelSlider);

    auto oversampleOnIcon = juce::ImageCache::getFromMemory(
        BinaryData::Oversample_On_png, BinaryData::Oversample_On_pngSize);
    auto oversampleOffIcon = juce::ImageCache::getFromMemory(
        BinaryData::Oversample_Off_png, BinaryData::Oversample_Off_pngSize);

    oversampleButton.setClickingTogglesState(true);
    oversampleButton.setImages(
        false, true, true,
        oversampleOffIcon, 1.0f, juce::Colours::transparentBlack,
        juce::Image(), 1.0f, juce::Colours::transparentBlack,
        oversampleOnIcon, 1.0f, juce::Colours::transparentBlack,
        0.0f);
    addAndMakeVisible(oversampleButton);

    auto bypassOnIcon = juce::ImageCache::getFromMemory(
        BinaryData::Bypass_On_png, BinaryData::Bypass_On_pngSize);
    auto bypassOffIcon = juce::ImageCache::getFromMemory(
        BinaryData::Bypass_Off_png, BinaryData::Bypass_Off_pngSize);

    bypassButton.setClickingTogglesState(true);
    bypassButton.setImages(
        false, true, true,
        bypassOffIcon, 1.0f, juce::Colours::transparentBlack,
        juce::Image(), 1.0f, juce::Colours::transparentBlack,
        bypassOnIcon, 1.0f, juce::Colours::transparentBlack,
        0.0f);
    addAndMakeVisible(bypassButton);

    setOpaque(true);
    setSize(320, 200);
}

void AudioProcessorEditor::paint(juce::Graphics& g)
{
    auto image = juce::ImageCache::getFromMemory(BinaryData::Background_png, BinaryData::Background_pngSize);
    g.drawImage(image, 0, 0, 320, 200, 0, 0, 640, 400);
}

void AudioProcessorEditor::resized()
{
    mixSlider.setBounds(10, 28, 90, 10);
    outputLevelSlider.setBounds(220, 28, 90, 10);
    krunchSlider.setBounds(35, 50, 250, 250);
    oversampleButton.setBounds(119, 175, 31, 16);
    bypassButton.setBounds(170, 175, 31, 16);
}
