#pragma once

#include <string>
#include <unordered_map>

struct ResultPar
{
    int par_id = 0;
    float value = 0.F;
    float sigma = 0.F;
};

class ResReader
{

  public:
    ResReader() = default;
    void read_file();
    auto get_pars() -> const auto& { return pars_; }

  private:
    std::string file_name_ = "millepede.res";
    std::unordered_map<int, ResultPar> pars_;
};
