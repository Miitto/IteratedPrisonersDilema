echo "========== Running Evolution =========="
./x64/Release/IteratedPrisonersDilema.exe --rounds 50 --epsilon 0.0 --evolve --strategies ALLC,ALLD,TFT,GRIM,PAVLOV,CONTRITE,PROBER  --save ./csv/evolution.csv --format csv
