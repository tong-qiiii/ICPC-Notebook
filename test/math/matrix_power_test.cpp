#include "code/math/matrix_power.hpp"
int main()
{
    mt19937 gen(20261016);
    Matrix<101> A(2);
    A[0][0] = A[0][1] = A[1][0] = 1;
    assert(A.pow(5)[0][0].v == 8);
    {
        Matrix<101> A(2);
        A[0][0] = A[0][1] = A[1][0] = 1;
        int a = 0, b = 1;
        for (int i = 0; i < 100; i++)
        {
            assert(A.pow(i)[0][1].v == a);
            int c = (a + b) % 101;
            a = b;
            b = c;
        }
        Matrix<101> empty(0);
        assert(empty.pow(0).n == 0);
    }
    cout << "PASS matrix_power\n";
}
