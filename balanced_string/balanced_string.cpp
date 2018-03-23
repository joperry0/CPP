#include "balanced_string.h"

using namespace std;

int main(int argc, char *argv[]){
	string example1 = "{}";
	string example2 = "[(<>])";
	string example3 = "";

	cout << example1 << " " << balanced_string(example1) << endl;
	cout << example2 << " " << balanced_string(example2) << endl;
	cout << example3 << " " << balanced_string(example3) << endl;

	return 0;
}