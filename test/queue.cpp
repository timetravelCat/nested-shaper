#include <nested-shaper/Queue.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

TEST_CASE("Queue", "[Queue]") {
    SECTION("Constructor") {
        Queue<int, 4> q;
        REQUIRE(q.extent == 4);
        REQUIRE(q.capacity() == 4);
        REQUIRE(q.size() == 0);
        REQUIRE(q.isEmpty());
        REQUIRE_FALSE(q.isFull());

        Queue<int, 8> q2(5);
        REQUIRE(q2.extent == 8);
        REQUIRE(q2.capacity() == 5);
        REQUIRE(q2.size() == 0);
        REQUIRE(q2.isEmpty());
        REQUIRE_FALSE(q2.isFull());
    }

    SECTION("Reset and Resize") {
        Queue<int, 4> q;
        q.push(1);

        q.reset();
        REQUIRE(q.size() == 0);
        REQUIRE(q.capacity() == 4);
        REQUIRE(q.isEmpty());
        REQUIRE_FALSE(q.isFull());

        q.resize(2);
        q.push(1);
        REQUIRE(q.size() == 1);
        REQUIRE(q.capacity() == 2);
        REQUIRE_FALSE(q.isEmpty());
        REQUIRE_FALSE(q.isFull());
    }

    SECTION("Push and Pop, Front and Back") {
        Queue<int, 4> q;
        q.push(1);
        REQUIRE(q.size() == 1);
        REQUIRE_FALSE(q.isFull());
        REQUIRE(q.front() == 1);
        REQUIRE(q.back() == 1);

        q.push(2);
        q.push(3);
        REQUIRE(q.size() == 3);
        REQUIRE_FALSE(q.isFull());
        REQUIRE(q.front() == 3);
        REQUIRE(q.front(1) == 2);
        REQUIRE(q.front(2) == 1);
        REQUIRE(q.back() == 1);
        REQUIRE(q.back(1) == 2);
        REQUIRE(q.back(2) == 3);

        q.push(4);
        REQUIRE(q.size() == 4);
        REQUIRE(q.isFull());
        REQUIRE(q.front() == 4);
        REQUIRE(q.front(1) == 3);
        REQUIRE(q.front(2) == 2);
        REQUIRE(q.front(3) == 1);
        REQUIRE(q.back() == 1);
        REQUIRE(q.back(1) == 2);
        REQUIRE(q.back(2) == 3);
        REQUIRE(q.back(3) == 4);

        q.push(5);
        REQUIRE(q.size() == 4);
        REQUIRE(q.isFull());
        REQUIRE(q.front() == 5);
        REQUIRE(q.front(1) == 4);
        REQUIRE(q.front(2) == 3);
        REQUIRE(q.front(3) == 2);
        REQUIRE(q.back() == 2);
        REQUIRE(q.back(1) == 3);
        REQUIRE(q.back(2) == 4);
        REQUIRE(q.back(3) == 5);

        q.pop();
        REQUIRE(q.size() == 3);
        REQUIRE_FALSE(q.isFull());
        REQUIRE(q.front() == 5);
        REQUIRE(q.back() == 3);

        q.pop();
        q.pop();
        q.pop();
        REQUIRE(q.size() == 0);
        REQUIRE(q.isEmpty());
        REQUIRE_FALSE(q.isFull());

        q.push(1);
        REQUIRE(q.size() == 1);
        REQUIRE_FALSE(q.isFull());
        REQUIRE(q.front() == 1);
        REQUIRE(q.back() == 1);

        q.push(2);
        q.push(3);
        REQUIRE(q.size() == 3);
        REQUIRE_FALSE(q.isFull());
        REQUIRE(q.front() == 3);
        REQUIRE(q.back() == 1);

        q.push(4);
        REQUIRE(q.size() == 4);
        REQUIRE(q.isFull());
        REQUIRE(q.front() == 4);
        REQUIRE(q.back() == 1);

        q.pop();
        REQUIRE(q.size() == 3);
        REQUIRE_FALSE(q.isFull());
        REQUIRE(q.front() == 4);
        REQUIRE(q.back() == 2);

        q.pop();
        q.pop();
        q.pop();
        REQUIRE(q.size() == 0);
        REQUIRE(q.isEmpty());
        REQUIRE_FALSE(q.isFull());
    }

    SECTION("Iterators") {
        Queue<int, 5> q;
        q.push(1);
        q.push(2);
        q.push(3);
        q.push(4);
        q.push(5);
        q.push(6);
        q.pop();
        q.pop();
        // Queue contains 4, 5, 6

        QueueIterator<int> forwardIterator = q.forwardIterator();
        REQUIRE(*(forwardIterator + 1U) == 5);
        REQUIRE(*(forwardIterator + 2U) == 6);

        for(size_t i = 0; i < forwardIterator.size; ++i, ++forwardIterator) {
            REQUIRE(*forwardIterator == int(i) + 4);
            *forwardIterator += 1;
            REQUIRE(*forwardIterator == int(i) + 5);
            *forwardIterator -= 1;
        }

        QueueIterator<int> backwardIterator = q.backwardIterator();
        REQUIRE(*(backwardIterator - 1U) == 5);
        REQUIRE(*(backwardIterator - 2U) == 4);

        for(size_t i = 0; i < backwardIterator.size; ++i, --backwardIterator) {
            REQUIRE(*backwardIterator == 6 - int(i));
            *backwardIterator += 1;
            REQUIRE(*backwardIterator == 7 - int(i));
            *backwardIterator -= 1;
        }

        QueueConstIterator<int> forwardConstIterator = q.forwardConstIterator();
        REQUIRE(*(forwardConstIterator + 1U) == 5);
        REQUIRE(*(forwardConstIterator + 2U) == 6);

        for(size_t i = 0; i < forwardConstIterator.size; ++i, ++forwardConstIterator) {
            REQUIRE(*forwardConstIterator == int(i) + 4);
        }

        QueueConstIterator<int> backwardConstIterator = q.backwardConstIterator();
        REQUIRE(*(backwardConstIterator - 1U) == 5);
        REQUIRE(*(backwardConstIterator - 2U) == 4);

        for(size_t i = 0; i < backwardConstIterator.size; ++i, --backwardConstIterator) {
            REQUIRE(*backwardConstIterator == 6 - int(i));
        }
    }

    SECTION("Fill") {
        Queue<int, 4> q;
        q.fill(1);
        REQUIRE(q.front() == 1);
        REQUIRE(q.back() == 1);

        q.fill(2);
        REQUIRE(q.front() == 2);
        REQUIRE(q.back() == 2);
    };
}