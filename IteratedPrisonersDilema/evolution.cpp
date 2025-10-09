#include "evolution.hpp"
#include <iostream>
#include <numeric>

std::vector<double> replicationDynamics(const std::vector<double>& distribution,
                                        const std::vector<double>& fitnesses) {
  double totalFitness =
      std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
  double avgFitness = totalFitness / fitnesses.size();

  std::vector<double> newDistribution(distribution.size(), 0.0);

  for (size_t i = 0; i < distribution.size(); ++i) {
    if (avgFitness > 0) {
      newDistribution[i] = distribution[i] * (fitnesses[i] / avgFitness);
    } else {
      newDistribution[i] = distribution[i]; // Avoid division by zero
    }
  }

  // Normalize to ensure proportions sum to 1
  double sum =
      std::accumulate(newDistribution.begin(), newDistribution.end(), 0.0);
  if (sum > 0) {
    for (auto& val : newDistribution) {
      val /= sum;
    }
  }

  return newDistribution;
}

void Evolution::run() {
  std::vector<double> distribution(m_args.strategies.size(),
                                   1.0 / m_args.strategies.size());

  m_proportions.reserve(m_args.generations);

  for (uint32_t gen = 0; gen < m_args.generations; ++gen) {
    m_proportions.push_back(distribution);
    std::vector<cli::Strategy> strategies;
    strategies.reserve(m_args.population);

    for (size_t i = 0; i < distribution.size(); ++i) {
      for (int j = 0; j < static_cast<int>(
                              std::round(distribution[i] * m_args.population));
           ++j) {
        strategies.push_back(m_args.strategies[i]);
      }
    }

    if (m_args.verbose) {
      std::cout << "Generation " << gen + 1 << "\n";
      for (size_t i = 0; i < m_args.strategies.size(); ++i) {
        auto pop = distribution[i] * m_args.population;
        std::clog << m_args.strategies[i] << ": " << pop << " ("
                  << (pop / m_args.population) * 100 << "%)\n";
      }
    }

    Tournament tournament(m_args, strategies);
    tournament.run();

    if (m_args.verbose) {
      std::clog << "Generation " << (gen + 1) << " done\n";
    }

    auto scores = tournament.getScoreMatrix().getResults();

    std::vector<double> fitnesses(scores.size());

    for (const auto& [strat, result] : scores) {
      auto it =
          std::find(m_args.strategies.begin(), m_args.strategies.end(), strat);
      size_t index = std::distance(m_args.strategies.begin(), it);
      fitnesses[index] = result.mean;
    }

    distribution = replicationDynamics(distribution, fitnesses);

    if (m_args.mutationRate == 0.0) {
      uint32_t aboveZero = false;
      for (const auto& val : distribution) {
        if (val > 0.0) {
          ++aboveZero;
        }
      }

      if (aboveZero < 2) {
        if (m_args.verbose) {
          std::clog << "All but one strategies extinct, stopping early\n";
        }
        break;
      }
    }
  }
}