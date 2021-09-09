#pragma once
#include <string>

class MASSValue{};

class MASSInt : public MASSValue {
public:
	MASSInt(int val) { value = val; }
	int value;
};

class MASSFloat : public MASSValue {
public:
	MASSFloat(double val) { value = val; }
	double value;
};

class MASSString : public MASSValue {
public:
	MASSString(std::wstring& val) { value = val; }
	std::wstring value;
};

template <class T1, class T2>
class MASSPair : public MASSValue {
public:
	MASSPair(T1 val1, T2 val2) { value = std::pair<T1, T2>(val1, val2); }
	std::pair<T1, T2> value;
};