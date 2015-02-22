#include<stdlib.h>
#include<stdio.h>
#include<math.h>
int main() {

	FILE *ptr_file;
	double rho;
	double var[25],y[25],z[25];
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
	double lambda=(1-rho)/(8*rho);
	for(i = 0; i < 25; i++){
		y[i]=(-1/lambda)*log(1-exp(-lambda*var[i]));	
		z[i]=(var[i]+y[i])/2;
	}
	double zn;
	for(i=0; i<25; i++){
	zn=z[i]/25;
	}
	double sd=0.0;
	int j;
	
	for(j=0;j<25; j++){
//	sd+= pow((mean-var[j]),2);
	sd+=pow(z[i]-zn,2);	
	}
	//sd=pow((sd/25),0.5);
	sd=sd/24;
	printf("%lf\t",rho);
	printf("%lf\t",zn);
	double z1=(zn-(1.96*(sd/pow(25,0.5))));
	printf("%lf\t",z1);
	double z2=(zn+(1.96*(sd/pow(25,0.5))));
	printf("%lf\t",z2);
	double lamb=((8.0*rho)/(1-rho));
	printf("%lf\t",lamb);
	return 0;
}

