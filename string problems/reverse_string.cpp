// reverse_string.cpp
// author:  Joseph Perry
// desc:    Reverses a given string or set of strings and outputs them to cout

#include <iostream>

using namespace std;

// Reverses a string in place without a temporary variable
string reverse(string str){
  size_t length = str.length();

  if(length > 1){
    for(int i = 0; i < length/2; ++i){
      str[i] = str[i] + str[length-i-1];
      str[length-i-1] = str[i] - str[length-i-1];
      str[i] = str[i] - str[length-i-1];
    }
  }
  
  return str;
}

int main(int argc, char* argv[]){
  string str = "hello world";

  if(argc > 1)
    for(int i = 1; i < argc; ++i)
      cout << argv[i] << endl << reverse(argv[i]) << endl;
  else
    cout << str << " " << reverse(str) << endl;

  return 0;
}