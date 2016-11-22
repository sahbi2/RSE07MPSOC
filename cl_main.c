#include<stdio.h>
#include<stdlib.h>
#include "cluster.h"
#include "cluster.c"
#include <math.h>
#include <time.h>
#include "data.h"
#include<string.h>

#define EPS 0.01


int main( int argc, char * argv[])
{
// INPUTS 
//data set features:
printf("\n=========================================================================\n");
printf("\n Data set feautres: nbPoints= %d, nbCoordinates= %d \n", nbPoints,nbCoordinates);
printf("\n=========================================================================\n");    
    
float * data_x;
data_x= malloc(nbPoints*nbCoordinates*sizeof(float));
//memcpy(data_x,data, nbPoints*nbCoordinates*sizeof(data[0][0]));

int c,d;
for( c=0;c<nbPoints; c++)
    for(d=0;d<nbCoordinates; d++){
        int pos=d+ c*nbCoordinates;
        data_x[pos]= (float)data[c][d];
        //printf("data_x =%f",data_x[pos]);
    }
// initialisation
 // centroids

  srand(time(NULL));

 float *centroids;
 centroids= malloc(k*nbCoordinates*sizeof(float));
 

for( c=0;c<k; c++){
    int randomIndex = rand() % nbPoints;
    for(d=0;d<nbCoordinates; d++){
         int pos=d+ c*nbCoordinates;
         centroids[pos]= (float)data[randomIndex][d];
         //printf("data_x =%f",data_x[pos]);
     }
 }
float sum=0;
float min=0;
for(d=0;d<nbCoordinates;d++){
    min+=minDistance(data_x[d],centroids[d]);
}

int sampleCluster_idx[nbPoints];
int budget=1;
//Compute
 while (budget>0){
//cluster assignment step
int sampleIdx=0;
    while(sampleIdx<nbPoints){
        float distXCluster[k];
        
        for(c=0;c<k;c++){
            float distance=0;
            for(d=0;d<nbCoordinates;d++){
                int pos_d= d+c*nbCoordinates;
                distance+=sqrt(minDistance(data_x[sampleIdx*nbCoordinates+d],centroids[pos_d]));
            }
        distXCluster[c]=distance;
        }
    
        sampleCluster_idx[sampleIdx]=mintab(distXCluster);
        sampleIdx++;   
     }
int s;
for(s=0;s<nbPoints;s++)
    printf("sampleCluster_idx,%d= %d\n",s,sampleCluster_idx[s]);
//Move centroids
int clusteIdx;
for (clusteIdx=0; clusteIdx<k; clusteIdx++){
   float acc[nbCoordinates];
   int counter=0;
    for (c=0; c<nbPoints; c++){
        if (sampleCluster_idx[c] == clusteIdx){
            counter+=1;
            for(d=0;d<nbCoordinates;d++){
            int pos_x=d+c*nbCoordinates;
            acc[d]+=data_x[pos_x];
            }
        }
    }
for (d=0;d<nbCoordinates;d++)
    centroids[d+clusteIdx*nbCoordinates]=acc[d]/(float)counter;

} 
budget-=1;
}
//affichage
 int clusteIdx;
for (clusteIdx=0;clusteIdx<k;clusteIdx++){
    for(d=0;d<nbCoordinates;d++){
    int posCt=d+clusteIdx*nbCoordinates;
      printf("centroid i:%d,j:%d=  %f,",clusteIdx,d,centroids[posCt]); 
     }
    printf("\n");
}

//print results in a txt file Print a text file to vizualize in matlab and a header
/*FILE* dataOut fopen("centroid_coordinates.txt", "w");
    if(!dataOut){ 
    printf("Error while opening the output file.\n");
    return -2;
    }

for(c=0;c<k;c++){
    for(d=;d<nbCoordinates;d++){
        fprintf(dataOut, "%d", (int) centroids[d+c*nbCoordinates]);
    } 
   fputc("\n", dataOut);
// }
  fclose(dataOut);*/ 
//desallocate data_x
free(data_x);
free(centroids);
return 0;
}