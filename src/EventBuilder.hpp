#pragma once

#include "Detector.hpp"
#include <Mille.hpp>
#include <PedeResReader.hpp>
#include <TRandom3.h>
#include <concepts>
#include <string>
#include <vector>

template <typename UnaryOp>
concept DataPointConsumer = requires(UnaryOp optn) { optn(DataPoint{}); };

class EventBuilder
{
  public:
    using json = nlohmann::json;
    EventBuilder() = default;
    void build_event_data(DataPointConsumer auto UnaryOp);
    void set_log_file(std::string_view filename);
    void set_offset_range(float val) { offset_range_ = val; }
    void set_error_y(float val) { detector_.set_err_y(val); }
    void set_init_offset(float val) { init_offset_ = val; }
    void set_init_scale(float val) { init_scale_ = val; }
    void init();

    [[nodiscard]] auto get_event_data() const -> const auto& { return event_data_; }
    [[nodiscard]] auto get_detector() const -> const auto& { return detector_; }
    auto get_rnd() -> TRandom3* { return &rnd_; }

  private:
    float init_offset_ = 0;
    float init_scale_ = 1;

    float offset_range_ = 0.1;
    float scale_range_ = 1.;

    TRandom3 rnd_{ 0 };
    std::string offset_filename = "offsets.json";
    Detector detector_{ &rnd_ };
    std::vector<Signal> output_signals_;
    DataPoint event_data_;
    std::ofstream event_log_file_;
    ResReader result_reader_;
    json event_log_;

    void reset();
    auto generate_offset_file(std::string_view filename) -> std::vector<std::pair<float, float>>;
    static auto signal_filter(const std::vector<Signal>& signals) -> bool;
};

void EventBuilder::build_event_data(DataPointConsumer auto UnaryOp)
{
    constexpr auto SLOPE_RANGE = 5;
    constexpr auto OFFSET_RANGE = 10;

    auto line = Line{ static_cast<float>(rnd_.Uniform(-SLOPE_RANGE, SLOPE_RANGE)),
                      static_cast<float>(rnd_.Uniform(-OFFSET_RANGE, OFFSET_RANGE)) };

    detector_.generate_signals(line, output_signals_);
    if (signal_filter(output_signals_))
    {
        return;
    }

    event_log_ = json::array();
    const auto& pars = result_reader_.get_pars();
    for (const auto& signal : output_signals_)
    {
        reset();

        auto scale = pars.empty() ? init_scale_ : pars.at(signal.bar_num + DEFAULT_BAR_NUM).value;
        // auto scale_times_offset = pars.empty() ? init_offset_ * scale : pars.at(signal.bar_num).value;
        auto offset = pars.empty() ? init_offset_ : pars.at(signal.bar_num).value;

        event_data_.locals.emplace_back(signal.pos_x * scale);
        event_data_.locals.emplace_back(scale);
        event_data_.globals.emplace_back(signal.bar_num, scale);
        event_data_.globals.emplace_back(signal.bar_num + DEFAULT_BAR_NUM, offset - 1);
        event_data_.measurement = signal.pos_y;
        event_data_.sigma = signal.err_y;
        UnaryOp(event_data_);

        if (event_log_file_.is_open())
        {
            event_log_.emplace_back(json{ signal });
        }
    }
    if (event_log_file_.is_open())
    {
        event_log_file_ << event_log_.dump(2) << ",\n";
    }
}
