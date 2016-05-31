#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std;
double **mat;
int *baseT;
int m11, nm11, n, m;
double z = 0;
bool noSolution = false;

void mat_multEscLine(int line, double mult)
{
	for(int i = 0; i < nm11; ++i)
		if(mat[line][i] != 0)
			mat[line][i] *= mult;
}

void mat_addLineEscLine(int line1, double mult, int line2)
{
	for(int i = 0; i < nm11; ++i)
		mat[line1][i] += mult*mat[line2][i];
}

void imprimeTablo()
{
	cout << "\t";
	for(int i = 1; i < nm11; ++i)
		cout << "\tx" << i;
	cout << endl;
	
	cout << "q\t" << mat[0][nm11-1];
	for(int i = 0; i < nm11-1; ++i)
		cout << "\t" << mat[0][i];

	cout << endl << "z\t" << mat[1][nm11-1];
	for(int i = 0; i < nm11-1; ++i)
		cout << "\t" << mat[1][i];
	
	cout << endl;

	for(int i = 2, k = 0; i < m11; ++i)
	{
		cout << "x" << baseT[k++]+1 << "\t" << mat[i][nm11-1];

		for(int j = 0; j < nm11-1; ++j)
			cout << "\t" << mat[i][j];
		cout << endl;
	}

	cout << endl;
}

void primeiraFase()
{
	imprimeTablo();
	mat_multEscLine(0, -1);

	for(int j = 0; j < nm11-1; ++j)
	{
		if(mat[0][j] == -1)
			for(int i = 2; i < m11; ++i)
				if(mat[i][j] == 1)
					mat_addLineEscLine(0, 1, i);
	}

	bool solved = false;

	do{
		imprimeTablo();
		int costIndex = 0, pivot = 2;

		for(int j = 0; j < nm11-1; ++j)
			if(mat[0][j] > mat[0][costIndex])
				costIndex = j;

		if(mat[0][costIndex] <= 0)
		{
			solved = true;
			break;
		}
		//cout << "zjcj = "  << costIndex << " -> " << mat[0][costIndex] << endl;

		for(int i = 2; i < m11; ++i)
			if(mat[i][costIndex] > 0)
				if(mat[pivot][nm11-1]/mat[pivot][costIndex] > mat[i][nm11-1]/mat[i][costIndex])
					pivot = i;
		
		//cout << pivot << " " << mat[pivot][nm11-1] << "/" << mat[pivot][costIndex] << endl;
		mat_multEscLine(pivot, 1/mat[pivot][costIndex]);
		for(int i = 0; i < m11; ++i)
		{
			if(i == pivot)
				continue;
			mat_addLineEscLine(i, -mat[i][costIndex], pivot);
		}

		if(!solved)
			baseT[pivot - 2] = costIndex;

	}while(!solved);
}

void segundaFase()
{

	bool solved = false;
	do{
		imprimeTablo();
		int costIndex = 0, pivot = 2;
		for(int j = 0; j < nm11-1 ;++j)
			if(mat[1][j] < mat[1][costIndex])
				costIndex = j;

		if(mat[1][costIndex] >= 0)
		{
			solved = true;
			break;
		}

		//cout << "zjcj = "  << costIndex << " -> " << mat[1][costIndex] << endl;
		for(int i = 2; i < m11; ++i)
			if(mat[i][costIndex] > 0)
			{
				//cout << mat[i][nm11-1] << "/" << mat[i][costIndex] << " = " <<  mat[i][nm11-1]/mat[i][costIndex] << endl;
				if(mat[pivot][nm11-1]/mat[pivot][costIndex] > mat[i][nm11-1]/mat[i][costIndex])
					pivot = i;
			}

		if(pivot == 2 && mat[pivot][costIndex] < 0)
		{
			//cout << pivot << " " << mat[pivot][costIndex] << endl;
			if(mat[0][costIndex] == 0)
				noSolution = true;
			solved = true;
			break;
		}

		mat_multEscLine(pivot, 1/mat[pivot][costIndex]);
		for(int i = 0; i < m11; ++i)
		{
			if(i == pivot)
				continue;
			mat_addLineEscLine(i, -mat[i][costIndex], pivot);
		}
		
		if(!solved)
			baseT[pivot - 2] = costIndex;

	}while(!solved);

	imprimeTablo();
}

int main(int argc, char** argv)
{
	if(argc == 1)
	{
		cerr << "Falta arquivo de entrada" << endl;
		return -1;
	}
	
	int fase = 0;
	string line;
	ifstream inFile(argv[1]);
	if(inFile.is_open())
	{
		getline(inFile, line);
		fase = line[0] - '0';

		getline(inFile, line);
		m11 = line[0] - '0';
		nm11 = line[2] - '0';
		
		//cout << fase << endl << m11 << " " << nm11 << endl;
		//cout << n << " " << m << endl;
		
		//TEST
		cout.precision(3);

		mat = (double**)calloc(m11, sizeof(double*));
		for(int i = 0; i < m11; ++i)
		{
			mat[i] = (double*)calloc(nm11, sizeof(double));

			getline(inFile, line);
			string delimiter = " ";
			string token;
			size_t pos = 0;
			int j = 0;
			while((pos = line.find(delimiter)) != string::npos)
			{
				token = line.substr(0, pos);
				mat[i][j++] = stod(token);
				line.erase(0, pos + delimiter.length());
			}
			mat[i][j] = stod(line);
		}
		inFile.close();
		/*for(int i = 0; i < m11; ++i)
		{
			for(int j = 0; j < nm11; ++j)
				cout << mat[i][j] << "\t";

			cout << endl;
		}*/
		
		int artVar = 0;
		for(int j = 0; j < nm11; ++j)
			if(mat[0][j] == 1)
				artVar++;

		m = m11 - 2;
		n = nm11 - m - 1 - artVar;

		baseT = (int*)calloc(m, sizeof(int));

		for(int j = n, k = 0; j < nm11; ++j)
			for(int i = 2; i < m11; ++i)
				if(mat[i][j] == 1)
				{
					baseT[k++] = j;
				}
		
		cout << endl;

		if(fase == 1)
			primeiraFase();
		segundaFase();
	
		cout << "za* = " << mat[0][nm11-1] << endl;
		if(mat[0][nm11-1] != 0)
			cout << "Conjunto de souções viáveis é vazio" << endl;
		else{
			if(noSolution)
				cout << "z* = -inf" << endl;
			else{
				cout << "z* = " << mat[1][nm11-1] << "\t";
				
				bool mult = false;
				cout << "x* = (";
				for(int j = 0; j < nm11-1; ++j)
					if(mat[1][j] == 0)
					{
						bool found = false;
						for(int k = 0; k < m; ++k)
							if(baseT[k] == j)
								found = true;

						if(!found)
							mult = true;
					}
			
				double *fBase = (double*)calloc(nm11-1, sizeof(double));
				for(int i = 2; i < m11; ++i)
				{
					//cout << baseT[i-2] << "->" << mat[i][nm11-1] << "\t";
					fBase[baseT[i-2]] = mat[i][nm11-1];
				}
				
				for(int l = 0; l < nm11-1; ++l)
					cout << " " << fBase[l];
				cout << " )" << endl;
			}

		}
			

		}else{
			cerr << "Arquivo de entrada não encontrado" << endl;
		}

	return 0;
}
