// 区間木を使わずにstabbing queryを実行するプログラム
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

struct Interval
{
    int start;
    int end;
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <num_intervals> <query_value>" << std::endl;
        return 1;
    }

    int num_intervals = std::stoi(argv[1]);
    int query_value = std::stoi(argv[2]);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> start_dist(1, 1000000 - 10000);
    std::uniform_int_distribution<> length_dist(1, 10000);

    std::vector<Interval> intervals;
    for (int i = 0; i < num_intervals; ++i)
    {
        int start = start_dist(gen);
        int end = start + length_dist(gen);
        intervals.push_back({start, end});
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    int count = 0;
    for (const auto &interval : intervals)
    {
        if (interval.start <= query_value && query_value <= interval.end)
        {
            count++;
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "<Stabbing Query>\n";
    std::cout << "Intervals containing " << query_value << ": " << count << std::endl;
    std::cout << "Time taken to execute stabbing query: " << duration.count() << " microseconds" << std::endl;

    return 0;
}
