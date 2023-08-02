import numpy as np
import matplotlib.pyplot as plt
import sys

TIMESTEP = 3.0

ANALYSIS_DIRECTORY = "/SSD2/bgkang/PROJECTS/CURRENT_PROJECTS/supercell_analysis/"

data = []

data.append(np.genfromtxt(ANALYSIS_DIRECTORY + "crystal.dat"))
data.append(np.genfromtxt(ANALYSIS_DIRECTORY + "tetrahedra.dat"))
data.append(np.genfromtxt(ANALYSIS_DIRECTORY + "random.dat"))

plt.figure(figsize = (8,6))

for i in range(len(data)):
    title = "From Crystal" if i == 0 else "From Tetrahedra" if i == 1 else "From Random"
    plt.plot(data[i][:,0] * TIMESTEP, data[i][:,1], label = title)

plt.title(f"Radial Distribution Function")
plt.xlabel("r ($10^{-10}$ m)")
plt.ylabel("RDF")

plt.legend()
plt.grid()

plt.show()
