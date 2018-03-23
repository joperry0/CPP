// lcs.cpp
// author:  Joseph Perry
// desc:    This file computes the least common subsequence between two strings.

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// builds a MxN table of 2-tuples to compute the least common subsequence
vector<vector<tuple<int, char>>> lcs_length(string x, string y){
	int m = x.size();
	int n = y.size();
	int i, j;

	vector<vector<tuple<int, char>>> table(m+1, vector<tuple<int, char>>(n+1, make_tuple(0, ' ')));

	for(i = 1; i <= m; ++i){
		for(j = 1; j <= n; ++j){
			if(x[i-1] == y[j-1]){
				table[i][j] = make_tuple(get<0>(table[i-1][j-1])+1, 'Q'); // Left diagonal

			} else if(get<0>(table[i-1][j]) >= get<0>(table[i][j-1])){
				table[i][j] = make_tuple(get<0>(table[i-1][j]), 'W');     // Up

			} else {
				table[i][j] = make_tuple(get<0>(table[i][j-1]), 'A');     // Left
			}
		}
	}

	return table;
}

// walks the table previously built and prints out the least common subsequence
void print_lcs(vector<vector<tuple<int, char>>> &table, string x, int i, int j){
	if(i == 0 || j == 0)
		return;

	if(get<1>(table[i][j]) == 'Q'){ 		     // Left diagonal
		print_lcs(table, x, i-1, j-1);
		cout << x[i-1];

	} else if(get<1>(table[i][j]) == 'W'){ 	 // Up
		print_lcs(table, x, i-1, j);

	} else { 								                // Left
		print_lcs(table, x, i, j-1);
	}
}

int main(int argc, char *argv[]){
	vector<vector<tuple<int, char>>> table;
	string x = "ABCBDAB";
	string y = "BDCABA";

	if(argc >= 3){
		x = argv[1];
		y = argv[2];
	}
	
	table = lcs_length(x, y);

	cout << x << endl;
	cout << y << endl;
	print_lcs(table, x, x.size(), y.size());

	return 0;
}