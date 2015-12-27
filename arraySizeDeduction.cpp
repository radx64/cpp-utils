#include <iostream>

template<typename T, int length>
void ProcessArray(T (&array)[length]) 
{
	for (int i=0; i<length; ++i)
	{
		std::cout << array[i] << std::endl;
	}
}

int main()
{
	int a[] = {1,2,3,4,5};
	ProcessArray(a);
}