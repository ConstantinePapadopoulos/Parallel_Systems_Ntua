#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "mpi.h"
#include "utils.h"
#define PRINT_RESULTS


int main (int argc, char ** argv) {

    int rank,size;
    int global[2],local[2]; //global matrix dimensions and local matrix dimensions (2D-domain, 2D-subdomain)
    int global_padded[2];   //padded global matrix dimensions (if padding is not needed, global_padded=global)
    int grid[2];            //processor grid dimensions
    int i,j,t;
    int global_converged=0,converged=0; //flags for convergence, global and per process
    MPI_Datatype dummy;     //dummy datatype used to align user-defined datatypes in memory
    double omega;           //relaxation factor - useless for Jacobi

    struct timeval tts,ttf,tcs,tcf; //tcvs, tcvf;   //Timers: total-> tts,ttf, computation -> tcs,tcf
    double ttotal=0,tcomp=0,tconv=0,total_time=0,comp_time=0,conv_time=0;

    double ** U, ** u_current, ** u_previous, ** swap; //Global matrix, local current and previous matrices, pointer to swap between current and previous

    

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    //----Read 2D-domain dimensions and process grid dimensions from stdin----//

    if (argc!=5) {
        fprintf(stderr,"Usage: mpirun .... ./exec X Y Px Py");
        exit(-1);
    }
    else {
        global[0]=atoi(argv[1]);
        global[1]=atoi(argv[2]);
        grid[0]=atoi(argv[3]);
        grid[1]=atoi(argv[4]);
    }

    //----Create 2D-cartesian communicator----//
    //----Usage of the cartesian communicator is optional----//

    MPI_Comm CART_COMM;         //CART_COMM: the new 2D-cartesian communicator
    int periods[2]={0,0};       //periods={0,0}: the 2D-grid is non-periodic
    int rank_grid[2];           //rank_grid: the position of each process on the new communicator
    MPI_Status status;       
    MPI_Cart_create(MPI_COMM_WORLD,2,grid,periods,0,&CART_COMM);    //communicator creation
    MPI_Cart_coords(CART_COMM,rank,2,rank_grid);                    //rank mapping on the new communicator

    //----Compute local 2D-subdomain dimensions----//
    //----Test if the 2D-domain can be equally distributed to all processes----//
    //----If not, pad 2D-domain----//
    
    for (i=0;i<2;i++) {
        if (global[i]%grid[i]==0) {
            local[i]=global[i]/grid[i];
            global_padded[i]=global[i];
        }
        else {
            local[i]=(global[i]/grid[i])+1;
            global_padded[i]=local[i]*grid[i];
        }
    }

    //Initialization of omega
    omega=2.0/(1+sin(3.14/global[0]));

    //----Allocate global 2D-domain and initialize boundary values----//
    //----Rank 0 holds the global 2D-domain----//
    if (rank==0) {
        U=allocate2d(global_padded[0],global_padded[1]);   
        init2d(U,global[0],global[1]);
    }

    //----Allocate local 2D-subdomains u_current, u_previous----//
    //----Add a row/column on each size for ghost cells----//

    u_previous=allocate2d(local[0]+2,local[1]+2);
    u_current=allocate2d(local[0]+2,local[1]+2);   
       
    //----Distribute global 2D-domain from rank 0 to all processes----//
         
    //----Appropriate datatypes are defined here----//
    /*****The usage of datatypes is optional*****/
    
    //----Datatype definition for the 2D-subdomain on the global matrix----//

    MPI_Datatype global_block;
    MPI_Type_vector(local[0],local[1],global_padded[1],MPI_DOUBLE,&dummy);
    MPI_Type_create_resized(dummy,0,sizeof(double),&global_block);
    MPI_Type_commit(&global_block);

    //----Datatype definition for the 2D-subdomain on the local matrix----//

    MPI_Datatype local_block;
    MPI_Type_vector(local[0],local[1],local[1]+2,MPI_DOUBLE,&dummy);
    MPI_Type_create_resized(dummy,0,sizeof(double),&local_block);
    MPI_Type_commit(&local_block);

    //----Rank 0 defines positions and counts of local blocks (2D-subdomains) on global matrix----//
    int * scatteroffset, * scattercounts;
    if (rank==0) {
        scatteroffset=(int*)malloc(size*sizeof(int));
        scattercounts=(int*)malloc(size*sizeof(int));
        for (i=0;i<grid[0];i++)
            for (j=0;j<grid[1];j++) {
                scattercounts[i*grid[1]+j]=1;
                scatteroffset[i*grid[1]+j]=(local[0]*local[1]*grid[1]*i+local[1]*j);
            }
    }


    //----Rank 0 scatters the global matrix----//

    //*************TODO*******************//



    /*Fill your code here*/
    MPI_Scatterv(&(U[0][0]), scattercounts, scatteroffset, global_block, &(u_previous[1][1]), 1, local_block, 0, MPI_COMM_WORLD);

    for (i = 1; i <= local[0]; i++) {
    	for (j = 1; j <= local[1]; j++) {
           u_current[i][j] = u_previous[i][j];
    	}
    }


    /*Make sure u_current and u_previous are
        both initialized*/







     //************************************//


    if (rank==0)
        free2d(U);

 

   
    //----Define datatypes or allocate buffers for message passing----//

    //*************TODO*******************//



    /*Fill your code here*/

MPI_Datatype row_type;
MPI_Type_contiguous(local[1], MPI_DOUBLE, &row_type);
MPI_Type_commit(&row_type);
MPI_Datatype column;
MPI_Type_vector(local[0], 1, local[1]+2, MPI_DOUBLE, &dummy);
MPI_Type_create_resized(dummy, 0, sizeof(double), &column);
MPI_Type_commit(&column);







    //************************************//


    //----Find the 4 neighbors with which a process exchanges messages----//

    //*************TODO*******************//
    int north, south, east, west;

    MPI_Cart_shift(CART_COMM, 0, 1, &north, &south); // 0 -> axonas y +-1 neighbout
    MPI_Cart_shift(CART_COMM, 1, 1, &west, &east); // 1 -> axonas x +-1 neighbour

    /*Fill your code here*/


    /*Make sure you handle non-existing
        neighbors appropriately*/





    //************************************//



    //---Define the iteration ranges per process-----//
    //*************TODO*******************//

    int i_min,i_max,j_min,j_max;



    /*Fill your code here*/
//    int valid_i = (rank_grid[0] == grid[0] - 1) ? (global[0] - rank_grid[0] * local[0]) : local[0];
//    int valid_j = (rank_grid[1] == grid[1] - 1) ? (global[1] - rank_grid[1] * local[1]) : local[1];
//    i_min = 1;
//    i_max = valid_i; //ola einai local[0] & [1] alliws de xerw isws einai swsto isws oxi logika einai alla elpizw na mi xreiastei na ma8w
 //   j_min = 1;
//    j_max = valid_j;

if (rank_grid[0] == 0) {
    i_min = 2; // Skip left ghost layer + Dirichlet boundary
} else {
    i_min = 1; // Start after left ghost layer
}

if (rank_grid[0] == grid[0] - 1) {
    i_max = local[0] - 1; // Skip right ghost layer + Dirichlet boundary
} else {
    i_max = local[0] ; // End before right ghost layer
}

// Adjust j_min and j_max based on y-direction position
if (rank_grid[1] == 0) {
    j_min = 2; // Skip bottom ghost layer + Dirichlet boundary
} else {
    j_min = 1; // Start after bottom ghost layer
}

if (rank_grid[1] == grid[1] - 1) {
    j_max = local[1] - 1; // Skip top ghost layer + Dirichlet boundary
} else {
    j_max = local[1] ; // End before top ghost layer
}
/*i_min = 1;
    i_max = local[0] + 1;

    // boundary process - no possible padding 
    if (north == MPI_PROC_NULL) {
        i_min = 2;  // ghost cell + boundary
    }

    // boundary process and padded global array 
    if (south == MPI_PROC_NULL){
        i_max -= (global_padded[0] - global[0]) + 1;
    }

    // internal process (ghost cell only) 
    j_min = 1;
    j_max = local[1] + 1;

    // boundary process - no possible padding 
    if (west == MPI_PROC_NULL) {
        j_min = 2;  //ghost cell + boundary
    }

    // boundary process and padded global array 
    if (east == MPI_PROC_NULL){
        j_max -= (global_padded[1] - global[1]) + 1;
    }
*/


    /*Three types of ranges:
        -internal processes
        -boundary processes
        -boundary processes and padded global array
    */





    //************************************//




    //----Computational core----//   
    gettimeofday(&tts, NULL);



    // #ifdef TEST_CONV
    // for (t=0;t<T && !global_converged;t++) {
    // #endif
    // #ifndef TEST_CONV
    // #undef T
    // #define T 256
    // for (t=0;t<T;t++) {
    // #endif
    #define T 256  // Set T to 12
    for (t=0; t<T; t++) {

        //*************TODO*******************//
        swap=u_previous;
        u_previous=u_current;
        u_current=swap;
     
         MPI_Sendrecv( & u_previous[1][1], 1, row_type, north, 0, &
          u_previous[0][1], 1, row_type, north, 0,
          CART_COMM, MPI_STATUS_IGNORE);
        MPI_Sendrecv( & u_previous[local[0]][1], 1, row_type, south, 0, &
          u_previous[local[0]+1][1],1, row_type, south, 0,
          CART_COMM, MPI_STATUS_IGNORE);
        MPI_Sendrecv( & u_previous[1][local[1]], 1, column, east, 0, &
          u_previous[1][local[1]+1], 1, column, east, 0,
          CART_COMM, MPI_STATUS_IGNORE);
        MPI_Sendrecv( & u_previous[1][1], 1, column, west, 0, &
          u_previous[1][0], 1, column, west, 0,
          CART_COMM, MPI_STATUS_IGNORE);
/*
printf("rank: %d\n",rank );
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank==500 &&(t==220|| t==221)){

printf("Rank %d local matrix: with min/max i= %d %d and min/max j = %d %d\n", rank, i_min,i_max, j_min,j_max );
for (i = 0; i <= local[0]+1; i++) {
    for (j = 0; j <= local[1]+1; j++) {
        printf("%6.7f ", u_previous[i][j]);
    }
    printf("\n");
}
printf("\n");
}
MPI_Barrier(MPI_COMM_WORLD);


  MPI_Barrier(MPI_COMM_WORLD);
if (rank==500 &&(t==221|| t==220)){
printf("Rank %d local matrix: with min/max i= %d %d and min/max j = %d %d\n", rank, i_min,i_max, j_min,j_max );
for (i = 0; i <= local[0]+1; i++) {
    for (j = 0; j <= local[1]+1; j++) {
        printf("%6.7f ", u_previous[i][j]);
    }
    printf("\n");
}
printf("\n");
}
MPI_Barrier(MPI_COMM_WORLD);*/
/*
if (north != MPI_PROC_NULL){
            MPI_Sendrecv(&u_previous[1][1], local[1], MPI_DOUBLE, north, 0, &u_previous[0][1],
                            local[1], MPI_DOUBLE, north, 0, MPI_COMM_WORLD, &status );
        }

        // Communicate with south
        if (south != MPI_PROC_NULL){
            MPI_Sendrecv(&u_previous[local[0]][1], local[1], MPI_DOUBLE, south, 0, &u_previous[local[0]+1][1],
                            local[1], MPI_DOUBLE, south, 0, MPI_COMM_WORLD, &status );
        }

        // Communicate with east
        if (east != MPI_PROC_NULL){
            MPI_Sendrecv(&u_previous[1][local[1]], 1, column, east, 0, &u_previous[1][local[1]+1],
                            1, column, east, 0, MPI_COMM_WORLD, &status );
        }

        // Communicate with west
        if (west != MPI_PROC_NULL){
            MPI_Sendrecv(&u_previous[1][1], 1, column, west, 0, &u_previous[1][0],
                            1, column, west, 0, MPI_COMM_WORLD, &status );
        }
*/

        gettimeofday(&tcs, NULL);

         for(i= i_min; i <= i_max; i++) {
          for (j = j_min; j <= j_max; j++) {
            u_current[i][j] = (u_previous[i - 1][j] + u_previous[i + 1][j] +
              u_previous[i][j - 1] + u_previous[i][j + 1]) / 4.0;
          }
        }//Fill y ur code here*/


        /*Add appropriate timers for computation*/
    

        

        gettimeofday(&tcf, NULL);








        tcomp += (tcf.tv_sec-tcs.tv_sec)+(tcf.tv_usec-tcs.tv_usec)*0.000001;








       // #ifdef TEST_CONV
        if (t%10==0) {
	int converged = converge(u_previous, u_current, i_min, i_max, j_min, j_max);
        MPI_Allreduce(&converge, &global_converged, 1, MPI_INT, MPI_LAND, CART_COMM);
        if(rank == 28) printf("Converge is :%6.7f at t = %d\n", u_previous[i_max/2][j_max/2]-u_current[i_max/2][j_max/2],t);
	}       
//        #endif


        //************************************//
 
         
        
    }

    gettimeofday(&ttf,NULL);

    ttotal=(ttf.tv_sec-tts.tv_sec)+(ttf.tv_usec-tts.tv_usec)*0.000001;

    MPI_Reduce(&ttotal,&total_time,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
    MPI_Reduce(&tcomp,&comp_time,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
    MPI_Reduce(&tconv,&conv_time,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);



    //----Rank 0 gathers local matrices back to the global matrix----//
   
    if (rank==0) {
            U=allocate2d(global_padded[0],global_padded[1]);
    }

       
 //*************TODO*******************//



    /*Fill your code here*/

MPI_Gatherv(
    &u_current[1][1], 1, local_block,
    &(U[0][0]),
    scattercounts, scatteroffset, global_block,
    0, MPI_COMM_WORLD
);







     //************************************//


    
   
    //----Printing results----//

    //**************TODO: Change "Jacobi" to "GaussSeidelSOR" or "RedBlackSOR" for appropriate printing****************//
    if (rank==0) {
        printf("Jacobi X %d Y %d Px %d Py %d Iter %d ComputationTime %lf TotalTime %lf midpoint %lf\n",global[0],global[1],grid[0],grid[1],t,comp_time,total_time,U[global[0]/2][global[1]/2]);
    
        #ifdef PRINT_RESULTS
        char * s=malloc(50*sizeof(char));
        sprintf(s,"resJacobiMPI_%dx%d_%dx%d",global[0],global[1],grid[0],grid[1]);
        fprint2d(s,U,global[0],global[1]);
        free(s);
        #endif

    }
    MPI_Finalize();
    return 0;
}


