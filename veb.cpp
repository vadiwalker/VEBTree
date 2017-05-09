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

template <unsigned int S>
class VEBTree : AbstractVEBTree<S> {
private:
	std::map < unsigned long long, VEBTree<SH> > buckets;

	VEBTree<S - SH> summary;
	unsigned long long min;
	unsigned long long max;

	inline unsigned long long low(unsigned long long x) const {
		return x & ((1ULL << SH) - 1);
	}

	inline unsigned long long high(unsigned long long x) const {
		return x >> SH;
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
	unsigned long long mask;

public:

	bool empty() const {
		return mask == 0;
	}

	VEBTree() : min(NO), max(NO), mask(0) {
	
	}

	void remove(unsigned long long x) {
		mask &= ~(1ULL << x);
	}

	void add(unsigned long long x) {
		mask |= (1ULL << x);
	}

	unsigned long long next(unsigned long long x) const {
		unsigned long long remainder = mask & (~((1ULL << (x + 1)) - 1));
		if (remainder == 0) 
			return NO;
		return __builtin_ffsll(remainder) - 1;		
	}

	unsigned long long prev(unsigned long long x) const {
		unsigned long long remainder = mask & ((1ULL << x) - 1);
		if (remainder == 0)
			return NO;
		return 63 - __builtin_clzll(remainder);
	}

	unsigned long long getMin() const {
		if (empty())
			return NO;
		return __builtin_ffsll(mask) - 1;	
		
	}

	unsigned long long getMax() const {
		if (empty()) 	 
			return NO;
		return 63 - __builtin_clzll(mask);
	}
};


template<>
class VEBTree<2> : AbstractVEBTree<2> {

private:
	unsigned long long min;
	unsigned long long max;
	unsigned long long mask;

public:

	bool empty() const {
		return mask == 0;
	}

	VEBTree() : min(NO), max(NO), mask(0) {
	
	}

	void remove(unsigned long long x) {
		mask &= ~(1ULL << x);
	}

	void add(unsigned long long x) {
		mask |= (1ULL << x);
	}

	unsigned long long next(unsigned long long x) const {
		unsigned long long remainder = mask & (~((1ULL << (x + 1)) - 1));
		
		if (remainder == 0) 
			return NO;
		unsigned long long ret = __builtin_ffsll(remainder) - 1;
		return __builtin_ffsll(remainder) - 1;		
	}

	unsigned long long prev(unsigned long long x) const {
		unsigned long long remainder = mask & ((1ULL << x) - 1);
		if (remainder == 0)
			return NO;
		return 63 - __builtin_clzll(remainder);
	}

	unsigned long long getMin() const {
		if (empty())
			return NO;
		return __builtin_ffsll(mask) - 1;	
		
	}

	unsigned long long getMax() const {
		if (empty()) 	 
			return NO;
		return 63 - __builtin_clzll(mask);
	}
};

template<>
class VEBTree<3> : AbstractVEBTree<3> {

private:
	unsigned long long min;
	unsigned long long max;
	unsigned long long mask;

public:

	bool empty() const {
		return mask == 0;
	}

	VEBTree() : min(NO), max(NO), mask(0) {
	
	}

	void remove(unsigned long long x) {
		mask &= ~(1ULL << x);
	}

	void add(unsigned long long x) {
		mask |= (1ULL << x);
	}

	unsigned long long next(unsigned long long x) const {
		unsigned long long remainder = mask & (~((1ULL << (x + 1)) - 1));
		if (remainder == 0) 
			return NO;
		return __builtin_ffsll(remainder) - 1;		
	}

	unsigned long long prev(unsigned long long x) const {
		unsigned long long remainder = mask & ((1ULL << x) - 1);
		if (remainder == 0)
			return NO;
		return 63 - __builtin_clzll(remainder);
	}

	unsigned long long getMin() const {
		if (empty())
			return NO;
		return __builtin_ffsll(mask) - 1;	
		
	}

	unsigned long long getMax() const {
		if (empty()) 	 
			return NO;
		return 63 - __builtin_clzll(mask);
	}
};



template<>
class VEBTree<4> : AbstractVEBTree<4> {

private:
	unsigned long long min;
	unsigned long long max;
	unsigned long long mask;

public:

	bool empty() const {
		return mask == 0;
	}

	VEBTree() : min(NO), max(NO), mask(0) {
	
	}

	void remove(unsigned long long x) {
		mask &= ~(1ULL << x);
	}

	void add(unsigned long long x) {
		mask |= (1ULL << x);
	}

	unsigned long long next(unsigned long long x) const {
		unsigned long long remainder = mask & (~((1ULL << (x + 1)) - 1));
		if (remainder == 0) 
			return NO;
		return __builtin_ffsll(remainder) - 1;		
	}

	unsigned long long prev(unsigned long long x) const {
		unsigned long long remainder = mask & ((1ULL << x) - 1);
		if (remainder == 0)
			return NO;
		return 63 - __builtin_clzll(remainder);
	}

	unsigned long long getMin() const {
		if (empty())
			return NO;
		return __builtin_ffsll(mask) - 1;	
		
	}

	unsigned long long getMax() const {
		if (empty()) 	 
			return NO;
		return 63 - __builtin_clzll(mask);
	}
};

template<>
class VEBTree<5> : AbstractVEBTree<5> {

private:
	unsigned long long min;
	unsigned long long max;
	unsigned long long mask;

public:

	bool empty() const {
		return mask == 0;
	}

	VEBTree() : min(NO), max(NO), mask(0) {
	
	}

	void remove(unsigned long long x) {
		mask &= ~(1ULL << x);
	}

	void add(unsigned long long x) {
		mask |= (1ULL << x);
	}

	unsigned long long next(unsigned long long x) const {
		unsigned long long remainder = mask & (~((1ULL << (x + 1)) - 1));
		if (remainder == 0) 
			return NO;
		return __builtin_ffsll(remainder) - 1;		
	}

	unsigned long long prev(unsigned long long x) const {
		unsigned long long remainder = mask & ((1ULL << x) - 1);
		if (remainder == 0)
			return NO;
		return 63 - __builtin_clzll(remainder);
	}

	unsigned long long getMin() const {
		if (empty())
			return NO;
		return __builtin_ffsll(mask) - 1;	
		
	}

	unsigned long long getMax() const {
		if (empty()) 	 
			return NO;
		return 63 - __builtin_clzll(mask);
	}
};


template<>
class VEBTree<6> : AbstractVEBTree<6> {

private:
	unsigned long long min;
	unsigned long long max;
	unsigned long long mask;

public:

	bool empty() const {
		return mask == 0;
	}

	VEBTree() : min(NO), max(NO), mask(0) {
	
	}

	void remove(unsigned long long x) {
		mask &= ~(1ULL << x);
	}

	void add(unsigned long long x) {
		mask |= (1ULL << x);
	}

	unsigned long long next(unsigned long long x) const {
		unsigned long long remainder = mask & (~((1ULL << (x + 1)) - 1));
		if (remainder == 0) 
			return NO;
		return __builtin_ffsll(remainder) - 1;		
	}

	unsigned long long prev(unsigned long long x) const {
		unsigned long long remainder = mask & ((1ULL << x) - 1);
		if (remainder == 0)
			return NO;
		return 63 - __builtin_clzll(remainder);
	}

	unsigned long long getMin() const {
		if (empty())
			return NO;
		return __builtin_ffsll(mask) - 1;	
		
	}

	unsigned long long getMax() const {
		if (empty()) 	 
			return NO;
		return 63 - __builtin_clzll(mask);
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
	const int S = 48;
	
	VEBTree <S> t;
	std::set <int> s;
	const int n = 1e5;

	std::vector <int> elements;

	for (int i = 0; i < n; i++) {
		int element = rand();
		if (s.find(element) == s.end()) {
			s.insert(element);
			t.add(element);
			elements.push_back(element);	
		}
	}

	assert(size(t) == s.size());
	assert(t.empty() == s.empty());

	random_shuffle(elements.begin(), elements.end());

	for (int i = 0; i < n; i++) {
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

int main() {
	test();
#ifdef KOBRA
	std::cout << "Time elapsed: " << (double) clock() / CLOCKS_PER_SEC << "\n";
#endif
	return 0;
}
