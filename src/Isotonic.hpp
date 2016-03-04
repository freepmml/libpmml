
#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <cassert>
#include <stdexcept>

namespace mPMML {

    struct Exception : std::runtime_error {
        Exception(const std::string& s) : std::runtime_error(s) {}
    };

    class LinearNorm {
        std::vector<double> norm;
        std::vector<double> orig;
    public:

        template<class LN>
        LinearNorm(const LN& vList) {
            assert(vList.size() >= 2);
            norm.reserve(vList.size());
            orig.reserve(vList.size());
            for (auto i = vList.begin(); i != vList.end(); i++) {
                norm.push_back(i->norm());
                orig.push_back(i->orig());
            }
        }

        double operator()(double x) const;
    };

    class Regression {
        double intercept;
        double coefficient;
        double exponent;
    public:
        Regression() : intercept(0), coefficient(1), exponent(1) {};
        template <class T, class V>
        Regression(const T& t, const V& v)
        {
            intercept = t.intercept();
            coefficient = v.coefficient();
            exponent = v.exponent();
        }
        double operator()(double x) const {
            return intercept + coefficient * std::pow(x, exponent);
        }
    };

    class Transformations {
        Regression regr;
        LinearNorm norm;
    public:
        template<class T, class V>
        Transformations(const T& t, const V& v)
        : norm(t), regr(v)
        {
            ;;
        }
        double operator()(double x) {
            return regr(norm(x));
        }
    };

    // throw Exception and xml_schema::exception
    Transformations read_linear_norm(const std::string& fname, const std::string& name);
}

