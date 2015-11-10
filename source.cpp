#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <random>

using namespace std;

int main()
{
	const int memory = 2;
	int sequence_no = -3;
	int mem[3] = {-3, -1, 0};


	int **cell = new int*[1 << memory], **edge = new int*[1 << memory], **temp = new int*[1 << memory];

	for (int i = 0; i < (1 << memory); i++)
		cell[i] = new int[2], edge[i] = new int[1 << memory], temp[i] = new int[1 << memory];

	for (int i = 0; i < (1 << memory); i++)
		cell[i][0] = cell[i][1] = 0;

	for (int i = 0; i < (1 << memory); i++)
		for (int j = 0; j < (1 << memory); j++)
			edge[i][j] = temp[i][j] = INT_MAX;
	edge[0][0] = -4; edge[0][1] = -2;
	edge[1][2] = 0; edge[1][3] = 2;
	edge[2][0] = -2; edge[2][1] = 0;
	edge[3][2] = 2; edge[3][3] = 4;

	string message[1 << memory] = {
		"00",
		"01",
		"10",
		"11"
	};
	string next_msg[1 << memory];
	cout << "Enter transmited bits (space separated) : ";
	bool input;
	cin >> input;
	int seq = 0;



	default_random_engine generator;
	normal_distribution<double> distribution(0.0, 1.0);

	while (true)
	{
		seq += 1;
		int multiplier;
		if (input)
			multiplier = 1;
		else
			multiplier = -1;
		mem[0] += 1*multiplier ;
		mem[1] += 2*multiplier;
		mem[2] += 1*multiplier;

		sequence_no = mem[0];
		mem[0] = mem[1];
		mem[1] = mem[2];
		mem[2] = 0;

		cout << "ISI seq: " << sequence_no << endl;;
		double number = distribution(generator);
		int add;
		if (number < 0.000001)
			add = floor(number);
		else add = ceil(number);
		sequence_no += add;

		if (seq == 1){
			cell[0][1] = cell[0][0] + pow(sequence_no + 4, 2);
			cell[1][1] = cell[1][0] + pow(sequence_no + 2, 2);
			cout << "0 (" << cell[0][1] << "), 1 (" << cell[1][1] << ") ";
		}
		else if (seq == 2){
			cell[0][1] = cell[0][0] + pow(sequence_no + 4, 2) ;
			cell[1][1] = cell[0][0] + pow(sequence_no + 2, 2);
			cell[2][1] = cell[1][0] + pow(sequence_no, 2);
			cell[3][1] = cell[1][0] + pow(sequence_no - 2, 2);
			cout << "00 (" << cell[0][1] << "), 01 (" << cell[1][1];
			cout << "), 10 (" << cell[2][1] << "), 11 (" << cell[3][1] << ") ";
		}
		else{
			for (int i = 0; i < (1 << memory); i++){
				for (int j = 0; j < (1 << memory); j++){
					if (edge[i][j] != INT_MAX)
						temp[i][j] = pow(sequence_no - edge[i][j], 2);
				}
			}
			for (int i = 0; i < (1 << memory); i++){
				if (temp[i >> 1][i] + cell[i >> 1][0] < temp[(i >> 1) + 2][i] + cell[(i >> 1) + 2][0]){
					next_msg[i] = message[i >> 1] + to_string(i % 2);
					cell[i][1] = cell[i >> 1][0] + temp[i >> 1][i];
				}
				else{
					next_msg[i] = message[(i >> 1) + 2] + to_string(i % 2);
					cell[i][1] = cell[(i >> 1) + 2][0] + temp[(i >> 1) + 2][i];
				}
			}
		}

		for (int i = 0; i < (1 << memory); i++){
			cell[i][0] = cell[i][1];
			cell[i][1] = INT_MAX;
			if (seq > 2){
				message[i] = next_msg[i];
				cout << message[i] << " (" << cell[i][0] << "), " ;
			}
		} cout << "\n"<< endl;
		cin >> input;
	}
	return 0;
}
