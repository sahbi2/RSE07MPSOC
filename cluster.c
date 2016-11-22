#include <stdio.h>
#include "cluster.h"

void myPrintHelloMake(void) {

  printf("Hello makefiles!\n");

  return;
}

float minDistance(float a, float b){
    return (a-b)*(a-b);
}


int mintab(float dist_x[]){
  float minimum=dist_x[0];
  
  int ci=0;
  int i=1;
while(i<k){
    if (dist_x[i] < minimum)  {minimum=dist_x[i]; ci=i;}
    else i++;

    
}

return ci;

}