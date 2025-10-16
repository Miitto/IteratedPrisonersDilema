#include "evolution.hpp"
#include "tournament.hpp"
#include <iostream>
#include <numeric>

namespace {
  std::vector<PopDist>
  replicationDynamics(const std::vector<PopDist>& distribution,
                      const std::vector<double>& fitnesses,
                      const uint32_t population) {
    double totalFitness =
        std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
    double avgFitness = totalFitness / fitnesses.size();

    std::vector<double> proportions(distribution.size(), 0.0);

    for (size_t i = 0; i < distribution.size(); ++i) {
      proportions[i] = distribution[i].proportion * (fitnesses[i] / avgFitness);
    }

    // Normalize to ensure proportions sum to 1
    double sum = std::accumulate(proportions.begin(), proportions.end(), 0.0);
    if (sum > 0) {
      for (auto& val : proportions) {
        val /= sum;
      }
    }

    std::vector<PopDist> newDistribution;
    newDistribution.reserve(distribution.size());

    std::transform(
        proportions.begin(), proportions.end(),
        std::back_inserter(newDistribution),
        [population](double prop) { return PopDist(prop, population); });

    return newDistribution;
  }
} // namespace

void Evolution::run() {
  std::vector<PopDist> distribution(
      m_args.strategies.size(),
      PopDist(1.0 / m_args.strategies.size(), m_args.population));

  m_proportions.reserve(m_args.generations);

  for (uint32_t gen = 0; gen < m_args.generations; ++gen) {
    m_proportions.push_back(distribution);
    std::vector<cli::Strategy> strategies;
    strategies.reserve(m_args.population);

    for (size_t i = 0; i < distribution.size(); ++i) {
      for (int j = 0; j < distribution[i].count; ++j) {
        strategies.push_back(m_args.strategies[i]);
      }
    }

    std::clog << "Generation " << gen + 1;
    if (m_args.verbose) {
      std::clog << ":\n";
      for (size_t i = 0; i < m_args.strategies.size(); ++i) {
        auto& pop = distribution[i];
        std::clog << m_args.strategies[i] << ": " << pop.count << " ("
                  << pop.proportion * 100 << "%)\n";
      }
    } else {
      std::clog << "\r";
    }

    Tournament tournament(m_args, strategies);
    tournament.run();

    auto scores = tournament.getScoreMatrix().getResults();

    std::vector<double> fitnesses(scores.size());

    for (const auto& [strat, result] : scores) {
      auto it =
          std::find(m_args.strategies.begin(), m_args.strategies.end(), strat);
      size_t index = std::distance(m_args.strategies.begin(), it);
      fitnesses[index] = result.mean;
    }

    distribution =
        replicationDynamics(distribution, fitnesses, m_args.population);

    if (m_args.mutationRate == 0.0) {
      uint32_t aboveZero = false;
      for (const auto& val : distribution) {
        if (val.count > 0.0) {
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

namespace {
  void printEvoCSV(std::ostream& os, const std::vector<cli::Strategy>& strats,
                   const std::vector<std::vector<PopDist>>& dist) {
    for (auto& strat : strats) {
      os << strat << ",";
    }
    os << "\n";
    for (auto& generation : dist) {
      for (auto& dist : generation) {
        os << dist.count << ",";
      }
      os << "\n";
    }
  }
} // namespace

void Evolution::printResults(std::ostream& os) const {
  switch (m_args.format) {
  case cli::Format::CSV: {
    printEvoCSV(os, m_args.strategies, m_proportions);
    break;
  }
  }
}
