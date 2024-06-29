#include <matplot/matplot.h>
#include <nested-shaper/NestedShaper.hpp>
#include <ruckig/ruckig.hpp>
#include <vector>

using namespace ns;
using namespace ruckig;
using namespace matplot;

int main(int argc, char const* argv[]) {
    // Create Reference Trajectory (by ruckig)
    InputParameter<1> input;
    input.current_position = {0.0};
    input.current_velocity = {0.0};
    input.current_acceleration = {0.0};

    input.target_position = {5.0};
    input.target_velocity = {0.0};
    input.target_acceleration = {0.0};

    input.max_velocity = {4.0};
    input.max_acceleration = {10.0};
    input.max_jerk = {40.0};

    Ruckig<1> otg;
    Trajectory<1> trajectory;
    otg.calculate(input, trajectory);

    std::cout << "Trajectory duration: " << trajectory.get_duration() << " [s]." << std::endl;
    std::array<double, 1> position, velocity, acceleration, jerk;
    size_t section;

    const double dt = 0.01;
    std::vector<double> p_reference;
    std::vector<double> v_reference;
    std::vector<double> a_reference;
    std::vector<double> j_reference;
    for(double t = 0.0; t < trajectory.get_duration(); t += dt) {
        trajectory.at_time(t, position, velocity, acceleration, jerk, section);
        p_reference.push_back(position[0]);
        v_reference.push_back(velocity[0]);
        a_reference.push_back(acceleration[0]);
        j_reference.push_back(jerk[0]);
    }
    std::vector<double> t_reference = linspace(0, trajectory.get_duration(), p_reference.size());

    // Create Shaped Trajectory (by NestedShaper)
    std::vector<double> p_shaped;
    std::vector<double> v_shaped;
    std::vector<double> a_shaped;
    std::vector<double> j_shaped;

    /**
     * @brief NestedShaperEuclideanRecursive
     * 
     * Second template parameter: DerivativeOrder, Here is 5, because we need to calculate position, velocity, acceleration, and jerk. (snap also calculated, but not used in this example)
     * Third template parameter: Here is 20, So amplitude of first filter is 20 * dt = 0.2[s]
     * Fourth template parameter: Here is 10, So amplitude of second filter is 10 * dt = 0.1[s]
     */
    NestedShaperEuclideanRecursive<double, 5, 20, 10> nested_shaper{0.0f};
    std::vector<double> t_shaped = t_reference;
    for(size_t i = 0; i < t_reference.size(); i++) {
        ns::array<double, 5UL> derivatives = nested_shaper.convolute(p_reference[i], dt);
        p_shaped.push_back(derivatives[0]);
        v_shaped.push_back(derivatives[1]);
        a_shaped.push_back(derivatives[2]);
        j_shaped.push_back(derivatives[3]);
    }

    // after reference trajectory end, proceed to the end of shaped trajectory, during sum of filter size, 0.3[s] + 0.03[s]sample time delay(for derivative calculation)
    for(double t = 0.0; t < 0.33; t += dt) {
        ns::array<double, 5UL> derivatives = nested_shaper.convolute(5.0, dt);
        p_shaped.push_back(derivatives[0]);
        v_shaped.push_back(derivatives[1]);
        a_shaped.push_back(derivatives[2]);
        j_shaped.push_back(derivatives[3]);
        t_shaped.push_back(trajectory.get_duration() + t);
    }

    /**
     * @brief Plot Reference Trajectory
     */
    figure()->size(640, 480);
    hold(on);
    grid(on);

    plot(t_reference, p_reference)->line_width(2);
    plot(t_reference, v_reference)->line_width(2);
    plot(t_reference, a_reference)->line_width(2);
    plot(t_reference, j_reference)->line_width(2);

    xlabel("Time [s]");
    xticks(iota(0, 0.2, trajectory.get_duration() + 0.33 + 0.2));
    xrange(std::array<double, 2>{0.0, trajectory.get_duration() + 0.33 + 0.2});
    yrange(std::array<double, 2>{-2.0, 6.0});
    legend({"Position [m]", "Velocity [m/2]", "Acceleration [m/s^2]", "Jerk [m/s^3]"})->location(legend::general_alignment::topleft);
    title("Reference Trajectory");
    save("../docs/image/euclidean_reference_trajectory.png");

    /**
     * @brief Plot Shaped Trajectory
     */
    figure()->size(640, 480);
    hold(on);
    grid(on);

    plot(t_shaped, p_shaped)->line_width(2);
    plot(t_shaped, v_shaped)->line_width(2);
    plot(t_shaped, a_shaped)->line_width(2);
    plot(t_shaped, j_shaped)->line_width(2);

    xlabel("Time [s]");
    xrange(std::array<double, 2>{0.0, trajectory.get_duration() + 0.33 + 0.2});
    xticks(iota(0, 0.2, trajectory.get_duration() + 0.33 + 0.2));
    yrange(std::array<double, 2>{-2.0, 6.0});
    legend({"Position [m]", "Velocity [m/2]", "Acceleration [m/s^2]", "Jerk [m/s^3]"})->location(legend::general_alignment::topleft);
    title("Shaped Trajectory");
    save("../docs/image/euclidean_shaped_trajectory.png");

    // show();
    return 0;
}