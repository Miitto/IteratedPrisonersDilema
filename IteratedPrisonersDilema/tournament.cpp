#include "tournament.hpp"
#include "game.hpp"
#include <iostream>

void Tournament::run() {
  for (size_t i = 0; i < m_args.strategies.size(); ++i) {
    for (size_t j = i + 1; j < m_args.strategies.size(); ++j) {
      for (uint32_t r = 0; r < m_args.repeats; ++r) {
        Game game(m_args, m_args.strategies[i], m_args.strategies[j], r);
        game.play();
        game.printResults(std::cout);
      }
    }
  }
}