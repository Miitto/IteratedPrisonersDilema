#pragma once

#include "../colors.hpp"
#include <concepts>
#include <filesystem>
#include <string_view>
#include <variant>
#include <vector>

namespace cli {
  /// <summary>
  /// Converts command-line arguments to a vector of string views.
  /// </summary>
  /// <param name="argc">The number of command-line arguments.</param>
  /// <param name="argv">An array of C-style strings representing the
  /// command-line arguments.</param> <returns>A vector containing string views
  /// of each command-line argument.</returns>

  std::vector<std::string_view> get_cli_args(int argc,
                                             const char* const argv[]);

  /// <summary>
  /// Holds the payoff values for the Prisoner's Dilemma game.
  /// </summary>
  struct Payoffs {
    double temptation;
    double reward;
    double punishment;
    double sucker;
  };

  /// <summary>
  /// Union-like class to hold either a simple predefined strategy or a double
  /// for a random strategy.
  /// Uses bit fields to store a flag for whether it's random to save space.
  /// </summary>
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

    /// <summary>
    /// Returns true if the strategy is a random strategy, false if it is a
    /// simple strategy.
    /// </summary>
    /// <returns></returns>
    bool isRnd() const { return isRandom; }
    /// <summary>
    /// Returns the randomness value if the strategy is a random strategy.
    /// It is undefined behavior to call this if the strategy is not random.
    /// </summary>
    /// <returns></returns>
    double rnd() const {
      uint64_t b = bits;
      double& r = reinterpret_cast<double&>(b);
      return r;
    }

    bool operator==(const Strategy& other) const {
      return isRandom == other.isRandom && bits == other.bits;
    }

    /// <summary>
    /// Returns the simple strategy if the strategy is a simple strategy.
    /// It is undefined behavior to call this if the strategy is random.
    /// </summary>
    /// <returns></returns>
    Simple simple() const { return static_cast<Simple>(bits); }
  };

  enum class Format { TEXT, JSON, CSV };

  std::ostream& operator<<(std::ostream& os, const Payoffs& p);
  std::ostream& operator<<(std::ostream& os, const Strategy s);
  std::ostream& operator<<(std::ostream& os, const std::vector<Strategy>& s);
  std::ostream& operator<<(std::ostream& os, const Format f);

  /// <summary>
  /// Holds the parsed command-line arguments for the program.
  /// </summary>
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

    /// <summary>
    /// Parses command-line arguments into an Args struct or returns an error
    /// </summary>
    /// <param name="args">Command line arguments</param>
    /// <returns>std::variant of the Args struct on success, or a string
    /// containing the error and the error code to return on failure. Help text
    /// is returned as an error with return code 0.</returns>
    static std::variant<Args, std::tuple<std::string, uint32_t>>
    fromArgs(const std::vector<std::string_view> args);

    friend std::ostream& operator<<(std::ostream& os, const Args& args);
  };

  RGBColor getStrategyColor(cli::Strategy s);
} // namespace cli