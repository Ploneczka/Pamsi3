#include "Warcaby.cpp"

int main()
{
    std::cout << std::endl;
    std::cout << "Kilka prostych zasad: "<<std::endl;
    std::cout << "1. Poruszanie się wykonuję się za pomocą podania wspolrzednych obecnych pionka i tych na ktore ma sie poruszyc." <<std::endl;
    std::cout << "2. Bicie jest obowiazkowe." <<std::endl;
    std::cout << "3. Gdy pionek zamieni sie w damke moze bic do tylu oraz sie poruszać do tylu." <<std::endl;
    Warcaby Gra1;
    Gra1.graj();
    return 0;
}