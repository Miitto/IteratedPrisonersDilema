#include "game.hpp"
#include <random>

#include "strats/allc.hpp"
#include "strats/alld.hpp"

Game::Game(const cli::Args& args, cli::Strategy strat1, cli::Strategy strat2)
    : m_args(args) {

  auto makeStrat =
      [this](cli::Strategy s) -> std::unique_ptr<strats::Strategy> {
    if (s.isRnd()) {
      // TODO: Implement random strategy
      throw std::runtime_error("Random strategies are not yet implemented");
    }

    switch (s.simple()) {
    case cli::Strategy::ALLC: {
      return std::make_unique<strats::AllC>(m_args.payoffs);
    }
    case cli::Strategy::ALLD: {
      return std::make_unique<strats::AllD>(m_args.payoffs);
    }
    case cli::Strategy::TFT:
    case cli::Strategy::GRIM:
    case cli::Strategy::PAVLOV:
    case cli::Strategy::CONTRITE:
    case cli::Strategy::PROBER:
      throw std::runtime_error("Strategy not yet implemented");
    }

    // Should be unreachable
    throw std::runtime_error("Unknown strategy");
  };

  m_strat1 = makeStrat(strat1);
  m_strat2 = makeStrat(strat2);
}

namespace {
  constexpr std::pair<strats::Payoff, strats::Payoff> RESULT_MATRIX[2][2] = {
      {{strats::Payoff::Reward, strats::Payoff::Sucker},          // C, C
       {strats::Payoff::Temptation, strats::Payoff::Punishment}}, // C, D
      {{strats::Payoff::Punishment, strats::Payoff::Temptation},  // D, C
       {strats::Payoff::Sucker, strats::Payoff::Sucker}}          // D, D
  };
}

void Game::playRound() {
  auto& s1 = *m_strat1;
  auto& s2 = *m_strat2;

  auto choice1 = s1.getChoice();
  auto choice2 = s2.getChoice();

  std::mt19937 gen(m_args.seed);
  std::uniform_real_distribution dist(0.0, 1.0);

  auto flipChoice = [](strats::Choice c) -> strats::Choice {
    return (c == strats::Choice::COOPERATE) ? strats::Choice::DEFECT
                                            : strats::Choice::COOPERATE;
  };

  if (s1.suseptibleToError() &&
      dist(gen) < m_args.epsilon) { // Mistake for strat1
    choice1 = flipChoice(choice1);
  }

  if (s2.suseptibleToError() &&
      dist(gen) < m_args.epsilon) { // Mistake for strat2
    choice2 = flipChoice(choice2);
  }

  auto pair =
      RESULT_MATRIX[static_cast<int>(choice1)][static_cast<int>(choice2)];

  auto& [result1, result2] = pair;

  s1.giveResult(result1);
  s2.giveResult(result2);

  m_results.push_back(pair);
}

void Game::play() {
  for (uint32_t round = 0; round < m_args.rounds; ++round) {
    playRound();
  }
}
