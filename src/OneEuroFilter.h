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

    void setKrunch(double newKrunch) noexcept
    {
        krunch = newKrunch;
    }

    double operator()(double x) noexcept
    {
        double dx = (x - z) * 44100.0;
        dy += dalpha * (dx - dy);

// TODO: get dy working on a different scale so we don't need 44100 + 20000 factors

        // This is exactly krunch^5 but I like krunch^4 better
        //double skewed = (krunch > 0.0) ? std::exp(std::log(krunch) / 0.2) : 0.0;

        const double kk = krunch * krunch;
        double skewed = kk * kk;
        double beta = 1.0 + 20000.0 * skewed;
        double cutoff = 1.0 + beta * std::abs(dy);

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
        double r = 2.0 * M_PI * cutoff;
        return r / (r + sampleRate);

//        cutoff = std::min(cutoff, sampleRate * 0.5 - 1.0);
//        return 1.0 - std::exp(-2.0 * M_PI * cutoff / sampleRate);
    }

    double sampleRate = 44100.0;
    double krunch = 0.0;
    double dalpha = 0.0;
    double y = 0.0;
    double z = 0.0;
    double dy = 0.0;
};
