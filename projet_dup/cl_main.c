#include<stdio.h>
#include<stdlib.h>
#include "cluster.h"
#include "cluster.c"
#include <math.h>
#include <time.h>
#include "data.h"
#include<string.h>

#define IterMAX 10
#define inf INFINITY


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
       // printf("data_x =%f",data_x[pos]);
    }
}

int sampleCluster_idx[nbPoints];
int ctrNoMove, doKmeans=0;
printf("\n===================begin K-means Algorithm =================================\n");
//Compute
do{
fflush(stdin); 
doKmeans+=1;
ctrNoMove=nbCluster*nbPoints;

tempCentroids = malloc(nbCluster*nbCoordinates*sizeof(float));

//save the old centroids
for(c=0;c<nbCluster;c++){
    for(d==0;d<nbCoordinates;d++){
         tempCentroids[d+c*nbCoordinates]=centroids[d+c*nbCoordinates];
     }
}

//cluster assignment step
int sampleIdx=0;
    while(sampleIdx<nbPoints)
    
    {
        
        
        float distXCluster[nbCluster];
        for(c=0;c<nbCluster;c++)
        {
          float distance;
          distance=0;
          for(d=0; d<nbCoordinates; d++)
          {
            int pos_d;
            pos_d= d+c*nbCoordinates;
            distance += distEuclid(data_x[sampleIdx*nbCoordinates+d],tempCentroids[pos_d]);
          }
          distance = sqrt(distance);
          distXCluster[c] = distance;
        }
        sampleCluster_idx[sampleIdx] = minItem(distXCluster, nbCluster);
        sampleIdx++;   
        
        }

//Move centroids
int clusterIdx;
for (clusterIdx=0; clusterIdx<nbCluster; clusterIdx++){
    
    float acc[nbCoordinates];
    int counter;
    counter=0;
    for (c=0; c<nbPoints; c++){
        
        if (sampleCluster_idx[c] == clusterIdx){
            counter+=1;
            for(d=0;d<nbCoordinates;d++)
            {
              int pos_x;
              pos_x=d+c*nbCoordinates;
              acc[d]+=data_x[pos_x];
            }
        }
        }    
    for (d=0;d<nbCoordinates;d++)
    {
     
        tempCentroids[d+clusterIdx*nbCoordinates]=acc[d]/(float)counter;
        
    }
        
    }
    

    
// restore centroids and testing if there is difference between vectors
for(c=0;c<nbCluster;c++){
    for(d==0;d<nbCoordinates;d++){
        if (centroids[d+c*nbCoordinates]== tempCentroids[d+c*nbCoordinates]) ctrNoMove-=1;
        centroids[d+c*nbCoordinates]= tempCentroids[d+c*nbCoordinates];
    }
} 
printf("ctrNoMove= %d , \n", ctrNoMove);
}while( ctrNoMove >0 && doKmeans < IterMAX+1);
printf("\n========================the result of K-means Algorithm ==========================\n");

 
//affichage
  int clusteIdx;
 for (clusteIdx=0;clusteIdx<nbCluster;clusteIdx++)
 {
     for(d=0;d<nbCoordinates;d++)
     {
      int posCt=d+clusteIdx*nbCoordinates;
      printf("centroid i:%d,j:%d=  %d, ",clusteIdx,d,(int)centroids[posCt]); 
     }
     printf("\n");
 }
//print results in a txt file Print a text file to vizualize in matlab and a header
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
    else
    {
        printf("Impossible d'ouvrir le fichier centroids.txt");
    }


  fclose(dataOut); 
//desallocate data_x

free(data_x);
free(tempCentroids);

free(centroids);

return 0;
}