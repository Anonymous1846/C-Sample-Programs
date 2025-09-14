#include<iostream>
#include<vector>
#include<numeric>

int main(){
	int number;
	std::vector<double>marks;
	std::cout<<"How many students are there: ";
	std::cin>>number;
	for(int i=0;i<number;i++){
		double mark;
		std::cout<<"Enter mark for student "<<i<< ": ";
		std::cin>>mark;
		marks.push_back(mark);
	}
	//total marks
	double sum = accumulate(marks.begin(),marks.end(),0);
	double average = sum/number;

	std::cout<<"Total Marks: "<<sum<<";"<<std::endl<<"Average: "<<average<<std::endl;
	return 0;
}
