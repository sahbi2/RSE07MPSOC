#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_COORD 1000
#define NOISE_OVERSHOT (5./4)
#define MIN_CLUSTER_PROPORTION 0.3

#define max(a, b) (a) > (b) ? (a) : (b)
#define min(a, b) (a) < (b) ? (a) : (b)

int randInt(int min, int max);
int* randPoint(uint pointSize, uint maxCoordinate);
unsigned long distanceSquared(int* A, int* B, uint pointSize);
float norm(uint* p, uint pointSize);

int main(int argc, char* argv[])
{
	//Attente du nombre de points
	//		  du nombre de coordonnées
	//		  du nombre de clusters
	//		  de la présence de bruit
	if(argc < 5){
		printf("The syntax must be as follows:\n./header_generator [nbPoints] [nbCoordinates] [nbClusters] [NoiseProportion]");

		return -1;
	}

	srand(time(NULL));

	uint nbPoints 			= atoi(argv[1]),
		 nbCoordinates 	 	= atoi(argv[2]),
		 nbClusters 	 	= atoi(argv[3]),
		 NoiseProportion 	= atoi(argv[4]);

	//Génération des graines pour les clusters aléatoires dans l'espace
	int** clustersSeeds = malloc(nbClusters * sizeof(int*));
	for(int i = 0 ; i < nbClusters ; i++)
		clustersSeeds[i] = randPoint(nbCoordinates, MAX_COORD);

	//Calcul des distances inter-clusters
	unsigned long** interClustersDist = malloc(nbClusters * sizeof(long*));
	for(int i = 0 ; i < nbClusters ; i++)
		interClustersDist[i] = malloc(nbClusters * sizeof(long));
	
	for(int i = 0 ; i < nbClusters ; i++)
		for(int j = 0 ; j < nbClusters ; j++)
			interClustersDist[i][j] = (unsigned long) sqrt(
						distanceSquared(clustersSeeds[i],
										clustersSeeds[j],
										nbCoordinates));

	//Attribution des populations aux clusters
	uint noisePop = (int) nbPoints * NoiseProportion / 100.0,
		 clustersRemPop = nbPoints - noisePop;

	//Restrictions sur les populations des clusters
	uint minClusterPop = nbPoints / nbClusters * MIN_CLUSTER_PROPORTION, //Au moins 50% de la moyenne
		 maxClusterPop = clustersRemPop / 2;		//Au plus 50% du total
	
	uint*clustersPop = malloc(nbClusters * sizeof(int));
	for(int i = 0 ; i < nbClusters-1 ; i++){
		clustersPop[i] = randInt(minClusterPop, maxClusterPop);

		clustersRemPop -= clustersPop[i];
		maxClusterPop = min(maxClusterPop,
							clustersRemPop - (nbClusters - 1 - i) * minClusterPop);
	}
	clustersPop[nbClusters-1] = clustersRemPop;


	//Remplissage des clusters
	uint** data = malloc(nbPoints * sizeof(int*));
	int d = 0;
	for(int i = 0 ; i < nbClusters ; i++){
		//Calcul d'une borne assez large sur les coordonnées des points
		int maxDistToSeed = interClustersDist[i][0];
		for(int j = 1 ; j < nbClusters ; j++)
			maxDistToSeed = max(maxDistToSeed, interClustersDist[i][j]);
		maxDistToSeed /= 5;

		for(int j = 0 ; j < clustersPop[i] ; j++){
			//Créer un point aléatoirement en bornant grossièrement ses coordonnées
			//le décaler au voisinage du centre de son cluster,
			//vérifier qu'il est dans le bon cluster,
			//recommencer sinon.
			int* point = NULL;
			int goOn = 1;
			while(goOn){
				free(point);
				point = randPoint(nbCoordinates, maxDistToSeed);

				for(int k = 0 ; k < nbCoordinates ; k++)
					point[k] += clustersSeeds[i][k];

				long dist = distanceSquared(point, clustersSeeds[0], nbCoordinates);
				int index = 0;
				for(int l = 1 ; l < nbClusters ; l++){
					long newDist = distanceSquared(point, clustersSeeds[l], nbCoordinates);
					if(newDist < dist){
						dist = newDist;
						index = l;
					}
				}

				goOn = index != i;
			}
			data[d++] = point;
		}
	}
	//Points complètement aléatoires associés au bruit
	while(d < nbPoints)
		data[d++] = randPoint(nbCoordinates, MAX_COORD * NOISE_OVERSHOT);


	//Print a text file to vizualize in matlab and a header
	FILE* text 	 = fopen("data.txt", "w");
	FILE* header = fopen("data.h", 	 "w");
	if(!text || ! header){
		printf("Error while opening the output files.\n");
		return -2;
	}

	fprintf(header, "#ifndef _DATA_H_\n#define _DATA_H_\n\n");
	fprintf(header, "static const uint nbPoints = %u;\n", nbPoints);
	fprintf(header, "static const uint nbCoordinates = %u;\n", nbCoordinates);
	fprintf(header, "static const uint nbClusters = %u;\n\n", nbClusters);

	fprintf(header, "static const uint data[%u][%u] = \n{", nbPoints, nbCoordinates);

	for(int i = 0 ; i < nbPoints ; i++){
		fprintf(header, "{");
		for(int j = 0 ; j < nbCoordinates ; j++){
			fprintf(text, "%u ", data[i][j]);
			fprintf(header, "%u, ", data[i][j]);
		}
			fprintf(text, "\n");
			fseek(header, -2, SEEK_CUR);
			fprintf(header, "},\n");
	}
	fseek(header, -2, SEEK_CUR);
	fprintf(header, "};\n");
	fprintf(header, "\n#endif //_DATA_H_\n");

	// for(int i = 0 ; i < nbClusters ; i++){
	// 	for(int j = 0 ; j < nbClusters ; j++)
	// 		printf("%ld ", interClustersDist[i][j]);
	// 	printf("\n");
	// }

	//Libération de la mémoire
	for(int i = 0 ; i < nbClusters ; i++)
		free(clustersSeeds[i]);
	free(clustersSeeds);
	for(int i = 0 ; i < nbPoints ; i++)
		free(data[i]);
	free(data);
	free(clustersPop);
	free(interClustersDist);

	fclose(text);
	fclose(header);

	return 0;
}


int randInt(int min, int max)
{
	return (rand() % (max - min + 1) + min);
}

int* randPoint(uint pointSize, uint maxCoordinate)
{
	int* genPoint = malloc(pointSize * sizeof(int));
	
	for(int i = 0 ; i < pointSize ; i++)
		genPoint[i] = randInt(0, maxCoordinate);

	return genPoint;
}

unsigned long distanceSquared(int* a, int* b, uint pointSize)
{
	int distance = 0;
	for(int i = 0 ; i < pointSize ; i++){
		int diff = a[i] - b[i];
		distance += diff*diff;
	}

	return distance;
}

float norm(uint* p, uint pointSize)
{
	long normSquared = 0 ;
	for(int i = 0 ; i < pointSize ; i++)
		normSquared += p[i] * p[i];

	return sqrt(normSquared);
}