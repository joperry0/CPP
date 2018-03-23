#include <iostream>
#include <stack>
#include <sstream>

using namespace std;

int move(int, stack<int>*, stack<int>*, stack<int>*);

int towers_hanoi(int n){
	stack<int> one, two, three;
	int moves = 0;

	for(int i = n; i > 0; --i)
		one.push(i);

	moves = move(n, &one, &two, &three);

	return moves;
}

int move(int n, stack<int> *a, stack<int> *b, stack<int> *c){
	int i = 0;
	if(n > 0){
		i += move(n-1, a, c, b);
		c->push(a->top());
		a->pop();
		i++;
		i += move(n-1, c, b, a);
	}
	return i;
}

int main(int argc, char *argv[]){
	if(argc < 2){
		cerr << "Usage: ./towers.o <int>" << endl;
		return 1;
	}
	istringstream ss(argv[1]);
	int n;
	if(!(ss >> n)){
		cerr << "Invalid input : " << argv[1] << " (expected int, defaulting to n=3)" << endl;
		n = 3;
	}

	int moves = towers_hanoi(n);

	cout << "Number of moves = " << moves << endl;

	return 0;
}