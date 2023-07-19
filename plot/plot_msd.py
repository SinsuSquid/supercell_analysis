import numpy as np
import matplotlib.pyplot as plt
import sys

TIMESTEP = 3.0

plt.figure(figsize = (8,6))

data = np.genfromtxt(sys.argv[1])

plt.plot(data[:,0] * 3.0, data[:,1], label = 'All Atoms');
plt.plot(data[:,0] * 3.0, data[:,2], label = 'Li+ ions');

title = sys.argv[2]
plt.title(f"Mean Squared Variance\n-{title}-")
plt.xlabel("time (fs)")
plt.ylabel("MSD")

plt.legend()
plt.grid()

plt.show()
