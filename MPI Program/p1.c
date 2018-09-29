// Single Author Info:

// kchopra Kapil   Chopra


#include<stdio.h>

#include<mpi.h>

#include<stdlib.h>

#include<math.h>


int main (int argc, char *argv[])
{

    //General purpose variables

    int x=0;

    long i,data_size,asize = 32; // data_size keep track of data to be sent next and asize keeps track of size of data to be sent next

    unsigned int count,rank,size; // rank stores the rank of the process, size stores the total number of processes running this task

    unsigned int data[524288]; // data stores the data to be transmitted

    //Variables to record start and end time of the data transmission

    double start_time, end_time;

    //Variables to record the round-trip time for each transmission and its average.

    double times[170],avg_times[17];

    //Variable to record the Standard Deviation

    double sd[17];

    //Initialize MPI

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);


    //Initialization loop

    if(rank == 0)

    {

        //Initializing data

        for(i=0;i<524288;i++)

        {

            data[i] = -9;

        }

        //Initializing the array of round-trip times

        for(i=0;i<170;i++)

        {

            times[i] == 0;

        }

        //Initializing the arrays of Average round-trip times and Standard Deviation

        for(i=0;i<17;i++)

        {

            avg_times[i]=0;

            sd[i]=0;

        }

    }


    //Message passing and receiving loop

    while(asize<=2097152)

    {

        //The number of unsigned integers to be sent/received

        data_size = asize/4;


        //Loop to do the send/receive task 10 times

        for(count=0;count<10;count++)

        {

            if(rank == 0)

            {

                //Start timer

                start_time = MPI_Wtime();

                //Send data

                MPI_Send(&data, (data_size), MPI_INT, (rank+1)%size, rank%size, MPI_COMM_WORLD);

                //Receive data

                MPI_Recv(&data, (data_size), MPI_INT, (rank-1)%size, (rank-1)%size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                //Stop timer

                end_time = MPI_Wtime();

                //Store the round trip time in the times array

                times[(x*10) + count] = end_time - start_time;

            }

            else

            {

                //Receive the data

                MPI_Recv(&data, (data_size), MPI_INT, (rank-1)%size, (rank-1)%size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                //Send the data

                MPI_Send(&data, (data_size), MPI_INT, (rank+1)%size, rank%size, MPI_COMM_WORLD);

            }

        }

        //increment the data to be actually sent to twice its previous value

        asize = asize*2;

        //increment the variable used to keep track of the number of send/receive process performed for different size of data

        x++;

    }


    //Now compute the Average time and Standard Deviation

    if(rank == 0)

    {

        //Loop to compute the Average time

        for(i=0;i<17;i++)

        {

            //The loop starts with 1 to skip the 0th reading

            for(x=1;x<10;x++)

            {

                avg_times[i] += times[10*i + x];

            }

            //Total readings are 9

            avg_times[i] /= 9;

        }


        //Setting a variable to keep track of the size to be printed

        data_size = 32;

        //Loop to compute standard deviation and display results

        for(i=0;i<17;i++)

        {

            for(x=1;x<10;x++)

            {

                sd[i] += pow((times[10*i+x]-avg_times[i]),2);

            }

            sd[i] = sqrt(sd[i]/9);

            //Standard Deviation computed for data of size data_size

            //Print the results

            printf("%ld %e %e\n",(data_size),avg_times[i],sd[i]);

            //Increment the variable data_size to the size of data which was transmitted next i.e. twice its previous value

            data_size *= 2;

        }

    }

    MPI_Finalize();

}
