#pragma once

#include <concepts>
#include <filesystem>
#include <string_view>
#include <variant>
#include <vector>

namespace cli {
  std::vector<std::string_view> get_cli_args(int argc,
                                             const char* const argv[]);

  template <typename T>
  auto parse(std::vector<std::string_view>::const_iterator strs,
             std::vector<std::string_view>::const_iterator end)
      -> std::variant<T, std::string>;

  template <typename T>
  concept Parsable = requires {
    {
      parse<T>(std::declval<std::vector<std::string_view>::const_iterator>(),
               std::declval<std::vector<std::string_view>::const_iterator>())
    } -> std::same_as<std::variant<T, std::string>>;
  };

  template <typename ARG>
    requires Parsable<ARG>
  class Option {
    const std::string_view name;

  public:
    static bool isAnOption(const std::string_view str) {
      return str.starts_with("--");
    }

    constexpr Option(const std::string_view name) : name{name} {}

    bool is(const std::string_view str) const {
      return isAnOption(str) && str.substr(2) == name;
    }

    std::variant<ARG, std::string>
    parse(std::vector<std::string_view>::const_iterator strs,
          std::vector<std::string_view>::const_iterator end) {
      if (!(isAnOption(*strs) && strs->substr(2) == name)) {
        return std::string{"Not an option --" + std::string(name)};
      }

      ++strs;

      if constexpr (std::is_same_v<ARG, bool>) {
        return true;
      }

      if (strs == end || isAnOption(*strs)) {
        return std::string{"Missing argument for option --" +
                           std::string(name) + "."};
      }

      const auto parsed = parse(strs, end);
      if (parsed.index() == 0) {
        return std::get<ARG>(parsed);
      } else {
        return std::string{"Failed to parse argument for option --" +
                           std::string(name) + ": " +
                           std::get<std::string>(parsed)};
      }
    }
  };

  enum class Payoff { T, R, P, S };
  enum class Strategy { ALLC, ALLD, TFT, GRIM, PAVLOV, RNDO, CONTRITE, PROBER };
  enum class Format { TEXT, JSON, CSV };

  class Args {
  public:
    uint32_t rounds;
    uint32_t repeats;
    double epsilon;
    uint32_t seed;
    Payoff payoff;
    std::vector<Strategy> strategies;
    Format format;
    std::filesystem::path savePath;
    std::filesystem::path loadFile;
    bool evolve;
    uint32_t population;
    uint32_t generations;
    double mutationRate;

    static std::optional<Args>
    fromArgs(const std::vector<std::string_view> args) {
      Option<uint32_t> roundsOpt("rounds");
      uint32_t rounds = 100;

      auto it = args.cbegin();
      for (; it != args.cend(); ++it) {
        if (roundsOpt.is(*it)) {
          auto r = roundsOpt.parse(it, args.cend());
          if (r.index() == 0) {
            rounds = std::get<uint32_t>(r);
          }
        }
      }
    }
  };
} // namespace cli