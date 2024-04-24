#!/bin/bash

echo "arrivalRate, serviceTime, throughput, turnaroundTime, core0_util, core1_util, core2_util, core3_util, avg_rqLen" > cmh171s2.txt

for ((i=50; i<=150; i+=10)); do
    echo "Running ./p2 with arguments: $i 0.02 2 4"
    ./p2 $i 0.02 2 4
done