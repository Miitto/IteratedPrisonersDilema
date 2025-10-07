#include "tournament.hpp"
#include "colors.hpp"
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

namespace {
  void printTextMatrix(std::ostream& os,
                       const std::vector<std::vector<double>>& matrix,
                       const std::vector<cli::Strategy>& strategies,
                       bool useColors, double maxScore) {
    size_t n = matrix.size();
    size_t maxStratLen = 0;
    for (const auto& strat : strategies) {
      std::ostringstream ss;
      ss << strat;
      maxStratLen = std::max(maxStratLen, ss.str().length());
    }
    os << "Score Matrix:\n";
    os << std::setw(maxStratLen) << ' ';
    for (size_t j = 0; j < n; ++j) {
      os << std::setw(maxStratLen + 1) << strategies[j];
    }
    os << "\n";
    for (size_t i = 0; i < n; ++i) {
      os << std::setw(maxStratLen) << strategies[i] << ":";
      for (size_t j = 0; j < n; ++j) {
        if (useColors) {
          auto value = matrix[i][j];
          auto lerpAmount = std::min(1.0, value / maxScore);

          RGBColor startColor(255, 50, 50); // Red
          RGBColor endColor(50, 255, 50);   // Green

          RGBColor lerpedColor = startColor.lerp(endColor, lerpAmount);

          os << "\033[38;2;" << lerpedColor.r << ";" << lerpedColor.g << ";"
             << lerpedColor.b << "m";
        }
        os << std::fixed << std::setprecision(2) << std::setw(maxStratLen)
           << matrix[i][j] << "\033[39m ";
      }
      os << "\n";
    }
  }

  void printTextAverageScores(
      std::ostream& os,
      const std::vector<std::pair<cli::Strategy, double>>& avgScores,
      bool useColors, double maxScore) {
    os << "\nAverage Scores:\n";
    size_t maxStratLen = 0;
    for (const auto& [strat, _] : avgScores) {
      std::ostringstream ss;
      ss << strat;
      maxStratLen = std::max(maxStratLen, ss.str().length());
    }
    for (const auto& [strat, score] : avgScores) {
      os << std::setw(maxStratLen) << strat << ": ";
      if (useColors) {
        RGBColor startColor(255, 0, 0); // Red
        RGBColor endColor(0, 255, 0);   // Green
        double lerpAmount =
            std::clamp(score / maxScore, 0.0, 1.0); // Assuming max score ~5
        RGBColor lerpedColor = startColor.lerp(endColor, lerpAmount);
        os << "\033[38;2;" << lerpedColor.r << ";" << lerpedColor.g << ";"
           << lerpedColor.b << "m";
      }
      os << std::fixed << std::setprecision(2) << score;
      if (useColors) {
        os << "\033[39m"; // Reset to default color
      }
      os << "\n";
    }
  }

  void printJsonMatrix(std::ostream& os,
                       const std::vector<std::vector<double>>& matrix,
                       const std::vector<cli::Strategy>& strats) {
    os << "{\n";
    for (int i = 0; i < matrix.size(); ++i) {
      os << "  \"" << strats[i] << "\": [\n";
      for (int j = 0; j < matrix[i].size(); ++j) {
        os << "    {\n      \"name\": \"" << strats[j]
           << "\",\n      \"avg\": " << matrix[i][j] << "\n    }";
        if (j != matrix[i].size()) {
          os << ",";
        }
        os << "\n";
      }
      os << "  ]";
      if (i != matrix.size() - 1) {
        os << ",";
      }
      os << "\n";
    }
    os << "}\n";
  }
} // namespace

std::ostream& operator<<(std::ostream& os, const ScoreMatrix& sm) {
  const auto& matrix = sm.getMatrix();
  size_t n = matrix.size();

  switch (sm.m_args.format) {
  case cli::Format::TEXT: {
    printTextMatrix(os, matrix, sm.m_args.strategies, sm.m_args.savePath == "",
                    sm.m_args.payoffs.temptation);
    auto averages = sm.getAverageScores();
    printTextAverageScores(os, averages, sm.m_args.savePath == "",
                           sm.m_args.payoffs.temptation);
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
    printJsonMatrix(os, matrix, sm.m_args.strategies);
    break;
  }
  }
  return os;
}