#pragma once

#include "../cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  class CTft : public Strategy {
    bool didAccidentallyDefect = false;
    Choice m_opponentLastChoice = Choice::COOPERATE;
    Choice m_ownLastChoice = Choice::COOPERATE;

  public:
    CTft(const cli::Args& args) : Strategy(args) {}

    cli::Strategy getStrat() override { return cli::Strategy::CONTRITE; }

    Choice getChoice() override {
      m_ownLastChoice = m_opponentLastChoice;
      return m_opponentLastChoice;
    }
    void giveResult(Payoff p, Choice oppChoice) override {
      if (didAccidentallyDefect && oppChoice == Choice::DEFECT) {
        m_opponentLastChoice = Choice::COOPERATE;
      } else {
        m_opponentLastChoice = oppChoice;
      }

      Choice intended = m_ownLastChoice;
      Choice actual;
      switch (p) {
      case Payoff::Reward:
      case Payoff::Sucker:
        actual = Choice::COOPERATE;
        break;
      case Payoff::Temptation:
      case Payoff::Punishment:
        actual = Choice::DEFECT;
        break;
      }

      if (intended == Choice::COOPERATE && actual == Choice::DEFECT) {
        didAccidentallyDefect = true;
      } else {
        didAccidentallyDefect = false;
      }
    }

    ~CTft() override = default;
  };
} // namespace strats
