#include "evolution.hpp"
#include <iostream>
#include <numeric>

void Evolution::run() {
  std::vector<double> distribution(m_args.strategies.size(),
                                   1.0 / m_args.strategies.size());

  m_proportions.reserve(m_args.generations);

  for (int gen = 0; gen < m_args.generations; ++gen) {
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

    std::vector<double> fitnesses;
    fitnesses.reserve(scores.size());

    for (const auto& [strat, result] : scores) {
      fitnesses.push_back(result.mean);
    }

    double totalFitness =
        std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0);

    double fitnessAvg = totalFitness / fitnesses.size();

    std::vector<double> newDistribution(m_args.strategies.size(), 0);

    for (size_t i = 0; i < fitnesses.size(); ++i) {
      double proportion = fitnesses[i] / totalFitness;
      newDistribution[i] =
          static_cast<double>(distribution[i]) * (proportion + 0.5);
    }

    distribution = std::move(newDistribution);
  }
}