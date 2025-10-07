#include "tournament.hpp"
#include "game.hpp"

#include <iostream>

void Tournament::run() {
  for (size_t i = 0; i < m_args.strategies.size(); ++i) {
    for (size_t j = i; j < m_args.strategies.size(); ++j) {
      auto& strat1 = m_args.strategies[i];
      auto& strat2 = m_args.strategies[j];

      if (m_args.verbose) {
        std::clog << "Playing " << strat1 << " vs " << strat2 << "...\n";
      }

      for (uint32_t r = 0; r < m_args.repeats; ++r) {
        Game game(m_args, strat1, strat2, r);
        game.play();
        m_games.push_back(std::move(game));
      }
    }
  }
}

ScoreMatrix Tournament::getScoreMatrix() const {
  size_t n = m_args.strategies.size();
  std::vector<std::vector<double>> matrix(n, std::vector<double>(n, 0.0));

  auto getStratIndex = [&](const cli::Strategy& strat) {
    for (size_t i = 0; i < m_args.strategies.size(); ++i) {
      if (strat == m_args.strategies[i])
        return i;
    }
    return size_t(-1); // Not found
  };

  for (const auto& game : m_games) {
    auto [strat1, strat2] = game.getAverageScores();
    auto idx1 = getStratIndex(game.getStrat1().getStrat());
    auto idx2 = getStratIndex(game.getStrat2().getStrat());
    if (idx1 == size_t(-1) || idx2 == size_t(-1)) {
      throw std::runtime_error(
          "Strategy not found in tournament strategies, should be unreachable");
    }
    matrix[idx1][idx2] += strat1 / m_args.repeats;
    matrix[idx2][idx1] += strat2 / m_args.repeats;
  }

  for (int i = 0; i < n; ++i) {
    matrix[i][i] /= 2.0; // Each strategy plays itself twice as often
  }

  return ScoreMatrix(m_args, std::move(matrix));
}

std::vector<std::pair<cli::Strategy, double>>
ScoreMatrix::getAverageScores() const {
  size_t n = m_matrix.size();
  std::vector<std::pair<cli::Strategy, double>> totals;
  totals.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    double total = 0.0;
    for (size_t j = 0; j < n; ++j) {
      total += m_matrix[i][j];
    }
    totals.emplace_back(m_args.strategies[i], total / m_args.strategies.size());
  }
  std::sort(totals.begin(), totals.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
  return totals;
}

std::ostream& operator<<(std::ostream& os, const ScoreMatrix& sm) {
  const auto& matrix = sm.getMatrix();
  size_t n = matrix.size();

  switch (sm.m_args.format) {
  case cli::Format::TEXT: {
    size_t maxStratLen = 0;
    for (const auto& strat : sm.m_args.strategies) {
      std::ostringstream ss;
      ss << strat;
      maxStratLen = std::max(maxStratLen, ss.str().length());
    }

    os << "Score Matrix:\n";
    os << std::setw(maxStratLen) << ' ';
    for (size_t j = 0; j < n; ++j) {
      os << std::setw(maxStratLen + 1) << sm.m_args.strategies[j];
    }
    os << "\n";
    for (size_t i = 0; i < n; ++i) {
      os << std::setw(maxStratLen) << sm.m_args.strategies[i] << ":";
      for (size_t j = 0; j < n; ++j) {
        os << std::setw(maxStratLen) << std::fixed << std::setprecision(2)
           << matrix[i][j] << ' ';
      }
      os << "\n";
    }

    auto totals = sm.getAverageScores();
    os << "\nAverage Scores:\n";
    for (const auto& [strat, total] : totals) {
      os << strat << ": " << std::fixed << std::setprecision(2) << total
         << "\n";
    }

    break;
  }
  case cli::Format::CSV: {
    os << "Player";
    for (size_t j = 0; j < n; ++j) {
      os << ",P" << j;
    }
    os << "\n";
    for (size_t i = 0; i < n; ++i) {
      os << "P" << i;
      for (size_t j = 0; j < n; ++j) {
        os << "," << std::fixed << std::setprecision(2) << matrix[i][j];
      }
      os << "\n";
    }
    break;
  }
  case cli::Format::JSON: {
    os << "{\n  \"score_matrix\": [\n";
    for (size_t i = 0; i < n; ++i) {
      os << "    [";
      for (size_t j = 0; j < n; ++j) {
        os << std::fixed << std::setprecision(2) << matrix[i][j];
        if (j < n - 1)
          os << ", ";
      }
      os << "]";
      if (i < n - 1)
        os << ",";
      os << "\n";
    }
    os << "  ]\n}\n";
    break;
  }
  }
  return os;
}