#include <matplot/matplot.h>
#include <nested-shaper/NestedShaper.hpp>
#include <vector>

using namespace ns;
using namespace matplot;

int main(int argc, char const* argv[]) {
    // Create Reference step trajectory
    const double dt = 0.01;
    const double step_duration = 10.0;  // [s]
    const double step_amplitude = 10.0; // [m]

    constexpr size_t FIRST_FILTER_SIZE = 200U;  // 200 * dt = 2.0 [s]
    constexpr size_t SECOND_FILTER_SIZE = 200U; // 200 * dt = 2.0 [s]
    constexpr size_t THIRD_FILTER_SIZE = 100U;  // 100 * dt = 1.0 [s]
    constexpr size_t FOURTH_FILTER_SIZE = 50U;  // 50 * dt = 0.5 [s]
    constexpr size_t FIFTH_FILTER_SIZE = 50U;   // 50 * dt = 0.50 [s]

    const double filter_duration = FIRST_FILTER_SIZE * dt + SECOND_FILTER_SIZE * dt + THIRD_FILTER_SIZE * dt + FOURTH_FILTER_SIZE * dt + FIFTH_FILTER_SIZE * dt;

    /**
     * @brief NestedShaperEuclideanRecursive
     * 
     * Second template parameter: DerivativeOrder, Here is 5, because we need to calculate position, velocity, acceleration, jerk and snap.
     * Assumed trajectory before the step is 0.0, so the initial value is 0.0.
     */
    NestedShaperEuclideanRecursive<double, 5, FIRST_FILTER_SIZE, SECOND_FILTER_SIZE, THIRD_FILTER_SIZE, FOURTH_FILTER_SIZE, FIFTH_FILTER_SIZE> nested_shaper{0.0f};
    std::vector<double> t_reference;
    std::vector<double> p_reference;
    std::vector<double> p_shaped;
    std::vector<double> v_shaped;
    std::vector<double> a_shaped;
    std::vector<double> j_shaped;
    std::vector<double> s_shaped;

    // Assume that the step starts at t = 0.0, and ends at t = step_duration.
    // Check step_duration is greater than filter_duration (in order to see the effect of filters ends)
    for(double t = 0.0; t < step_duration; t += dt) {
        t_reference.push_back(t);
        p_reference.push_back(step_amplitude);
        ns::array<double, 5UL> derivatives = nested_shaper.convolute(step_amplitude, dt);
        p_shaped.push_back(derivatives[0]);
        v_shaped.push_back(derivatives[1]);
        a_shaped.push_back(derivatives[2]);
        j_shaped.push_back(derivatives[3]);
        s_shaped.push_back(derivatives[4]);
    }

    // Plot
    figure()->size(640, 480);
    hold(on);
    grid(on);

    plot(t_reference, p_reference, "k")->line_width(2);
    plot(t_reference, p_shaped)->line_width(2);
    plot(t_reference, v_shaped)->line_width(2);
    plot(t_reference, a_shaped)->line_width(2);
    plot(t_reference, j_shaped)->line_width(2);
    plot(t_reference, s_shaped)->line_width(2);

    // Plot Limits
    plot(t_reference, transform(t_reference, [](auto t) { return 5.0; }), "--k")->line_width(1);
    plot(t_reference, transform(t_reference, [](auto t) { return 2.5; }), "--k")->line_width(1);
    plot(t_reference, transform(t_reference, [](auto t) { return -5.0; }), "--k")->line_width(1);
    plot(t_reference, transform(t_reference, [](auto t) { return -2.5; }), "--k")->line_width(1);

    xlabel("Time [s]");
    xticks(iota(0, 0.5, step_duration));
    xrange(std::array<double, 2>{0.0, step_duration});
    yrange(std::array<double, 2>{-11.0, 11.0});
    legend({"Step [m]", "Position [m]", "Velocity [m/2]", "Acceleration [m/s^2]", "Jerk [m/s^3]", "Snap [m/s^4]"})->location(legend::general_alignment::topright);
    save("../docs/image/euclidean_step.png");

    return 0;
}
