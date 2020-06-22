//
// Created by the_E on 20/06/20.
//

#ifndef BRANCHES_VEC_H
#define BRANCHES_VEC_H

#include <vector>
#include "DataTypes.h"
#include <cmath>
#include <algorithm>
#include "../algebra_tools_/exception.h"

template<class t>
class vec
{
private:
protected:
    std::vector<t> v;
public:
    vec(std::initializer_list<t> lst);

    explicit vec(std::vector<t> lst);

    explicit vec(unsigned len, t val = 0);

    [[nodiscard]] unsigned size() const
    { return v.size(); }

    t *begin() const;

    t *end() const;


    t &operator[](unsigned i);

    t operator[](unsigned i) const;

    operator t();

    //the effective size is considered to be the min of the sizes
    vec<t> operator+(vec a) const;

    vec<t> operator-(vec a) const;

    t operator*(vec a) const;

};


template<class t>
vec<t>::vec(std::initializer_list<t> lst):v(lst)
{}

template<class t>
vec<t>::vec(std::vector<t> lst):v(lst)
{}

template<class t>
vec<t>::vec(unsigned int len, t val)
{
    v.reserve(len);
    for (unsigned i = 0; i < len; i++)
        v.push_back(val);
}


template<class t>
t &vec<t>::operator[](unsigned int i)
{
    if (i >= size())throw algebra_tools_::except("Index out of range");
    return v[i];
}

template<class t>
t vec<t>::operator[](unsigned int i) const
{
    if (i >= size())throw algebra_tools_::except("Index out of range");
    return v[i];
}

template<class t>
vec<t> vec<t>::operator+(vec a) const
{
    unsigned s = min(size(), a.size());
    std::vector<t> rv;
    rv.reserve(s);
    for (int i = 0; i < s; i++)
    {
        rv[i] = (*this)[i] + a[i];
    }
    return rv;
}

template<class t>
vec<t> vec<t>::operator-(vec a) const
{
    unsigned s = min(size(), a.size());
    std::vector<t> rv;
    rv.reserve(s);
    for (int i = 0; i < s; i++)
    {
        rv[i] = (*this)[i] - a[i];
    }
    return rv;
}

template<class t>
t vec<t>::operator*(vec a) const
{
    t ret = 0;
    unsigned s = min(size(), a.size());
    for (int i = 0; i < s; i++)
    {
        ret += (*this)[i] + a[i];
    }
    return ret;
}

template<class t>
t *vec<t>::begin() const
{
    return v.begin();
}

template<class t>
t *vec<t>::end() const
{
    return v.end();
}

template<class t>
vec<t>::operator t()
{
    if (size() != 1)throw algebra_tools_::except("Converting to component a vector longer than 1");
    return v[0];
}


template<class t, class d>
t operator*(const vec<t> &v, const d &k)
{
    vec<t> tmp = v;
    for (t &x:tmp)
    {
        x *= k;
    }
    return tmp;
}

template<class t, class d>
t operator*(const d &k, const vec<t> &v)
{
    return v * k;
}

template<class t>
t mod(const vec<t> &v)
{
    t tmp = 0;
    for (unsigned i = 0; i < v.size(); i++)
    {
        tmp += pow(abs(v[i]), 2);//abs for complex numbers
    }
    return sqrt(tmp);
}

#endif //BRANCHES_VEC_H
