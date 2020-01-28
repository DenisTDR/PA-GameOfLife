#include <iostream>
#include <cstdlib>
#include <fstream>
#include <omp.h>
#include <cstring>

using namespace std;

int N, generations;

char** current_iteration;
char** new_iteration;

void read_initial_state(char* filename)
{
	//open file
	ifstream file(filename);
	if (!strlen(filename)) {
		cout << "Input file not specified" << endl;
		exit(-1);
		}

	if (!file)
	{
		cout << "Error opening file"<<endl;
		exit(-1);
	}

	//read the size of the matrix and the nr generations
	file>>N>>generations;

	N = N + 2;	

	current_iteration = new char*[N];
	new_iteration = new char*[N];

	for(int i = 0; i<N; i++)
	{
		current_iteration[i] = new char[N];
	}

	for(int i = 0; i<N; i++)
	{
		new_iteration[i] = new char[N];
	}

	//read the matrix
	while (!file.eof()){
		int x, y;
		char space;
		file >> x >> space >> y;
		current_iteration[x+1][y+1]=1;
	}

	file.close();
}


void add_border()
{
	int i;
	// top line
	#pragma omp parallel for private(i)
	for(int i = 1; i<N - 1; i++)
	{
		current_iteration[0][i] = 0;
	}

	// bottom line
	#pragma omp parallel for private(i)
	for(int i = 1; i<N - 1; i++)
	{
		current_iteration[N - 1][i] = 0;
	}

	// left column	
	#pragma omp parallel for private(i)
	for(int i = 1; i<N - 1; i++)
	{
		current_iteration[i][0] = 0;
	}

	// right column	
	#pragma omp parallel for private(i)
	for(int i = 1; i<N - 1; i++)
	{
		current_iteration[i][N - 1] = 0;
	}

	// corners
	current_iteration[0][0] = 0; // upper left
	current_iteration[0][N - 1] = 0; // upper right
	current_iteration[N - 1][0] = 0; // lower left
	current_iteration[N - 1][N - 1] = 0; // lower right
}

char analyze_cell(int i, int j)
{
	int alive_neighbours = 0;

	for (int k = -1; k < 2; k++)
	{
		for (int l = -1; l < 2; l++){
			if (!k && !l) continue;
			alive_neighbours += current_iteration[i + k][j + l];
		}
	}
	
	if(alive_neighbours < 2) 
		return 0;

	else if(alive_neighbours > 3)
		return 0;

	else if(current_iteration[i][j] == 1 && (alive_neighbours == 2 || alive_neighbours == 3))
		return 1;

	else if(current_iteration[i][j] == 0 && alive_neighbours == 3)
		return 1;
	return 0;
}

void apply_algorithm(int number_threads)
{
	int i, j = 0;

	#pragma omp parallel for private(i, j) num_threads(number_threads)
	for(i = 1; i<N - 1; i++)
	{
		for(j = 1; j<N - 1; j++)
		{
			new_iteration[i][j] = analyze_cell(i, j);
		}
	}
}

void copy_matrix()
{
	int i, j = 0;

	#pragma omp parallel for private(i, j)
	for(i = 1; i<N - 1; i++)
	{
		for(j = 1; j<N - 1; j++)
		{
			current_iteration[i][j] = new_iteration[i][j];
		}
	}
}

void write_final_state(string filename, int gen)
{
	ofstream f(filename, ofstream::app);

	f << "Generation " << gen<<":"<<endl;
	for(int i = 1; i<N - 1; i++)
	{
		for(int j = 1; j<N - 1; j++)
		{
			if (current_iteration[i][j] ){
				f<<i-1<<','<<j-1<<endl;
			}
		}
	}
	f<<endl;
	f<<endl;

	f.close();
}

int main(int argc, char** argv)
{
	omp_set_dynamic(0);
	string output_file = "output_openmp_2D.txt";

	//delete the output file content
	std::ofstream ofs;
	ofs.open(output_file, std::ofstream::out | std::ofstream::trunc);
	ofs.close();

	if (argc < 3){
		cout << "Not enough argument" << endl;
		return -1;
	}

	read_initial_state(argv[2]);
	int number_threads = atoi(argv[1]);
	
	for(int i = 0; i<generations; i++)
	{
		add_border();
		// for (int i = 0; i < N; i++){
		// 	for (int j = 0; j < N; j++){
		// 		cout << char(current_iteration[i][j] + '0');
		// 	}
		// 	cout << endl;
		// }

		apply_algorithm(number_threads);

		copy_matrix();
		write_final_state(output_file, i);
	}

}
