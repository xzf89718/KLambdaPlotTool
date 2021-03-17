#!usr/bin/python3
import matplotlib.pyplot as plt
import numpy as np

def GetTruthK(KLambda):
    
    return (70.3874 - 50.4111 * KLambda + 11.0595 * KLambda **2)# / (70.3874 - 50.4111 * 1. + 11.0595 * 1. **2)

def my_pow(KLambda, times):
    return KLambda ** times
    
def GetKs(KLambda):
    Kappa_t = 1.0
    d_lambda = 20.0
    k1 = Kappa_t * Kappa_t * \
              ((my_pow(d_lambda, 2) - d_lambda) * my_pow(Kappa_t, 2) + \
               (d_lambda - 1) * my_pow(KLambda, 2) - (my_pow(d_lambda, 2) -1.) * Kappa_t * KLambda) / (my_pow(d_lambda, 2) - d_lambda)
    k2 = Kappa_t * Kappa_t * \
              (my_pow(d_lambda, 2) * Kappa_t * KLambda - d_lambda * my_pow(KLambda, 2)) / (my_pow(d_lambda, 2) - d_lambda)
    k3 = Kappa_t * Kappa_t * \
              (my_pow(KLambda, 2) - Kappa_t * KLambda) / (my_pow(d_lambda, 2) - d_lambda)

    return k1, k2, k3

list_kt = []
list_combine = []
list_k1 = []
list_k2 = []
list_k3 = []
for i in np.linspace(-20., 20., 100):
    k1,k2,k3 = GetKs(i)
    list_k1.append(k1 * GetTruthK(0.))
    list_k2.append(k2 * GetTruthK(1.))
    list_k3.append(k3 * GetTruthK(20.))
    list_combine.append(np.sqrt((k1*GetTruthK(0.))**2 + (k2*GetTruthK(1.))**2 + (k3*GetTruthK(20.))**2))
    list_kt.append(GetTruthK(i))

# plt.plot(np.linspace(-20, 20, 100), list_k1, label='k1')
# plt.plot(np.linspace(-20, 20, 100), list_k2, label='k2')
# plt.plot(np.linspace(-20, 20, 100), list_k3, label='k3')
plt.plot(np.linspace(-20, 20, 100), list_kt, label='kt')
plt.plot(np.linspace(-20, 20, 100), list_combine, label='kcombined')
plt.legend()
plt.xlabel(r'$\kappa_{\lambda}$')
plt.ylabel('Arbitrary Unit')
plt.title('Error propagate and k1,k2,k3,kt,kcombined')
plt.show()
