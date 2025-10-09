#pragma once

#include "../colors.hpp"
#include <concepts>
#include <filesystem>
#include <string_view>
#include <variant>
#include <vector>

namespace cli {
  std::vector<std::string_view> get_cli_args(int argc,
                                             const char* const argv[]);

  struct Payoffs {
    double temptation;
    double reward;
    double punishment;
    double sucker;
  };

  class XX {
    enum Strats {
      // ..
    };

    bool isRnd;
    union U {
      Strats strat;
      double rnd;
    };

    U u;
  };

  // TODO: Change this to a union of Strategy or double for RND's randomness.
  // Can use top bit as the flag since sign bit is irrelevent. Then remove RND
  // from this enum
  class Strategy {
  public:
    enum Simple {
      ALLC,
      ALLD,
      TFT,
      GRIM,
      PAVLOV,
      CONTRITE,
      PROBER,
      FGRIM,
      SPROBER
    };

  private:
    bool isRandom : 1;
    uint64_t bits : 63;

  public:
    Strategy(double randomness)
        : isRandom(true), bits(reinterpret_cast<uint64_t&>(randomness)) {}
    Strategy(Simple s) : isRandom(false), bits(static_cast<uint64_t>(s)) {}

    bool isRnd() const { return isRandom; }
    double rnd() const {
      uint64_t b = bits;
      double& r = reinterpret_cast<double&>(b);
      return r;
    }

    bool operator==(const Strategy& other) const {
      return isRandom == other.isRandom && bits == other.bits;
    }

    Simple simple() const { return static_cast<Simple>(bits); }
  };

  enum class Format { TEXT, JSON, CSV };

  std::ostream& operator<<(std::ostream& os, const Payoffs& p);
  std::ostream& operator<<(std::ostream& os, const Strategy s);
  std::ostream& operator<<(std::ostream& os, const std::vector<Strategy>& s);
  std::ostream& operator<<(std::ostream& os, const Format f);

  class Args {
  public:
    uint32_t rounds;
    uint32_t repeats;
    double epsilon;
    uint32_t seed;
    Payoffs payoffs;
    std::vector<Strategy> strategies;
    Format format;
    std::filesystem::path savePath;
    std::filesystem::path loadFile;
    bool evolve;
    uint32_t population;
    uint32_t generations;
    double mutationRate;
    bool verbose;
    bool enableBudget;

    static std::variant<Args, std::tuple<std::string, uint32_t>>
    fromArgs(const std::vector<std::string_view> args);

    friend std::ostream& operator<<(std::ostream& os, const Args& args);
  };

  RGBColor getStrategyColor(cli::Strategy s);
} // namespace cli