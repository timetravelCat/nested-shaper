#include <nested-shaper/nested_shaper.hpp>
#include <cmath>
#include <matplot/matplot.h>

// #include <cmath>

using namespace std;
using namespace matplot;
using namespace ns;

void reference_plot(string _title, size_t _idx, vector<double> &ref_x, vector<double> &ref_y, vector<double> &y);

int main(int argc, char const *argv[])
{
    // First figure, for Example 1, 2, 3
    figure()->size(1920, 1080);

    // Example 1
    // Step position trajectory - 1 depth
    {
        constexpr double dt = 0.01;
        constexpr double duration = 10.0;
        constexpr size_t samples = size_t(duration / dt);
        constexpr size_t kernal_size = size_t(2.0 / dt); // 2s duration kernal

        vector<double> t = linspace(0, duration, samples); // Duration 10s, 1ms interval
        vector<double> y = transform(t, [](auto x)
                                     { return 1.0; }); // Step input with 1.0
        vector<double> y_filtered;
        y_filtered.resize(t.size());

        nested_filterd<1, kernal_size> simple_1d_shaper; // Simple 1D moving average filter with 1s kernal
        simple_1d_shaper.fill(0.0);                      // fill with zero (zero position, zero velocity initial condition)

        for (size_t i = 0; i < t.size(); i++)
        {
            simple_1d_shaper.update(1.0);
            y_filtered[i] = simple_1d_shaper.peek(dt)[0];
        }

        reference_plot("1D position, 2s kernal", 0, t, y, y_filtered);
    }

    // Example 2
    // Step position trajectory - 2 depth
    {
        constexpr double dt = 0.01;
        constexpr double duration = 10.0;
        constexpr size_t samples = size_t(duration / dt);
        constexpr size_t kernal_size_1 = size_t(2.0 / dt); // 2s duration first  kernal
        constexpr size_t kernal_size_2 = size_t(1.0 / dt); // 1s duration second kernal

        vector<double> t = linspace(0, duration, samples); // Duration 10s, 1ms interval
        vector<double> y = transform(t, [](auto x)
                                     { return 1.0; }); // Step input with 1.0
        vector<double> y_filtered_pos;
        y_filtered_pos.resize(t.size());
        vector<double> y_filtered_vel;
        y_filtered_vel.resize(t.size());

        // nested_filterd<1, kernal_size_1, kernal_size_2> is possible, but velocity value is not available.
        nested_filterd<2, kernal_size_1, kernal_size_2> simple_1d_shaper; // Simple 2D moving average filter with 2s, 1s kernal
        simple_1d_shaper.fill(0.0);                                       // fill with zero (zero position, zero velocity initial condition)

        for (size_t i = 0; i < t.size(); i++)
        {
            simple_1d_shaper.update(1.0);
            const double *peek = simple_1d_shaper.peek(dt);

            y_filtered_pos[i] = peek[0];
            y_filtered_vel[i] = peek[1];
        }

        reference_plot("2D position, [2s,1s] kernal", 1, t, y, y_filtered_pos);
        reference_plot("2D velocity, [2s,1s] kernal", 2, t, y, y_filtered_vel);
    }

    // Example 3
    // Step position trajectory - 4 depth
    // Step position with 4 depth will create continuous jerk, second order acceleration
    {
        constexpr double dt = 0.01;
        constexpr double duration = 10.0;
        constexpr size_t samples = size_t(duration / dt);
        constexpr size_t kernal_size_1 = size_t(2.0 / dt); // 2s duration first  kernal
        constexpr size_t kernal_size_2 = size_t(1.0 / dt); // 1s duration second kernal
        constexpr size_t kernal_size_3 = size_t(2.0 / dt); // 1s duration second kernal
        constexpr size_t kernal_size_4 = size_t(1.0 / dt); // 1s duration second kernal

        vector<double> t = linspace(0, duration, samples); // Duration 10s, 1ms interval
        vector<double> y = transform(t, [](auto x)
                                     { return 1.0; }); // Step input with 1.0
        vector<double> y_filtered_pos;
        y_filtered_pos.resize(t.size());
        vector<double> y_filtered_vel;
        y_filtered_vel.resize(t.size());
        vector<double> y_filtered_acc;
        y_filtered_acc.resize(t.size());

        // uses nested_filterd<4, kernal_size_1, kernal_size_2, kernal_size_3, kernal_size_4> if you want jerk results.
        nested_filterd<3, kernal_size_1, kernal_size_2, kernal_size_3, kernal_size_4> simple_1d_shaper; // Simple 2D moving average filter with 2s, 1s kernal
        simple_1d_shaper.fill(0.0);                                                                     // fill with zero (zero position, zero velocity, zero acceleration ... initial condition)

        for (size_t i = 0; i < t.size(); i++)
        {
            simple_1d_shaper.update(1.0);
            const double *peek = simple_1d_shaper.peek(dt);

            y_filtered_pos[i] = peek[0];
            y_filtered_vel[i] = peek[1];
            y_filtered_acc[i] = peek[2];
        }

        reference_plot("4D position, [2s,1s,2s,1s] kernal", 3, t, y, y_filtered_pos);
        reference_plot("4D velocity, [2s,1s,2s,1s] kernal", 4, t, y, y_filtered_vel);
        reference_plot("4D acceleration, [2s,1s,2s,1s] kernal", 5, t, y, y_filtered_acc);
    }

    // First figure, for Example 1, 2, 4
    figure()->size(1920, 1080);
    // Example 4
    // Polynomial trajectory during 5s
    // p(t) = 0.1*t^3 + 0.2*t^2 + 0.4*t + 0.8
    // v(t) = 0.3*t^2 + 0.4*t   + 0.4
    // a(t) = 0.6*t   + 0.4;
    // j(t) = 0.6



    show();
    return 0;
}

void reference_plot(string _title, size_t _idx, vector<double> &ref_x, vector<double> &ref_y, vector<double> &y)
{
    subplot(2, 3, _idx);
    title(_title);
    plot(ref_x, ref_y, "r");
    hold(on);
    plot(ref_x, y, "b");
    grid(on);
}