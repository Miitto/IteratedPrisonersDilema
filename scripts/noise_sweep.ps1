echo "========== Running Noise Sweep =========="
./x64/Release/IteratedPrisonersDilema.exe --rounds 100 --repeats 10 --epsilon 0.00 --seed 7 --strategies ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER --format csv --save ./csv/noise_sweep/0.00.csv
./x64/Release/IteratedPrisonersDilema.exe --rounds 100 --repeats 10 --epsilon 0.05 --seed 7 --strategies ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER --format csv --save ./csv/noise_sweep/0.05.csv
./x64/Release/IteratedPrisonersDilema.exe --rounds 100 --repeats 10 --epsilon 0.10 --seed 7 --strategies ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER --format csv --save ./csv/noise_sweep/0.10.csv
./x64/Release/IteratedPrisonersDilema.exe --rounds 100 --repeats 10 --epsilon 0.15 --seed 7 --strategies ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER --format csv --save ./csv/noise_sweep/0.15.csv
./x64/Release/IteratedPrisonersDilema.exe --rounds 100 --repeats 10 --epsilon 0.20 --seed 7 --strategies ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER --format csv --save ./csv/noise_sweep/0.20.csv
