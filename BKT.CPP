// C++ program to demonstrate working of BK-Tree
#include "bits/stdc++.h"
using namespace std;

// maximum number of words in dict[]
#define MAXN 1000

// defines the tolerance value
#define TOL 2

// defines maximum length of a word
#define LEN 10

struct Node
{
	// stores the word of the current Node
	string word;

	// links to other Node in the tree
	int next[2*LEN];

	// constructors
	Node(string x):word(x)
	{
		// initializing next[i] = 0
		for(int i=0; i<2*LEN; i++)
			next[i] = 0;
	}
	Node() {}
};

// stores the root Node
Node RT;

// stores every Node of the tree
Node tree[MAXN];

// index for current Node of tree
int ptr;

int min(int a, int b, int c)
{
	return min(a, min(b, c));
}

// Edit Distance
// Dynamic-Approach O(m*n)
int editDistance(string& a,string& b)
{
	int m = a.length(), n = b.length();
	int dp[m+1][n+1];

	// filling base cases
	for (int i=0; i<=m; i++)
		dp[i][0] = i;
	for (int j=0; j<=n; j++)
		dp[0][j] = j;

	// populating matrix using dp-approach
	for (int i=1; i<=m; i++)
	{
		for (int j=1; j<=n; j++)
		{
			if (a[i-1] != b[j-1])
			{
				dp[i][j] = min( 1 + dp[i-1][j], // deletion
								1 + dp[i][j-1], // insertion
								1 + dp[i-1][j-1] // replacement
							);
			}
			else
				dp[i][j] = dp[i-1][j-1];
		}
	}
	return dp[m][n];
}

// adds curr Node to the tree
void add(Node& root,Node& curr)
{
	if (root.word == "" )
	{
		// if it is the first Node
		// then make it the root Node
		root = curr;
		return;
	}

	// get its editDistance from the Root Node
	int dist = editDistance(curr.word,root.word);

	if (tree[root.next[dist]].word == "")
	{
		/* if no Node exists at this dist from root
		* make it child of root Node*/

		// incrementing the pointer for curr Node
		ptr++;

		// adding curr Node to the tree
		tree[ptr] = curr;

		// curr as child of root Node
		root.next[dist] = ptr;
	}
	else
	{
		// recursively find the parent for curr Node
		add(tree[root.next[dist]],curr);
	}
}

vector <string> getSimilarWords(Node& root,string& s)
{
	vector < string > ret;
	if (root.word == "")
	return ret;

	// calculating editdistance of s from root
	int dist = editDistance(root.word,s);

	// if dist is less than tolerance value
	// add it to similar words
	if (dist <= TOL) ret.push_back(root.word);

	// iterate over the string having tolerance
	// in range (dist-TOL , dist+TOL)
	int start = dist - TOL;
	if (start < 0)
	start = 1;

	while (start <= dist + TOL)
	{
		vector <string> tmp =
			getSimilarWords(tree[root.next[start]],s);
		for (auto i : tmp)
			ret.push_back(i);
		start++;
	}
	return ret;
}

// driver program to run above functions
int main(int argc, char const *argv[])
{
	// dictionary words
	string dictionary[] = {"hell","help","shell","smell",
						"fell","felt","oops","pop","oouch","halt"
						};
	ptr = 0;
	int sz = sizeof(dictionary)/sizeof(string);

	// adding dict[] words on to tree
	for(int i=0; i<sz; i++)
	{
		Node tmp = Node(dictionary[i]);
		add(RT,tmp);
	}

	string w1 = "ops";
	string w2 = "helt";
	vector < string > match = getSimilarWords(RT,w1);
	cout << "similar words in dictionary for : " << w1 << ":\n";
	for (auto x : match)
		cout << x << endl;

	match = getSimilarWords(RT,w2);
	cout << "Correct words in dictionary for " << w2 << ":\n";
	for (auto x : match)
		cout << x << endl;

	return 0;
}
