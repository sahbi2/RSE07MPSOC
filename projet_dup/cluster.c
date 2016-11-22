#include <stdio.h>
#include "cluster.h"
#include<math.h>



float distEuclid(float a, float b)
{
    fflush(stdin);
	//float test = (a-b)*(a-b);
	// printf("%f\n", test);
	return (a-b) * (a-b);
}


int minItem(float dist_x[], int k){
        fflush(stdin);
	float minimum ;
	minimum= dist_x[0];
        //printf("minimum here= %f",minimum);

	int ci;
	int i;
        i=1;ci=0;
	while(i<k)
	{
		if (dist_x[i] < minimum)
		{
			minimum = dist_x[i];
			ci=i;
		}
		i++;
	}
	// if (minimum==0)
	// {
	// 	// printf("k=%d\n", k);
	// 	// printf("i=%d\n", i);
	// 	printf("ci=%d\n", ci);
	// }
	return ci;
}