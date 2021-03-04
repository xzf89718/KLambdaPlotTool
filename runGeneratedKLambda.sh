#!/bin/bash

list_KLambda="3.0 5.0 -20.0 -10.0 -5.0 -1.0 0.0"
for KLambda in $list_KLambda
do
        echo "now we are woking on Klambda= "$KLambda
        root -l -b -q GenerateKLambaSamples.C+\($KLambda\)
done
