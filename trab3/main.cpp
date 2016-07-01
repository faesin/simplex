#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std;
double **mat;
int *baseT;
int m1, nm1, n, m;
double z = 0;
bool noSolution = false;

void mat_multEscLine(int line, double mult)
{
	for(int i = 0; i < nm1; ++i)
		if(mat[line][i] != 0)
			mat[line][i] *= mult;
}

void mat_addLineEscLine(int line1, double mult, int line2)
{
	for(int i = 0; i < nm1; ++i)
		mat[line1][i] += mult*mat[line2][i];
}

void imprimeTablo()
{
	cout << "\t";
	for(int i = 1; i < nm1; ++i)
		cout << "\tx" << i;
	cout << endl;
	
	cout << "z\t" << mat[0][nm1-1];
	for(int i = 0; i < nm1-1; ++i)
		cout << "\t" << mat[0][i];
	
	cout << endl;

	for(int i = 1, k = 0; i < m1; ++i)
	{
		cout << "x" << baseT[k++]+1 << "\t" << mat[i][nm1-1];

		for(int j = 0; j < nm1-1; ++j)
			cout << "\t" << mat[i][j];
		cout << endl;
	}

	cout << endl;
}

void dualSimplex()
{
	imprimeTablo();
	mat_multEscLine(0, -1);

	for(int j = 0; j < nm1-1; ++j)
	{
		if(mat[0][j] == -1)
			for(int i = 2; i < m1; ++i)
				if(mat[i][j] == 1)
					mat_addLineEscLine(0, 1, i);
	}

	bool solved = false;

	do{
		imprimeTablo();
		int mostNeg = 1, pivot = 0;

		for(int i = 1; i < m1; ++i)
			if(mat[i][nm1-1] < 0 && mat[i][nm1-1] < mat[mostNeg][nm1-1])
				mostNeg = i;

		//cout << "mostNeg = " << mat[mostNeg][nm1-1] << " -> " << mostNeg << endl; 
		if(mat[mostNeg][nm1-1] >= 0)
		{
			solved = true;
			break;
		}

		for(int j = 0; j < nm1-1; ++j)
			if(mat[0][j] <= 0 && mat[mostNeg][j] < 0)
			{
				if((mat[0][j]/mat[mostNeg][j] < mat[0][pivot]/mat[mostNeg][pivot]) || mat[mostNeg][pivot] == 0)
					pivot = j;
			}
		
		//cout << pivot << " " << mat[0][pivot] << "/" << mat[mostNeg][pivot] << " = " << mat[0][pivot]/mat[mostNeg][pivot] << endl;
		if(mat[0][pivot]/mat[mostNeg][pivot] < 0 || mat[mostNeg][pivot] == 0)
		{
			solved = true;
			if(mat[0][pivot]/mat[mostNeg][pivot] < 0)
				noSolution = true;
			break;
		}

		mat_multEscLine(mostNeg, 1/mat[mostNeg][pivot]);
		for(int i = 0; i < m1; ++i)
		{
			if(i == mostNeg)
				continue;
			mat_addLineEscLine(i, -mat[i][pivot], mostNeg);
		}

		if(!solved)
			baseT[pivot - 2] = pivot;

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
	
	string line;
	ifstream inFile(argv[1]);
	if(inFile.is_open())
	{
		getline(inFile, line);
		m1 = line[0] - '0';
		nm1 = line[2] - '0';
		
		//TEST
		cout.precision(3);

		mat = (double**)calloc(m1, sizeof(double*));
		for(int i = 0; i < m1; ++i)
		{
			mat[i] = (double*)calloc(nm1, sizeof(double));

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

		/*for(int i = 0; i < m1; ++i)
		{
			for(int j = 0; j < nm1; ++j)
				cout << mat[i][j] << "\t";

			cout << endl;
		}*/
		
		m = m1 - 1;
		n = nm1 - m - 1;

		baseT = (int*)calloc(m, sizeof(int));

		for(int j = n, k = 0; j < nm1; ++j)
			for(int i = 1; i < m1; ++i)
				if(mat[i][j] == 1)
				{
					baseT[k++] = j;
				}

		dualSimplex();
	
		if(noSolution)
			cout << "w* = +inf" << endl;
		else{
			cout << "z* = " << mat[0][nm1-1] << "\t";
			bool mult = false;
						
			double *fBase = (double*)calloc(nm1-1, sizeof(double));
			for(int i = 1; i < m1; ++i)
			{
				//cout << baseT[i-2] << "->" << mat[i][nm1-1] << "\t";
				fBase[baseT[i-2]] = mat[i][nm1-1];
			}
				
			cout << "x* = (";
			for(int l = 0; l < nm1-1; ++l)
				cout << " " << fBase[l];
			cout << " ) ";

			for(int i = 1; i < m1; ++i)
				if(mat[i][nm1-1] == 0)
				{
					for(int j = 0; j < nm1-1; ++j)
						if(mat[i][j] < 0)
						{
							mult = true;
							break;
						}
				}
				

			if(mult)
				cout << "Soluções Multiplas" << endl;
			else
				cout << "Solução Única" << endl;
		}
			

		}else{
			cerr << "Arquivo de entrada não encontrado" << endl;
		}

	return 0;
}
