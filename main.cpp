#include <EventBuilder.hpp>
#include <Mille.hpp>
#include <concepts>
#include <fmt/ranges.h>
#include <iostream>
#include <ranges>

auto main() -> int
{
    constexpr auto num_of_events = 1000000;
    auto event_builder = EventBuilder{};
    event_builder.set_offset_range(5.);
    event_builder.set_error_y(0.3);

    event_builder.init();
    // event_builder.set_log_file("event_data.json");
    auto mille = Mille{ "test.bin" };

    for (auto loop : std::ranges::views::iota(0, num_of_events))
    {
        event_builder.build_event_data([&mille](const DataPoint& data_point) { mille.mille(data_point); });
        mille.end();
    }

    return 0;
}
