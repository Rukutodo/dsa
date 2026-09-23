# DSA

Data Structures & Algorithms practice — problems, notes, and solutions.

## Editing & running the code

- **Browsing/editing only**: press `.` on this repo (or go to `https://github.dev/<your-username>/dsa`). This is just an editor — it cannot compile or run code.
- **Compiling/running in the cloud**: open this repo in **GitHub Codespaces** (Code → Codespaces → Create codespace). The included `.devcontainer/` config gives you a full Linux VM with g++ preinstalled and a real terminal. Uses your free monthly Codespaces hours, then billed.
- **Compiling/running locally**: install a C++ compiler (e.g., MinGW-w64/g++ on Windows) and use the terminal:
  ```bash
  g++ -std=c++17 -Wall -o solution solution.cpp
  ./solution
  ```

## Structure

- `data-structures/` — implementations (arrays, linked lists, trees, graphs, heaps, etc.)
- `algorithms/` — sorting, searching, dynamic programming, greedy, graph algorithms, etc.
- `problems/` — solutions to practice problems (LeetCode, etc.), organized by topic
- `notes/` — concept notes and complexity references

## Conventions

Each solution file includes:
- Problem statement / link
- Approach summary
- Time and space complexity
