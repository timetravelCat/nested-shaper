#include <matplot/matplot.h>
#include <nested-shaper/NestedShaper.hpp>
#include <ruckig/ruckig.hpp>
#include <vector>

using namespace ns;
using namespace ruckig;
using namespace matplot;

int main(int argc, char const* argv[]) {
    // Create Reference 3D Trajectory (by ruckig)
    InputParameter<3> input;
    input.current_position = {0.0, 0.0, 0.0};
    input.current_velocity = {0.0, 0.0, 0.0};
    input.current_acceleration = {0.0, 0.0, 0.0};

    input.target_position = {5.0, 10.0, 8.0};
    input.target_velocity = {0.0, 0.0, 0.0};
    input.target_acceleration = {0.0, 0.0, 0.0};

    input.max_velocity = {4.0, 4.0, 4.0};
    input.max_acceleration = {10.0, 10.0, 20.0};
    input.max_jerk = {40.0, 40.0, 10.0};

    Ruckig<3> otg;
    Trajectory<3> trajectory;
    otg.calculate(input, trajectory);

    std::cout << "Trajectory duration: " << trajectory.get_duration() << " [s]." << std::endl;
    std::array<double, 3> position, velocity, acceleration, jerk;
    size_t section;

    const double dt = 0.01;
    std::vector<double> p_reference[3];
    std::vector<double> v_reference[3];
    std::vector<double> a_reference[3];
    std::vector<double> j_reference[3];
    for(double t = 0.0; t < trajectory.get_duration(); t += dt) {
        trajectory.at_time(t, position, velocity, acceleration, jerk, section);
        p_reference[0].push_back(position[0]);
        p_reference[1].push_back(position[1]);
        p_reference[2].push_back(position[2]);
        v_reference[0].push_back(velocity[0]);
        v_reference[1].push_back(velocity[1]);
        v_reference[2].push_back(velocity[2]);
        a_reference[0].push_back(acceleration[0]);
        a_reference[1].push_back(acceleration[1]);
        a_reference[2].push_back(acceleration[2]);
        j_reference[0].push_back(jerk[0]);
        j_reference[1].push_back(jerk[1]);
        j_reference[2].push_back(jerk[2]);
    }
    std::vector<double> t_reference = linspace(0, trajectory.get_duration(), p_reference[0].size());

    // Create Shaped Trajectory (by NestedShaper)
    std::vector<double> p_shaped[3];
    std::vector<double> v_shaped[3];
    std::vector<double> a_shaped[3];
    std::vector<double> j_shaped[3];
    std::vector<double> s_shaped[3];

    /**
     * @brief NestedShaperEuclideanRecursive
     * 
     * Second template parameter: Dimension, 3
     * Third template parameter: DerivativeOrder, Here is 5, because we need to calculate position, velocity, acceleration, and jerk. (snap also calculated, but not used in this example)
     * Forth template parameter: Here is 20, So amplitude of first filter is 20 * dt = 0.2[s]
     * Fifth template parameter: Here is 10, So amplitude of second filter is 10 * dt = 0.1[s]
     */
    NestedShaperEuclideanRecursiveArray<double, 3, 5, 20, 10> nested_shaper{ns::array<double, 3>{0.0, 0.0, 0.0}};
    std::vector<double> t_shaped = t_reference;
    for(size_t i = 0; i < t_reference.size(); i++) {
        auto derivatives = nested_shaper.convolute(ns::array<double, 3>{p_reference[0][i], p_reference[1][i], p_reference[2][i]}, dt);
        p_shaped[0].push_back(derivatives[0][0]);
        p_shaped[1].push_back(derivatives[1][0]);
        p_shaped[2].push_back(derivatives[2][0]);
        v_shaped[0].push_back(derivatives[0][1]);
        v_shaped[1].push_back(derivatives[1][1]);
        v_shaped[2].push_back(derivatives[2][1]);
        a_shaped[0].push_back(derivatives[0][2]);
        a_shaped[1].push_back(derivatives[1][2]);
        a_shaped[2].push_back(derivatives[2][2]);
        j_shaped[0].push_back(derivatives[0][3]);
        j_shaped[1].push_back(derivatives[1][3]);
        j_shaped[2].push_back(derivatives[2][3]);
        s_shaped[0].push_back(derivatives[0][4]);
        s_shaped[1].push_back(derivatives[1][4]);
        s_shaped[2].push_back(derivatives[2][4]);
    }

    // after reference trajectory end, proceed to the end of shaped trajectory, during sum of filter size, 0.3[s] + 0.03[s]sample time delay(for derivative calculation)
    for(double t = 0.0; t < 0.33; t += dt) {
        auto derivatives = nested_shaper.convolute(ns::array<double, 3>{5.0, 10.0, 8.0}, dt);
        p_shaped[0].push_back(derivatives[0][0]);
        p_shaped[1].push_back(derivatives[1][0]);
        p_shaped[2].push_back(derivatives[2][0]);
        v_shaped[0].push_back(derivatives[0][1]);
        v_shaped[1].push_back(derivatives[1][1]);
        v_shaped[2].push_back(derivatives[2][1]);
        a_shaped[0].push_back(derivatives[0][2]);
        a_shaped[1].push_back(derivatives[1][2]);
        a_shaped[2].push_back(derivatives[2][2]);
        j_shaped[0].push_back(derivatives[0][3]);
        j_shaped[1].push_back(derivatives[1][3]);
        j_shaped[2].push_back(derivatives[2][3]);
        s_shaped[0].push_back(derivatives[0][4]);
        s_shaped[1].push_back(derivatives[1][4]);
        s_shaped[2].push_back(derivatives[2][4]);

        t_shaped.push_back(trajectory.get_duration() + t);
    }

    /**
     * @brief Plot Reference Trajectory
     */
    figure()->size(640, 480);
    hold(on);
    grid(on);

    plot(t_reference, p_reference[0])->line_width(2);
    plot(t_shaped, p_shaped[0])->line_width(2);
    plot(t_reference, p_reference[1])->line_width(2);
    plot(t_shaped, p_shaped[1])->line_width(2);
    plot(t_reference, p_reference[2])->line_width(2);
    plot(t_shaped, p_shaped[2])->line_width(2);

    xlabel("Time [s]");
    xticks(iota(0, 0.2, trajectory.get_duration() + 0.33 + 0.2));
    xrange(std::array<double, 2>{0.0, trajectory.get_duration() + 0.33 + 0.2});

    legend({"Reference Position X [m]",
            "Shaped Position X [m]",
            "Reference Position Y [m]",
            "Shaped Position Y [m]",
            "Reference Position Z [m]",
            "Shaped Position Z [m]"})
      ->location(legend::general_alignment::topleft);
    title("Trajectory Position");
    save("../docs/image/euclidean_array_position.png");

    figure()->size(640, 480);
    hold(on);
    grid(on);

    plot(t_reference, v_reference[0])->line_width(2);
    plot(t_shaped, v_shaped[0])->line_width(2);
    plot(t_reference, v_reference[1])->line_width(2);
    plot(t_shaped, v_shaped[1])->line_width(2);
    plot(t_reference, v_reference[2])->line_width(2);
    plot(t_shaped, v_shaped[2])->line_width(2);

    xlabel("Time [s]");
    xticks(iota(0, 0.2, trajectory.get_duration() + 0.33 + 0.2));
    xrange(std::array<double, 2>{0.0, trajectory.get_duration() + 0.33 + 0.2});

    legend({"Reference Velocity X [m/s]",
            "Shaped Velocity X [m/s]",
            "Reference Velocity Y [m/s]",
            "Shaped Velocity Y [m/s]",
            "Reference Velocity Z [m/s]",
            "Shaped Velocity Z [m/s]"})
      ->location(legend::general_alignment::topleft);
    title("Trajectory Velocity");
    save("../docs/image/euclidean_array_velocity.png");

    figure()->size(640, 480);
    hold(on);
    grid(on);

    plot(t_reference, a_reference[0])->line_width(2);
    plot(t_shaped, a_shaped[0])->line_width(2);
    plot(t_reference, a_reference[1])->line_width(2);
    plot(t_shaped, a_shaped[1])->line_width(2);
    plot(t_reference, a_reference[2])->line_width(2);
    plot(t_shaped, a_shaped[2])->line_width(2);

    xlabel("Time [s]");
    xticks(iota(0, 0.2, trajectory.get_duration() + 0.33 + 0.2));
    xrange(std::array<double, 2>{0.0, trajectory.get_duration() + 0.33 + 0.2});

    legend({"Reference Acceleration X [m/s^2]",
            "Shaped Acceleration X [m/s^2]",
            "Reference Acceleration Y [m/s^2]",
            "Shaped Acceleration Y [m/s^2]",
            "Reference Acceleration Z [m/s^2]",
            "Shaped Acceleration Z [m/s^2]"})
      ->location(legend::general_alignment::topleft);
    title("Trajectory Acceleration");
    save("../docs/image/euclidean_array_acceleration.png");

    figure()->size(640, 480);
    hold(on);
    grid(on);

    plot(t_reference, j_reference[0])->line_width(2);
    plot(t_shaped, j_shaped[0])->line_width(2);
    plot(t_reference, j_reference[1])->line_width(2);
    plot(t_shaped, j_shaped[1])->line_width(2);
    plot(t_reference, j_reference[2])->line_width(2);
    plot(t_shaped, j_shaped[2])->line_width(2);

    xlabel("Time [s]");
    xticks(iota(0, 0.2, trajectory.get_duration() + 0.33 + 0.2));
    xrange(std::array<double, 2>{0.0, trajectory.get_duration() + 0.33 + 0.2});

    legend({"Reference Jerk X [m/s^3]",
            "Shaped Jerk X [m/s^3]",
            "Reference Jerk Y [m/s^3]",
            "Shaped Jerk Y [m/s^3]",
            "Reference Jerk Z [m/s^3]",
            "Shaped Jerk Z [m/s^3]"})
      ->location(legend::general_alignment::topleft);
    title("Trajectory Jerk");
    save("../docs/image/euclidean_array_jerk.png");

    figure()->size(640, 480);
    hold(on);
    grid(on);

    plot(t_shaped, s_shaped[0])->line_width(2);
    plot(t_shaped, s_shaped[1])->line_width(2);
    plot(t_shaped, s_shaped[2])->line_width(2);

    xlabel("Time [s]");
    xticks(iota(0, 0.2, trajectory.get_duration() + 0.33 + 0.2));
    xrange(std::array<double, 2>{0.0, trajectory.get_duration() + 0.33 + 0.2});

    legend({"Shaped Snap X [m/s^4]", "Shaped Snap Y [m/s^4]", "Shaped Snap Z [m/s^4]"})
      ->location(legend::general_alignment::topleft);
    title("Trajectory Snap");
    save("../docs/image/euclidean_array_snap.png");

    return 0;
}
