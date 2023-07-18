import numpy as np
import matplotlib.pyplot as plt
import sys

TIMESTEP = 3.0

data = np.genfromtxt(sys.argv[1])

plt.figure(figsize = (8,6))

for i in range(100,1001,100):
    interval = data[data[:,0] == i][:,[1,2]]
    plt.plot(interval[:,0], interval[:,1],
             label = f'{int(i*TIMESTEP)} fs', alpha = 0.5)

title = sys.argv[2]
plt.title(f"Radial Distribution Function\n-{title}-")
plt.xlabel("r ($10^{-10}$ m)")
plt.ylabel("RDF")

plt.legend()
plt.grid()

plt.show()
