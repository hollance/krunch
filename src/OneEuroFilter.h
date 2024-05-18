#pragma once

#include <cmath>

class OneEuroFilter
{
public:
    void prepare(double newSampleRate) noexcept
    {
        sampleRate = newSampleRate;
        dalpha = smoothingFactor(1.0);
    }

    void reset() noexcept
    {
        y = 0.0;
        z = 0.0;
        dy = 0.0;
    }

    void setKrunch(double krunch) noexcept
    {
        const double kk = krunch * krunch;
        double skewed = kk * kk;
        beta = 1.0 + 20000.0 * skewed;
    }

    double operator()(double x) noexcept
    {
        // Calculate derivative
        double dx = x - z;

        // Smoothen derivative very slowly
        dy += dalpha * (dx * 40000.0 - dy);

        // Calculate adaptive cutoff frequency
        double cutoff = 1.0 + beta * std::abs(dy);

        // Filter the input signal using the new cutoff
        double alpha = smoothingFactor(cutoff);
        y += alpha * (x - y);
        z += alpha * (y - z);
        return z;
    }

    float operator()(float sample) noexcept
    {
        return float((*this)(double(sample)));
    }

private:
    double smoothingFactor(double cutoff) const noexcept
    {
        // This is not the typical formula for calculating the coefficient
        // of a one-pole lowpass filter. It comes from the 1€ Filter paper
        // and allows the cutoff to go over Nyquist.
        double r = 6.28318530717958647692528676655900577 * cutoff;
        return r / (r + sampleRate);
    }

    double sampleRate = 44100.0;
    double beta = 0.0;    // krunch amount
    double dalpha = 0.0;  // filter coefficient for derivative
    double y = 0.0;       // first filter
    double z = 0.0;       // second filter
    double dy = 0.0;      // filtered derivative
};
