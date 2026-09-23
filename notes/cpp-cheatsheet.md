# C++ Cheat Sheet for DSA

Quick reference for solving DSA problems in C++. Focused on the STL, memory model, and gotchas that trip up beginners.

## Table of Contents
- [Fast I/O](#fast-io)
- [Memory Model Basics](#memory-model-basics)
- [Pass by Value vs Reference vs Pointer](#pass-by-value-vs-reference-vs-pointer)
- [Vectors](#vectors)
- [Pointers & Dynamic Memory](#pointers--dynamic-memory)
- [Smart Pointers](#smart-pointers)
- [Other STL Containers](#other-stl-containers)
- [Iterators](#iterators)
- [Useful `<algorithm>` Functions](#useful-algorithm-functions)
- [Strings](#strings)
- [Common Gotchas](#common-gotchas)
- [Complexity Cheat Sheet](#complexity-cheat-sheet)
- [Handy Snippets](#handy-snippets)

---

## Fast I/O

Always add this at the top of `main()` for competitive/DSA problems — default `cin`/`cout` is slow.

```cpp
ios_base::sync_with_stdio(false);
cin.tie(NULL);
```

Don't mix `scanf/printf` with `cin/cout` after disabling sync — pick one.

---

## Memory Model Basics

C++ programs use two main memory regions you'll actually interact with:

| Region | What lives there | Lifetime | Speed |
|---|---|---|---|
| **Stack** | Local variables, function params, fixed-size arrays (`int arr[10]`) | Freed automatically when scope ends | Fast |
| **Heap** | Anything from `new`, `malloc`, or STL containers' internal buffers (`vector`, `string`, `map`...) | You control it (or a container/smart pointer does) | Slower, more flexible |

Key rule: **the stack is small** (~1–8 MB typically). A local array like `int arr[10000000];` will crash (stack overflow). Use `vector<int> arr(10000000);` instead — it allocates on the heap.

```cpp
int arr[10];              // stack — fine, small, fixed size
int big[10000000];        // BAD — stack overflow risk
vector<int> big(10000000); // GOOD — heap-allocated
```

---

## Pass by Value vs Reference vs Pointer

This matters a lot for both correctness and performance in DSA code.

```cpp
void byValue(vector<int> v);        // COPIES the whole vector — slow for large inputs
void byRef(vector<int>& v);         // no copy, CAN modify caller's vector
void byConstRef(const vector<int>& v); // no copy, CANNOT modify — use this for read-only params
void byPointer(vector<int>* v);     // no copy, must dereference (*v), can be nullptr
```

**Default habit:** pass large objects (`vector`, `string`, custom structs) by `const&` unless you need to modify them, then pass by `&`.

```cpp
int sumVec(const vector<int>& v) {
    int total = 0;
    for (int x : v) total += x;   // range-based for, read-only copy of each element
    return total;
}

void doubleAll(vector<int>& v) {
    for (int& x : v) x *= 2;      // note the & — modifies the actual elements
}
```

---

## Vectors

`vector<T>` is a dynamic, resizable array — the workhorse container for DSA.

### Declaration & initialization

```cpp
vector<int> v;                     // empty
vector<int> v(5);                  // size 5, all zero-initialized
vector<int> v(5, 10);              // size 5, all initialized to 10
vector<int> v = {1, 2, 3, 4};      // initializer list
vector<vector<int>> grid(3, vector<int>(4, 0)); // 3x4 2D grid of zeros
```

### Core operations

```cpp
v.push_back(x);      // add to end, O(1) amortized
v.pop_back();        // remove from end, O(1)
v.size();            // number of elements (unsigned! see gotchas)
v.empty();           // true if size == 0
v.clear();           // remove all elements
v[i];                // access, NO bounds check (undefined behavior if out of range)
v.at(i);             // access WITH bounds check (throws std::out_of_range)
v.front(); v.back(); // first / last element
v.insert(v.begin() + i, x); // insert at index i, O(n)
v.erase(v.begin() + i);     // remove at index i, O(n)
v.resize(n);          // grow/shrink to size n
v.reserve(n);         // pre-allocate capacity (avoids repeated reallocation)
```

### Why `reserve` matters

Every time a `vector` outgrows its capacity, it reallocates a bigger buffer and copies everything over. If you know roughly how many elements you'll push, `reserve()` avoids repeated reallocations:

```cpp
vector<int> v;
v.reserve(100000);   // one allocation up front instead of ~17 reallocations
for (int i = 0; i < 100000; i++) v.push_back(i);
```

### Iterating

```cpp
for (int i = 0; i < v.size(); i++) { ... }     // index-based
for (int x : v) { ... }                        // read-only copy of each element
for (int& x : v) { ... }                       // reference — can modify in place
for (auto it = v.begin(); it != v.end(); it++) { ... } // iterator-based
```

### Passing vectors to functions (recap)

```cpp
int findMax(const vector<int>& v);   // read-only, no copy — preferred
void sortInPlace(vector<int>& v);    // modifies caller's vector
```

### 2D vectors (grids/matrices)

```cpp
int rows = 3, cols = 4;
vector<vector<int>> grid(rows, vector<int>(cols, 0));
grid[1][2] = 5;
for (auto& row : grid)
    for (int val : row)
        cout << val << " ";
```

---

## Pointers & Dynamic Memory

You'll mostly use `vector`/`string`/smart pointers instead of raw `new`/`delete` in modern DSA code, but you need to recognize the syntax (linked lists, trees, graphs use raw pointers constantly).

```cpp
int x = 5;
int* p = &x;      // p holds the address of x
cout << *p;       // dereference — prints 5
*p = 10;          // modifies x through the pointer, x is now 10

int* arr = new int[10];   // heap-allocate array of 10 ints
delete[] arr;              // MUST free with delete[] (matches new[])

int* single = new int(5); // heap-allocate single int
delete single;             // MUST free with delete (matches new)
```

**Rule: every `new` needs exactly one matching `delete`.** Forgetting it = memory leak. Using `delete` on a `new[]` (or vice versa) = undefined behavior.

### Linked list node (classic DSA pattern)

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* head = new ListNode(1);
head->next = new ListNode(2);
// access value: head->val   (shorthand for (*head).val)
```

### Binary tree node

```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

Always initialize pointers to `nullptr` — an uninitialized pointer is garbage and dereferencing it crashes or corrupts memory unpredictably.

---

## Smart Pointers

If you're writing your own heap-allocated structures outside of a fixed template (e.g., LeetCode gives you raw `ListNode*`), prefer smart pointers to avoid manual `delete`:

```cpp
#include <memory>

unique_ptr<int> p = make_unique<int>(5);   // sole owner, auto-freed when out of scope
shared_ptr<int> sp = make_shared<int>(5);  // reference-counted, freed when last owner goes away
```

For most LeetCode/HackerRank-style DSA problems you won't need these — the judge's node structs use raw pointers — but know they exist for real-world C++.

---

## Other STL Containers

| Container | Use case | Key traits |
|---|---|---|
| `pair<A,B>` | Store 2 related values | `.first`, `.second` |
| `array<T,N>` | Fixed-size stack array | Like a safer C array, knows its size |
| `string` | Text/char sequences | Mutable, like a `vector<char>` |
| `stack<T>` | LIFO | `push`, `pop`, `top` |
| `queue<T>` | FIFO | `push`, `pop`, `front`, `back` |
| `deque<T>` | Double-ended queue | O(1) push/pop both ends |
| `priority_queue<T>` | Max-heap by default | `top()` = largest; use `greater<T>` for min-heap |
| `set<T>` | Sorted unique elements | O(log n) insert/find, no duplicates |
| `unordered_set<T>` | Hash-set | O(1) avg insert/find, no order |
| `map<K,V>` | Sorted key-value | O(log n), iterates in sorted key order |
| `unordered_map<K,V>` | Hash map | O(1) avg insert/find, no order — most common for DSA |

```cpp
pair<int,int> p = {1, 2};
cout << p.first << " " << p.second;

unordered_map<int,int> freq;
for (int x : nums) freq[x]++;         // auto-creates entry with value 0 if missing

priority_queue<int> maxHeap;
priority_queue<int, vector<int>, greater<int>> minHeap;

stack<int> st;
st.push(1); st.push(2);
cout << st.top(); st.pop();
```

---

## Iterators

```cpp
vector<int>::iterator it = v.begin();
auto it = v.begin();      // prefer auto — less typing, same thing
*it;                       // dereference to get value
it++;                      // move to next element
v.end();                   // one PAST the last element (never dereference this)
```

**Iterator invalidation**: modifying a container (e.g., `push_back` causing reallocation, or `erase`) can invalidate existing iterators/pointers into it. Don't hold onto an iterator across a mutation unless you know it's safe.

```cpp
// WRONG: erasing while iterating like this skips elements / invalidates it
for (auto it = v.begin(); it != v.end(); it++) {
    if (*it == target) v.erase(it);   // bug: it is now invalid
}

// RIGHT: erase returns the next valid iterator
for (auto it = v.begin(); it != v.end(); ) {
    if (*it == target) it = v.erase(it);
    else it++;
}
```

---

## Useful `<algorithm>` Functions

```cpp
#include <algorithm>

sort(v.begin(), v.end());                       // ascending, O(n log n)
sort(v.begin(), v.end(), greater<int>());       // descending
sort(v.begin(), v.end(), [](int a, int b){      // custom comparator
    return a > b;
});

reverse(v.begin(), v.end());
max_element(v.begin(), v.end());   // returns ITERATOR, dereference with *
min_element(v.begin(), v.end());
accumulate(v.begin(), v.end(), 0); // sum, needs <numeric>, 0 = initial value

// binary search — vector MUST be sorted first
binary_search(v.begin(), v.end(), x);   // returns bool
lower_bound(v.begin(), v.end(), x);     // iterator to first element >= x
upper_bound(v.begin(), v.end(), x);     // iterator to first element > x

unique(v.begin(), v.end());   // removes consecutive duplicates (sort first!), returns new logical end
```

```cpp
int target = 5;
auto it = lower_bound(v.begin(), v.end(), target);
if (it != v.end() && *it == target) {
    int index = it - v.begin();   // convert iterator back to index
}
```

---

## Strings

```cpp
string s = "hello";
s.length(); s.size();      // same thing
s[i];                       // char access
s.substr(start, len);       // substring — O(n), makes a copy
s + "world";                // concatenation
s.push_back('!');
to_string(42);               // int -> string
stoi("42");                   // string -> int
s.find("ll");                // returns index or string::npos if not found

for (char c : s) { ... }     // iterate chars
```

Strings are mutable in C++ (unlike Java/Python) — `s[0] = 'H';` works directly.

---

## Common Gotchas

- **`.size()` returns `size_t` (unsigned)**. `v.size() - 1` when `v` is empty underflows to a huge number, causing an infinite/crashing loop. Guard with `if (!v.empty())` or cast: `(int)v.size() - 1`.
- **Integer overflow**: `int` maxes out around 2.1 billion. Multiplying two ~50,000 values overflows. Use `long long` for sums/products that could get large.
- **Out-of-bounds access with `[]` doesn't throw** — it's undefined behavior (might silently corrupt memory, might crash, might "work" until it doesn't). Use `.at()` while debugging if unsure.
- **Passing large containers by value** silently copies the whole thing every call — a classic hidden O(n) cost inside a loop that turns O(n) into O(n²).
- **Dangling pointers**: returning the address of a local (stack) variable from a function gives you a pointer to memory that no longer exists.
  ```cpp
  int* bad() {
      int x = 5;
      return &x;   // BUG: x is destroyed when the function returns
  }
  ```
- **Uninitialized variables** have garbage values in C++ (unlike Java). Always initialize: `int count = 0;` not `int count;`.
- **`==` on floating point** is unreliable due to precision. Compare with an epsilon: `abs(a - b) < 1e-9`.
- **Recursion depth**: deep recursion (e.g., DFS on a skewed tree/graph with 100,000+ nodes) can stack-overflow. Consider converting to an iterative approach with an explicit stack for very deep recursion.
- **Modifying a container while range-based-for iterating over it** is undefined behavior — same root cause as iterator invalidation above.

---

## Complexity Cheat Sheet

| Operation | `vector` | `deque` | `list` | `set`/`map` | `unordered_set`/`map` |
|---|---|---|---|---|---|
| Access by index | O(1) | O(1) | O(n) | — | — |
| Insert/remove at end | O(1)* | O(1) | O(1) | O(log n) | O(1) avg |
| Insert/remove at front | O(n) | O(1) | O(1) | O(log n) | O(1) avg |
| Search | O(n) | O(n) | O(n) | O(log n) | O(1) avg |

\* amortized; occasional O(n) reallocation

`sort()`: O(n log n). `unordered_map`/`unordered_set` worst case degrades to O(n) with hash collisions (rare, but be aware in adversarial problems).

---

## Handy Snippets

```cpp
// Standard competitive-programming template header
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // your code here

    return 0;
}
```

```cpp
// Read n integers into a vector
int n; cin >> n;
vector<int> v(n);
for (int i = 0; i < n; i++) cin >> v[i];
```

```cpp
// Min/max of two values
min(a, b); max(a, b);
```

```cpp
// Swap
swap(a, b);
```
