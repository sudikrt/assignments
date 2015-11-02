#include <cassert>
#include "recp.hpp"
double reciprocal (int i)
{
	assert (i != 0); //check i shuld non-zero
	return 1.0 / i;
}
