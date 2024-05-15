#include <gtest/gtest.h>

#include <string>
#include <sstream>

#include "../src/headers/Runner.h"

TEST(correctness, example) {
    std::string input = R"(3
09:00 19:00
10
08:48 1 client1
09:41 1 client1
09:48 1 client2
09:52 3 client1
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
11:30 1 client4
11:35 2 client4 2
11:45 3 client4
12:33 4 client1
12:43 4 client2
15:52 4 client4
)";

    std::string output = R"(09:00
08:48 1 client1
08:48 13 NotOpenYet
09:41 1 client1
09:48 1 client2
09:52 3 client1
09:52 13 ICanWaitNoLonger!
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
11:30 1 client4
11:35 2 client4 2
11:35 13 PlaceIsBusy
11:45 3 client4
12:33 4 client1
12:33 12 client4 1
12:43 4 client2
15:52 4 client4
19:00 11 client3
19:00
1 70 05:58
2 30 02:18
3 90 08:01
)";

    std::stringstream ssInput(input);
    std::stringstream ssOutput;

    runner::run(ssInput, ssOutput);
    ASSERT_EQ(output, ssOutput.str());
}

TEST(correctness, one_minute_occupy) {
    std::string input = R"(3
09:00 19:00
10
09:41 1 client1
09:42 2 client1 1
09:43 4 client1
)";

    std::string output = R"(09:00
09:41 1 client1
09:42 2 client1 1
09:43 4 client1
19:00
1 10 00:01
)";

    std::stringstream ssInput(input);
    std::stringstream ssOutput;

    runner::run(ssInput, ssOutput);
    ASSERT_EQ(output, ssOutput.str());
}

TEST(correctness, close_next_day) {
    std::string input = R"(3
23:00 06:00
10
23:41 1 client1
23:42 2 client1 1
00:00 1 client2
04:59 2 client2 2
05:42 4 client1
)";

    std::string output = R"(23:00
23:41 1 client1
23:42 2 client1 1
00:00 1 client2
04:59 2 client2 2
05:42 4 client1
06:00 11 client2
06:00
1 60 06:00
2 20 01:01
)";

    std::stringstream ssInput(input);
    std::stringstream ssOutput;

    runner::run(ssInput, ssOutput);
    ASSERT_EQ(output, ssOutput.str());
}

TEST(correctness, alphabetic_order_in_close) {
    std::string input = R"(5
09:00 19:00
10
09:41 1 client1
09:42 1 aaaclient
09:43 1 _client
09:44 1 client2
10:00 2 client1 1
10:01 2 aaaclient 2
10:02 2 _client 3
10:03 2 client2 4
)";

    std::string output = R"(09:00
09:41 1 client1
09:42 1 aaaclient
09:43 1 _client
09:44 1 client2
10:00 2 client1 1
10:01 2 aaaclient 2
10:02 2 _client 3
10:03 2 client2 4
19:00 11 _client
19:00 11 aaaclient
19:00 11 client1
19:00 11 client2
19:00
1 90 09:00
2 90 08:59
3 90 08:58
4 90 08:57
)";

    std::stringstream ssInput(input);
    std::stringstream ssOutput;

    runner::run(ssInput, ssOutput);
    ASSERT_EQ(output, ssOutput.str());
}

TEST(exception, incorrect_data) {
    std::string input = R"(5
09:00 19:00
10
09:41 1 client1
09:42 1 aaaclient
09:43 1 _client
09:44 1 client2
10:00 2 client1 1
10:01 2 aaaclient 2
0:02 2 _client 3
10:03 2 client2 4
)";

    std::stringstream ssInput(input);
    std::stringstream ssOutput;

    runner::run(ssInput, ssOutput);
    ASSERT_EQ("0:02 2 _client 3", ssOutput.str());
}

TEST(exception, incorrect_line) {
    std::string input = R"(5
09:00 19:00
10
09:41 1 client1
09:42 1 aaaclient
09:43 1 _client 3
09:44 1 client2
10:00 2 client1 1
10:01 2 aaaclient 2
10:02 2 _client 3
10:03 2 client2 4
)";

    std::stringstream ssInput(input);
    std::stringstream ssOutput;

    runner::run(ssInput, ssOutput);
    ASSERT_EQ("09:43 1 _client 3", ssOutput.str());
}
