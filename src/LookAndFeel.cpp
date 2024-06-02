#include "LookAndFeel.h"

LookAndFeel::LookAndFeel()
{
    // Color for the text inside the popup
    setColour(juce::TooltipWindow::textColourId, juce::Colours::lightgrey);

    // Colors for the popup bubble
    setColour(juce::BubbleComponent::backgroundColourId, juce::Colours::black);
    setColour(juce::BubbleComponent::outlineColourId, juce::Colours::lightgrey);

    auto dropShadow = juce::DropShadow(juce::Colours::black.withAlpha(0.5f), 2, {0, 1});
    bubbleShadow.setShadowProperties(dropShadow);
}

juce::Slider::SliderLayout LookAndFeel::getSliderLayout(juce::Slider& slider)
{
    juce::Slider::SliderLayout layout;
    layout.sliderBounds = slider.getLocalBounds();
    layout.textBoxBounds = layout.sliderBounds;
    return layout;
}

void LookAndFeel::drawLinearSlider(
    juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPos,
    [[maybe_unused]] float minSliderPos,
    [[maybe_unused]] float maxSliderPos,
    [[maybe_unused]] const juce::Slider::SliderStyle style,
    [[maybe_unused]] juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
    auto rect = bounds.withWidth(std::ceil(sliderPos - bounds.getX()));
    auto cornerRadius = 3.0f;

    auto gradient = juce::ColourGradient(
        {0, 112, 255}, bounds.getX(), 0.0f,
        {0, 255, 186}, bounds.getRight(), 0.0f, false);

    juce::Path path;
    path.addRoundedRectangle(bounds, cornerRadius);

    g.saveState();
    g.reduceClipRegion(path);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(rect, cornerRadius);
    g.restoreState();
}

void LookAndFeel::drawRotarySlider(
     juce::Graphics& g,
     int x,
     int y,
     int width,
     [[maybe_unused]] int height,
     float sliderPos,
     float rotaryStartAngle,
     float rotaryEndAngle,
     [[maybe_unused]] juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int>(x, y, width, width).toFloat();
    auto lineWidth = 22.0f;
    auto arcRadius = (bounds.getWidth() - lineWidth) / 2.0f;

    auto gradient = juce::ColourGradient(
        {255, 0, 110}, bounds.getX(), 0.0f,
        {255, 220, 0}, bounds.getRight(), 0.0f, false);

    juce::Path arc;
    arc.addCentredArc(
        bounds.getCentreX(),
        bounds.getCentreY(),
        arcRadius,
        arcRadius,
        0.0f,
        rotaryStartAngle,
        rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle),
        true);

    g.setGradientFill(gradient);
    g.strokePath(arc, juce::PathStrokeType(lineWidth));
}

juce::Font LookAndFeel::getSliderPopupFont(juce::Slider&)
{
    return juce::Font(11.0f);
}

int LookAndFeel::getSliderPopupPlacement(juce::Slider& slider)
{
    if (slider.isRotary()) {
        return juce::BubbleComponent::above;
    } else {
        return juce::BubbleComponent::below;
    }
}

void LookAndFeel::drawBubble(
    juce::Graphics& g,
    juce::BubbleComponent& comp,
    const juce::Point<float>& tip,
    const juce::Rectangle<float>& body)
{
    // Draw tip above or below the bubble?
    float adjustedY = tip.y + ((tip.y > body.getBottom()) ? -6.0f : 6.0f);

    juce::Path path;
    path.addBubble(
        body.reduced(0.5f),
        body.getUnion(juce::Rectangle<float>(tip.x, tip.y, 1.0f, 1.0f)),
        {tip.x, adjustedY},
        2.0f,
        juce::jmin(15.0f, body.getWidth() * 0.2f, body.getHeight() * 0.2f));

    g.setColour(comp.findColour(juce::BubbleComponent::backgroundColourId));
    g.fillPath(path);

    g.setColour(comp.findColour(juce::BubbleComponent::outlineColourId));
    g.strokePath(path, juce::PathStrokeType(1.0f));
}

void LookAndFeel::setComponentEffectForBubbleComponent(juce::BubbleComponent& bubbleComponent)
{
    bubbleComponent.setComponentEffect(&bubbleShadow);
}
