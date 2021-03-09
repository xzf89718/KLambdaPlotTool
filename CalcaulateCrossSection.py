#!/usr/bin/python
import numpy as np
import sympy as sp
import matplotlib.pyplot as plt
def GetCrossSection(int_KLambda):
    """
    Calculate cross section using:
    simga = 70.3874 - 50.4111 * KLambda + 11.0595 * KLambda ^2 in fb

    input: Klambda
    output: CrossSection in different KLambda
    """

    return 70.3874 - 50.4111 * int_KLambda  + 11.0595 * int_KLambda**2

def Getkone(int_kappa_lambda):

    return ((20**2 - 20) + (20 - 1) * int_kappa_lambda**2 - (20**2 -1)*int_kappa_lambda) / (20**2 -20)


if __name__ == '__main__':
    output_string = "Cross Section for KLambda {0} is: {1}"
    list_KLambda = [1.0, 10.0, 20.0]
    for KLambda in list_KLambda:
        print(output_string.format(str(KLambda), GetCrossSection(KLambda)))
    for i in range(-20, 20):
        print(Getkone(i))

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


    # solve the equation
    A,B,C=sp.symbols(['A', 'B','C'])
    sigma1,sigma2,sigma3=sp.symbols(['sigma1','sigma2','sigma3'])
    kappa_lambda = sp.symbols(['kappa_lambda'])
    rst=sp.solve([A+B+C-sigma1, A+10*B+100*C-sigma2,A+20*B+400*C-sigma3],[A,B,C])
    print(rst)
    # solve equation for kappa = 0, 1, 20
    rst2=sp.solve([A-sigma1, A+B+C-sigma2, A+kappa_lambda*B+kappa_lambda*kappa_lambda*C-simga3], [A,B,C])
    print(rst2)

    # Let's draw the Cross-section 
    list_KLambda = np.linspace(-20, 20, 1000)
    list_Cross = GetCrossSection(list_KLambda)

    plt.plot(list_KLambda, list_Cross)
    plt.show()


    for i in range(-20, 20):
        print(Getkone(i))
