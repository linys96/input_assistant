#ifndef __LYS_NONCOPYABLE_HPP__ 
#define __LYS_NONCOPYABLE_HPP__


namespace lys{

class Noncopyable
{
protected:
    Noncopyable() {}
    ~Noncopyable() {}

    Noncopyable(const Noncopyable&)=delete;
    Noncopyable &operator=(const Noncopyable&)=delete; 
};
}//end of namespace lys

#endif
