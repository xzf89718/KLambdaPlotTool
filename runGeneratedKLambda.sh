#!/bin/bash
list_KLambda="-20.0 -19.0 -18.0 -17.0 -16.0 -15.0 -14.0 -13.0 -12.0 -11.0 -10.0 -9.0 -8.0 -7.0 -6.0 -5.0 -4.0 -3.0 -2.0 -1.0"
for KLambda in $list_KLambda
do
        echo "now we are woking on Klambda= "$KLambda
        root -l -b -q GenerateKLambaSamples.C+\($KLambda\)
done

unset list_KLambda

list_KLambda="20.0 19.0 18.0 17.0 16.0 15.0 14.0 13.0 12.0 11.0 10.0 9.0 8.0 7.0 6.0 5.0 4.0 3.0 2.0 1.0 0.0"
for KLambda in $list_KLambda
do
        echo "now we are woking on Klambda= "$KLambda
        root -l -b -q GenerateKLambaSamples.C+\($KLambda\)
done
unset list_KLambda

#list_KLambda="-1.4 -1.2 -0.8 -0.6 -0.4 -0.2 1.2 1.4 1.6 1.8 2.2 2.4 2.6 2.8 3.2 3.4 3.6 3.8 4.2 4.4 4.6 4.8 5.2 5.4"
for KLambda in $list_KLambda
do
        echo "now we are woking on Klambda= "$KLambda
        root -l -b -q GenerateKLambaSamples.C+\($KLambda\)
done
unset list_KLambda
