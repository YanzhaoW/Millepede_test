#include "EventBuilder.hpp"
#include <filesystem>
#include <fmt/format.h>
#include <vector>

void EventBuilder::reset()
{
    event_data_.locals.clear();
    event_data_.globals.clear();
    event_data_.measurement = 0.F;
    event_data_.sigma = 0.F;
}
void EventBuilder::set_log_file(std::string_view filename)
{
    event_log_file_.open(filename, std::ios_base::out | std::ios_base::trunc);
}

void EventBuilder::init()
{
    auto bar_size = detector_.get_bar_size();
    result_reader_.read_file();
    output_signals_.reserve(bar_size);
    auto input_file = std::ifstream(offset_filename, std::ios_base::in);
    if (input_file.is_open())
    {
        auto offsets = std::vector<std::pair<float, float>>{};
        json data = json::parse(input_file);
        data.get_to(offsets);
        if (offsets.size() == bar_size)
        {
            detector_.set_offsets(offsets);
            return;
        }
    }
    detector_.set_offsets(generate_offset_file(offset_filename));
}

auto EventBuilder::generate_offset_file(std::string_view filename) -> std::vector<std::pair<float, float>>
{
    auto output_file = std::ofstream{ filename, std::ios_base::trunc | std::ios_base::out };
    auto bar_size = detector_.get_bar_size();
    auto offsets = std::vector<std::pair<float, float>>{};
    offsets.resize(bar_size);

    for (auto& [scale, offset] : offsets)
    {
        scale = static_cast<float>(rnd_.Uniform(scale_range_ - 0.01, scale_range_));
        offset = static_cast<float>(rnd_.Uniform(-offset_range_, offset_range_));
    }
    offsets.front().second = 0.F;
    offsets.back().second = 0.F;
    auto json_vec = json(offsets);
    output_file << json_vec.dump(1) << "\n";
    return offsets;
}

auto EventBuilder::signal_filter(const std::vector<Signal>& signals) -> bool { return signals.size() < 25; }
