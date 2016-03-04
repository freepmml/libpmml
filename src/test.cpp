
#include <iostream>
#include <fstream>
#include <Isotonic.hpp>
#include "TimeMeter.hpp"

/*
 * prepare test data
 * cat test.txt  | awk '{print $6,$11}'
 *
 */

int main(void)
{
    const std::string fname="test.pmml";
    try {
        auto ln = mPMML::read_linear_norm(fname, "isotonic");

        std::ifstream test2("test2.txt");
        Util::TimeMeter start_time;
        size_t iters = 0;
        const size_t iter_step = 1000000;
        while (!test2.eof())
        {
            double x = 0;
            test2 >> x;
            if (!test2.good()) {
                break;
            }
            double good_res = 0;
            test2 >> good_res;
            double res = 0;

            for (size_t c = 0; c < iter_step; c++)
            {
                res = ln(x);
            }
            iters += iter_step;
            std::cerr << "x = " << x << ", res = " << res << ", expected = " << good_res << std::endl;
        }
        const double ela = start_time.get().to_double();
        std::cerr << "Ela " << ela << " for " << iters << " computations, " << ela/(double)iters*1000*1000 << " us per comp." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;;
        return -1;
    }

    return 0;
}

