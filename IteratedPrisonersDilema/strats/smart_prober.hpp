#pragma once

#include "../cli/cli.hpp"
#include "ctft.hpp"

#include <iostream>

namespace strats {
  class SmartProber : public CTft {
    enum class Mode { PROBING, EXPLOITING, RETALIATING };

    Choice m_ourThisChoice = Choice::COOPERATE;
    Choice m_ownLastChoice = Choice::COOPERATE;

    uint32_t m_turn = 0;
    uint32_t m_swaps = 0;
    uint32_t m_oppenentDefections = 0;
    uint32_t m_opponentMimics = 0;

    constexpr static Choice probeSequence[4] = {
        Choice::COOPERATE, Choice::DEFECT, Choice::COOPERATE,
        Choice::COOPERATE};

    double calcNoiseRate() const {
      return static_cast<double>(m_swaps) / static_cast<double>(m_turn);
    }

    bool isLikelyAllc() {
      auto noiseRate = calcNoiseRate();
      double opponentDefectionRate = static_cast<double>(m_oppenentDefections) /
                                     static_cast<double>(m_turn);

      if (opponentDefectionRate < noiseRate + 0.05) {
        return true;
      }

      return false;
    }

    bool isLikelyAlld() {
      auto noiseRate = calcNoiseRate();
      double opponentCooperationRate =
          static_cast<double>(m_turn - m_oppenentDefections) /
          static_cast<double>(m_turn);
      if (opponentCooperationRate < noiseRate + 0.05) {
        return true;
      }

      return false;
    }

  public:
    SmartProber(const cli::Args& args) : CTft(args) {}

    cli::Strategy getStrat() const override { return cli::Strategy::SPROBER; }
    double getBudget() const override { return 4.0; }

    Choice getChoice() override {
      auto pick = [this]() {
        if (m_turn < 4)
          return probeSequence[m_turn];

        // Farm points off ALLC, and don't get messed up by ALLD (will also
        // catch Grim)
        if (isLikelyAllc() || isLikelyAlld())
          return Choice::DEFECT;

        return CTft::getChoice();
      };

      m_ourThisChoice = pick();

      return m_ourThisChoice;
    }
    void giveResult(Payoff p, Choice oppChoice) override {

      if (oppChoice == m_ownLastChoice) {
        ++m_opponentMimics;
      }

      if (CTft::didSwap(p, m_ourThisChoice)) {
        ++m_swaps;
        m_ownLastChoice = !m_ourThisChoice;
      } else {
        m_ownLastChoice = m_ourThisChoice;
      }

      if (oppChoice == Choice::DEFECT) {
        ++m_oppenentDefections;
      }

      CTft::giveResult(p, oppChoice);
      ++m_turn;
    }

    ~SmartProber() override = default;
  };
} // namespace strats
