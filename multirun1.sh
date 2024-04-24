#!/bin/bash

echo "arrivalRate, serviceTime, throughput, turnaroundTime, core0_util, core1_util, core2_util, core3_util, avg_core0_rqLen, avg_core1_rqLen, avg_core2_rqLen, avg_core3_rqLen" > cmh171s1.txt

for ((i=50; i<=150; i+=10)); do
    echo "Running ./p2 with arguments: $i 0.02 1 4"
    ./p2 $i 0.02 1 4
done