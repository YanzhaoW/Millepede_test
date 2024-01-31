#include "Detector.hpp"
#include <TRandom3.h>
#include <algorithm>

Detector::Detector(TRandom3* rnd, int num_of_bars)
    : num_of_bars_{ num_of_bars }
    , rnd_{ rnd }
{
    offsets_.resize(num_of_bars_);

    if (rnd == nullptr)
    {
        throw std::logic_error("Random generator is nullptr!");
    }

    if (num_of_bars % 2 != 0)
    {
        throw std::logic_error("Number of bars in the detector must be even!");
    }
}
void Detector::set_offsets(const std::vector<float>& offsets)
{
    if (offsets.size() != num_of_bars_)
    {
        throw std::logic_error("Cannot set the offsets with different size!");
    }
    offsets_ = offsets;
}

void Detector::generate_signals(Line line, std::vector<Signal>& output_signals) const
{
    output_signals.clear();
    for (const auto offset : offsets_)
    {
        auto current_size = output_signals.size();
        auto pos_x = -(static_cast<float>(num_of_bars_) / 2 - static_cast<float>(current_size) - 0.5) * bar_width_;
        auto pos_y = offset - line.offset + pos_x * line.slope;
        output_signals.emplace_back(current_size + 1, pos_x, pos_y + rnd_->Gaus(0, error_y_), error_y_);
    }
    auto trail = std::ranges::partition(output_signals,
                                        [bar_length = bar_length_](const auto& signal)
                                        { return signal.pos_y < bar_length / 2. && signal.pos_y > -bar_length / 2.; });
    output_signals.erase(trail.begin(), trail.end());
}
