#include "vectormap.hpp"

void VectorMap::init(int size)
{
	this->size = size;
	vectorMap.resize(size*size);

}
