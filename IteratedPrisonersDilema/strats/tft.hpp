#pragma once

#include "../cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  class Tft : public Strategy {
    Choice m_opponentLastChoice = Choice::COOPERATE;

  public:
    Tft(const cli::Payoffs& payoffs) : Strategy(payoffs) {}

    cli::Strategy getStrat() override { return cli::Strategy::TFT; }

    Choice getChoice() const override { return m_opponentLastChoice; }
    void giveResult(Payoff, Choice oppChoice) override {
      m_opponentLastChoice = oppChoice;
    }

    ~Tft() override = default;
  };
} // namespace strats
