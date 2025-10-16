echo "========== Running Basic CSV =========="
./x64/Release/IteratedPrisonersDilema.exe --save ./csv/basic.csv --rounds 100 --repeats 10 --epsilon 0.00 --seed 7 --strategies ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER --format csv
