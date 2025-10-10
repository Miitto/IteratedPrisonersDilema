#pragma once

#include "../cli/cli.hpp"
#include "ctft.hpp"

#include <iostream>

namespace strats {
  /// <summary>
  /// Strategy that starts by probing the opponent, then tries to exploit ALLC
  /// and defend against ALLD. Otherwise it plays CTFT to handle noise and
  /// cooperate with nice strategies.
  ///
  /// Unlike regular Prober, it attempts to calculate the noise rate to not be
  /// fooled by noise.
  /// </summary>
  class SmartProber : public CTft {
    enum class Mode { PROBING, EXPLOITING, RETALIATING };

    Choice m_ourThisChoice = Choice::COOPERATE;

    uint32_t m_turn = 0;
    uint32_t m_swaps = 0;
    uint32_t m_oppenentDefections = 0;

    constexpr static Choice probeSequence[4] = {
        Choice::COOPERATE, Choice::DEFECT, Choice::COOPERATE,
        Choice::COOPERATE};

    double calcNoiseRate() const {
      return static_cast<double>(m_swaps) / static_cast<double>(m_turn);
    }

    /// <summary>
    /// Check if the opponent is cooperating almost all the time, allowing for
    /// noise
    /// </summary>
    /// <returns></returns>
    bool isLikelyAllc() {
      auto noiseRate = calcNoiseRate();
      double opponentDefectionRate = static_cast<double>(m_oppenentDefections) /
                                     static_cast<double>(m_turn);

      if (opponentDefectionRate < noiseRate + 0.05) {
        return true;
      }

      return false;
    }

    /// <summary>
    /// Check if the opponent is defecting almost all the time, allowing for
    /// noise
    /// </summary>
    /// <returns></returns>
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
        // Probe
        if (m_turn < 4)
          return probeSequence[m_turn];

        // Farm points off ALLC, and don't get messed up by ALLD (will also
        // catch Grim)
        if (isLikelyAllc() || isLikelyAlld())
          return Choice::DEFECT;

        // Delegate to CTFT
        return CTft::getChoice();
      };

      m_ourThisChoice = pick();

      return m_ourThisChoice;
    }
    void giveResult(Payoff p, Choice oppChoice) override {
      // Store swaps to estimate noise rate
      if (CTft::didSwap(p, m_ourThisChoice)) {
        ++m_swaps;
      }

      // Count opponent defections to estimate their strategy
      if (oppChoice == Choice::DEFECT) {
        ++m_oppenentDefections;
      }

      // Delegate to CTFT so it can handle accidents
      CTft::giveResult(p, oppChoice);
      ++m_turn;
    }

    ~SmartProber() override = default;
  };
} // namespace strats
