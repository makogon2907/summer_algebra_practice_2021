#include <math.h>

#include <algorithm>
#include <cassert>
#include <complex>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;
using ll = unsigned long long;

// ----------------------------------------------------------------

ll char_to_number(char symbol) {
    if (symbol >= 48 && symbol <= 57)
        return symbol - 48;
    if (symbol >= 65 && symbol <= 90)
        return symbol - 55;
    if (symbol >= 97 && symbol <= 122)
        return symbol - 61;
    if (symbol == 32)
        return 62;
    if (symbol == 46)
        return 63;
    return 64;
}

template <typename T>
T binpow(T a, ll t, ll mod) {
    if (t == 0) {
        return 1;
    }
    T ans = 1;
    while (t > 0) {
        if (t & 1) {
            ans = (ans * a) % mod;
        }
        a = (a * a) % mod;
        t >>= 1;
    }
    return ans;
}

vector<ll> add(const vector<ll>& a, const vector<ll>& b, ll base) {
    if (a.size() == 0) {
        return b;
    }
    if (b.size() == 0) {
        return a;
    }

    vector<ll> ans;
    ll got = 0;
    for (int i = 0; i < min(a.size(), b.size()); i++) {
        ans.push_back((a[i] + b[i] + got) % base);
        got = (a[i] + b[i] + got) / base;
    }
    for (int i = min(a.size(), b.size()); i < a.size(); i++) {
        ans.push_back((a[i] + got) % base);
        got = (a[i] + got) / base;
    }
    for (int i = min(a.size(), b.size()); i < b.size(); i++) {
        ans.push_back((b[i] + got) % base);
        got = (b[i] + got) / base;
    }
    while (got > 0) {
        ans.push_back(got % base);
        got /= base;
    }
    return ans;
}

vector<ll> mul(const vector<ll>& a, ll b, ll base) {
    vector<ll> ans;
    ll got = 0;
    for (auto el : a) {
        ans.push_back((el * b + got) % base);
        got = (el * b + got) / base;
    }
    while (got > 0) {
        ans.push_back(got % base);
        got /= base;
    }
    return ans;
}

int main() {
    srand(time(NULL));

    ll p, elem, public_key;
    cin >> p >> elem >> public_key;

    string message;

    getline(cin, message);
    getline(cin, message);

    vector<ll> number;
    vector<ll> deg = {1};

    for (auto el : message) {
        number = add(number, mul(deg, char_to_number(el), p), p);
        deg = mul(deg, 64, p);
    }

    for (auto el : number) {
        cout << el << " ";
    }
    return 0;

    // cout << number << endl;

    for (auto el : number) {
        int a = rand() % p + 1;
        cout << binpow(elem, a, p) << " " << (binpow(public_key, a, p) * el) % p
             << "\n";
    }

    return 0;
}
