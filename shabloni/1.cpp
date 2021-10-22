#include <iostream>

template <typename T>
T min(T value1, T value2)
{
	if (value1 > value2)
		return value2;
	else
		return value1;
}

template <typename T>
T max(T value1, T value2)
{
	if (value1 > value2)
		return value1;
	else
		return value2;
}

template <typename T>
void Swap(T& value1, T& value2)
{
	T temp = value1;
	value1 = value2;
	value2 = temp;
}

template <typename T>
T Abs(T value)
{
	return max<T>(value, -value);
}

int main()
{
	int a = 1;
	int b = 2;
	double c = -3.1;
	double d = 1.1;
	std::cout << max<int>(a,b) << " " << min<double>(c, d) << " ";
	Swap<double>(c, d);
	std::cout << Abs<double>(c) << " " << min<double>(c, d) << " ";
}