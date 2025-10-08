#include "bracket.hpp"
#include <algorithm>
#include <numeric>

#include <mutex>
#include <sstream>
#include <thread>

std::ostream& operator<<(std::ostream& os, const BracketResult& res) {
  std::stringstream ss;

  ss << std::setprecision(2) << std::fixed << res.mean << " ["
     << std::setprecision(2) << std::fixed << res.low << ", "
     << std::setprecision(2) << std::fixed << res.high << ']';

  return os << ss.str();
}

void Bracket::play() {
  m_games.reserve(m_args.repeats);

  std::vector<std::thread> threads;
  threads.reserve(m_args.repeats);

  std::mutex mtx;

  for (int i = 0; i < m_args.repeats; ++i) {
    threads.emplace_back([this, i, &mtx]() {
      Game game(m_args, m_strat1, m_strat2, i);
      game.play();

      mtx.lock();
      m_games.push_back(std::move(game));
      mtx.unlock();
    });
  }

  for (auto& t : threads) {
    t.join();
  }
}

double Bracket::getStrat1Mean() const {
  std::vector<double> scores(m_games.size());
  std::transform(m_games.begin(), m_games.end(), scores.begin(),
                 [](const auto& g) { return g.getAverageScores().first; });

  return std::accumulate(scores.begin(), scores.end(), 0.0) / scores.size();
}

double Bracket::getStrat2Mean() const {
  std::vector<double> scores(m_games.size());
  std::transform(m_games.begin(), m_games.end(), scores.begin(),
                 [](const auto& g) { return g.getAverageScores().second; });
  return std::accumulate(scores.begin(), scores.end(), 0.0) / scores.size();
}

double Bracket::getStdDev1(std::optional<double> mean) const {
  if (!mean) {
    mean = getStrat1Mean();
  }
  std::vector<double> scores(m_games.size());
  std::transform(m_games.begin(), m_games.end(), scores.begin(),
                 [](const auto& g) { return g.getAverageScores().first; });

  double sum = 0.0;
  for (const auto& score : scores) {
    auto s = score - *mean;
    sum += s * s;
  }
  return std::sqrt(sum / scores.size());
}

double Bracket::getStdDev2(std::optional<double> mean) const {
  if (!mean) {
    mean = getStrat2Mean();
  }
  std::vector<double> scores(m_games.size());
  std::transform(m_games.begin(), m_games.end(), scores.begin(),
                 [](const auto& g) { return g.getAverageScores().second; });
  double sum = 0.0;
  for (const auto& score : scores) {
    auto s = score - *mean;
    sum += s * s;
  }
  return std::sqrt(sum / scores.size());
}

std::pair<double, double>
Bracket::getConfInterval1(std::optional<double> stdDev) const {
  if (!stdDev) {
    stdDev = getStdDev1();
  }
  double mean = getStrat1Mean();
  double margin = 1.96 * (*stdDev / std::sqrt(m_games.size()));
  return {mean - margin, mean + margin};
}

std::pair<double, double>
Bracket::getConfInterval2(std::optional<double> stdDev) const {
  if (!stdDev) {
    stdDev = getStdDev2();
  }
  double mean = getStrat2Mean();
  double margin = 1.96 * (*stdDev / std::sqrt(m_games.size()));
  return {mean - margin, mean + margin};
}

BracketResult Bracket::strat1Result() const {
  double mean = getStrat1Mean();
  auto [low, high] = getConfInterval1();
  return BracketResult{mean, low, high};
}

BracketResult Bracket::strat2Result() const {
  double mean = getStrat2Mean();
  auto [low, high] = getConfInterval2();
  return BracketResult{mean, low, high};
}
