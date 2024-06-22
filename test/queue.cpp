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
        Queue<int, 4> q;
        q.push(1);
        q.push(2);
        q.push(3);
        q.push(4);
        q.push(5);
        q.pop();
        q.pop();

        Queue<int, 4>::Iterator iter = q.begin();
        for(int i = 4; iter != q.end(); ++iter, ++i) {
            REQUIRE(*iter == i);
            *iter += 1;
            REQUIRE(*iter == i + 1);
        }

        Queue<int, 4>::ConstIterator citer = q.cbegin();
        for(int i = 4; citer != q.cend(); ++citer, ++i) {
            REQUIRE(*citer == i + 1);
        }
    }
}