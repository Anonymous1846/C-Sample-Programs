#include<iostream>
#include<utility>


int main(){
	int a = 10;
	int b = 20;
	std::cout<< "Value of a before swap: "<< a<<std::endl;
	std::cout<< "Value of b before swap: "<< b<<std::endl;
	int *pointerA = &a;
	int *pointerB = &b;
	pointerA= &b;
	pointerB= &a;
	std::cout<< "Address location for a: "<<pointerA<<std::endl;
	std::cout<< "Address location for b: "<<pointerB<<std::endl;
	std::cout<< "Value of a after swap: "<< *pointerA<<std::endl;
	std::cout<< "Value of b after swap: "<< *pointerB<<std::endl;
	std::swap(a,b);
	std::cout<< "Value of a after swap: "<< a<<std::endl;
	std::cout<< "Value of b after swap: "<< b<<std::endl;
	return 0;
}
