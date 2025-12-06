//
// Created by Manju Muralidharan on 11/22/25.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Directions for DFS (students must use these)
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

// to explore DFS, use
// for (int i = 0; i < 4; i++) {
//     int nr = r + dr[i]; // new row
//     int nc = c + dc[i]; // new column
// }
// -1-0 up, 0-1 right, 1-0 down, 0-(-1) left
//
// ----------------------------------------------------------
// DO NOT MODIFY: Maze generation
// ----------------------------------------------------------
void generateMaze(vector<vector<int>>& maze, int N, int M) {
    srand(time(0));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int roll = rand() % 100;
            maze[r][c] = (roll < 70) ? 0 : 1;   // 0 = open, 1 = wall
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Choose a random boundary cell that is open
// ----------------------------------------------------------
pair<int,int> chooseBoundaryCell(const vector<vector<int>>& maze) {
    int N = maze.size();
    int M = maze[0].size();

    while (true) {
        int side = rand() % 4;
        int r, c;

        if (side == 0) {          // top row
            r = 0;
            c = rand() % M;
        } else if (side == 1) {   // bottom row
            r = N - 1;
            c = rand() % M;
        } else if (side == 2) {   // left column
            r = rand() % N;
            c = 0;
        } else {                  // right column
            r = rand() % N;
            c = M - 1;
        }

        if (maze[r][c] == 0) {
            return {r, c};
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the maze with S and E markers
// ----------------------------------------------------------
void printMaze(const vector<vector<int>>& maze,
               int ent_r, int ent_c,
               int exit_r, int exit_c)
{
    int N = maze.size();
    int M = maze[0].size();

    cout << "\nMaze:\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (r == ent_r && c == ent_c) {
                cout << "S ";
            } else if (r == exit_r && c == exit_c) {
                cout << "E ";
            } else {
                cout << maze[r][c] << " ";
            }
        }
        cout << "\n";
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the reconstructed path
// Students must fill parent[][] correctly during DFS
// ----------------------------------------------------------
void printPath(pair<int,int> exitcell,
               const vector<vector<int>>& parent_r,
               const vector<vector<int>>& parent_c,
               int ent_r, int ent_c)
{
    int r = exitcell.first;
    int c = exitcell.second;

    vector<pair<int,int>> path;

    // Walk backward from exit to entrance
    while (!(r == ent_r && c == ent_c)) {
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr;
        c = pc;
    }
    path.push_back({ent_r, ent_c});

    cout << "\nPath from entrance to exit:\n";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << "(" << path[i].first << ", " << path[i].second << ")\n";
    }
}

// ----------------------------------------------------------
// STUDENTS IMPLEMENT DFS HERE
// Add arguments, return type, and logic
// ----------------------------------------------------------
// note r is row and c is column, so we pass in two to know the coord of entrance/exit
bool dfs(int ent_r, int ent_c, const vector<vector<int>>& maze, vector<vector<bool>>& visited,
     vector<vector<int>>& parent_r, vector<vector<int>>& parent_c, int exit_r, int exit_c) {
    // outline:
    // mark the current cell as visited
    // check if exit
    // then check all adjacent ones (use the arrays at the top)
    // for each checked cell:
    // check within bounds OR not a wall
    // check if visited
    // then store as parent
    // now recurse ie check nodes next to the parent

    // first set coord we are at
    int r = ent_r;
    int c = ent_c;
    // immediately add to visited
    visited[r][c] = true;


        // 5. Checking if (r, c) is the exit
        // check if at exit yet
        if (r == exit_r && c == exit_c) {
            return true;
        }

        // check the 4 different adjacent grid parts
        // 6. exploring neighbors using dr and dc
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i]; // neighbor row
            int nc = c + dc[i]; // column


            // 1. Out-of-bounds checks
            if (nr < 0 || nr >= maze.size()) {
                continue;
            }
            if (nc < 0 || nc >= maze[0].size()) {
                continue;
            }

            // 2. Wall checks (maze[r][c] == 1)
            if (maze[nr][nc] == 1) {
                continue;
            }
            // check if already seen
            // 3. Visited checks
            if (visited[nr][nc] == true) {
                continue;
            }

            // 4. Marking the neighbor cell we are looking at as visited
            visited[nr][nc] = true;

            // save parent before moving onto neighbor
            parent_r[nr][nc] = r;
            parent_c[nr][nc] = c;

            // note that nc and nr are our parents to now look through
            // 7. Assigning the parent before recursing

            // recurse ie now check everything next to parent
            // 8. Returning true when the exit is found
            if (dfs(nr, nc, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
                return true;
            }
        }
    return false;
    }

// ----------------------------------------------------------
// MAIN PROGRAM (students add DFS calls and logic)
// ----------------------------------------------------------
int main() {
    int N, M;

    cout << "Enter maze dimensions N M: ";
    cin >> N >> M;

    vector<vector<int>> maze(N, vector<int>(M));
    generateMaze(maze, N, M);

    // Pick entrance and exit
    pair<int,int> entrance = chooseBoundaryCell(maze);
    pair<int,int> exitcell = chooseBoundaryCell(maze);

    while (exitcell == entrance) {
        exitcell = chooseBoundaryCell(maze);
    }

    int ent_r = entrance.first;
    int ent_c = entrance.second;
    int exit_r = exitcell.first;
    int exit_c = exitcell.second;

    // Display the maze
    printMaze(maze, ent_r, ent_c, exit_r, exit_c);

    // Students must use these
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    vector<vector<int>> parent_r(N, vector<int>(M, -1));
    vector<vector<int>> parent_c(N, vector<int>(M, -1));

    // ------------------------------------------------------
    // STUDENT WORK:
    // Call your DFS, track visited, and fill parent_r and parent_c
    // ------------------------------------------------------
    bool found = dfs(ent_r, ent_c, maze, visited, parent_r, parent_c, exit_r, exit_c);

    // ------------------------------------------------------
    // STUDENT WORK:
    // If found, print the path
    // ------------------------------------------------------
    if (found) {
         printPath(exitcell, parent_r, parent_c, ent_r, ent_c);
     } else {
         cout << "\nNo path exists.\n";
     }

    return 0;
}