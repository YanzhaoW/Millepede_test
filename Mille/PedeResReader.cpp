#include "PedeResReader.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <optional>

using std::make_pair;

auto split_string_view(std::string_view str, std::string_view del) -> std::vector<std::string_view>
{
    auto splits = std::vector<std::string_view>{};

    for (auto sub_begin = str.find_first_not_of(del); sub_begin != std::string_view::npos;
         sub_begin = str.find_first_not_of(del, sub_begin))
    {
        auto sub_end = str.find_first_of(del, sub_begin);
        sub_end = (sub_end == std::string_view::npos) ? str.size() : sub_end;
        auto sub_str = str.substr(sub_begin, sub_end - sub_begin);
        splits.push_back(sub_str);
        sub_begin = sub_end;
    }
    return splits;
}

auto parse_parameter(std::string_view line_string) -> std::optional<ResultPar>
{
    auto splits_view = split_string_view(line_string, " ");
    auto result = ResultPar{};

    constexpr auto max_size = 3;
    constexpr auto min_size = 5;
    if (splits_view.size() != min_size && splits_view.size() != max_size)
    {
        return {};
    }

    result.par_id = std::stoi(std::string{ splits_view.front() });
    result.sigma = std::stof(std::string{ splits_view.back() });
    result.value = std::stof(std::string{ splits_view[1] });
    return result;
}

void ResReader::read_file()
{

    pars_.clear();
    auto ifile = std::ifstream(file_name_, std::ios_base::in);
    auto buffer = std::string{};
    if (ifile.is_open())
    {
        std::getline(ifile, buffer);
        while (std::getline(ifile, buffer))
        {
            auto result = parse_parameter(buffer);
            if (result.has_value())
            {
                pars_.emplace(result->par_id, result.value());
            }
        }
    }
}
