#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#pragma push_macro("ll")
#pragma push_macro("x")
#pragma push_macro("y")
#pragma push_macro("db")
#pragma push_macro("endl")
#pragma push_macro("debug")
#undef ll
#undef x
#undef y
#undef db
#undef endl
#undef debug
#include <boost/multiprecision/cpp_int.hpp>
#pragma pop_macro("debug")
#pragma pop_macro("endl")
#pragma pop_macro("db")
#pragma pop_macro("y")
#pragma pop_macro("x")
#pragma pop_macro("ll")
using BigInt = boost::multiprecision::cpp_int;
