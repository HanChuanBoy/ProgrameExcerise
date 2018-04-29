#include <stdio.h>

const char* const s1 = "string";
char const *s2 = "string";

int main()
{
	//s1 = "w";
	//*s1 = 'w';
	s2 = "w";
	//*s2 = 'w';
}
/*c99
jichu3.cpp(8) : error C3892: 's1' : you cannot assign to a variable that is const
jichu3.cpp(11) : error C3892: 's2' : you cannot assign to a variable that is const
for jichu3:9 :run error ,because s1 point to a const string;

*/