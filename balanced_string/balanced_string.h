// balanced_string.h
// author:  Joseph Perry
// desc:    This file defines a function that determines whether a given input
//          string is balanced, e.g. contains a matching set of [], {}. (), <>
//          where these characters are encountered in-order.

#ifndef BALANCED_STRING_H
#define BALANCED_STRING_H

#include <iostream>
#include <string>
#include <stack>

bool balanced_string(std::string s){
  std::stack<char> st;
  char ch;
  bool flag = true;
  for(int i = 0; i < s.size(); ++i){
    ch = s[i];

    switch(ch){
      case '{':
      case '[':
      case '<':
      case '(':   st.push(ch);
                  break;

      case '}':   if('{' != st.top())
                    flag = false;
                  st.pop();
                  break;

      case ']':   if('[' != st.top())
                    flag = false;
                  st.pop();
                  break;

      case '>':   if('<' != st.top())
                    flag = false;
                  st.pop();
                  break;

      case ')':   if('(' != st.top())
                    flag = false;
                  st.pop();
                  break;

      default:    flag = false;
                  break;
    }

    if(flag == false)
      break;
  }

  return flag;
}

#endif