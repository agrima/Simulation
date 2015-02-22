#include<stdlib.h>
#include<stdio.h>
#include<math.h>
int main() {

	FILE *ptr_file;
	double rho;
	double var[25];
	int i;

	ptr_file =fopen("abc.txt","r");
	if (!ptr_file)
		return 1;
	fscanf(ptr_file, "%lf", &rho);
	for(i = 0; i < 25; i++)
		fscanf(ptr_file, "%lf", &var[i]);

	fclose(ptr_file);
	printf("%lf ", rho);
	int sum=0;
	for(i = 0; i < 25; i++){
		sum+=sum+var[i];
	}
	double mean;
	mean=sum/25;
	float sd=0.0;
	int j;
	
	for(j=0;j<25; j++){
	sd+= pow((mean-var[j]),2);
	
	}
	sd=pow((sd/25),0.5);
	printf("%lf",rho);
	printf("%lf",mean);
	double z1=(mean-(1.96*(sd/pow(25,0.5))));
	printf("%lf",z1);
	double z2=(mean+(1.96*(sd/pow(25,0.5))));
	printf("%lf",z2);
	double lambda=((8.0*rho)/(1-rho));
	printf("%lf",lambda);
	return 0;
}

