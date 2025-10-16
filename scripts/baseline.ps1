echo "========== Running Baseline =========="
./x64/Release/IteratedPrisonersDilema.exe --rounds 100 --repeats 10 --epsilon 0.00 --seed 7 --strategies ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,RND0.7 --format text
