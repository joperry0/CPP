#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <sstream>

using namespace std;

enum class Philosopher_state : int { Dining, Thinking };

class Philosopher {
	public:
		Philosopher(int, int);
		void can_eat(vector<int> &);
	private:
		Philosopher_state state;
		int id;
		int num_dine;
		int num_philosophers;
		int left_chopstick;
		int right_chopstick;
		void get_chopsticks(vector<int> &);
		void put_back_chopsticks(vector<int> &);
		void eat();
		void think();
		static mutex mtx;
		static condition_variable cv;
};

mutex Philosopher::mtx;
condition_variable Philosopher::cv;

Philosopher::Philosopher(int id, int num_philosophers) : id(id),
										state(Philosopher_state::Thinking),
										 num_philosophers(num_philosophers) {

    left_chopstick = id;
    right_chopstick = (id + 1) % num_philosophers;
    num_dine = 0;
}

void Philosopher::can_eat(vector<int> &chopsticks){
	unique_lock<mutex> lck(mtx);

	while(chopsticks[left_chopstick] == 0 || chopsticks[right_chopstick] == 0){
		cv.wait(lck);
	}

	get_chopsticks(chopsticks);

	eat();

	put_back_chopsticks(chopsticks);

	cv.notify_all();

	think();
}

void Philosopher::get_chopsticks(vector<int> &chopsticks){
	chopsticks[left_chopstick] = 0;
	chopsticks[right_chopstick] = 0;
}

void Philosopher::put_back_chopsticks(vector<int> &chopsticks){
	chopsticks[left_chopstick] = 1;
	chopsticks[right_chopstick] = 1;
}

void Philosopher::eat(){
	state = Philosopher_state::Dining;
	num_dine++;
	cout << id << " eating ... " << num_dine << "\n";
}

void Philosopher::think(){
	state = Philosopher_state::Thinking;
	this_thread::sleep_for(chrono::seconds(2));
}

int main(){
	int id = 0;
	int num_philosophers = 5;

	vector<int> chopsticks(num_philosophers, 1);

	Philosopher *a = new Philosopher(id++, num_philosophers);
	Philosopher *b = new Philosopher(id++, num_philosophers);
	Philosopher *c = new Philosopher(id++, num_philosophers);
	Philosopher *d = new Philosopher(id++, num_philosophers);
	Philosopher *e = new Philosopher(id, num_philosophers);

	thread a_thread([&](){
		while(true)
			a->can_eat(chopsticks);
	});

	thread b_thread([&](){
		while(true)
			b->can_eat(chopsticks);
	});

	thread c_thread([&](){
		while(true)
			c->can_eat(chopsticks);
	});

	thread d_thread([&](){
		while(true)
			d->can_eat(chopsticks);
	});

	thread e_thread([&](){
		while(true)
			e->can_eat(chopsticks);
	});


	a_thread.join();
	b_thread.join();
	c_thread.join();
	d_thread.join();
	e_thread.join();

	return 0;
}