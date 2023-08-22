#include <nested-shaper/nested_shaper.hpp>
#include <cmath>
#include <matplot/matplot.h>

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
        constexpr size_t kernel_size = size_t(2.0 / dt); // 2s duration kernel

        vector<double> t = linspace(0, duration, samples); // Duration 10s, 1ms interval
        vector<double> y = transform(t, [](auto x)
                                     { return 1.0; }); // Step input with 1.0
        vector<double> y_filtered;
        y_filtered.resize(t.size());

        nested_filterd<1, kernel_size> simple_1d_shaper; // Simple 1D moving average filter with 1s kernel
        simple_1d_shaper.fill(0.0);                      // fill with zero (zero position, zero velocity initial condition)

        for (size_t i = 0; i < t.size(); i++)
        {
            simple_1d_shaper.update(1.0);
            y_filtered[i] = simple_1d_shaper.peek(dt)[0];
        }

        reference_plot("1D position, 2s kernel", 0, t, y, y_filtered);
    }

    // Example 2
    // Step position trajectory - 2 depth
    {
        constexpr double dt = 0.01;
        constexpr double duration = 10.0;
        constexpr size_t samples = size_t(duration / dt);
        constexpr size_t kernel_size_1 = size_t(2.0 / dt); // 2s duration first  kernel
        constexpr size_t kernel_size_2 = size_t(1.0 / dt); // 1s duration second kernel

        vector<double> t = linspace(0, duration, samples); // Duration 10s, 1ms interval
        vector<double> y = transform(t, [](auto x)
                                     { return 1.0; }); // Step input with 1.0
        vector<double> y_filtered_pos;
        y_filtered_pos.resize(t.size());
        vector<double> y_filtered_vel;
        y_filtered_vel.resize(t.size());

        // nested_filterd<1, kernel_size_1, kernel_size_2> is possible, but velocity value is not available.
        nested_filterd<2, kernel_size_1, kernel_size_2> simple_1d_shaper; // Simple 2D moving average filter with 2s, 1s kernel
        simple_1d_shaper.fill(0.0);                                       // fill with zero (zero position, zero velocity initial condition)

        for (size_t i = 0; i < t.size(); i++)
        {
            simple_1d_shaper.update(1.0);
            const double *peek = simple_1d_shaper.peek(dt);

            y_filtered_pos[i] = peek[0];
            y_filtered_vel[i] = peek[1];
        }

        reference_plot("2D position, [2s,1s] kernel", 1, t, y, y_filtered_pos);
        reference_plot("2D velocity, [2s,1s] kernel", 2, t, y, y_filtered_vel);
    }

    // Example 3
    // Step position trajectory - 4 depth
    // Step position with 4 depth will create continuous jerk, second order acceleration
    {
        constexpr double dt = 0.01;
        constexpr double duration = 10.0;
        constexpr size_t samples = size_t(duration / dt);
        constexpr size_t kernel_size_1 = size_t(2.0 / dt); // 2s duration first  kernel
        constexpr size_t kernel_size_2 = size_t(1.0 / dt); // 1s duration second kernel
        constexpr size_t kernel_size_3 = size_t(2.0 / dt); // 2s duration third  kernel
        constexpr size_t kernel_size_4 = size_t(1.0 / dt); // 1s duration 4th    kernel

        vector<double> t = linspace(0, duration, samples); // Duration 10s, 1ms interval
        vector<double> y = transform(t, [](auto x)
                                     { return 1.0; }); // Step input with 1.0
        vector<double> y_filtered_pos;
        y_filtered_pos.resize(t.size());
        vector<double> y_filtered_vel;
        y_filtered_vel.resize(t.size());
        vector<double> y_filtered_acc;
        y_filtered_acc.resize(t.size());

        // uses nested_filterd<4, kernel_size_1, kernel_size_2, kernel_size_3, kernel_size_4> if you want jerk results.
        nested_filterd<3, kernel_size_1, kernel_size_2, kernel_size_3, kernel_size_4> simple_1d_shaper; // Simple 2D moving average filter with 2s, 1s kernel
        simple_1d_shaper.fill(0.0);                                                                     // fill with zero (zero position, zero velocity, zero acceleration ... initial condition)

        for (size_t i = 0; i < t.size(); i++)
        {
            simple_1d_shaper.update(1.0);
            const double *peek = simple_1d_shaper.peek(dt);

            y_filtered_pos[i] = peek[0];
            y_filtered_vel[i] = peek[1];
            y_filtered_acc[i] = peek[2];
        }

        reference_plot("4D position, [2s,1s,2s,1s] kernel", 3, t, y, y_filtered_pos);
        reference_plot("4D velocity, [2s,1s,2s,1s] kernel", 4, t, y, y_filtered_vel);
        reference_plot("4D acceleration, [2s,1s,2s,1s] kernel", 5, t, y, y_filtered_acc);
    }

    // Second figure, for Example 4
    figure()->size(1920, 1080);

    // Example 4
    // Assume a third order polynomial trajectory during 5s
    // Continuous acc, jerk, snap, crackle can be created by nested_filter or nested_shaper
    // p(t) = 1.0 + 0.12*t^2 - 0.016*t^3 (t < 5.0, p = 2 if t >= 5.0)
    // v(t) = 0.24*t - 0.048*t^2         (t < 5.0, v = 0 if t >= 5.0)
    // a(t) = 0.24 - 0.096*t             (t < 5.0, a = 0 if t >= 5.0)
    // j(t) = - 0.096                    (t < 5.0, j = 0 if t >= 5.0)
    {
        constexpr double dt = 0.01;
        constexpr double duration = 10.0;
        constexpr size_t samples = size_t(duration / dt);
        constexpr size_t kernel_size_1 = size_t(2.0 / dt); // 2.0s duration first  kernel
        constexpr size_t kernel_size_2 = size_t(1.0 / dt); // 1.0s duration second kernel
        constexpr size_t kernel_size_3 = size_t(0.5 / dt); // 0.5s duration third  kernel

        vector<double> t = linspace(0, duration, samples); // Duration 10s, 1ms interval
        vector<double> y_pos = transform(t, [](auto t)
                                         { return t > 5.0 ? 2.0 : 1.0 + 0.12 * t * t - 0.016 * t * t * t; });
        vector<double> y_vel = transform(t, [](auto t)
                                         { return t > 5.0 ? 0.0 : 0.24 * t - 0.048 * t * t; });
        vector<double> y_acc = transform(t, [](auto t)
                                         { return t > 5.0 ? 0.0 : 0.24 - 0.096 * t; });
        vector<double> y_jrk = transform(t, [](auto t)
                                         { return t > 5.0 ? 0.0 : -0.096; });

        vector<double> y_filtered_pos;
        y_filtered_pos.resize(t.size());
        vector<double> y_filtered_vel;
        y_filtered_vel.resize(t.size());
        vector<double> y_filtered_acc;
        y_filtered_acc.resize(t.size());
        vector<double> y_filtered_jerk;
        y_filtered_jerk.resize(t.size());
        vector<double> y_filtered_snap;
        y_filtered_snap.resize(t.size());
        vector<double> y_filtered_crackle;
        y_filtered_crackle.resize(t.size());

        // Basically same with, nested_filter<3, kernel_size_1, kernel_size_2, kernel_size_3>, however has flexible features for non-position(or velocity ...) controllers.
        // 4 channel means requires pos, vel, acc, jerk for input, 3 depth enables jerk, snap, crackle outputs (we can get pos to crackle output).
        ns::nested_shaperd<4, 3, kernel_size_1, kernel_size_2, kernel_size_3> _ns_4c_3d;
        const double input_before_start[] = {1.0, 0.0, 0.0, 0.0}; // nested filter, nested shaper has limitations of input. input vel, acc, jerk ... must be zero.
        _ns_4c_3d.fill(input_before_start);

        for (size_t i = 0; i < t.size(); i++)
        {
            const double input[] = {y_pos[i], y_vel[i], y_acc[i], y_jrk[i]};
            _ns_4c_3d.update(input);

            // peek results size of  Channel + Depth - 1 array pointer. (in this example, 6)
            const double *peek = _ns_4c_3d.peek(dt);
            y_filtered_pos[i] = peek[0];
            y_filtered_vel[i] = peek[1];
            y_filtered_acc[i] = peek[2];
            y_filtered_jerk[i] = peek[3];
            y_filtered_snap[i] = peek[4];
            y_filtered_crackle[i] = peek[5];
        }

        vector<double> y_dummy = transform(t, [](auto t) { return 0.0f; });
        reference_plot("nested shaper position, [2s,1s,0.5s] kernel", 0, t, y_pos, y_filtered_pos);
        reference_plot("nested shaper velocity, [2s,1s,0.5s] kernel", 1, t, y_vel, y_filtered_vel);
        reference_plot("nested shaper acceleration, [2s,1s,0.5s] kernel", 2, t, y_acc, y_filtered_acc);
        reference_plot("nested shaper jerk, [2s,1s,0.5s] kernel", 3, t, y_jrk, y_filtered_jerk);
        reference_plot("nested shaper snap, [2s,1s,0.5s] kernel", 4, t, y_dummy, y_filtered_snap);
        reference_plot("nested shaper crackle, [2s,1s,0.5s] kernel", 5, t, y_dummy, y_filtered_crackle);
    }

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