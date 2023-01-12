#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double func( double );
void write_file( int num_of_pts, double x1, double delta, FILE * f_out, char group_name [], char name [] );
void write_bin( int num_of_pts, double x1, double delta, FILE * f_out_bin );
double ** create_array ( int num_of_pts );
void read_bin( double** arr_of_pts, FILE * f_out_bin );
void print_array( double** arr_of_pts, int num_of_pts );

int main(){

 double x1, x2, delta;
 int num_of_pts;
 const int N = 30;
 char group_name[N], name[N];
 FILE * f_in, * f_out, * f_out_bin;

 f_in = fopen( "txt_in.txt", "r" );
 f_out = fopen( "txt_out.txt", "w" );
 f_out_bin = fopen( "bin_out.bin", "wb" );

 if ( f_in == NULL || f_out == NULL || f_out_bin == NULL ){
    printf("\nCannot open a file!");
    printf("Press any key to exit...");
    getch();
    exit (1);
 }

 fscanf( f_in, "%lf %lf %d %lf ", &x1, &x2, &num_of_pts, &delta );
 fgets( &group_name, N, f_in );
 fgets( &name, N, f_in );
 fclose( f_in );

 if( delta == 0 && num_of_pts != 0 || ((x2-x1)/ delta)+1 != num_of_pts ){
    delta = (x2-x1)/(num_of_pts-1);
 }

 if ( num_of_pts == 0 && delta != 0 ){
    num_of_pts = ((x2-x1)/ delta)+1;
 }

 else if( delta == 0 && num_of_pts == 0 ){
    printf("No delta or number of points specified");
    printf("Please specify at least one of the parameters in txt_in.txt");
    fclose( f_out );
    fclose( f_out_bin );
 }

 double** arr_of_pts = create_array( num_of_pts );
    if ( arr_of_pts == NULL )
        printf("\n\tMemory has not been allocated");

 write_file( num_of_pts,x1,delta, f_out, group_name, name );
 write_bin( num_of_pts,x1, delta,f_out_bin );
 read_bin( arr_of_pts, f_out_bin );
 print_array ( arr_of_pts, num_of_pts );

 for(int i=0; i<num_of_pts; i++) 
  free(arr_of_pts[i]);
    free( arr_of_pts );

 return 0;
}

// Обрахунок функції

double func(double x1){

 return (x1-50)*cos(x1/10)-3*x1+500;

}

// Запис таблиці до файлу txt_out.txt

void write_file( int num_of_pts, double x1, double delta, FILE * f_out, char group_name [], char name [] ){

 fprintf(f_out, "\n+-------+-----------+-----------+");
 fprintf(f_out, "\n|   N   |     x     |    F(x)   |");
 fprintf(f_out, "\n+-------+-----------+-----------+");

 for(int N=1;N<=num_of_pts;N++){
    fprintf( f_out, "\n|%7.d|%11.3f|%11.3f|",N,x1,func(x1) );
    fprintf( f_out,"\n+-------+-----------+-----------+" );
    x1+=delta;
 }

 fprintf( f_out, "\n\n %s %s", group_name, name );
 fclose( f_out );

}

// Запис даних до бінарного файлу bin_out.bin

void write_bin( int num_of_pts, double x1, double delta, FILE * f_out_bin ){

 double res_func;

 fwrite( &num_of_pts, sizeof(int), 1, f_out_bin );

 for(int N=1;N<=num_of_pts;N++){
    fwrite( &x1, sizeof(double), 1, f_out_bin );
    res_func = func(x1);
    fwrite( &res_func, sizeof(double), 1, f_out_bin );
    x1+=delta;
 }

   fclose( f_out_bin );

}

// Створення масиву

double** create_array ( int num_of_pts ){

 double** arr_of_pts =(double**) malloc( num_of_pts * sizeof(double) );

 for(int i=0;i<num_of_pts;i++)
    arr_of_pts[i]=(double*) malloc(3 * sizeof(double));

 return arr_of_pts;

}

// Читання даних з бінарного файлу та запис їх до масиву у вигляді таблиці

void read_bin( double** arr_of_pts, FILE * f_out_bin ){

 int N = 0;
 double temp;

 f_out_bin = fopen( "bin_out.bin", "rb" );
 fread( &N, sizeof(int), 1, f_out_bin );

 for (int i=0;i<N;i++){
    arr_of_pts[i][0]= (double)i+1;
    for (int j=1;j<3;j++){
        fread( &temp, sizeof(double), 1, f_out_bin );
        arr_of_pts[i][j]= temp;
    }

 }

 fclose(f_out_bin);

}

// Виведення масиву у консоль

void print_array ( double** arr_of_pts, int num_of_pts ){

 printf("    N         x        F(x)    \n\n");

 for (int i=0;i<num_of_pts;i++){
    printf( "%5.0lf ", arr_of_pts [i][0] );
    for (int j=1;j<3;j++){
        printf( "%11.3lf ", arr_of_pts [i][j] );
    }
    printf( "\n" );
 }

}
