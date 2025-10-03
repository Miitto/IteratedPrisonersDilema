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
  std::variant<T, std::string>
  parse(std::vector<std::string_view>::const_iterator strs,
        const std::vector<std::string_view>::const_iterator end);

  template <typename T>
  concept Parsable = requires {
    {
      parse<T>(
          std::declval<std::vector<std::string_view>::const_iterator>(),
          std::declval<const std::vector<std::string_view>::const_iterator>())
    } -> std::same_as<std::variant<T, std::string>>;
  };

  template <typename ARG>
    requires Parsable<ARG>
  class Option {
    const std::string_view name;

  public:
    ARG value;

    operator ARG() { return value; }

    static bool isAnOption(const std::string_view str) {
      return str.starts_with("--");
    }

    constexpr Option(const std::string_view name, const ARG defaultVal)
        : name{name}, value(defaultVal) {}

    bool is(const std::string_view str) const {
      return isAnOption(str) && str.substr(2) == name;
    }

    std::variant<ARG, std::string>
    parse(std::vector<std::string_view>::const_iterator strs,
          const std::vector<std::string_view>::const_iterator end) {
      if (strs == end) {
        return std::string{"Expected option --" + std::string(name) + "."};
      }

      if (!(isAnOption(*strs) && strs->substr(2) == name)) {
        return std::string{"Not an option " + std::string{*strs}};
      }

      ++strs;

      if constexpr (std::is_same_v<ARG, bool>) {
        return true;
      }

      if (strs == end || isAnOption(*strs)) {
        return std::string{"Missing argument for option --" +
                           std::string(name) + ", expected `" +
                           typeid(ARG).name() + "`."};
      }

      const auto parsed = cli::parse<ARG>(strs, end);
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

    static std::variant<Args, std::tuple<std::string, uint32_t>>
    fromArgs(const std::vector<std::string_view> args);
  };
} // namespace cli