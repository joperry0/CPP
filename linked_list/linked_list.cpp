#include "linked_list.h"

using namespace std;

int main(int argc, char *argv[]){
    LinkedList<string> linked_list;
    LinkedList<string> l2(linked_list);
    string k, l;

    try{
        l = linked_list.find_first("5");
    } catch(ZeroLengthException &e) {
        cout << e.what() << endl;
    }

    l2.print_list();

    linked_list.push_back("4");
    linked_list.push_back("5");
    linked_list.push_back("6");
    linked_list.push_back("7");
    linked_list.push_back("cheese");
    linked_list.push_back("5");
    linked_list.push_front("swiss");

    linked_list.print_list();
    linked_list.reverse();
    cout << linked_list.back() << endl;
    linked_list.print_list();

    l2.print_list();
    l2 = linked_list;
    l2.print_list();

    LinkedList<string> l3 = l2;
    l3.print_list();
    cout << l3.size() << endl;

    l3.push_back("17");

    if(l3 != l2)
        cout << "l3 != l2" << endl;
    else
        cout << "l3 == l2" << endl;

    cout << l3.back() << endl;

    k = linked_list.find_last("5");

    cout << k << endl;

    cout << linked_list.size() << endl;
    cout << linked_list.pop_front() << endl;
    cout << linked_list.size() << endl;
    linked_list.print_list();

    l2.selection_sort();
    l2.print_list();

    // cout << linked_list << endl;

    return 0;
}