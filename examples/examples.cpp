#include <nested-shaper/nested_shaper.hpp>
#include <cmath>
#include <matplot/matplot.h>

#include <cmath>

using namespace std;
using namespace matplot;

void reference_plot(string _title, size_t _idx, vector<double> &ref_x, vector<double> &ref_y, vector<double> &x, vector<double> &y);

int main(int argc, char const *argv[])
{
    // Example 1
    // Step position trajectory - 1 depth
    {
        std::vector<double> x = linspace(0, 5.0);
        std::vector<double> y = transform(x, [](auto x) { return sin(x); });
        std::vector<double> x2 = linspace(0, 5.0, 200);
        std::vector<double> y2 = transform(x2, [](auto x) { return cos(x); });

        figure(0)->size(1920, 1080);
        reference_plot("Position", 0, x, y, x2, y2);
        reference_plot("Velocity", 1, x, y, x2, y2);
        reference_plot("Acceleration", 1, x, y, x2, y2);
        reference_plot("Jerk", 1, x, y, x2, y2);
        reference_plot("Snap", 1, x, y, x2, y2);
        reference_plot("Crackle", 1, x, y, x2, y2);
        // figure(0)->save()
    }
    
    // Example 2
    // Step position trajectory - 4 depth
    {

    }

    // Example 3 
    // Polynomial trajectory
    // Channel 4, Depth 3
    // p(t) =  A*t^3 +  B*t^2 + C*t + D
    // v(t) = 3A*t^2 + 2B*t   + C
    // a(t) = 6A*t   + 2B
    // j(t) = 6A
    {

    }
    
    show();
    return 0;
}

void reference_plot(string _title, size_t _idx, vector<double> &ref_x, vector<double> &ref_y, vector<double> &x, vector<double> &y)
{
    subplot(2, 3, _idx);
    title(_title);
    plot(ref_x, ref_y, "r");
    hold(on);
    plot(x, y, "b");
    grid(on);
}