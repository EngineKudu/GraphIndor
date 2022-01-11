#!/bin/bash
set -x

../build/bin/baseline_test ~/dataset/citeseer.g 3 011101110 > ./citeseer.g_motif_p1.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/citeseer.g 3 011100100 > ./citeseer.g_motif_p2.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/citeseer.g 4 0111101111011110 > ./citeseer.g_motif_p3.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/citeseer.g 4 0111101111001100 > ./citeseer.g_motif_p4.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/citeseer.g 4 0111101011001000 > ./citeseer.g_motif_p5.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/citeseer.g 4 0111100010001000 > ./citeseer.g_motif_p6.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/citeseer.g 4 0110100110010110 > ./citeseer.g_motif_p7.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/citeseer.g 4 0110100110000100 > ./citeseer.g_motif_p8.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/wiki-vote.g 3 011101110 > ./wiki-vote.g_motif_p1.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/wiki-vote.g 3 011100100 > ./wiki-vote.g_motif_p2.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/wiki-vote.g 4 0111101111011110 > ./wiki-vote.g_motif_p3.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/wiki-vote.g 4 0111101111001100 > ./wiki-vote.g_motif_p4.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/wiki-vote.g 4 0111101011001000 > ./wiki-vote.g_motif_p5.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/wiki-vote.g 4 0111100010001000 > ./wiki-vote.g_motif_p6.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/wiki-vote.g 4 0110100110010110 > ./wiki-vote.g_motif_p7.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/wiki-vote.g 4 0110100110000100 > ./wiki-vote.g_motif_p8.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/patents.g 3 011101110 > ./patents.g_motif_p1.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/patents.g 3 011100100 > ./patents.g_motif_p2.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/patents.g 4 0111101111011110 > ./patents.g_motif_p3.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/patents.g 4 0111101111001100 > ./patents.g_motif_p4.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/patents.g 4 0111101011001000 > ./patents.g_motif_p5.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/patents.g 4 0111100010001000 > ./patents.g_motif_p6.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/patents.g 4 0110100110010110 > ./patents.g_motif_p7.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/patents.g 4 0110100110000100 > ./patents.g_motif_p8.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/mico.g 3 011101110 > ./mico.g_motif_p1.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/mico.g 3 011100100 > ./mico.g_motif_p2.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/mico.g 4 0111101111011110 > ./mico.g_motif_p3.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/mico.g 4 0111101111001100 > ./mico.g_motif_p4.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/mico.g 4 0111101011001000 > ./mico.g_motif_p5.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/mico.g 4 0111100010001000 > ./mico.g_motif_p6.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/mico.g 4 0110100110010110 > ./mico.g_motif_p7.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/mico.g 4 0110100110000100 > ./mico.g_motif_p8.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/livejournal.g 3 011101110 > ./livejournal.g_motif_p1.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/livejournal.g 3 011100100 > ./livejournal.g_motif_p2.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/livejournal.g 4 0111101111011110 > ./livejournal.g_motif_p3.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/livejournal.g 4 0111101111001100 > ./livejournal.g_motif_p4.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/livejournal.g 4 0111101011001000 > ./livejournal.g_motif_p5.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/livejournal.g 4 0111100010001000 > ./livejournal.g_motif_p6.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/livejournal.g 4 0110100110000100 > ./livejournal.g_motif_p8.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/orkut.g 3 011101110 > ./orkut.g_motif_p1.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/orkut.g 3 011100100 > ./orkut.g_motif_p2.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/orkut.g 4 0111101111011110 > ./orkut.g_motif_p3.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/orkut.g 4 0111101111001100 > ./orkut.g_motif_p4.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/orkut.g 4 0111101011001000 > ./orkut.g_motif_p5.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/orkut.g 4 0111100010001000 > ./orkut.g_motif_p6.log_$(date -Iseconds)
../build/bin/baseline_test ~/dataset/orkut.g 4 0110100110000100 > ./orkut.g_motif_p8.log_$(date -Iseconds)
