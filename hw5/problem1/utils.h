double uniformx(double *dseed) {
   double d2p31m = 2147483647.0,
          d2p31  = 2147483711.0;

      *dseed = 16807*(*dseed) - floor(16807*(*dseed)/d2p31m) * d2p31m;
      return( fabs((*dseed / d2p31)) ); 
}
      
double exponx(double *dseed, float xm) {

      return( (-(xm) * log((double)uniformx(dseed))) );
}
