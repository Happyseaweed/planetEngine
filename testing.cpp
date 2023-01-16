#include <iostream>

using namespace std;

int main(){
    double a = 1e30;
    double b = 1e24;
    double c = 6e-11;

    double ans = a * b / (c * c);
    cout << ans << endl;

    return 0;
}