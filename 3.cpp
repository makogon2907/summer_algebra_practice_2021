#include <math.h>
#include <pthread.h>

#include <algorithm>
#include <cassert>
#include <complex>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = unsigned long long;

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

vector<ll> get_base_p(const string& message, ll p) {
    vector<ll> number;
    vector<ll> deg = {1};

    for (auto el : message) {
        number = add(number, mul(deg, char_to_number(el), p), p);
        deg = mul(deg, 64, p);
    }
    return number;
}

vector<ll> get_poly_from_string(const string& s, ll p) {
    stringstream stream;
    stream << s;
    vector<ll> ans;
    long long coeff;
    while (stream >> coeff) {
        if (coeff < 0) {
            coeff += (long long)p;
        }
        ans.push_back(coeff);
    }
    return ans;
}

template <typename T>
void print_vector(const vector<T>& foo) {
    for (auto el : foo) {
        cout << el << " ";
    }
    cout << "\n";
}

void reduce(vector<ll>& ans, const vector<ll>& field_poly, ll p) {
    while (ans.size() >= field_poly.size()) {
        if (ans.back() == 0) {
            ans.pop_back();
            continue;
        }
        for (int i = 2; i <= field_poly.size(); i++) {
            ans[ans.size() - i] += ((p - field_poly[field_poly.size() - i]) * ans.back()) % p;
            ans[ans.size() - i] %= p;
        }
        ans.pop_back();
    }
}

vector<ll> poly_mul(const vector<ll>& a, const vector<ll>& b, const vector<ll>& field_poly, ll p) {
    if (a.size() == 0) {
        return b;
    }
    if (b.size() == 0) {
        return a;
    }
    vector<ll> ans(a.size() + b.size() - 1);

    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b.size(); j++) {
            ans[i + j] = (ans[i + j] + (a[i] * b[j]) % p) % p;
        }
    }
    reduce(ans, field_poly, p);
    return ans;
}

vector<ll> poly_binpow(vector<ll> poly, const vector<ll>& field_poly, ll deg, ll p) {
    vector<ll> ans = {1};
    while (deg > 0) {
        if (deg & 1) {
            ans = poly_mul(ans, poly, field_poly, p);
        }
        poly = poly_mul(poly, poly, field_poly, p);
        deg >>= 1;
    }
    return ans;
}

int main() {
    ll p;
    cin >> p;

    string input;
    getline(cin, input);

    getline(cin, input);
    auto field_poly = get_poly_from_string(input, p);

    // cout << input << endl;
    // print_vector(field_poly);

    getline(cin, input);
    auto generator = get_poly_from_string(input, p);

    // cout << input << endl;
    // print_vector(generator);

    getline(cin, input);
    auto public_key = get_poly_from_string(input, p);

    // cout << input << endl;
    // print_vector(public_key);

    getline(cin, input);
    auto message = get_base_p(input, p);

    print_vector(message);

    vector<vector<ll>> elements;

    for (int i = 0; i < message.size(); i++) {
        if (i % (field_poly.size() - 1) == 0) {
            elements.emplace_back();
        }
        elements.back().push_back(message[i]);
    }

    // for (auto el : elements) {
    //     print_vector(el);
    // }

    for (auto el : elements) {
        int a = rand() % p + 1;
        print_vector(poly_binpow(generator, field_poly, a, p));
        print_vector(poly_mul(el, poly_binpow(public_key, field_poly, a, p), field_poly, p));
    }

    return 0;
}
