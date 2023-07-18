import numpy as np
import matplotlib.pyplot as plt
import sys

TIMESTEP = 3.0

data = np.genfromtxt('./supercell_MSD.dat')

plt.figure(figsize = (8,6))

plt.plot(data[:,0] * 3.0, data[:,1], label = 'MSD');

title = sys.argv[1]
plt.title(f"Mean Squared Variance\n-{title}-")
plt.xlabel("time (fs)")
plt.ylabel("MSD")

plt.legend()
plt.grid()

plt.show()
