// fastに時間計測機能を追加
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

struct Interval
{
    int start;
    int end;
};

struct Node
{
    int xmid;
    vector<Interval> intervals;
    Node *left;
    Node *right;
};

Node *buildIntervalTree(vector<Interval> &intervals, vector<int> &sorted_endpoints);
void printIntervalTree(Node *node, int depth = 0);
vector<Interval> findIntervalsContainingValue(Node *node, int value);
int computeDepth(Node *node, int depth = 0);

int main()
{
    // ランダムな区間を生成
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000000 - 10000); // 1から100の間でランダムな数値を生成
    uniform_int_distribution<> length_dist(1, 10000);
    const int num_intervals = 1000000; // 生成する区間の数
    vector<Interval> intervals(num_intervals);
    for (int i = 0; i < num_intervals; ++i)
    {
        int start = dis(gen);
        int end = start + length_dist(gen);
        intervals[i] = {start, end};
    }

    vector<int> endpoints;
    for (int i = 0; i < intervals.size(); ++i)
    {
        endpoints.push_back(intervals[i].start);
        endpoints.push_back(intervals[i].end);
    }

    sort(endpoints.begin(), endpoints.end());

    auto start = high_resolution_clock::now();
    Node *interval_tree = buildIntervalTree(intervals, endpoints);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Depth of interval tree: " << computeDepth(interval_tree) << "\n";
    cout << "Time taken to build interval tree: " << duration.count() << " microseconds\n";

    cout << "<Interval Tree>\n";
    // printIntervalTree(interval_tree);

    int value_to_find = 10000;
    cout << "\n\n<Stabbing Query>";
    cout << "\nIntervals containing " << value_to_find << ":\n";

    start = high_resolution_clock::now();
    vector<Interval> found_intervals = findIntervalsContainingValue(interval_tree, value_to_find);
    stop = high_resolution_clock::now();

    duration = duration_cast<microseconds>(stop - start);
    cout << "Number of intervals found: " << found_intervals.size() << endl;
    cout << "Time taken to execute stabbing query: " << duration.count() << " microseconds\n";

    for (const Interval &interval : found_intervals)
    {
        // cout << "[" << interval.start << ", " << interval.end << "]\n";
    }

    return 0;
}

Node *buildIntervalTree(vector<Interval> &intervals, vector<int> &sorted_endpoints)
{
    if (intervals.empty() || sorted_endpoints.empty())
        return nullptr;

    int xmid = sorted_endpoints[sorted_endpoints.size() / 2];
    Node *node = new Node();
    node->xmid = xmid;
    vector<Interval> left_intervals, right_intervals;
    vector<int> left_endpoints, right_endpoints;

    for (size_t i = 0; i < intervals.size(); ++i)
    {
        if (intervals[i].end < xmid)
        {
            left_intervals.push_back(intervals[i]);
            left_endpoints.push_back(intervals[i].start);
            left_endpoints.push_back(intervals[i].end);
        }
        else if (intervals[i].start > xmid)
        {
            right_intervals.push_back(intervals[i]);
            right_endpoints.push_back(intervals[i].start);
            right_endpoints.push_back(intervals[i].end);
        }
        else
        {
            node->intervals.push_back(intervals[i]);
        }
    }

    node->left = buildIntervalTree(left_intervals, left_endpoints);
    node->right = buildIntervalTree(right_intervals, right_endpoints);
    return node;
}

void printIntervalTree(Node *node, int depth)
{
    if (node == nullptr)
        return;

    string indent(depth * 2, ' ');
    cout << indent << "Node (Depth " << depth << ", xmid: " << node->xmid << "): ";
    for (size_t i = 0; i < node->intervals.size(); ++i)
        cout << "[" << node->intervals[i].start << ", " << node->intervals[i].end << "] ";
    cout << '\n';

    cout << indent << "Left Subtree:\n";
    printIntervalTree(node->left, depth + 1);

    cout << indent << "Right Subtree:\n";
    printIntervalTree(node->right, depth + 1);
}

vector<Interval> findIntervalsContainingValue(Node *node, int value)
{
    vector<Interval> result;

    if (node == nullptr)
    {
        return result;
    }

    for (const Interval &interval : node->intervals)
    {
        if (interval.start <= value && interval.end >= value)
        {
            result.push_back(interval);
        }
    }

    if (value < node->xmid && node->left != nullptr)
    {
        vector<Interval> left_intervals = findIntervalsContainingValue(node->left, value);
        result.insert(result.end(), left_intervals.begin(), left_intervals.end());
    }
    else if (value >= node->xmid && node->right != nullptr)
    {
        vector<Interval> right_intervals = findIntervalsContainingValue(node->right, value);
        result.insert(result.end(), right_intervals.begin(), right_intervals.end());
    }

    return result;
}

int computeDepth(Node *node, int depth)
{
    if (node == nullptr)
    {
        return depth;
    }

    int left_depth = computeDepth(node->left, depth + 1);
    int right_depth = computeDepth(node->right, depth + 1);

    return max(left_depth, right_depth);
}