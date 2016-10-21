#include <iostream>

using namespace std;

int main(int argc, char * argv[]){

	int a = 0;
	int b = 1;

	while (a < b){
		a++;
		b++;	
	}

	std::cout << "Max int is: " << a << std::endl;

	return 0;
}
