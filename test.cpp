#include <iostream>
#include "algorithm.h"
#include "array.h"
#include "vector.h"
#include <vector>
#include "iterator.h"

extern void array_test();

struct testObject{
	int id=1023;
	testObject(){
		std::cout<<"默认构造函数"<<std::endl;
	}
	testObject& operator=(const testObject& other){
		this->id=other.id;
		std::cout<<"复制运算符"<<std::endl;
		return *this;
	}
	testObject(const testObject& other){
		*this=other;
		std::cout<<"复制构造函数"<<std::endl;
	}
	testObject& operator=(testObject&& other) noexcept {
		this->id=other.id;
		other.id=0;
		std::cout<<"移动运算符"<<std::endl;
		return *this;
	}
	testObject(testObject&& other) noexcept {
		*this=std::move(other);
		std::cout<<"移动构造函数"<<std::endl;
	}
	testObject(int x):id(x){
		std::cout<<"初始化id构造函数"<<std::endl;
	}
	~testObject(){
		std::cout<<"析构函数"<<std::endl;
	}
};


int main(){
	std::cout<<__VERSION__<<std::endl;



//	array_test();
}