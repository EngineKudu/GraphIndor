#!/bin/bash
set -x

./build/bin/baseline_test Wiki-Vote ~/dataset/wiki-vote_input 5 0111010011100011100001100 > ./logs/wiki-vote_p1.log_$(date -Iseconds)
./build/bin/baseline_test Wiki-Vote ~/dataset/wiki-vote_input 6 011011101110110101011000110000101000 > ./logs/wiki-vote_p2.log_$(date -Iseconds)
./build/bin/baseline_test Wiki-Vote ~/dataset/wiki-vote_input 6 011111101000110111101010101101101010 > ./logs/wiki-vote_p3.log_$(date -Iseconds)
./build/bin/baseline_test Wiki-Vote ~/dataset/wiki-vote_input 6 011110101101110000110000100001010010 > ./logs/wiki-vote_p4.log_$(date -Iseconds)
./build/bin/baseline_test Wiki-Vote ~/dataset/wiki-vote_input 7 0111111101111111011101110100111100011100001100000 > ./logs/wiki-vote_p5.log_$(date -Iseconds)
./build/bin/baseline_test Wiki-Vote ~/dataset/wiki-vote_input 7 0111111101111111011001110100111100011000001100000 > ./logs/wiki-vote_p6.log_$(date -Iseconds)
./build/bin/baseline_test Patents ~/dataset/patents_input 5 0111010011100011100001100 > ./logs/patents_p1.log_$(date -Iseconds)
./build/bin/baseline_test Patents ~/dataset/patents_input 6 011011101110110101011000110000101000 > ./logs/patents_p2.log_$(date -Iseconds)
./build/bin/baseline_test Patents ~/dataset/patents_input 6 011111101000110111101010101101101010 > ./logs/patents_p3.log_$(date -Iseconds)
./build/bin/baseline_test Patents ~/dataset/patents_input 6 011110101101110000110000100001010010 > ./logs/patents_p4.log_$(date -Iseconds)
./build/bin/baseline_test Patents ~/dataset/patents_input 7 0111111101111111011101110100111100011100001100000 > ./logs/patents_p5.log_$(date -Iseconds)
./build/bin/baseline_test Patents ~/dataset/patents_input 7 0111111101111111011001110100111100011000001100000 > ./logs/patents_p6.log_$(date -Iseconds)
./build/bin/baseline_test MiCo ~/dataset/mico_input 5 0111010011100011100001100 > ./logs/mico_p1.log_$(date -Iseconds)
./build/bin/baseline_test MiCo ~/dataset/mico_input 6 011011101110110101011000110000101000 > ./logs/mico_p2.log_$(date -Iseconds)
./build/bin/baseline_test MiCo ~/dataset/mico_input 6 011111101000110111101010101101101010 > ./logs/mico_p3.log_$(date -Iseconds)
./build/bin/baseline_test MiCo ~/dataset/mico_input 6 011110101101110000110000100001010010 > ./logs/mico_p4.log_$(date -Iseconds)
./build/bin/baseline_test MiCo ~/dataset/mico_input 7 0111111101111111011101110100111100011100001100000 > ./logs/mico_p5.log_$(date -Iseconds)
./build/bin/baseline_test MiCo ~/dataset/mico_input 7 0111111101111111011001110100111100011000001100000 > ./logs/mico_p6.log_$(date -Iseconds)
./build/bin/baseline_test LiveJournal ~/dataset/livejournal_input 5 0111010011100011100001100 > ./logs/livejournal_p1.log_$(date -Iseconds)
./build/bin/baseline_test LiveJournal ~/dataset/livejournal_input 6 011011101110110101011000110000101000 > ./logs/livejournal_p2.log_$(date -Iseconds)
./build/bin/baseline_test LiveJournal ~/dataset/livejournal_input 6 011111101000110111101010101101101010 > ./logs/livejournal_p3.log_$(date -Iseconds)
./build/bin/baseline_test LiveJournal ~/dataset/livejournal_input 6 011110101101110000110000100001010010 > ./logs/livejournal_p4.log_$(date -Iseconds)
./build/bin/baseline_test LiveJournal ~/dataset/livejournal_input 7 0111111101111111011101110100111100011100001100000 > ./logs/livejournal_p5.log_$(date -Iseconds)
./build/bin/baseline_test LiveJournal ~/dataset/livejournal_input 7 0111111101111111011001110100111100011000001100000 > ./logs/livejournal_p6.log_$(date -Iseconds)
./build/bin/baseline_test Orkut ~/dataset/orkut_input 5 0111010011100011100001100 > ./logs/orkut_p1.log_$(date -Iseconds)
./build/bin/baseline_test Orkut ~/dataset/orkut_input 6 011011101110110101011000110000101000 > ./logs/orkut_p2.log_$(date -Iseconds)
./build/bin/baseline_test Orkut ~/dataset/orkut_input 6 011111101000110111101010101101101010 > ./logs/orkut_p3.log_$(date -Iseconds)
./build/bin/baseline_test Orkut ~/dataset/orkut_input 6 011110101101110000110000100001010010 > ./logs/orkut_p4.log_$(date -Iseconds)
./build/bin/baseline_test Orkut ~/dataset/orkut_input 7 0111111101111111011101110100111100011100001100000 > ./logs/orkut_p5.log_$(date -Iseconds)
./build/bin/baseline_test Orkut ~/dataset/orkut_input 7 0111111101111111011001110100111100011000001100000 > ./logs/orkut_p6.log_$(date -Iseconds)
