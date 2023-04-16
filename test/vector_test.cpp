#pragma once

#include <iostream>
#include "../array.h"

#define CHECK(x) if(!(x)){std::cerr<<__FUNCTION__<<"() Line "<<__LINE__<<" failed.";abort();}

void vector_test(){
	using namespace ciel;
}