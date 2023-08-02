#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BIN_NUMBER 300
#define TIME_GAP 100
#define	EQ_LIMIT 300 // First 400 step is for equilibration

typedef struct{
	long double x;
	long double y;
	long double z;
	int atomId;
	int atomType;
} atom;

typedef struct{
	int step;
	atom particle[100];
} timestep;

typedef struct{
	long double x;
	long double y;
	long double z;
} vector;

vector a, b, c;
int numParticle;
int maxTimestep;
timestep trajectory[100000];
timestep *perTime;

/*
	Timestep starts from 1, atomId starts from 0.
	I know it's fucked up, but VASP did it.
	Don't blame me.
*/

long double getDisplacement(int atomId, int this, int next);

int main(int argc, char* argv[])
{
	// Argument check
	if (argc != 2){
		printf("\tUSAGE : ./supercell_MSD XDATCAR\n");
		exit(1);
	}
	FILE *input = fopen(argv[1], "r");
	
	// File check
	if (input != NULL)
		printf("Successfully loaded %s.\n", argv[1]);
	else{
		printf("Something wrong with opening %s.\n", argv[1]);
		printf("Terminating.");
		exit(2);
	}

	// Reading Trajectory
	char line[200];
	int thisStep = 1;

	int numLi, numAl, numCl;

	while(fgets(line, sizeof(line), input)){
		// printf("line : %s", line);
		if (strncmp("Direct configuration=", line, strlen("Direct configuration=")) == 0){
			perTime = (timestep*)malloc(sizeof(timestep));
			perTime->step = thisStep;
			for (int i = 0; i < numParticle; i++){
				fscanf(input, "%Lf %Lf %Lf",  
					&perTime->particle[i].x,
					&perTime->particle[i].y,
					&perTime->particle[i].z);
				perTime->particle[i].atomId = i;
				if (i < numLi)
					perTime->particle[i].atomType = 0;
				else if (i >= numLi && i < numLi+numAl)
					perTime->particle[i].atomType = 1;
				else if (i >= numLi + numAl)
					perTime->particle[i].atomType = 2;
			}
			trajectory[thisStep++] = *perTime;
			free(perTime);
		}
		else if (strcmp("   Li   Al   Cl\n", line) == 0){
			fscanf(input, "%d %d %d", &numLi, &numAl, &numCl);
			numParticle = numLi + numAl + numCl;
			// printf("numParticle : %d\n", numParticle);
		}
		else if (strcmp("   Li   Cl   Al\n", line) == 0){
			fscanf(input, "%d %d %d", &numLi, &numCl, &numAl);
			numParticle = numLi + numAl + numCl;
			// printf("numParticle : %d\n", numParticle);
		}
		else if (strcmp("           1\n", line) == 0){
			fscanf(input, "%Lf %Lf %Lf",
					&a.x, &a.y, &a.z);
			fscanf(input, "%Lf %Lf %Lf",
					&b.x, &b.y, &b.z);
			fscanf(input, "%Lf %Lf %Lf",
					&c.x, &c.y, &c.z);
			/*
			printf("a : %Lf %Lf %Lf\n", a.x, a.y, a.z);
			printf("b : %Lf %Lf %Lf\n", b.x, b.y, b.z);
			printf("c : %Lf %Lf %Lf\n", c.x, c.y, c.z);
			*/
		}
	}

	maxTimestep = thisStep-1;
	
	printf("Reading trajectory from \"%s\" is complete.\n", argv[1]);
	printf("Number of Particles : %d\n", numParticle);
	printf("Maximum Timestep : %d\n", maxTimestep);

	// Output file
	FILE *output = fopen("./supercell_MSD.dat","w");
	if (output != NULL){
		fprintf(output, "#\ttimestep\tmsd\tmsd_Li\n");
	}
	else{
		printf("Error occured while opening \"supercell_MSD.dat\" file.\n");
		exit(4);
	}

	long double squared[100000];
	long double squared_Li[100000];
	int numData[100000] = { 0 };
	int numData_Li[100000] = { 0 };
	long double temp, temp_Li;

	/*
		i : timegap
		j : timegap starting point
		k : particle index 
	*/

	for (int i = 1; i < maxTimestep - EQ_LIMIT; i++){
		squared[i] = 0.0;
		squared_Li[i] = 0.0;
		if (i % 100 == 0) printf("Calculating MSD for t = %d...\n", i);
		for (int j = 1; j <= maxTimestep - i; j++){
			if (j < EQ_LIMIT) continue;
			temp = 0.0; temp_Li = 0.0;
			for (int k = 0; k < numParticle; k++){
				temp += getDisplacement(k, j, j+i);
				if (trajectory[1].particle[k].atomType == 0)
					temp_Li += getDisplacement(k, j, j+i);
			}
			temp /= numParticle;
			temp_Li /= numLi;
			squared[i] += temp;
			squared_Li[i] += temp_Li;
			numData[i] += 1; numData_Li[i] += 1;
		}
		squared[i] /= numData[i];
		squared_Li[i] /= numData_Li[i];
		fprintf(output, "%d\t%10.8Lf\t%10.8Lf\n", i, squared[i], squared_Li[i]);
	}

	printf("Done!\n");

	fclose(input);
	fclose(output);

	return 0;
}

long double getDisplacement(int atomId, int this, int next){
	atom thisAtom, nextAtom;
	thisAtom = trajectory[this].particle[atomId];
	nextAtom = trajectory[next].particle[atomId];

	long double displacement = 0;
	long double da, db, dc;
	long double dx, dy, dz;
	
	da = thisAtom.x - nextAtom.x;
	db = thisAtom.y - nextAtom.y;
	dc = thisAtom.z - nextAtom.z;

	dx = a.x * da + b.x * db + c.x * dc;
	dy = a.y * da + b.y * db + c.y * dc;
	dz = a.z * da + b.z * db + c.z * dc;

	displacement = dx * dx + dy * dy + dz * dz;

	return displacement;
}
