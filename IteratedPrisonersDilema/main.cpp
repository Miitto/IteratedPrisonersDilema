#include "cli/cli.hpp"
#include <fstream>
#include <iostream>

#include "evolution.hpp"
#include "tournament.hpp"

int runRoundRobin(const cli::Args& args) {
  Tournament tournament(args, args.strategies);
  tournament.run();

  auto scoreMatrix = tournament.getScoreMatrix();
  if (args.savePath != "") {
    std::ofstream ofs(args.savePath);
    if (!ofs) {
      std::cerr << "Error: Could not open file for writing: " << args.savePath
                << std::endl;
      return 1;
    }
    ofs << scoreMatrix;
    ofs.close();
    std::cout << "Score matrix saved to " << args.savePath << std::endl;
  } else {
    std::cout << scoreMatrix << std::endl;
  }

  std::cout << "\033[38;2;0;255;0mTournament completed successfully.\033[0m"
            << std::endl;

  return 0;
}

int runEvolution(const cli::Args& args) {
  Evolution evolution{args};
  evolution.run();

  if (args.savePath != "") {
    std::ofstream ofs(args.savePath);
    if (!ofs) {
      std::cerr << "Error: Could not open file for writing: " << args.savePath
                << std::endl;
      return 1;
    }
    evolution.printResults(ofs);
    ofs.close();
    std::cout << "Score matrix saved to " << args.savePath << std::endl;
  } else {
    evolution.printResults(std::cout);
    std::cout << std::endl;
  }

  std::cout << "\033[38;2;0;255;0mEvolution completed successfully.\033[0m"
            << std::endl;

  return 0;
}

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

  if (args.evolve) {
    runEvolution(args);
  } else {
    return runRoundRobin(args);
  }
}
