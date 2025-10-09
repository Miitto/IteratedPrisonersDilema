# Question 1

## Tournament Engine:
Each strategy is represented as a class deriving from `strats::Strategy` which defines two pure virtual methods:
- `getChoice() -> Choice`
- `giveResult(Payoff yourPayoff, Choice opponentsChoice) -> void`.
Each derived class is expected to hold its own state, although the base `strats::Strategy` class holds a `protected` constant reference to the parsed CLI parameters should they be needed.

The `Game` class stores two `std::unique_ptr<strats::Strategy>`s and runs a provided number of rounds between these two strategies, which may store persisted state between rounds.

## Cli Interface
The help text is shown below:
```
Usage: `IteratedPrisonersDilema [options]`

Options:
  --rounds <uint32_t>        Number of rounds per match (default: 100)
  --repeats <uint32_t>       Number of matches per pair of strategies (default: 10)
  --epsilon <double>         Probability of a mistake per move (default: 0.0)
  --seed <uint32_t>          Seed for random number generator (default: 0)
  --payoff <T,R,P,S>         Payoff values as four doubles separated by commas or spaces (default: 5,3,1,0)
  --strategies <STRATEGIES>  Comma or space separated list of strategies to use. Strategies are one of ALLC, ALLD, TFT, GRIM, PAVLOV, CONTRITE, PROBER, or RNDX where X is a floating point number in [0,1] with one digit before the decimal point and at least one after (e.g `RND0.3` or `RND1.0`, not `RND.3` or `RND1.`). (default: ALLC,ALLD)
  --format <text|json|csv>   Output format (default: text)
  --save <path>              Path to save results to (default: none)
  --load <path>              Path to load results from (default: none)
  --evolve                   Whether to run an evolution. Its presence indicates true and its absence indicates false, although `0`, `1`, `true` and `false` can be used to set the value explicitly.simulation instead of a round robin tournament (default: false)
  --population <uint32_t>    Starting population size per strategy for evolution (default: 100)
  --generations <uint32_t>   Number of generations for evolution (default: 50)
  --mutation <double>        Mutation rate for evolution (default: 0.0)
  --verbose                  Whether to print verbose output.
  --help                     Show this help message
```

The aformentions `Game` class uses the `--rounds` input for the number of rounds per match.

The `Tournament` class runs a round-robin tournament between all selected strategies, storing every completed game to calculate total/average payoffs later.
Each pair of strategies plays `--repeats N` times, incrementing the starting seed for each repeat. Strategies do not persist state between repeats, unlike rounds.

`--seed` can be used to set the seed for the random number generator used by strategies that require randomness. Each repeated tournament with the same seed will yield the same results.
This defaults to 0, but it could easily be changed to default to a random number.

TODO: Ask about what the expected output for "pairwise payoffs" should be, is it the matrix or a per-round breakdown?

## Baseline Test
The console output is given below with no save path and the `text`format. When printed to console with text format, the average payoffs would be coloured from red (low) to green (high) using ANSI escape codes.
The parsed arguments will only be logged to console regardless of output format or save path.
```
rounds: 100
repeats: 10
epsilon: 0
seed: 0
payoffs: {T: 5, R: 3, P: 1, S: 0}
strategies: [ALLC, ALLD, TFT, GRIM, PAVLOV, RND0.3, RND0.7]
format: TEXT
savePath:
loadFile:
evolve: false
population: 100
generations: 50
mutationRate: 0.01
verbose: false

Payoffs: T=5, R=3, P=1, S=0

Score Matrix:
                     ALLC              ALLD               TFT              GRIM            PAVLOV            RND0.3            RND0.7
  ALLC: 3.00 [3.00, 3.00] 0.00 [0.00, 0.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 0.84 [0.74, 0.94] 2.12 [2.06, 2.19]
  ALLD: 5.00 [5.00, 5.00] 1.00 [1.00, 1.00] 1.04 [1.04, 1.04] 1.04 [1.04, 1.04] 1.04 [1.04, 1.04] 2.12 [1.98, 2.25] 3.83 [3.74, 3.92]
   TFT: 3.00 [3.00, 3.00] 0.99 [0.99, 0.99] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 1.74 [1.66, 1.82] 2.62 [2.59, 2.66]
  GRIM: 3.00 [3.00, 3.00] 0.99 [0.99, 0.99] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 2.09 [1.96, 2.23] 3.79 [3.71, 3.88]
PAVLOV: 3.00 [3.00, 3.00] 0.99 [0.99, 0.99] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 1.74 [1.66, 1.82] 2.62 [2.59, 2.66]
RND0.3: 4.44 [4.38, 4.51] 0.72 [0.69, 0.75] 1.76 [1.69, 1.84] 0.78 [0.74, 0.81] 1.76 [1.69, 1.84] 1.56 [1.49, 1.62] 3.27 [3.19, 3.36]
RND0.7: 3.58 [3.54, 3.63] 0.29 [0.27, 0.31] 2.63 [2.60, 2.66] 0.37 [0.35, 0.40] 2.63 [2.60, 2.66] 1.13 [1.04, 1.22] 2.42 [2.37, 2.46]

Average Scores:
  GRIM: 2.70 [2.08, 3.31]
   TFT: 2.48 [1.93, 3.03]
PAVLOV: 2.48 [1.93, 3.03]
  ALLD: 2.15 [1.03, 3.27]
  ALLC: 2.14 [1.29, 2.99]
RND0.3: 2.04 [1.11, 2.97]
RND0.7: 1.87 [0.99, 2.74]

Tournament completed successfully.
```
The score matrix shows the average score for each strategy against every other strategy, with the rows being the strategy and the columns being the opponent.

As seen above, GRIM, TFT and PAVLOV are the top 3 strategies in this noiseless scenario.
The cooperative strategies did best, however the strategies more capable of resisting a persistent defector had an advantage - with Grim being the most consistent as it performed better against both random strategies.
RND0.7 did worst, as it suffered due to being too cooperative, yet not cooperative enough to not trigger retaliation from TFT or GRIM, while defecting too much to correctly exploit the more vulnerable strategies.

# Question 2

## Noise
The `Game` class uses the CLI `--epsilon` parameter to determine the probability of a mistake per move. For each player's choice, a random number in [0,1] is generated and if it is less than `epsilon`, the player's choice is flipped (C -> D or D -> C).
This is done independently for both players each round, so the player needs to compare the payoff with what their choice was to determine if their choice was flipped.

## Noise Sweep

All tournaments were run with the following parameters, with only the `--epsilon` value being modified:
```
rounds: 1000
repeats: 100
epsilon: 0.00
seed: 0
payoffs: {T: 5, R: 3, P: 1, S: 0}
strategies: [TFT, GRIM, PAVLOV, CONTRITE]
format: TEXT
savePath:
loadFile:
evolve: false
population: 100
generations: 50
mutationRate: 0.01
verbose: false
```

### 0.0
```
Score Matrix:
                        TFT              GRIM            PAVLOV          CONTRITE
     TFT: 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00]
    GRIM: 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00]
  PAVLOV: 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00]
CONTRITE: 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00]

Average Scores:
     TFT: 3.00 [3.00, 3.00]
    GRIM: 3.00 [3.00, 3.00]
  PAVLOV: 3.00 [3.00, 3.00]
CONTRITE: 3.00 [3.00, 3.00]
```
### 0.05
```
Score Matrix:
                        TFT              GRIM            PAVLOV          CONTRITE
     TFT: 2.25 [2.23, 2.27] 1.12 [1.11, 1.12] 2.26 [2.24, 2.27] 2.54 [2.52, 2.56]
    GRIM: 1.34 [1.34, 1.35] 1.17 [1.16, 1.18] 1.50 [1.49, 1.51] 1.35 [1.35, 1.36]
  PAVLOV: 2.25 [2.24, 2.27] 1.08 [1.07, 1.09] 2.05 [2.03, 2.07] 2.43 [2.41, 2.44]
CONTRITE: 2.47 [2.45, 2.48] 1.12 [1.11, 1.13] 2.34 [2.32, 2.36] 2.90 [2.90, 2.90]

Average Scores:
CONTRITE: 2.21 [1.56, 2.85]
     TFT: 2.04 [1.51, 2.58]
  PAVLOV: 1.95 [1.44, 2.46]
    GRIM: 1.34 [1.23, 1.46]
```

### 0.1
```
Score Matrix:
                        TFT              GRIM            PAVLOV          CONTRITE
     TFT: 2.25 [2.24, 2.27] 1.21 [1.21, 1.22] 2.26 [2.25, 2.27] 2.56 [2.55, 2.57]
    GRIM: 1.61 [1.61, 1.62] 1.30 [1.29, 1.31] 1.86 [1.84, 1.87] 1.64 [1.64, 1.65]
  PAVLOV: 2.26 [2.25, 2.27] 1.15 [1.14, 1.15] 2.10 [2.08, 2.11] 2.45 [2.43, 2.46]
CONTRITE: 2.42 [2.41, 2.43] 1.21 [1.20, 1.21] 2.30 [2.28, 2.31] 2.80 [2.79, 2.80]

Average Scores:
CONTRITE: 2.18 [1.60, 2.76]
     TFT: 2.07 [1.57, 2.57]
  PAVLOV: 1.99 [1.50, 2.48]
    GRIM: 1.60 [1.41, 1.80]
```

### 0.15
```
Score Matrix:
                        TFT              GRIM            PAVLOV          CONTRITE
     TFT: 2.26 [2.24, 2.27] 1.31 [1.30, 1.32] 2.25 [2.24, 2.26] 2.53 [2.52, 2.54]
    GRIM: 1.83 [1.82, 1.84] 1.44 [1.43, 1.45] 2.11 [2.10, 2.12] 1.90 [1.89, 1.90]
  PAVLOV: 2.26 [2.25, 2.27] 1.23 [1.23, 1.24] 2.14 [2.13, 2.15] 2.44 [2.43, 2.45]
CONTRITE: 2.36 [2.35, 2.37] 1.30 [1.29, 1.31] 2.25 [2.24, 2.26] 2.70 [2.70, 2.71]

Average Scores:
CONTRITE: 2.15 [1.64, 2.66]
     TFT: 2.09 [1.63, 2.54]
  PAVLOV: 2.02 [1.56, 2.47]
    GRIM: 1.82 [1.58, 2.06]
```

### 0.2
```
Score Matrix:
                        TFT              GRIM            PAVLOV          CONTRITE
     TFT: 2.26 [2.25, 2.27] 1.42 [1.41, 1.42] 2.25 [2.24, 2.26] 2.50 [2.50, 2.51]
    GRIM: 2.02 [2.01, 2.03] 1.57 [1.56, 1.58] 2.29 [2.27, 2.30] 2.10 [2.09, 2.11]
  PAVLOV: 2.25 [2.24, 2.26] 1.34 [1.33, 1.35] 2.16 [2.14, 2.17] 2.43 [2.42, 2.44]
CONTRITE: 2.30 [2.29, 2.31] 1.39 [1.39, 1.40] 2.23 [2.22, 2.24] 2.61 [2.61, 2.62]

Average Scores:
CONTRITE: 2.13 [1.69, 2.58]
     TFT: 2.11 [1.70, 2.51]
  PAVLOV: 2.04 [1.63, 2.46]
    GRIM: 1.99 [1.73, 2.25]
```

### Matrix
```
Noise Matrix:
              0.0     0.05      0.1     0.15      0.2
     TFT:    3.00     2.04     2.07     2.09     2.11
    GRIM:    3.00     1.34     1.60     1.82     1.99
  PAVLOV:    3.00     1.95     1.99     2.02     2.05
CONTRITE:    3.00     2.21     2.18     2.15     2.13
```

The matrix shows that Contrite is the most robust strategy smaller amounts of noise, but as noise increases TFT becomes more effective as the mistakes become more frequent.
This seems to be due to the cost of apologizing becoming greater than the benefits as strategy begins to be undermined with chaos.
If the trend continues, it looks as if TFT will overtake CONTRITE at around 0.25 noise, and PAVLOV will take the lead later on as noise increases further.

Grim performs the worst in all scenarios, as its unforgiving nature means that a single mistake can lead to a permanent loss of cooperation, which is very punishing in a noisy environment where all other strategies are more content to cooperate when possible.

# Question 3

## No Noise
```
Score Matrix:
                       ALLC               TFT          CONTRITE            PAVLOV            PROBER              ALLD
    ALLC: 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 0.01 [0.01, 0.01] 0.00 [0.00, 0.00]
     TFT: 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 1.00 [1.00, 1.00]
CONTRITE: 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 1.00 [1.00, 1.00]
  PAVLOV: 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 1.00 [1.00, 1.00]
  PROBER: 4.99 [4.99, 4.99] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 1.00 [1.00, 1.00]
    ALLD: 5.00 [5.00, 5.00] 1.00 [1.00, 1.00] 1.00 [1.00, 1.00] 1.00 [1.00, 1.00] 1.02 [1.02, 1.02] 1.00 [1.00, 1.00]

Average Scores:
  PROBER: 3.00 [2.07, 3.92]
     TFT: 2.67 [2.07, 3.26]
CONTRITE: 2.67 [2.07, 3.26]
  PAVLOV: 2.67 [2.07, 3.26]
    ALLC: 2.00 [0.87, 3.13]
    ALLD: 1.67 [0.48, 2.86]
```
## 0.2 Noise
```
Score Matrix:
                       ALLC               TFT          CONTRITE            PAVLOV            PROBER              ALLD
    ALLC: 2.76 [2.72, 2.80] 2.37 [2.35, 2.39] 2.45 [2.42, 2.47] 2.16 [2.13, 2.19] 2.06 [1.68, 2.44] 0.84 [0.82, 0.86]
     TFT: 2.98 [2.95, 3.01] 2.27 [2.24, 2.29] 2.52 [2.49, 2.54] 2.24 [2.21, 2.28] 2.30 [2.27, 2.34] 1.42 [1.39, 1.45]
CONTRITE: 2.94 [2.91, 2.97] 2.33 [2.30, 2.35] 2.62 [2.60, 2.64] 2.25 [2.21, 2.28] 2.32 [2.29, 2.35] 1.39 [1.36, 1.42]
  PAVLOV: 3.10 [3.07, 3.13] 2.23 [2.21, 2.25] 2.43 [2.40, 2.47] 2.14 [2.10, 2.18] 2.26 [2.24, 2.28] 1.32 [1.29, 1.35]
  PROBER: 3.16 [2.94, 3.37] 2.25 [2.23, 2.27] 2.52 [2.50, 2.54] 2.26 [2.24, 2.29] 2.20 [2.12, 2.27] 1.42 [1.39, 1.44]
    ALLD: 3.84 [3.80, 3.88] 2.00 [1.96, 2.03] 2.09 [2.06, 2.12] 2.29 [2.24, 2.35] 2.00 [1.96, 2.04] 1.56 [1.52, 1.60]

Average Scores:
CONTRITE: 2.31 [1.93, 2.69]
  PROBER: 2.30 [1.89, 2.71]
    ALLD: 2.30 [1.72, 2.88]
     TFT: 2.29 [1.92, 2.66]
  PAVLOV: 2.25 [1.83, 2.66]
    ALLC: 2.11 [1.62, 2.59]
```

## Analysis

### Noiseless vs Noise
In the noiseless scenario, PROBER is the best strategy as it can exploit ALLC while still cooperating with the other strategies effectively.
The cost of PROBER's initial probing move is negligible over a long series of rounds (1000), allowing it to gain the highest average payoff.
With fewer rounds it would fall behind TFT, CONTRITE and PAVLOV as the cost of probing would be more significant - especially if ALLC was not in the pool to exploit and inflate the average.
ALLD had a slightly higher average against PROBER and ALLC than the other cooperative strategies due to PROBER's delayed reaction due to the set starting probe, but this was not enough to make it competitive overall - coming solidly in last.

However, as noise is introduced, PROBER's effectiveness decreases as it can be misled into defecting against cooperative strategies due to mistakes, leading to lower overall payoffs, while also miss-detecting ALLC so it cannot exploit it.
This lets CONTRITE take the lead as it is very resistant to noise due to its forgiving nature, allowing it to maintain cooperation with other strategies even when mistakes occur.

ALLD did surprisingly well in the noisy scenario, coming second overall. This is likely due to its ability to capitalize on the mistakes of cooperative strategies, as they may get thrown off by the occasional cooperation from ALLD due to noise, leading to higher payoffs for ALLD when it returns to defecting almost immediatly.

### Exploiting strategies
- PROBER
  - Against ALLC, PROBER exploits it by essentially switching into ALLD mode after the initial probe, leading to a very high average payoff against it.
  - Against TFT, CONTRITE and PAVLOV, PROBER enters mutual cooperates after the initial probe, leading to mutual cooperation and high payoffs.
- ALLD
  - Against ALLC, ALLD exploits it fully, leading to the highest possible payoff.
  - Against all other strategies, ALLD is punished heavily, leading to low payoffs. Against TFT, CONTRITE and PAVLOV, ALLD gets a single T payoff, before being relegated to Ps for the rest of the game. It gets 4 Ts against PROBER due to PROBER's initial probing move, but then is punished to Ps for the rest of the game.

### Resisting Exploitation
Any form of TFT (TFT, CONTRITE, none-exploiting PROBER) resisted being exploited well, as any attempted exploitation would be met with retaliation resulting in a lower payoff for both strategies.
PAVLOV will end up playing the preferred choice of the opponent on average, so it will cooperate with cooperative strategies and defect against defectors, averaging to a moderate payoff against all strategies.

# Question 4

## Evolution Implementation

The `Evolution` class runs a simulation of an evolving population of strategies over a number of generations. It takes the following parameters from the CLI:
- `--evolve`: Whether to enable evolution
- `--population <uint32_t>`: The starting population. Each enabled strategy will have an equal share of this total population.
- `--mutation <double>`: The chance of a strategy mutating into another.

## Experiments

Graphs for various experiments are located in Excel spreadsheets at `docs/Evolution/`.

### ALLC, ALLD, TFT, GRIM, PAVLOV

In an environment with no noise, ALLD quickly went extinct as the other strategies fell into mutual cooperation.
The only strategy vulnerable to ALLD was ALLC, however ALLC managed to survive on the higher average given by cooperating with the other 4 strategies - albeit ending with a lower population.

After ALLD went extinct the other strategies fell into stable cooperation and the population for each stabilized there.

The chaos brought in by noise allowed ALLD to survive till the end since other strategies failed to maintain cooperation due to mistakes. This kept ALLC's average lower, nearly bringing it to extinction by generation 50.
ALLD was still the second worst strategy on average, but it was enough to keep it in the population as it decreased at a slower rate.

PAVLOV's population didn't change much in the noisy scenario, ending with barely more population than it began with.

TFT and GRIM were the most successful strategies in both scenarios, with GRIM outperforming TFT in the noisy scenario.
