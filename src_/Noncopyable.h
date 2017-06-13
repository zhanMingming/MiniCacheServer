#ifndef SPHCACHE_NONCOPYABLE_H
#define SPHCACHE_NONCOPYABLE_H

namespace zhanmm {

class Noncopyable
{
protected:
    Noncopyable(){}
private:
    Noncopyable(const Noncopyable &lhs);
    Noncopyable & operator=(const Noncopyable &rhs);
};

} // namespace zhanmm
#endif
