#include "Mono.h"

void p(char* t) 
{
	std::cout << t << std::endl;
}
void p(int& t)
{
	std::cout << t << std::endl;
}
void p(float& t)
{
	std::cout << t << std::endl;
}
void p(float* t)
{
	std::cout << t << std::endl;
}