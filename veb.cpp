#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <assert.h>
#include <iostream>
#include "veb.h"

#define DB(x) std::cerr << #x << " = " << x << "\n";
#define SH (S / 2) // S HALF  

template<unsigned int S>
class VEBTree : AbstractVEBTree<S> {
private:
	std::map < unsigned long long, VEBTree<SH> > buckets;

	VEBTree<SH> summary;
	unsigned long long min;
	unsigned long long max;

	inline unsigned long long low(unsigned long long x) const {
		return x & ((1ULL << SH) - 1);
	}

	inline unsigned long long high(unsigned long long x) const {
		return low(x >> SH);
	}
	
public:

	bool empty() const {
		return min == NO;
	}

	VEBTree() : min(NO), max(NO) {
		
	}

	void add(unsigned long long x) {

		if (empty()) {
			min = max = x;
			return;
		} 

		if (x < min) {
			std::swap(x, min);
		}

		max = std::max(max, x);
		unsigned long long hi = high(x);
		unsigned long long lo = low(x);

		if (!buckets.count(hi) || buckets[hi].empty()) {
			buckets[hi].add(lo); // O(1)
			summary.add(hi);
		} else {
			assert(!buckets[hi].empty());
			buckets[hi].add(lo);
		}
	}

	void remove(unsigned long long x) {
		if (empty()) 
			return;

		if (min == max) {
			if (min != x) {
				return;
			}
			min = max = NO;
			return;
		}

		if (x == min) {
			x = buckets[summary.getMin()].getMin() | (summary.getMin() << SH);
			min = x;
		}

		unsigned long long hi = high(x);
		unsigned long long lo = low(x);

		if (!buckets.count(hi)) {
			return;
		}

		buckets[hi].remove(lo);
	
		if (buckets[hi].empty()) {
			summary.remove(hi);
		}

		if (x == max) {
			if (summary.empty()) {
				max = min;
			} else {
				max = buckets[summary.getMax()].getMax() | (summary.getMax() << SH);
			}
		}
	}

	unsigned long long next(unsigned long long x) const {
		
		if (empty()) {
			return NO;
		}

		if (x < min) {
			return min;
		}

		unsigned long long hi = high(x);
		unsigned long long lo = low(x);
		unsigned long long ret = NO;

		if (buckets.count(hi) && buckets.at(hi).getMax() != NO && lo < buckets.at(hi).getMax()) {
			ret = buckets.at(hi).next(lo);
			assert(ret != NO);
			ret |= (hi << SH);
		} else {
			unsigned long long cur = summary.next(hi);
			if (cur != NO) {
				ret = buckets.at(cur).getMin();
				ret |= (cur << SH);
			}
		}
		return ret;
	}

	unsigned long long prev(unsigned long long x) const {
		if (empty()) {
			return NO;
		}

		unsigned long long ret = NO;

		if (x <= min) {
			return NO;
		} else {
			ret = min;
		}

		unsigned long long hi = high(x);
		unsigned long long lo = low(x);

		if(buckets.count(hi) && buckets.at(hi).getMin() != NO && buckets.at(hi).getMin() < lo) {
			ret = buckets.at(hi).prev(lo);
			assert(ret != NO);
			ret |= (hi << SH);
		} else {
			unsigned long long cur = summary.prev(hi);
			if (cur != NO) {
				ret = buckets.at(cur).getMax();
				ret |= (cur << SH);
			}
		}
		return ret;
	}

	unsigned long long getMin() const {
		return min;
	}

	unsigned long long getMax() const {
		return max;
	}
};

template<>
class VEBTree<1> : AbstractVEBTree<1> {

private:
	unsigned long long min;
	unsigned long long max;
	unsigned int mask;
	static const bool peril = true;

public:

	bool empty() const {
		return mask == 0;
	}

	VEBTree() : min(NO), max(NO), mask(0) {
	
	}

	void remove(unsigned long long x) {
		mask &= ~(1 << x);
	}

	void add(unsigned long long x) {
		mask |= (1 << x);
	}

	unsigned long long next(unsigned long long x) const {
		if (peril) {
			unsigned int remainder = mask & (~(1 << (x + 1) - 1));
			if (remainder == 0) 
				return NO;
			return __builtin_ffs(remainder) - 1;		
		} else {
			// x++;
			// while (x < 2) {
			// 	if (mask & (1 << x)) {
			// 		return x;
			// 	}
			// 	x++;
			// }
			// return NO;
		}
	}

	unsigned long long prev(unsigned long long x) const {
		if (peril) {
			unsigned int remainder = mask & ((1 << x) - 1);
			if (remainder == 0)
				return NO;
			return 1 - (__builtin_clz(remainder) - 30);
		} else {
			// if (!x) 
			// 	return NO;
			// x--;
			// while (x > 0) {
			// 	if (mask & (1 << x)) 
			// 		break;
			// 	x--;
			// }
			// if (mask & (1 << x)) {
			// 	return x;
			// } else {
			// 	return NO;
			// }
		}
	}

	unsigned long long getMin() const {
		if (empty()) 
			return NO;
		if (peril) {
			return __builtin_ffs(mask) - 1;	
		} else {
			// if (mask & 1) return 0;
			// else return next(0);
		}
		
	}

	unsigned long long getMax() const {
		if (empty()) 
			return NO;
		if (peril) {
			return 1 /*2^S - 1*/ - (__builtin_clz(mask) - 30);
		} else {
			// return prev(2);
		}
	}
};

void expect_true(bool val) {
	if (!val) {
		std::cout << "FAIL\n";
	}
}

template <unsigned int S>
int size(VEBTree<S> const& t) {
	if (t.empty()) return 0;
	
	int x = t.getMin();
	int result = 0;
	
	while (x != -1) {
		result++;
		x = t.next(x);
	}

	return result;
}


template <unsigned int S>
void enumerate(VEBTree <S> const & t) {
	int x = t.getMin();
	std::cout << "\n";
	while (x != -1) {
		std::cout << x << " ";
		x = t.next(x);
	}
}

template <unsigned int S> 
void rev_enumerate(VEBTree <S> const & t) {
	int x = t.getMax();
	std::cout << "\n";
	while (x != -1) {
		std::cout << x << " ";
		x = t.prev(x);
	}
}

void rev_enumerate(std::set <int> & s) {
	auto rit = s.rbegin();
	std::cout << "\n";
	while (rit != s.rend()) {
		std::cout << *rit++ << " ";
	}
}

void enumerate(std::set <int> & s) {
	std::cout << "\n";
	for (int x : s) {
		std::cout << x << " ";
	}
}

void test() {
	const int S = 32;
	
	VEBTree <S> t;
	std::set <int> s;
	const int n = 1e5;

	std::vector <int> elements;

	for (int i = 0; i < n; i++) {
		//int element = rand() % (1 << S);
		int element = rand();
		if (s.find(element) == s.end()) {
			s.insert(element);
			t.add(element);
			elements.push_back(element);	
		}
	}

	// enumerate(t);
	
	// enumerate(s);

	// rev_enumerate(s);

	// rev_enumerate(t);

	assert(size(t) == s.size());

	random_shuffle(elements.begin(), elements.end());

	for (int i = 0; i < n; i++) {
		// int element = rand() % (1 << S);
		int element = rand();

		auto it = s.upper_bound(element);
		int x = t.next(element);

		if (it == s.end()) {
			expect_true(x == -1);
		} else {
			expect_true(x == *it);
		}

		it = s.lower_bound(element);
		
		x = t.prev(element);

		if (it == s.begin()) {
			expect_true(x == -1);
		} else {
			it--;
			expect_true(x == *it);
		}

		if (i % 4 == 0) {
			if (!elements.empty()) {
				s.erase(elements.back());
				t.remove(elements.back());
				elements.pop_back();	
			}
		}

		if (i % 8 == 0) {
			int element = rand();
			if (s.find(element) == s.end()) {
				s.insert(element);
				t.add(element);
			}
		}
	}
}



void stress() {
	const int S = 16;

	const int n = 6;

	const bool debug = false;

	if (debug) {
		while (true) {

			VEBTree <S> t;
			std::set <int> s;
			std::unordered_map <int, bool> used;

			for (int i = 0; i < n; i++) {
				int element = rand() % (1 << S);

				if (used.count(element)) {
					continue;
				}
				used[element] = true;

				t.add(element);
				s.insert(element);
			
			}

			if (s.size() != size(t)) {
				std::cout << "YES\n";

				enumerate(s);
				enumerate(t);

				break;
			}
		}	
	} else {

		VEBTree <S> t;
		t.add(10);
		t.add(15);
		t.add(20);
		t.remove(10);
		t.add(50);
		t.remove(20);
		t.remove(50);
		t.add(200);
		t.add(100);
		rev_enumerate(t);
	}
}

int main() {
	test();
	return 0;
}
