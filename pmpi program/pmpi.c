//Single Author Info:
//kchopra Kapil Chopra
#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int *mydata, rank, size;

MPI_Init(int *argc, char ***argv)
{
int i;
PMPI_Init(argc, argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
if(rank == 0){
	mydata = (int *)malloc(sizeof(int) * size * size);
	for(i=0;i<size*size;i++){
		mydata[i]=0;
	}
}else{
	mydata = (int *)malloc(sizeof(int) * size);
	for(i=0;i<size;i++){
		mydata[i]=0;
	}
}
}

MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
    int tag, MPI_Comm comm)
{
mydata[dest]++;
PMPI_Send( buf, count, datatype, dest, tag, comm);

}

MPI_Finalize()
{
FILE *fp;
int i,j;
if(rank != 0){
	PMPI_Send(mydata, size, MPI_INT, 0, rank, MPI_COMM_WORLD);
}
if(rank == 0){
	//remove("matrix.data");
	//fp = fopen("matrix.data","w");
	//fprintf(fp, "%d ",rank);
	//for(i=0;i<size;i++){
		//fprintf(fp, "%d ",mydata[i]);
	//}
	for(i=1;i<size;i++){
		PMPI_Recv(&mydata[i*size], size, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//fprintf(fp, "\n%d ",i);
		//for(j=0;j<size;j++){
			//fprintf(fp, "%d ",mydata[i]);
		//}
	}
	fp = fopen("matrix.data", "w");
	for(i=0;i<size;i++){
		fprintf(fp,"%d",i);
		for(j=0;j<size;j++){
			fprintf(fp," %d", mydata[i*size + j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}
free(mydata);
PMPI_Finalize();

}
