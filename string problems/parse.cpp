// parse.cpp
// author:  Joseph Perry
// desc:    Parses a string formatted as a valid set of URL GETs into a map of key:value pairs

#include <iostream>
#include <map>

using namespace std;

// assuming input_string is properly formatted
map<string, string> parse(string input_string){
  map<string, string> result;
  string key, value;
  
  int offset = 0;
  int amp_pos = 0;
  int equ_pos = 0;

  while(amp_pos != string::npos){
    equ_pos = input_string.find('=', offset);
    amp_pos = input_string.find('&', offset);

    // key is found [offset, equ_pos)
    key = input_string.substr(offset, equ_pos - offset);

    // value is found [equ_pos + 1, amp_pos)
    value = input_string.substr(equ_pos + 1, amp_pos - equ_pos - 1);

    if(key.compare("") == 0)
      break;

    result[key] = value;
    offset = amp_pos + 1;
  }

  return result;
}

void print_result(map<string, string> result){
  for(auto res : result)
    cout << res.first << ": " << res.second << endl;  
}

int main(){
  map<string, string> result = parse("key1=value1&key2=value2&key3=value3");

  print_result(result);

  return 0;
}