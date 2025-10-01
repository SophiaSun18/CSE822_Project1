#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

void matmul(const vector<float>& A, const vector<float>& B, vector<float>& C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < N; ++k) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

int main(int argc, char* argv[]) {

    int N = atoi(argv[1]);
    srand(42); // seed random generator

    vector<float> A(N * N), B(N * N), C(N * N, 0.0f);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = static_cast<float>(rand()) / RAND_MAX;
            B[i * N + j] = static_cast<float>(rand()) / RAND_MAX;
        }
    }

    auto start = chrono::high_resolution_clock::now();

    matmul(A, B, C, N);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "matmul took: " << elapsed.count() << " seconds" << endl;

    return 0;
}
