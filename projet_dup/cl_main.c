// *********************************************************************************************************
// *                               Projet RSE07 : Algorithme K-means                                       *
// *                                       Sahbi MALLOULI                                                  *
// *********************************************************************************************************
#include<stdio.h>
#include<stdlib.h>
#include "cluster.h"
#include "cluster.c"
#include <math.h>
#include <time.h>
#include "data.h"
#include<string.h>



int main( int argc, char * argv[])
{
fflush(stdin);


// INPUTS 
printf("\n===============================================================================\n");
printf("\n Data set feautres: nbPoints= %d, nbCoordinates= %d \n", nbPoints,nbCoordinates);
printf("\n===============================================================================\n");    
//data
float * data_x;
data_x= malloc(nbPoints*nbCoordinates*sizeof(float));
int c,d;
for( c=0;c<nbPoints; c++)
    for(d=0;d<nbCoordinates; d++){
        int pos;
        pos=d+ c*nbCoordinates;
        data_x[pos]= (float)data[c][d];
    }
srand(time(NULL));
int r;
int randIdx[nbCluster];
randIdx[0]=rand() % nbPoints;
for(r=1;r<nbCluster;r++){
    fflush(stdin);
    int rd=rand() % nbPoints;
    if (rd!=randIdx[r-1]) randIdx[r]=rd;
}
   
float *centroids, *tempCentroids;
centroids = malloc(nbCluster*nbCoordinates*sizeof(float));

for( c=0;c<nbCluster; c++){
    for(d=0;d<nbCoordinates; d++)
    {
       int posCt ;
       posCt= d + c*nbCoordinates;
       centroids[posCt]= (float)data[randIdx[c]][d];
    }
}
 printf("\n===================================================\n");
int sampleCluster_idx[nbPoints];
int ctrNoMove, doKmeans=0;
printf("\n===================begin K-means Algorithm =================================\n");
//Compute K-means algorithm
do{

    doKmeans+=1;
    ctrNoMove=0;

    tempCentroids = malloc(nbCluster*nbCoordinates*sizeof(float));

//save the old centroids
    for(c=0;c<nbCluster;c++){
        for(d=0;d<nbCoordinates;d++){
            tempCentroids[d+c*nbCoordinates]=centroids[d+c*nbCoordinates];
        }
    }
//cluster assignment step
    int sampleIdx;
    for (sampleIdx=0; sampleIdx < nbPoints; sampleIdx++){ 
        double distXCluster[nbCluster];
        for(c=0;c<nbCluster;c++){
        double distance;
            distance=0;
            for(d=0; d<nbCoordinates; d++){ 
                distance += distEuclid(data_x[sampleIdx*nbCoordinates+d],tempCentroids[d+c*nbCoordinates]);
            }
            distXCluster[c] = sqrt(distance);
        }
        sampleCluster_idx[sampleIdx] = minItem(distXCluster, nbCluster);
    }
    
//Move centroids
    int clusterIdx=0;
    while( clusterIdx < nbCluster){
        double acc[nbCoordinates];
        int counter=0;
        for (c=0; c < nbPoints; c++){
            if (sampleCluster_idx[c] == clusterIdx){
                counter+=1;
                for(d=0;d < nbCoordinates;d++)
                {
                    acc[d]+=data_x[d+c*nbCoordinates];
                }
            }
        } 
        if (counter != 0){
            for (d=0;d < nbCoordinates;d++)
            {
                printf("\nacc[%d]=%2f ,with counter[%d]=%d ,\n",d,acc[d],clusterIdx,counter);
                tempCentroids[d+clusterIdx*nbCoordinates]=acc[d]/(float)counter;
                acc[d]=0;
            }
        }
        clusterIdx++;
    }
// restore centroids and testing if there is difference between vectors
    for(c=0;c<nbCluster;c++){
        for(d=0;d<nbCoordinates;d++){
            if (centroids[d+c*nbCoordinates]== tempCentroids[d+c*nbCoordinates]) ctrNoMove+=1;
            centroids[d+c*nbCoordinates]= tempCentroids[d+c*nbCoordinates];
        }
    } 
}while( ctrNoMove < nbCluster*nbCoordinates);
printf("\n========================the result of K-means Algorithm ==========================\n");
printf("Nombre d iteration= %d , \n", ctrNoMove);
//affichage des coordonnées des centroids
int clusteIdx;
for (clusteIdx=0;clusteIdx<nbCluster;clusteIdx++){
    for(d=0;d<nbCoordinates;d++){
        int posCt=d+clusteIdx*nbCoordinates;
        printf("centroid i:%d,j:%d=  %d, ",clusteIdx,d,(int)centroids[posCt]); 
    }
    printf("\n");
}
//print results in a txt file to plot it in matlab 
FILE* dataOut = NULL;
dataOut = fopen("centroids.txt", "w");
if (dataOut != NULL){
    for (clusteIdx=0;clusteIdx<nbCluster;clusteIdx++){
        for(d=0;d<nbCoordinates;d++){
            int posCt=d+clusteIdx*nbCoordinates;
            fprintf(dataOut,"%d ",(int)centroids[posCt]); 

        }   
        fputc('\n', dataOut);
    }
}
else{
    printf("Impossible d'ouvrir le fichier centroids.txt");
}


 
//housekeeping
 fclose(dataOut); 
free(data_x);
free(tempCentroids);
free(centroids);
// main results
return 0;
}
