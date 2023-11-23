#include <algorithm>
#include <vector>
#include <queue>
#include <limits>
#include <bits/stdc++.h>

using namespace std;

struct HeapItem
{
    int index;
    double dist;

    HeapItem(int index, double dist) : index(index), dist(dist) {}

    bool operator<(const HeapItem &o) const
    {
        return dist < o.dist;
    }
};

struct VpTreeNode
{
    int index;
    double threshold;
    VpTreeNode *left;
    VpTreeNode *right;

    VpTreeNode() : index(0), threshold(0.), left(nullptr), right(nullptr) {}

    ~VpTreeNode()
    {
        delete left;
        delete right;
    }
};

// Levenshtein distance function for strings
// Levenshtein distance function for strings
int levenshteinDistance(const string& s1, const string& s2) {
    int m = s1.length();
    int n = s2.length();

    // Create a matrix to store the distances
    vector<int> prev(n + 1), curr(n + 1, 0);
    // SC = O(n*2)
    // Initialize the matrix
    
    for (int i =0; i <=n;i++){
        prev[i]=i;
    }
    for (int i = 1; i <= m; ++i) {
        curr[0] = i;
        for (int j = 1; j <= n; ++j) {

           
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = 1 + min({prev[j], curr[j - 1], prev[j - 1]});
            }
        }
        prev = curr;
    }

    return prev[n];
}


// Inside the VpTree class
struct DistanceComparator
{
    const string &item;

    DistanceComparator(const string &item) : item(item) {}

    bool operator()(const string &a, const string &b)
    {
        int dist_a = levenshteinDistance(item, a);
        int dist_b = levenshteinDistance(item, b);

        return dist_a < dist_b;
    }
};

struct VpTree
{
    vector<string> items;
    VpTreeNode *root;

    VpTree() : root(nullptr) {}

    ~VpTree()
    {
        delete root;
    }

    void create(const vector<string> &items)
    {
        delete root;
        this->items = items;
        root = buildFromPoints(0, items.size());
    }

    void search(const string &target, int k, vector<string> &results, vector<double> &distances, int tau)
    {
        priority_queue<HeapItem> heap;

        // cout << "value of thresshold distance : " << tau << endl;
        search(root, target, k, heap, tau);

        results.clear();
        distances.clear();

        while (!heap.empty())
        {
            results.push_back(items[heap.top().index]);
            distances.push_back(heap.top().dist);
            heap.pop();
        }

        reverse(results.begin(), results.end());
        reverse(distances.begin(), distances.end());
    }

    // Inside the VpTree class
    struct DistanceComparator
    {
        const string &item;

        DistanceComparator(const string &item) : item(item) {}

        bool operator()(const string &a, const string &b)
        {
            double dist_a = levenshteinDistance(item, a);
            double dist_b = levenshteinDistance(item, b);

            return dist_a < dist_b;
        }
    };

    VpTreeNode *buildFromPoints(int lower, int upper)
    {
        if (upper == lower)
        {

            return nullptr;
        }

        VpTreeNode *node = new VpTreeNode();
        node->index = lower;

        if (upper - lower > 1)
        {
            int i = (int)((double)rand() / RAND_MAX * (upper - lower - 1)) + lower;
            swap(items[lower], items[i]);

            int median = (upper + lower) / 2;

            nth_element(
                items.begin() + lower + 1,
                items.begin() + median,
                items.begin() + upper,
                DistanceComparator(items[lower]));

            node->threshold = 10000; //  specific distance metric for strings
            
            node->index = lower;
            node->left = buildFromPoints(lower + 1, median);
            node->right = buildFromPoints(median, upper);
        }

        return node;
    }

    void search(VpTreeNode *node, const string &target, int k, priority_queue<HeapItem> &heap, int tau)
    {
        if (node == nullptr)
        {
            return;
        }

        double dist = levenshteinDistance(target, items[node->index]);

        if (dist < tau)
        {
            if (heap.size() == k)
                heap.pop();
            heap.push(HeapItem(node->index, dist));
            if (heap.size() == k)
                tau = heap.top().dist;
        }

        // cout << "Heap after processing node with index " << node->index << ":" << endl;
        priority_queue<HeapItem> temp = heap;
        while (!temp.empty())
        {
            // cout << "Index: " << temp.top().index << ", Distance: " << temp.top().dist << endl;
            temp.pop();
        }
        // cout << "------------------------" << endl;

        if (node->left == nullptr && node->right == nullptr)
        {
            return;
        }

        if (dist < node->threshold)
        {
            if (dist - tau <= node->threshold)
            {
                search(node->left, target, k, heap,tau);
            }

            if (dist + tau >= node->threshold)
            {
                search(node->right, target, k, heap,tau);
            }
        }
        else
        {
            if (dist + tau >= node->threshold)
            {
                search(node->right, target, k, heap,tau);
            }

            if (dist - tau <= node->threshold)
            {
                search(node->left, target, k, heap,tau);
            }
        }
    }
};

int main()
{
    // Create VpTree instance with string type and Levenshtein distance function
    VpTree tree;

    // Example strings
    string str1 = "example";
    string str2 = "exemplar";
    string str3 = "mplar";
    string str4 = "exemr";
    string str5 = "eplar";
    string target = "exampl";

    // Create a vector of strings
    vector<string> strings = {str1, str2, str3, str4, str5};

    // Create the VpTree with the vector of strings
    tree.create(strings);

    // Search for similar strings to the target string
    vector<string> results;
    vector<double> distances;

    // Output the results
    cout<<"value of radius : 2"<<endl;
    cout<<"value of threshold  : 1 "<<endl;
    tree.search(target, 1, results, distances, 6);

    cout<<endl;
    cout << "Target String: " << target << endl<<endl;
    cout << "Similar Strings:" << endl<<endl;

    for (size_t i = 0; i < results.size(); ++i)
    {
        cout << "String: " << results[i] << ", Levenshtein Distance: " << distances[i] << endl;
    }

    return 0;
}

// O(n * loh n)