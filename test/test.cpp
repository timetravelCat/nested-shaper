#include <catch2/catch_test_macros.hpp>
#include <nested-shaper/sized_queue.hpp>
#include <nested-shaper/summator/naive_add.hpp>
#include <nested-shaper/summator/kahan_babushka_neumaier_add.hpp>
#include <nested-shaper/summator/kahan_babushka_klein_add.hpp>
#include <nested-shaper/average_filter.hpp>
#include <nested-shaper/internal/derivative.hpp>
#include <nested-shaper/internal/pack.hpp>
#include <nested-shaper/nested_filter.hpp>
#include <nested-shaper/nested_shaper.hpp>

#include <iostream>
#include <iomanip>

TEST_CASE("Testing sized_queue class", "[sized_queue]")
{
    SECTION("sized_queue constructors")
    {
        ns::sized_queue<int, 5> sized_queue_default;
        REQUIRE(sized_queue_default.size() == 0);
        REQUIRE(sized_queue_default.capacity() == 5);
        REQUIRE(sized_queue_default.max_size() == sized_queue_default.capacity());

        ns::sized_queue<int, 5> sized_queue_max_size{3};
        REQUIRE(sized_queue_max_size.size() == 0);
        REQUIRE(sized_queue_max_size.capacity() == 5);
        REQUIRE(sized_queue_max_size.max_size() == 3);

        ns::sized_queue<int, 5> sized_queue_copy{sized_queue_max_size};
        REQUIRE(sized_queue_copy.size() == sized_queue_max_size.size());
        REQUIRE(sized_queue_copy.capacity() == sized_queue_max_size.capacity());
        REQUIRE(sized_queue_copy.max_size() == sized_queue_max_size.max_size());
    }

    SECTION("sized_queue methods")
    {
        ns::sized_queue<int, 5> sized_queue{3};
        REQUIRE(sized_queue.size() == 0);
        REQUIRE(sized_queue.empty() == true);
        REQUIRE(sized_queue.full() == false);
        sized_queue.push(17);
        REQUIRE(sized_queue.size() == 1);
        REQUIRE(sized_queue.empty() == false);
        REQUIRE(sized_queue.full() == false);
        REQUIRE(sized_queue.front() == 17);
        REQUIRE(sized_queue.back() == 17);
        REQUIRE(sized_queue.front(1) == 17);
        REQUIRE(sized_queue.back(1) == 17);
        REQUIRE(sized_queue.front(2) == 17);
        REQUIRE(sized_queue.back(2) == 17);
        sized_queue.push(21);
        REQUIRE(sized_queue.size() == 2);
        REQUIRE(sized_queue.empty() == false);
        REQUIRE(sized_queue.full() == false);
        REQUIRE(sized_queue.front() == 17);
        REQUIRE(sized_queue.front(1) == 21);
        REQUIRE(sized_queue.front(2) == 21);
        REQUIRE(sized_queue.back() == 21);
        REQUIRE(sized_queue.back(1) == 17);
        REQUIRE(sized_queue.back(2) == 17);
        sized_queue.push(27);
        REQUIRE(sized_queue.size() == 3);
        REQUIRE(sized_queue.empty() == false);
        REQUIRE(sized_queue.full() == true);
        REQUIRE(sized_queue.front() == 17);
        REQUIRE(sized_queue.front(1) == 21);
        REQUIRE(sized_queue.front(2) == 27);
        REQUIRE(sized_queue.front(3) == 27);
        REQUIRE(sized_queue.back() == 27);
        REQUIRE(sized_queue.back(1) == 21);
        REQUIRE(sized_queue.back(2) == 17);
        REQUIRE(sized_queue.back(3) == 17);
        sized_queue.push(31);
        REQUIRE(sized_queue.size() == 3);
        REQUIRE(sized_queue.empty() == false);
        REQUIRE(sized_queue.full() == true);
        REQUIRE(sized_queue.front() == 21);
        REQUIRE(sized_queue.front(1) == 27);
        REQUIRE(sized_queue.front(2) == 31);
        REQUIRE(sized_queue.front(3) == 31);
        REQUIRE(sized_queue.back() == 31);
        REQUIRE(sized_queue.back(1) == 27);
        REQUIRE(sized_queue.back(2) == 21);
        REQUIRE(sized_queue.back(3) == 21);
        sized_queue.push(34);
        REQUIRE(sized_queue.size() == 3);
        REQUIRE(sized_queue.empty() == false);
        REQUIRE(sized_queue.full() == true);
        REQUIRE(sized_queue.front() == 27);
        REQUIRE(sized_queue.front(1) == 31);
        REQUIRE(sized_queue.front(2) == 34);
        REQUIRE(sized_queue.front(3) == 34);
        REQUIRE(sized_queue.back() == 34);
        REQUIRE(sized_queue.back(1) == 31);
        REQUIRE(sized_queue.back(2) == 27);
        REQUIRE(sized_queue.back(3) == 27);
        sized_queue.pop();
        REQUIRE(sized_queue.size() == 2);
        REQUIRE(sized_queue.empty() == false);
        REQUIRE(sized_queue.full() == false);
        REQUIRE(sized_queue.front() == 31);
        REQUIRE(sized_queue.front(1) == 34);
        REQUIRE(sized_queue.front(2) == 34);
        REQUIRE(sized_queue.front(3) == 34);
        REQUIRE(sized_queue.back() == 34);
        REQUIRE(sized_queue.back(1) == 31);
        REQUIRE(sized_queue.back(2) == 31);
        REQUIRE(sized_queue.back(3) == 31);
        sized_queue.pop();
        REQUIRE(sized_queue.size() == 1);
        REQUIRE(sized_queue.empty() == false);
        REQUIRE(sized_queue.full() == false);
        REQUIRE(sized_queue.front() == 34);
        REQUIRE(sized_queue.front(1) == 34);
        REQUIRE(sized_queue.back() == 34);
        REQUIRE(sized_queue.back(1) == 34);
        sized_queue.pop();
        REQUIRE(sized_queue.size() == 0);
        REQUIRE(sized_queue.empty() == true);
        REQUIRE(sized_queue.full() == false);
        sized_queue.fill(5);
        REQUIRE(sized_queue.size() == 3);
        REQUIRE(sized_queue.empty() == false);
        REQUIRE(sized_queue.full() == true);
        REQUIRE(sized_queue.front() == 5);
        REQUIRE(sized_queue.front(1) == 5);
        REQUIRE(sized_queue.front(2) == 5);
        REQUIRE(sized_queue.front(3) == 5);
        REQUIRE(sized_queue.back() == 5);
        REQUIRE(sized_queue.back(1) == 5);
        REQUIRE(sized_queue.back(2) == 5);
        REQUIRE(sized_queue.back(3) == 5);
        sized_queue.reset();
        REQUIRE(sized_queue.size() == 0);
        REQUIRE(sized_queue.empty() == true);
        REQUIRE(sized_queue.full() == false);
        sized_queue.resize(4);
        sized_queue.fill(5);
        REQUIRE(sized_queue.size() == 4);
        REQUIRE(sized_queue.empty() == false);
        REQUIRE(sized_queue.full() == true);
    }

    SECTION("summator")
    {
        ns::summator<int, ns::SummatorType::NAIVE> summator_naive;
        REQUIRE(summator_naive.get() == 0);
        summator_naive += 2;
        REQUIRE(summator_naive.get() == 2);
        summator_naive.reset();
        REQUIRE(summator_naive.get() == 0);
        summator_naive -= 2;
        REQUIRE(summator_naive.get() == -2);

        ns::summator<float, ns::SummatorType::KBN> summator;
        REQUIRE(summator.get() == 0.0f);
        const float a = 0.1f;
        float sum = 0.0f;

        for (int i = 0; i < 10; i++)
        {
            sum += a;
            summator += a;
        }

        REQUIRE(summator.get() == 1.0f);
        REQUIRE(summator.get() != sum);

        for (int i = 0; i < 10; i++)
        {
            sum -= a;
            summator -= a;
        }

        REQUIRE(summator.get() == 0.0f);
        REQUIRE(summator.get() != sum);

        ns::summator<double, ns::SummatorType::KBN> summator_double_kbn;
        ns::summator<double, ns::SummatorType::NAIVE> summator_double_naive;

        summator_double_kbn += 1.0;
        summator_double_kbn += pow(10.0, 100.0);
        summator_double_kbn += 1.0;
        summator_double_kbn -= pow(10.0, 100.0);

        summator_double_naive += 1.0;
        summator_double_naive += pow(10.0, 100.0);
        summator_double_naive += 1.0;
        summator_double_naive -= pow(10.0, 100.0);

        REQUIRE(summator_double_kbn.get() == 2.0);
        REQUIRE(summator_double_naive.get() == 0.0);

        ns::summator<double, ns::SummatorType::KBN> summator_double_kbn2;
        summator_double_kbn2 = summator_double_kbn;
        REQUIRE(summator_double_kbn.get() == summator_double_kbn2.get());

        summator_double_kbn2.set(5.0);
        REQUIRE(summator_double_kbn2.get() == 5.0);
    }

    SECTION("average_filter")
    {
        ns::average_filter<float, 3> average_filter;
        average_filter.push(1.0f);
        REQUIRE(average_filter.get() == 1.0f);
        average_filter.push(2.0f);
        REQUIRE(average_filter.get() == 1.5f);
        average_filter.push(3.0f);
        REQUIRE(average_filter.get() == 2.0f);
        average_filter.push(4.0f);
        REQUIRE(average_filter.get() == 3.0f);
        average_filter.push(-7.0f);
        REQUIRE(average_filter.get() == 0.0f);

        ns::average_filter<float, 3, false> average_filter_non_recursive;
        average_filter_non_recursive.push(1.0f);
        REQUIRE(average_filter_non_recursive.get() == 1.0f);
        average_filter_non_recursive.push(2.0f);
        REQUIRE(average_filter_non_recursive.get() == 1.5f);
        average_filter_non_recursive.push(3.0f);
        REQUIRE(average_filter_non_recursive.get() == 2.0f);
        average_filter_non_recursive.push(4.0f);
        REQUIRE(average_filter_non_recursive.get() == 3.0f);
        average_filter_non_recursive.push(-7.0f);
        REQUIRE(average_filter_non_recursive.get() == 0.0f);
    }

    SECTION("derivative")
    {
        ns::average_filter<float, 3> average_filter;
        average_filter.push(1.0f);
        average_filter.push(2.0f);
        average_filter.push(4.0f);

        ns::__ns__internal::forward_derivative<0> _forward_zero_derivative;
        REQUIRE(_forward_zero_derivative(average_filter, 0.1f, 0) == 1.0f);
        REQUIRE(_forward_zero_derivative(average_filter, 0.1f, 1) == 2.0f);
        REQUIRE(_forward_zero_derivative(average_filter, 0.1f, 2) == 4.0f);

        ns::__ns__internal::forward_derivative<1> _forward_first_derivative;
        REQUIRE(_forward_first_derivative(average_filter, 0.1f, 0) == 10.0f);
        REQUIRE(_forward_first_derivative(average_filter, 0.1f, 1) == 20.0f);

        ns::__ns__internal::forward_derivative<2> _forward_second_derivative;
        REQUIRE(_forward_second_derivative(average_filter, 0.1f, 0) == 100.0f);

        ns::__ns__internal::backward_derivative<0> _backward_zero_derivative;
        REQUIRE(_backward_zero_derivative(average_filter, 0.1f, 0) == 4.0f);
        REQUIRE(_backward_zero_derivative(average_filter, 0.1f, 1) == 2.0f);
        REQUIRE(_backward_zero_derivative(average_filter, 0.1f, 2) == 1.0f);

        ns::__ns__internal::backward_derivative<1> _backward_first_derivative;
        REQUIRE(_backward_first_derivative(average_filter, 0.1f, 0) == 20.0f);
        REQUIRE(_backward_first_derivative(average_filter, 0.1f, 1) == 10.0f);

        ns::__ns__internal::backward_derivative<2> _backward_second_derivative;
        REQUIRE(_backward_second_derivative(average_filter, 0.1f, 0) == 100.0f);

        ns::__ns__internal::average_filter_pack<float, true, ns::SummatorType::NAIVE, 3, 5, 7> _pack;
        ns::__ns__internal::getter<float, 0>{}.get(_pack).push(1.0f);
        ns::__ns__internal::getter<float, 1>{}.get(_pack).push(2.0f);
        ns::__ns__internal::getter<float, 2>{}.get(_pack).push(3.0f);

        REQUIRE(_pack.v.front() == 1.0f);
        REQUIRE(_pack.p.v.front() == 2.0f);
        REQUIRE(_pack.p.p.v.front() == 3.0f);

        REQUIRE(_pack[0].front() == 1.0f);
        REQUIRE(_pack[1].front() == 2.0f);
        REQUIRE(_pack[2].front() == 3.0f);
    }

    SECTION("nested_filter")
    {
        ns::nested_filter<float, 4, true, ns::SummatorType::KBN, 3, 5> _nested_filter{2, 4};

        REQUIRE(_nested_filter.isReady() == false);
        _nested_filter.fill(1.0f);
        REQUIRE(_nested_filter.isReady() == true);
        _nested_filter.reset();
        REQUIRE(_nested_filter.isReady() == false);
        _nested_filter.fill(1.0f);
        _nested_filter.update(1.0f);
        _nested_filter.update(2.0f);
        _nested_filter.update(2.0f);
        _nested_filter.update(4.0f);
        const float *result = _nested_filter.peek(0.1f);
        REQUIRE(result[2] == 18.75f);
        REQUIRE(result[3] == 125.0f);
    }

    SECTION("nested_shaper")
    {
        ns::nested_shaper<2, float, 3, true, ns::SummatorType::KBN, 3, 5> _nested_shaper{2, 4};
        REQUIRE(_nested_shaper.isReady() == false);
        const float input[] = {1.0f, 0.0f};
        _nested_shaper.fill(input);
        REQUIRE(_nested_shaper.isReady() == true);
        _nested_shaper.reset();
        REQUIRE(_nested_shaper.isReady() == false);
        _nested_shaper.fill(input);
        _nested_shaper.update(input);
        const float *peek = _nested_shaper.peek(0.1f);
        REQUIRE(peek[0] == 1.0f);
        REQUIRE(peek[1] == 0.0f);
        REQUIRE(peek[2] == 0.0f);
        REQUIRE(peek[3] == 0.0f);
    }
}