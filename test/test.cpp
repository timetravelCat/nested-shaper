#include <catch2/catch_test_macros.hpp>
#include <nested-shaper/sized_queue.hpp>

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
}