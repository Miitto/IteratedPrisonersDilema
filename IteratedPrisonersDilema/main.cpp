#include "cli/cli.hpp"
#include <iostream>

#include "game.hpp"

int main(const int argc, const char* const argv[]) {
  auto cliArgs = cli::get_cli_args(argc, argv);

  auto options = cli::Args::fromArgs(cliArgs);
  if (options.index() == 1) {
    auto [msg, code] = std::get<std::tuple<std::string, uint32_t>>(options);
    std::cout << msg << std::endl;
    return code;
  }

  auto args = std::get<cli::Args>(options);
  std::cout << args << std::endl;

  Game game(args, args.strategies[0], args.strategies[1]);

  game.play();

  game.printResults(std::cout);
}
