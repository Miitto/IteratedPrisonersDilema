echo "========== Running SCB Evolution =========="
./x64/Release/IteratedPrisonersDilema.exe --rounds 50 --epsilon 0.0 --evolve --enable-budget --strategies ALLC,ALLD,TFT,GRIM,PAVLOV,CONTRITE,PROBER  --save ./csv/scb.csv --format csv
