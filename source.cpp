#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
using namespace std;

int main()
{
	const int memory = 2;
	/*cout << "Enter Decoder Memory States: ";
	cin >> memory;*/
	int sequence_no = -3;
	int mem[3] = {-3, -1, 0};

	// cout << "Initial default sequence numbers : -1";

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
	while (true)
	{
		seq += 1;
		int multiplier;
		if (input)
			multiplier = 1;
		else
			multiplier = -1;
		mem[0] += 1*multiplier;
		mem[1] += 2*multiplier;
		mem[2] += 1*multiplier;

		sequence_no = mem[0];
		mem[0] = mem[1];
		mem[1] = mem[2];
		mem[2] = 0;

		cout << sequence_no << " ";

		if (seq == 1){
			cell[0][1] = cell[0][0] + pow(sequence_no + 4, 2);
			cell[1][1] = cell[1][0] + pow(sequence_no + 2, 2);
			//cout << "Possibilities: 0 1" << endl;
			cout << "0 (" << cell[0][1] << "), 1 (" << cell[1][1] << ") ";
		}
		else if (seq == 2){
			cell[0][1] = cell[0][0] + pow(sequence_no + 4, 1);
			cell[1][1] = cell[1][0] + pow(sequence_no + 2, 2);
			cell[2][1] = cell[2][0] + pow(sequence_no, 2);
			cell[3][1] = cell[3][0] + pow(sequence_no - 2, 2);
			//cout << "Possibilities: 00 01 10 11" << endl;
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
				if (temp[i >> 1][i] < temp[(i >> 1) + 2][i]){
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
			cell[i][0] = cell[i][1], cell[i][1] = INT_MAX;
			if (seq > 2){
				message[i] = next_msg[i];
				cout << message[i] << " (" << cell[i][0] << "), " ;
			}
		} cout <<  endl;
		cin >> input;
	}
	return 0;
}
