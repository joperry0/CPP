#include "smoker.h"

int main() {
	std::map<Item, int> items = { { Item::Paper, 0 },
								{ Item::Tobacco, 0 },
								{ Item::Lighter, 0 } };

	Smoker paper = Smoker(Item::Paper, items);
	Smoker tobacco = Smoker(Item::Tobacco, items);
	Smoker lighter = Smoker(Item::Lighter, items);
	Merchant merchant = Merchant();

	std::thread paper_thread([&paper, &items](){
		while(true)
			paper.can_smoke(items);
	});
	std::thread tobacco_thread([&tobacco, &items](){
		while(true)
			tobacco.can_smoke(items);
	});
	std::thread lighter_thread([&lighter, &items](){
		while(true)
			lighter.can_smoke(items);
	});
	std::thread merchant_thread([&merchant, &items](){
		while(true)
			merchant.can_produce(items);
	});

	paper_thread.join();
	tobacco_thread.join();
	lighter_thread.join();
	merchant_thread.join();

	return 0;
}