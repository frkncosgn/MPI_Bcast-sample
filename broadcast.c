#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"

#define SIZE 5 

int main(int argc, char *argv[])
{
	int myid,nprocs,root,
	    master=0 ;

	int *buff;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	
	if (myid==0)
	{
		
		if(argc<2)
		{
  			fprintf(stdout,"please enter a rank number (< %d ) to broadcast its own data: broadcast.x 2\n", nprocs);
			
			MPI_Abort(MPI_COMM_WORLD,99);
			exit(99);
		}

		printf("here is the leader processor %d\n",myid);
		printf("argc: %d\n", argc);
			
		for(int i=0;i<argc;i++)
			printf("argv[%d]: %s\n", i, argv[i]);
		
		root=atoi(argv[1]);
		
		if(root >= nprocs)
		{
			fprintf(stderr, "ERROR: Invalid rank number ID (%d), aborting !...\n",root);
			MPI_Abort(MPI_COMM_WORLD,99);
			exit(99);	
		}
	}

	else
	{
		printf("here is the worker processor %d\n",myid);
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	MPI_Bcast(&root,1,MPI_INT,0,MPI_COMM_WORLD);
	
	buff=(int*)malloc(sizeof(int)*SIZE);
	
	for(int i=0;i<SIZE;i++)
		buff[i]=myid*i;
	
	if(myid == root)
		fprintf(stdout,"ID (%d) broadcasts an array with the size of %d\n",myid,SIZE);
	
	MPI_Bcast(buff,SIZE,MPI_INT,root,MPI_COMM_WORLD);
	
	printf("ID (%d): After broadcasting; buff[%d]=%d\t buff[%d]=%d\n",myid,1,buff[1],SIZE-1,buff[SIZE-1]);

	MPI_Finalize();

	return 0;
}
