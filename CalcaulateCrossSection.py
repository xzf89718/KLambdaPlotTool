#!/usr/bin/python
import numpy as np

def GetCrossSection(int_KLambda):
    """
    Calculate cross section using:
    simga = 70.3874 - 50.4111 * KLambda + 11.0595 * KLambda ^2 in fb

    input: Klambda
    output: CrossSection in different KLambda
    """

    return 70.3874 - 50.4111 * int_KLambda  + 11.0595 * int_KLambda**2


if __name__ == '__main__':
    output_string = "Cross Section for KLambda {0} is: {1}"
    list_KLambda = [1.0, 10.0, 20.0]
    for KLambda in list_KLambda:
        print(output_string.format(str(KLambda), GetCrossSection(KLambda)))


    # Solve the equation using numpy!
    # KT = 1
    # KL = 1, 10, 20   
    list_coefficient = [[1, 0, 0],
                        [1, 10, 100],
                        [1, 20, 400]]
    list_rightside = [GetCrossSection(1.0), GetCrossSection(10.0), GetCrossSection(20.0)]
    array_co = np.array(list_coefficient)
    array_ri = np.array(list_rightside)
    x = np.linalg.solve(array_co, array_ri)
    print(x)
