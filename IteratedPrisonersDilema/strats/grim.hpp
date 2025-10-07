#pragma once

#include "../cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  class Grim : public Strategy {
    bool m_triggered = false;

  public:
    Grim(const cli::Args& args) : Strategy(args) {}

    cli::Strategy getStrat() const override { return cli::Strategy::GRIM; }

    Choice getChoice() override {
      return m_triggered ? Choice::DEFECT : Choice::COOPERATE;
    }
    void giveResult(Payoff, Choice oppChoice) override {
      if (oppChoice == Choice::DEFECT)
        m_triggered = true;
    }

    ~Grim() override = default;
  };
} // namespace strats
