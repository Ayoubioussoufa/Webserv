#include <iostream>

void    function(int *i)
{
    *i = 10;
    return ;
}

int main()
{
    int i = 5;
    function(&i);
    std::cout << i << std::endl;
}