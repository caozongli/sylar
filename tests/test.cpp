#include <memory>
#include <iostream>
using namespace std;

int main()
{
	std::shared_ptr<int> p(new int(2));
	int a = *p;
	int *b = p.get();
	std::cout << p.use_count() << std::endl;

	return 0;
}