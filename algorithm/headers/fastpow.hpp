#pragma once

template<class T = long long>
T pw(T a, T b, const T base) {
    T r = 1, t = a;
    while (b) {
        if (b & 1) r = (r * t) % base;
        t = (t * t) % base;
        b >>= 1;
    }
    return r;
}
