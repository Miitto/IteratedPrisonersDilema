# Forgiving Grim

This strategy starts by cooperating and continues to cooperate until the opponent defects twice in a row. After two consecutive betrayals from the opponent, Forgiving Grim will defect forever.

FGRIM is slightly worse than GRIM in a noise-free environment due to the delay in punishment, but in a noisy environment it outperforms GRIM by avoiding unnecessary long-term retaliation due to accidental defections.

It also tends to lose out to PROBER since the single defection in the probing sequence is not enough to trigger the permanent defection state,
leading PROBER to attempt to exploit FGRIM's cooperation and ending up in endless defection.

This strategy can be run by passing in `FGRIM` as part of the strategies list.

# Smart Prober

This strategy begins the same way as Prober, starting with the sequence C, D, C, C.
However unlike Prober, Smart Prober tracks the number of times the opponent has defected and the number of times it's own choices have been switched.
This allows smart prober to estimate the noise level used in the game and use it as a threshold to determine if the opponent is likely ALLC or ALLD in a noisy environment.

Smart Prober is slightly worse than Prober due to a delay in responce as the average catches up, but in a noisy environment it outperforms it due to correctly identifying ALLC and ALLD.

This strategy can be run by passing in `SPROBER` as part of the strategies list.

# Results

## No Noise
```
rounds: 100
repeats: 10
epsilon: 0
seed: 7
payoffs: {T: 5, R: 3, P: 1, S: 0}
strategies: [ALLC, ALLD, TFT, GRIM, PAVLOV, RND0.3, RND0.7, CONTRITE, PROBER, FGRIM, SPROBER]
format: TEXT
savePath:
loadFile:
evolve: false
population: 100
generations: 50
mutationRate: 0
verbose: false

Payoffs: T=5, R=3, P=1, S=0

Score Matrix:
                       ALLC              ALLD               TFT              GRIM            PAVLOV            RND0.3            RND0.7          CONTRITE            PROBER             FGRIM           SPROBER
    ALLC: 3.00 [3.00, 3.00] 0.00 [0.00, 0.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 0.96 [0.90, 1.02] 2.16 [2.09, 2.24] 3.00 [3.00, 3.00] 0.09 [0.09, 0.09] 3.00 [3.00, 3.00] 0.09 [0.09, 0.09]
    ALLD: 5.00 [5.00, 5.00] 1.00 [1.00, 1.00] 1.04 [1.04, 1.04] 1.04 [1.04, 1.04] 1.04 [1.04, 1.04] 2.28 [2.21, 2.36] 3.88 [3.78, 3.98] 1.04 [1.04, 1.04] 1.16 [1.16, 1.16] 1.08 [1.08, 1.08] 1.12 [1.12, 1.12]
     TFT: 3.00 [3.00, 3.00] 0.99 [0.99, 0.99] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 1.85 [1.81, 1.90] 2.65 [2.61, 2.68] 3.00 [3.00, 3.00] 2.99 [2.99, 2.99] 3.00 [3.00, 3.00] 2.93 [2.93, 2.93]
    GRIM: 3.00 [3.00, 3.00] 0.99 [0.99, 0.99] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 2.26 [2.19, 2.34] 3.83 [3.73, 3.93] 3.00 [3.00, 3.00] 1.13 [1.13, 1.13] 3.00 [3.00, 3.00] 1.09 [1.09, 1.09]
  PAVLOV: 3.00 [3.00, 3.00] 0.99 [0.99, 0.99] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 1.85 [1.81, 1.90] 2.65 [2.61, 2.68] 3.00 [3.00, 3.00] 2.99 [2.99, 2.99] 3.00 [3.00, 3.00] 2.93 [2.93, 2.93]
  RND0.3: 4.36 [4.32, 4.40] 0.68 [0.66, 0.70] 1.89 [1.84, 1.93] 0.73 [0.71, 0.75] 1.89 [1.84, 1.93] 1.64 [1.60, 1.68] 3.24 [3.14, 3.35] 1.89 [1.84, 1.93] 1.95 [1.90, 1.99] 0.81 [0.76, 0.86] 1.92 [1.88, 1.96]
  RND0.7: 3.56 [3.51, 3.61] 0.28 [0.25, 0.30] 2.66 [2.63, 2.70] 0.39 [0.35, 0.42] 2.66 [2.63, 2.70] 1.24 [1.18, 1.30] 2.44 [2.39, 2.49] 2.66 [2.63, 2.70] 2.23 [1.66, 2.80] 0.74 [0.59, 0.90] 2.64 [2.60, 2.69]
CONTRITE: 3.00 [3.00, 3.00] 0.99 [0.99, 0.99] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 1.85 [1.81, 1.90] 2.65 [2.61, 2.68] 3.00 [3.00, 3.00] 2.99 [2.99, 2.99] 3.00 [3.00, 3.00] 2.93 [2.93, 2.93]
  PROBER: 4.94 [4.94, 4.94] 0.96 [0.96, 0.96] 2.99 [2.99, 2.99] 1.03 [1.03, 1.03] 2.99 [2.99, 2.99] 1.83 [1.78, 1.87] 2.87 [2.60, 3.13] 2.99 [2.99, 2.99] 2.98 [2.98, 2.98] 1.18 [1.18, 1.18] 2.92 [2.92, 2.92]
   FGRIM: 3.00 [3.00, 3.00] 0.98 [0.98, 0.98] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 3.00 [3.00, 3.00] 2.24 [2.16, 2.32] 3.65 [3.53, 3.77] 3.00 [3.00, 3.00] 1.03 [1.03, 1.03] 3.00 [3.00, 3.00] 1.11 [1.11, 1.11]
 SPROBER: 4.94 [4.94, 4.94] 0.97 [0.97, 0.97] 2.93 [2.93, 2.93] 1.04 [1.04, 1.04] 2.93 [2.93, 2.93] 1.84 [1.79, 1.88] 2.66 [2.62, 2.69] 2.93 [2.93, 2.93] 2.92 [2.92, 2.92] 1.16 [1.16, 1.16] 2.90 [2.90, 2.90]

Average Scores:
     TFT: 2.67 [2.30, 3.04]
  PAVLOV: 2.67 [2.30, 3.04]
CONTRITE: 2.67 [2.30, 3.04]
  PROBER: 2.52 [1.85, 3.18]
    GRIM: 2.48 [1.93, 3.03]
 SPROBER: 2.47 [1.82, 3.13]
   FGRIM: 2.46 [1.91, 3.00]
  RND0.7: 1.96 [1.34, 2.58]
    ALLC: 1.94 [1.17, 2.70]
  RND0.3: 1.91 [1.29, 2.52]
    ALLD: 1.79 [1.01, 2.57]
```

## 0.2 Noise
```
rounds: 100
repeats: 10
epsilon: 0.2
seed: 7
payoffs: {T: 5, R: 3, P: 1, S: 0}
strategies: [ALLC, ALLD, TFT, GRIM, PAVLOV, RND0.3, RND0.7, CONTRITE, PROBER, FGRIM, SPROBER]
format: TEXT
savePath:
loadFile:
evolve: false
population: 100
generations: 50
mutationRate: 0
verbose: false

Payoffs: T=5, R=3, P=1, S=0

Score Matrix:
                       ALLC              ALLD               TFT              GRIM            PAVLOV            RND0.3            RND0.7          CONTRITE            PROBER             FGRIM           SPROBER
    ALLC: 2.82 [2.71, 2.92] 0.93 [0.84, 1.01] 2.32 [2.21, 2.42] 1.02 [0.94, 1.10] 2.16 [2.02, 2.29] 1.46 [1.36, 1.57] 2.20 [2.12, 2.28] 2.38 [2.28, 2.48] 1.83 [1.42, 2.25] 1.35 [1.14, 1.56] 1.41 [1.12, 1.71]
    ALLD: 3.80 [3.70, 3.89] 1.52 [1.42, 1.62] 2.11 [1.99, 2.23] 1.71 [1.59, 1.84] 2.31 [2.17, 2.46] 2.32 [2.20, 2.43] 3.17 [3.07, 3.28] 2.23 [2.12, 2.34] 2.16 [2.05, 2.27] 1.74 [1.62, 1.85] 1.93 [1.86, 2.01]
     TFT: 3.01 [2.91, 3.11] 1.38 [1.28, 1.47] 2.27 [2.19, 2.36] 1.51 [1.43, 1.60] 2.26 [2.18, 2.34] 1.98 [1.88, 2.07] 2.57 [2.51, 2.63] 2.53 [2.45, 2.60] 2.22 [1.99, 2.44] 1.65 [1.58, 1.72] 2.39 [2.20, 2.59]
    GRIM: 3.74 [3.65, 3.83] 1.50 [1.40, 1.61] 2.09 [2.01, 2.17] 1.54 [1.45, 1.63] 2.30 [2.16, 2.43] 2.30 [2.19, 2.42] 3.16 [3.06, 3.26] 2.24 [2.14, 2.35] 2.07 [1.91, 2.22] 1.76 [1.65, 1.86] 1.91 [1.84, 1.99]
  PAVLOV: 3.11 [3.01, 3.21] 1.32 [1.22, 1.42] 2.18 [2.08, 2.27] 1.36 [1.27, 1.45] 2.22 [2.09, 2.35] 1.92 [1.80, 2.05] 2.57 [2.50, 2.64] 2.47 [2.38, 2.56] 2.07 [1.87, 2.27] 1.50 [1.40, 1.61] 2.38 [2.29, 2.46]
  RND0.3: 3.49 [3.39, 3.59] 1.32 [1.22, 1.42] 2.17 [2.08, 2.25] 1.36 [1.25, 1.46] 2.35 [2.23, 2.46] 2.03 [1.91, 2.16] 2.79 [2.70, 2.88] 2.36 [2.25, 2.48] 2.26 [2.12, 2.39] 1.48 [1.38, 1.58] 2.26 [2.16, 2.37]
  RND0.7: 3.09 [3.00, 3.17] 1.04 [0.95, 1.13] 2.31 [2.23, 2.39] 1.07 [1.01, 1.14] 2.28 [2.18, 2.38] 1.75 [1.64, 1.86] 2.48 [2.37, 2.58] 2.37 [2.29, 2.45] 2.21 [1.97, 2.44] 1.22 [1.13, 1.31] 2.16 [1.92, 2.41]
CONTRITE: 2.97 [2.88, 3.07] 1.34 [1.25, 1.42] 2.30 [2.23, 2.38] 1.36 [1.29, 1.44] 2.16 [2.10, 2.22] 1.95 [1.87, 2.03] 2.57 [2.49, 2.65] 2.57 [2.51, 2.62] 2.14 [1.94, 2.34] 1.65 [1.55, 1.75] 2.48 [2.34, 2.61]
  PROBER: 3.27 [3.01, 3.54] 1.36 [1.27, 1.46] 2.21 [2.13, 2.28] 1.43 [1.33, 1.52] 2.32 [2.19, 2.44] 1.99 [1.90, 2.08] 2.66 [2.50, 2.81] 2.41 [2.29, 2.52] 2.11 [1.88, 2.35] 1.67 [1.58, 1.76] 2.29 [2.11, 2.46]
   FGRIM: 3.57 [3.43, 3.71] 1.50 [1.40, 1.59] 2.09 [2.01, 2.18] 1.53 [1.44, 1.61] 2.35 [2.19, 2.52] 2.31 [2.19, 2.44] 3.14 [3.02, 3.26] 2.18 [2.11, 2.25] 2.00 [1.81, 2.20] 1.70 [1.57, 1.83] 1.97 [1.84, 2.09]
 SPROBER: 3.52 [3.30, 3.74] 1.44 [1.38, 1.51] 2.22 [2.12, 2.32] 1.48 [1.43, 1.54] 2.13 [2.08, 2.19] 1.99 [1.91, 2.07] 2.68 [2.52, 2.85] 2.50 [2.44, 2.56] 2.14 [1.92, 2.37] 1.53 [1.49, 1.58] 2.51 [2.42, 2.60]

Average Scores:
    ALLD: 2.27 [1.90, 2.65]
    GRIM: 2.24 [1.86, 2.62]
   FGRIM: 2.21 [1.85, 2.57]
 SPROBER: 2.20 [1.85, 2.54]
  RND0.3: 2.17 [1.81, 2.53]
     TFT: 2.16 [1.88, 2.44]
  PROBER: 2.16 [1.85, 2.47]
CONTRITE: 2.14 [1.84, 2.43]
  PAVLOV: 2.10 [1.79, 2.41]
  RND0.7: 2.00 [1.63, 2.36]
    ALLC: 1.81 [1.46, 2.15]
```