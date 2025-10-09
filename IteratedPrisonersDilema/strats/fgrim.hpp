#pragma once

#include "../cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  class FGrim : public Strategy {
    bool m_triggered = false;
    Choice m_oppLastChoice = Choice::COOPERATE;

  public:
    FGrim(const cli::Args& args) : Strategy(args) {}

    cli::Strategy getStrat() const override { return cli::Strategy::FGRIM; }

    Choice getChoice() override {
      return m_triggered ? Choice::DEFECT : Choice::COOPERATE;
    }
    void giveResult(Payoff, Choice oppChoice) override {
      if (oppChoice == Choice::DEFECT && m_oppLastChoice == Choice::DEFECT)
        m_triggered = true;

      m_oppLastChoice = oppChoice;
    }

    ~FGrim() override = default;
  };
} // namespace strats
