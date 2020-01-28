#include "mpi.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[])
{
	int **theBoard;	

	int size, rank, tag, rc, N, generations, s;
	MPI_Status Stat;
	ofstream output("output_mpi_1D.txt"); 	//output file
	rc=MPI_Init(&argc,&argv);
	if (rc!=0) {cout << "Error starting MPI." << endl; MPI_Abort(MPI_COMM_WORLD, rc);}
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank==0){
		// I am processor 0
		if (argc<2) {
			cout << "Input file not specified" << endl;
			MPI_Finalize();
			return -1;
			}
			
		ifstream file(argv[1]);	//input file

		if (!file)
		{
			cout << "Error opening file"<<endl;
			MPI_Finalize();
			return -1;
		}
		
		file >> N >> generations;	//first three variables from file
		theBoard = (int**)malloc(N*sizeof(int*));
		if(!theBoard){
				exit(-1);
			}
		for(int n=0; n<N;n++){
			theBoard[n] = (int*)calloc(N,sizeof(int));
			if(!theBoard[n]){
				exit(-1);
			}

		}
		s=N/size;	//how many slices 
		while (!file.eof()){
			int x, y;
			char space;
			file >> x >> space >> y;
			if(x>=N || y>=N || x <0|| y<0){
				exit(-1);
			}

			theBoard[x][y]=1;
		}
		file.close();

		//SENDING INITIAL INFORMATION (N, k, #generations, output points) TO EVERYONE
		int info[3];
		info[0]=N; info[1]=s; info[2]=generations; 
		for (int dest=1; dest<size; dest++) MPI_Send(&info, 3, MPI_INT, dest, 1, MPI_COMM_WORLD); //send info
		int slice[N/size][N];
		for (int z=size-1; z>0; z--)
		{
			for (int k=0; k<s; k++) 
				for (int l=0; l<N; l++) 
					slice[k][l]=theBoard[k+(z*s)][l];	//cut a slice from the the board
			
			MPI_Send(&slice, N*s, MPI_INT, z, 1, MPI_COMM_WORLD);	//and send it
		}

	} // end of processor 0 code
	//RECEIVED INITIAL INFORMATION
	int localinfo[3];		// local info for initial information
	if (rank != 0){
	}
	if (rank!=0){
		MPI_Recv(&localinfo, 3, MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat);	//receive info
		N = localinfo[0];			//assign variables
		s = localinfo[1];
		generations=localinfo[2];	//
	}
	int myslice[s][N]; //my own slice of the board
	if (rank!=0)
		MPI_Recv(&myslice, localinfo[0]*localinfo[1], MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat);	//receive slice
	else{
		for (int k=0; k<s; k++) 
				for (int l=0; l<N; l++) 
					myslice[k][l]=theBoard[k][l];	//cut a slice from the the board
	}
				//
	
	
	int todown[N];	int toup[N]; int fromdown[N]; int fromup[N]; //arrays to send and to receive
	for (int g=0; g<generations; g++) //generations forloop
	{	
		if (rank!=size-1) // all except for last send down
		{
			for (int j=0; j<N; j++) todown[j]=myslice[s-1][j];
			MPI_Send(&todown, N, MPI_INT, rank+1, 1, MPI_COMM_WORLD);

		} else {
			for (int k=0; k<N; k++) fromdown[k]=0; } // last one generates empty stripe "from down"

		if (rank!=0) // all except for first receive from up
		{
			MPI_Recv(&fromup, N, MPI_INT, rank-1, 1, MPI_COMM_WORLD, &Stat);	

		} else { for (int k=0; k<N; k++) fromup[k]=0; } // first one generats empty line "from up"	
	
		if (rank!=0) // all except for first send up
		{
			for (int j=0; j<N; j++) toup[j]=myslice[0][j];
			MPI_Send(&toup, N, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
		}
	
		if (rank!=size-1) // all except for last receive from down
		{
			MPI_Recv(&fromdown, N, MPI_INT, rank+1, 1, MPI_COMM_WORLD, &Stat);
		}

		//COUNTING NEIGHBORS
		int sum=0; // sum of neighbours
		int mynewslice[s][N];
		for (int x=0; x<s; x++) //for each row
		{	
			for (int y=0; y<N; y++) //for each column
			{
				if (x==0 && y==0) //upper-left cell
					sum = myslice[x+1][y]+myslice[x+1][y+1]+myslice[0][y+1]+fromup[0]+fromup[1];
				else if (x==0 && y==N-1) //upper-right cell
					sum = myslice[x][y-1]+myslice[x+1][y-1]+myslice[x+1][y]+fromup[N-1]+fromup[N-2];
				else if (x==s-1 && y==0) //lower-left cell
					sum = myslice[x][y+1]+myslice[x-1][y+1]+myslice[x-1][y]+fromdown[0]+fromdown[1];
				else if (x==s-1 && y==N-1) //lower-right cell
					sum = myslice[x-1][y]+myslice[x-1][y-1]+myslice[x][y-1]+fromdown[N-1]+fromdown[N-2];
				else // not corner cells    
				{
					if (y==0) // leftmost line, not corner
						sum=myslice[x-1][y]+myslice[x-1][y+1]+myslice[x][y+1]+myslice[x+1][y+1]+myslice[x+1][y];
					else if (y==N-1) //rightmost line, not corner
						sum=myslice[x-1][y]+myslice[x-1][y-1]+myslice[x][y-1]+myslice[x+1][y-1]+myslice[x+1][y];
					else if (x==0) //uppermost line, not corner
						sum=myslice[x][y-1]+myslice[x+1][y-1]+myslice[x+1][y]+myslice[x+1][y+1]+myslice[x][y+1]+fromup[y-1]+fromup[y]+fromup[y+1];
					else if (x==s-1) //lowermost line, not corner
						sum=myslice[x-1][y-1]+myslice[x-1][y]+myslice[x-1][y+1]+myslice[x][y+1]+myslice[x][y-1]+fromdown[y-1]+fromdown[y]+fromdown[y+1];
					else //general case, any cell within
						sum=myslice[x-1][y-1]+myslice[x-1][y]+myslice[x-1][y+1]+myslice[x][y+1]+myslice[x+1][y+1]+myslice[x+1][y]+myslice[x+1][y-1]+myslice[x][y-1];
				}
				
				//PUT THE NEW VALUE OF A CELL
				if (myslice[x][y]==1 && (sum==2 || sum==3)) mynewslice[x][y]=1;
				else if (myslice[x][y]==1 && sum>3) mynewslice[x][y]=0;
				else if (myslice[x][y]==1 && sum<1) mynewslice[x][y]=0;
				else if (myslice[x][y]==0 && sum==3) mynewslice[x][y]=1;
		 		else mynewslice[x][y]=0;
			
			}
		}
	
		// copy new slice onto myslice
		for (int x=0; x<s; x++)
			for (int y=0; y<N; y++)
				myslice[x][y]=mynewslice[x][y];
		//PRINTING THE RESULT TO FILE
		{
			if (rank==0) 
			{
				int aBoard[s][N];
				output << "Generation " << g << ":" << endl;
				for (int x=0; x<s; x++) //put your own slice
				{
					for (int y=0; y<N; y++){
						if (myslice[x][y] == 1){
							output << x << "," << y << endl;
						}
					}
				}
				for (int i=1; i<size; i++)
				{
					MPI_Recv(&aBoard, N*s, MPI_INT, i, 1, MPI_COMM_WORLD, &Stat); //receive all others'
					for (int x=0; x<s; x++)
					{
						for (int y=0; y<N; y++)
						{
							if (aBoard[x][y] == 1){
								output << x + s*i << "," << y << endl;
							}
						}
					}
				}
				output << endl << endl;
			}
			else MPI_Send(&myslice, N*s, MPI_INT, 0,1, MPI_COMM_WORLD);

		
		}	
	} // end of generation loop
output.close();
MPI_Finalize();
}
