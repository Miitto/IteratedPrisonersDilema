#pragma once

#include "../cli/cli.hpp"
#include "tft.hpp"

#include <iostream>

namespace strats {
  class Prober : public Tft {
    uint32_t m_turn = 0;
    bool m_everDefected = false;

    constexpr static Choice probeSequence[4] = {
        Choice::COOPERATE, Choice::DEFECT, Choice::COOPERATE,
        Choice::COOPERATE};

  public:
    Prober(const cli::Args& args) : Tft(args) {}

    cli::Strategy getStrat() override { return cli::Strategy::PROBER; }

    Choice getChoice() override {
      if (m_turn < 4)
        return probeSequence[m_turn];

      if (!m_everDefected)
        return Choice::DEFECT;

      return Tft::getChoice();
    }
    void giveResult(Payoff p, Choice oppChoice) override {
      if (m_turn > 3)
        return Tft::giveResult(p, oppChoice);

      if (oppChoice == Choice::DEFECT) {
        if (m_args.verbose) {
          std::clog << "Prober detected defection at turn " << m_turn
                    << ", switching to TFT behavior." << std::endl;
        }
        m_everDefected = true;
      }

      ++m_turn;
    }

    ~Prober() override = default;
  };
} // namespace strats
