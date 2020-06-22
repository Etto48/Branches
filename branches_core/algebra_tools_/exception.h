//
// Created by the_E on 20/06/22.
//

#ifndef BRANCHES_EXCEPTION_H
#define BRANCHES_EXCEPTION_H

#include <exception>
#include <string>

namespace algebra_tools_
{
    class except : public std::exception
    {
    protected:
        std::string s;
    public:
        explicit except(std::string ss) : s(std::move(ss))
        {}

        ~except() noexcept override = default;

        [[nodiscard]] const char *what() const noexcept override
        { return s.c_str(); }
    };
}
#endif //BRANCHES_EXCEPTION_H
