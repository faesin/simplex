#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

//#include "simplex.h"
using namespace std;
double **mat;
int m1, nm1, n, m;
double z = 0;

void mat_multLineEsc(int line, double mult)
{
	for(int i = 0; i < nm1; ++i)
		mat[line][i] *= mult;
}

void mat_multLineLine(int line1, double mult, int line2)
{
	for(int i = 0; i < nm1; ++i)
		mat[line1][i] += mult*mat[line2][i];
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

		//cout << m1 << " " << nm1 << endl;

		m = m1 - 1;
		n = nm1 - m - 1;
		
		//TEST
		//bool once = false;
		cout.precision(3);

		//cout << "m = " << m << "\tn= " << n << endl;

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

		/*for(int i = 0; i < m1; ++i)
		{
			for(int j = 0; j < nm1; ++j)
				cout << mat[i][j] << "\t";

			cout << endl;
		}*/

		for(int i = 0; i < nm1; ++i)
		{
			mat[0][i] *= -1;
		}

		inFile.close();
		bool solved = false;
		bool noSolution = false;

		do{
			int zjcj = 0, pivo = 1;
			//achar maior zj - cj
			for(int i = 0; i < nm1-1; ++i)
			{
				//cout << mat[0][i] << ">" << mat[0][zjcj] << endl;	
				if(mat[0][i] < mat[0][zjcj])
					zjcj = i;
			}

			if(mat[0][zjcj] >= 0)
				solved = true;

			//cout << "zjcj = " << mat[0][zjcj] << endl;

			//achar pivó
			for(int i = 1; i < m1; ++i)
			{
				if(mat[i][zjcj] > 0)
				{
					//cout << mat[i][zjcj] << endl;
					if(mat[pivo][nm1-1]/mat[pivo][zjcj] > mat[i][nm1-1]/mat[i][zjcj])
					{
						pivo = i;
					}
					//cout << mat[i][nm1-1] << "/" << mat[i][zjcj] << endl;
				}			
			}

			if(pivo == 1 && mat[pivo][zjcj] < 0)
			{
				solved = true;
				noSolution = true;
			}
			

			//imprime o tablo
			if(!solved || noSolution)
			{
				cout << "\t\t\t";
				for(int i = 0; i < nm1; ++i)
					if(i == zjcj)
						cout << "V";
					else
						cout << "\t";
				cout << endl;
			}
			
			cout << "\t\t";
			for(int i = 1; i < nm1; ++i)
				cout << "\tx" << i;
			cout << endl << "\tz\t" << mat[0][nm1-1];

			for(int i = 0; i < nm1-1; ++i)
				cout << "\t" << mat[0][i];
			cout << endl;

			for(int i = 1; i < m1; ++i)
			{
				if(i == pivo && !solved)
					cout << "<-";
				
				//this is wrongish
				int thisx = 0;
				for(int j = 0; j < nm1-1; ++j)
					if(mat[i][j] == 1)
						thisx = j;
						
				cout << "\tx" << thisx+1 << "\t" << mat[i][nm1-1];
				for(int j = 0; j < nm1-1; ++j)
					cout << "\t" << mat[i][j];
				cout << endl;
			}

			cout << endl;

			//Do the thangs
			mat_multLineEsc(pivo, 1/mat[pivo][zjcj]);
			for(int i = 0; i < m1; ++i)
			{
				if(i == pivo)
					continue;
				double mult = mat[i][zjcj];
				mat_multLineLine(i, -mult, pivo);
			}

			char cont;
			cin >> cont;
			if(cont == 'a')
				solved = true;
			

			if(solved)
			{
				if(!noSolution)
				{
					bool multiple = false;
					cout << "z* = "	<< mat[0][nm1-1] << " ";

					double* base = (double*)calloc(nm1-1, sizeof(double));
				
					for(int j = 0; j < nm1-1; ++j)
					{
						bool isBase = true;
						if(mat[0][j] == 0)
						{
							bool one = false;
							for(int i = 0; i < m1; ++i)
							{
								if(mat[i][j] == 1)
								{
									base[j] = mat[i][nm1-1];
									if(!one)
										one = true;
									else{
										isBase = false;
										break;
									}
								}

								if(mat[i][j] != 0 && mat[i][j] != 1)
								{
									isBase = false;
									break;
								}
							}

							if(!isBase)
							{
								multiple = true;
								base[j] = 0;
							}
						}
					}

					cout << "x* = (";
					for(int i = 0; i < nm1-1; ++i)
						cout << " " << base[i];
					cout << " ) ";

					if(multiple)
						cout << "soluções multiplas" << endl;
					else
						cout << "solução única" << endl;
					
					free(base);
				}else{
					cout << "z* = -inf" << endl;
				}

				for(int i = 0; i < m1; ++i)
					free(mat[i]);
				free(mat);
			}
		}while(!solved);
	}else{
		cerr << "Arquivo de entrada não encontrado" << endl;
	}

	return 1;
}
