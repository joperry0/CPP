// palindrome.cpp
// author:  Joseph Perry
// desc:    Computes whether a given string or set of strings are palindromes

#include <iostream>
#include <string>

using namespace std;

// computes if the given string is a palindrome
// returns true if it is, otherwise false
bool palindrome(string input_string){
  bool flag = true;
  size_t length = input_string.length();

  if(length > 1){
    if(input_string.front() == input_string.back())
      flag &= palindrome(input_string.substr(1, length-2));
    else
      flag = false;
  } else {
    flag = true;
  }

  return flag;
}

int main(int argc, char *argv[]){
  string input_string = "racecar";

  if(argc > 1)
    for(int i = 1; i < argc; ++i){
      input_string = argv[i];

      if(palindrome(input_string))
        cout << input_string << " is a palindrome" << endl;
      else
        cout << input_string << " is not a palindrome" << endl;
    }

  return 0;
}