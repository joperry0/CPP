// smoker.h
// author:  Joseph Perry
// desc:  This file defines a Smoker and Merchant class for the classic concurrency problem.

#ifndef SMOKER_H
#define SMOKER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <list>
#include <random>
#include <chrono>

// State and Inventory Enums
enum class Merchant_state : int { Waiting, Producing };
enum class Smoker_state : int { Waiting, Smoking };
enum class Item : int { Paper, Tobacco, Lighter };

// Inventory Helper function - prints the inventory
void print_inventory(std::map<Item, int> items){
  int i = 0;
  for(std::map<Item, int>::iterator mi=items.begin();
                 mi!=items.end();
                 ++mi){
    std::cout << i++ << " " << mi->second << "\n";
  }
}

class Smoker {
  public:
    // constructor function
    Smoker(Item, std::map<Item, int>);

    // main loop
    void can_smoke(std::map<Item, int> &);

  private:
    Smoker_state state;
    Item item, prev, next;
    std::map<Item, int> inventory;
    int num_smokes;
    int id;

    // gets the remaining two items from table
    void get_items(std::map<Item, int>&);

    // consume items in inventory
    void smoke();

    // wait for sometime after smoking
    void wait();

    // shared class locking mechanisms
    static std::mutex mtx;
    static std::condition_variable cv;

    // Merchant can access mtx and cv
    friend class Merchant;
};

// declaration of shared class locking mechanisms
std::mutex Smoker::mtx;
std::condition_variable Smoker::cv;

// constructor
Smoker::Smoker(Item item, std::map<Item, int> inventory) : item(item), inventory(inventory), state(Smoker_state::Waiting){

  inventory[item] = 1;

  if(item == Item::Paper){
    next = Item::Tobacco;
    prev = Item::Lighter;
    id = 0;

  } else if(item == Item::Tobacco){
    next = Item::Paper;
    prev = Item::Lighter;
    id = 1;

  } else {
    next = Item::Tobacco;
    prev = Item::Paper;
    id = 2;
  }

  num_smokes = 0;
}

// concurrent main loop, contains critical section
void Smoker::can_smoke(std::map<Item, int> &items){
  std::unique_lock<std::mutex> lck(Smoker::mtx);

  // wait for items
  while(items[prev] <= 0 || items[next] <= 0){
    Smoker::cv.wait(lck);
  }

  // critical section
  get_items(items);
  smoke();

  // unlock
  Smoker::cv.notify_all();
  wait();
}

// critical section
void Smoker::get_items(std::map<Item, int> &items){
  for(std::map<Item, int>::iterator mi=items.begin();
                 mi!=items.end();
                 ++mi){
    if(mi->first != item){
      ++inventory[mi->first];
      --mi->second;
    }
  }
}

// consume items in inventory
void Smoker::smoke(){
  state = Smoker_state::Smoking;

  std::cout << id << ": " << "Smoking... " << num_smokes + 1 << "\n";
  for(std::map<Item, int>::iterator mi=inventory.begin();
                 mi!=inventory.end();
                 ++mi){
    if(mi->first != item){
      --mi->second;
    }
  }

  num_smokes++;
  state = Smoker_state::Waiting;
}

// wait after smoking
void Smoker::wait(){
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

class Merchant {
  public:
    // constructor
    Merchant();

    // main loop
    void can_produce(std::map<Item, int>&);

  private:
    Merchant_state state;

    // critical section, adding items to shared pool
    void produce_items(std::map<Item, int>&);

    // wait after producing items
    void wait();
};

// constructor
Merchant::Merchant() : state(Merchant_state::Waiting) {}

// main loop, contains critical section
void Merchant::can_produce(std::map<Item, int> &items){
  std::unique_lock<std::mutex> lck(Smoker::mtx);

  // wait for items to be depleted, using the Smoker class locking mechanisms
  while(items[Item::Paper] > 0 || items[Item::Tobacco] > 0 || items[Item::Lighter] > 0){
    Smoker::cv.wait(lck);
  }

  // critical section
  produce_items(items);

  Smoker::cv.notify_all();
  wait();
}

// critical section
void Merchant::produce_items(std::map<Item, int> &items){
  int r = random() % 3;

  std::cout << "Producing ... " << r << "\n";
  state = Merchant_state::Producing;

  // corresponds to id
  if(r == 0){ // paper_smoker
    ++items[Item::Lighter];
    ++items[Item::Tobacco];

  } else if(r == 1){ // tobacco_smoker
    ++items[Item::Paper];
    ++items[Item::Lighter];

  } else { // lighter_smoker
    ++items[Item::Paper];
    ++items[Item::Tobacco];
  }

  state = Merchant_state::Waiting;
}

// wait after producing items
void Merchant::wait(){
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

#endif