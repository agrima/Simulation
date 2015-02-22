#include<stdlib.h>
#include<stdio.h>
#include<math.h>
int main() {

	FILE *ptr_file;
	double rho;
	double var[25],svtm[25];
	int i;

	ptr_file =fopen("abc.txt","r");
	if (!ptr_file)
		return 1;
	//fscanf(ptr_file, "%lf", &rho);
	for(i = 0; i < 25; i++)
		fscanf(ptr_file, "%lf	%lf	%lf",&rho, &var[i], &svtm[i]);

	fclose(ptr_file);
//	printf("%lf ", rho);
	int sum=0;
	for(i = 0; i < 15; i++){
		sum+=sum+svtm[i];
	}
	double mean_y;
	mean_y=sum/15;
	float cov=0.0;
	int j;
	
	for(j=0;j<15; j++){
	cov+= (var[j]-var[15])*(svtm[j]-mean_y);
	
	}
	cov=cov/14;
	double va;
	for(j=0;j<15;j++){
	
	va+=pow(svtm[j]-mean_y,2);
	}
	va=va/14;
	double c_star;
	c_star=-cov/va;
	double v[25];
	for(i=0;i<25;i++){
	v[i]=var[i]+c_star*(svtm[i]-8);
	}
	double vn=0.0;
	for(i=0;i<25;i++){
	vn+=v[i];

	}
	vn=vn/25;
	double sigma=0.0;
	for(i=0;i<25;i++){
	sigma+=pow(v[i]-vn,2);

	}
	sigma=sigma/24;
	double sd;
	sd=pow(sigma,0.5);	

	sd=pow((sd/25),0.5);
	printf("%lf\t",rho);
	printf("%lf\t",vn);
	double z1=(vn-(1.96*(sd/pow(25,0.5))));
	printf("%lf\t",z1);
	double z2=(vn+(1.96*(sd/pow(25,0.5))));
	printf("%lf\t",z2);
	double lamb=((8.0*rho)/(1-rho));
	printf("%lf\t",lamb);
	return 0;
}

