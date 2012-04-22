// Andrés Mejía
using namespace std;
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <fstream>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <bitset>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <queue>
#include <deque>
#include <stack>
#include <list>
#include <map>
#include <set>

////////////// Prewritten code follows. Look down for solution. ////////////////
#define foreach(x, v) for (typeof (v).begin() x=(v).begin(); x !=(v).end(); ++x)
#define For(i, a, b) for (int i=(a); i<(b); ++i)
#define D(x) cout << #x " is " << (x) << endl

const double EPS = 1e-9;
int cmp(double x, double y = 0, double tol = EPS) {
    return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}
////////////////////////// Solution starts below. //////////////////////////////

const int MAXN = 300 * 300 * 3 + 14 + 100;
unsigned char image[MAXN];

int read(int at, int bytes) {
    unsigned int ans = 0;
    for (int i = 0; i < bytes; ++i) {
        int add = image[at + i];
        add <<= (8 * i);
        ans += add;
    }
    return ans;
}


int r[300][300], g[300][300], b[300][300];

int nr[300][300], ng[300][300], nb[300][300];

int emboss[3][3] =  { {-4, -2, 0},
                      {-2,  1, 2},
                      { 0,  2, 4} };

// int emboss[3][3] =  { {-1, -1, 0},
//                      {-1,  0, 1},
//                      { 0,  1, 1} };

                  
int average(int i, int j, int color[300][300]) {
    int ans = 0;
    
    for (int di = -1; di <= +1; ++di) {
        for (int dj = -1; dj <= +1; ++dj) {
            ans += color[i + di][j + dj] * emboss[di + 1][dj + 1];
        }
    }
    
    int sum = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            sum += emboss[i][j];
        }
    }
    if (sum != 0)
        ans /= sum;
   if (ans < 0) ans = 0;
   if (ans > 255) ans = 255;
   assert(0 <= ans and ans < 256);
   return ans;
}

int main() {
    string infile;
    getline(cin, infile);
    string outfile;
    getline(cin, outfile);
    
    FILE * imagefile = fopen(infile.c_str(), "r");
    assert(imagefile);
    int n = fread(image, 1, MAXN, imagefile);
    fclose(imagefile);
    
    assert(image[0] == 'B');
    assert(image[1] == 'M');
    // D(read(2, 1));
    // D(read(3, 1));
    // D(read(4, 1));
    // D(read(5, 1));
    int width = read(14 + 4, 4);
    int height = read(14 + 8, 4);
    
    // D(width);
    // D(height);

    int offset = read(10, 4);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            b[i][j] = read(offset++, 1);
            g[i][j] = read(offset++, 1);
            r[i][j] = read(offset++, 1);
        }
    }
    
    puts("before filter:");
    for (int i = 30, j = 30, di = -1; di <= +1; ++di){
        for (int dj = -1; dj <= +1; ++dj) {
            printf("%3d %3d %3d   ", r[i + di][j + dj], g[i + di][j + dj], b[i + di][j + dj]);
        }
        puts("");
    }
    
    // for (int i = 0; i < height; ++i) {
    //     for (int j = 0; j < width; ++j) {
    //         printf("(%3d %3d %3d) ", r[i][j], g[i][j], b[i][j]);
    //     }
    //     puts("");
    // }
    
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j) {
            nr[i][j] = r[i][j];
            ng[i][j] = g[i][j];
            nb[i][j] = b[i][j];
        }
    }
    
    for (int i = 1; i + 1 < height; ++i) {
        for (int j = 1; j + 1 < width; ++j) {
            nr[i][j] = average(i, j, r);
            ng[i][j] = average(i, j, g);
            nb[i][j] = average(i, j, b);
        }
    }
    
    offset = read(10, 4);
    for (int i = 0; i < height; ++i) {
           for (int j = 0; j < width; ++j) {
               image[offset++] = nb[i][j];
               image[offset++] = ng[i][j];
               image[offset++] = nr[i][j];
               // r[299 - i][j] = read(offset++, 1);
               // g[299 - i][j] = read(offset++, 1);
               // b[299 - i][j] = read(offset++, 1);
           }
    }
    
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j) {
            r[i][j] = nr[i][j];
            g[i][j] = ng[i][j];
            b[i][j] = nb[i][j];
        }
    }
    
    puts("after filter:");
    for (int i = 30, j = 30, di = -1; di <= +1; ++di){
        for (int dj = -1; dj <= +1; ++dj) {
            printf("%3d %3d %3d   ", r[i + di][j + dj], g[i + di][j + dj], b[i + di][j + dj]);
        }
        puts("");
    }
    
    
   FILE * out = fopen(outfile.c_str(), "w");
   assert(out);
   fwrite(image, 1, n, out);
   fclose(out);
    return 0;
}