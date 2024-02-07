#pragma once
#define JSON_USE_IMPLICIT_CONVERSIONS 0

#include <nlohmann/json.hpp>
#include <vector>

constexpr auto DEFAULT_BAR_NUM = 50;

class TRandom3;
struct Line
{
    float slope = 1.;
    float offset = 0.;
};

struct Signal
{
    int bar_num = 0;
    float pos_x = 0.;
    float pos_y = 0.;
    float err_y = 0.;
};

class Detector
{
  public:
    explicit Detector(TRandom3* rnd, int num_of_bars = DEFAULT_BAR_NUM);
    void generate_signals(Line line, std::vector<Signal>& output_signals) const;

    void set_offsets(const std::vector<std::pair<float, float>>& offsets);
    void set_err_y(float val) { error_y_ = val; }

    [[nodiscard]] auto get_bar_size() const -> int { return num_of_bars_; }
    [[nodiscard]] auto get_pars() const -> std::vector<std::pair<float, float>> { return scale_offsets_; }

  private:
    int num_of_bars_ = 0;
    float bar_width_ = 5.0;
    float bar_length_ = 50.0;
    float error_y_ = 4.0;
    std::vector<std::pair<float, float>> scale_offsets_;
    TRandom3* rnd_ = nullptr;
};

inline void to_json(nlohmann::json& json_obj, const Signal& signal)
{
    json_obj = nlohmann::json{ { "bar_number", signal.bar_num },
                               { "pos_x", signal.pos_x },
                               { "pos_y", signal.pos_y },
                               { "err_y", signal.err_y } };
}
