echo "========== Running SCB Evolution =========="
./x64/Release/IteratedPrisonersDilema.exe --epsilon 0.0 --evolve --enable-budget --strategies ALLD,TFT,GRIM,PAVLOV,CONTRITE,PROBER  --save ./csv/evo.csv --format csv --verbose
