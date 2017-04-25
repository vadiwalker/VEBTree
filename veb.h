#ifndef VEB_H
#define VEB_H

unsigned long long const NO = -1;

template<unsigned int S>
class AbstractVEBTree {
public:
  virtual void add(unsigned long long x) = 0;
  virtual void remove(unsigned long long x) = 0;
  virtual unsigned long long next(unsigned long long x) const = 0;
  virtual unsigned long long prev(unsigned long long x) const = 0;
  virtual unsigned long long getMin() const = 0;
  virtual unsigned long long getMax() const = 0;
  virtual bool empty() const = 0;
};

// template <unsigned int S>
// class VEBTree <S> : AbstractVEBTree <S> {
// private:
// 	std::map < unsigned long long, VEBTree<SH> > buckets;

// 	VEBTree<SH> summary;
// 	unsigned long long min;
// 	unsigned long long max;

// 	inline unsigned long long low(unsigned long long x) const {
// 		return x & ((1ULL << SH) - 1);
// 	}

// 	inline unsigned long long high(unsigned long long x) const {
// 		return low(x >> SH);
// 	}
// public:

// }


#endif
