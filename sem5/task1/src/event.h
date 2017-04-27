#pragma once
#include <functional>
#include <vector>


class event {
public:
	typedef std::function<void(const char *arg)> callback_t;
	void subscribe(callback_t f) {
		subscribers.push_back(f);
	}
	void fire(const char* str) {
		for (uint i = 0; i < subscribers.size(); i++) {
			subscribers[i](str);
		}
	}
private:
	std::vector<callback_t> subscribers;
};
