#include "cli.hpp"

namespace cli {
  std::vector<std::string_view> get_cli_args(const int argc,
                                             const char* const argv[]) {
    std::vector<std::string_view> args;
    args.reserve(static_cast<std::size_t>(argc));

    for (int i = 0; i < argc; ++i) {
      args.emplace_back(argv[i]);
    }

    return args;
  }

  template <uint32_t>
  auto parse(std::vector<std::string_view>::iterator strs,
             std::vector<std::string_view>::iterator end)
      -> std::variant<uint32_t, std::string> {
    return std::stoi(*strs);
  }
} // namespace cli