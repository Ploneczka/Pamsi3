#pragma once
#include <limits>

#include "Warcaby.h"

struct Node
{
    Warcaby pionki;
    Warcaby::Move ruch;
    int wartosc;
    Node* rodzic;
};

std::vector<Node> makeNodes(Warcaby warcaby, bool gracz)
{
    std::vector<Warcaby::Move> ruchy;
    std::vector<Node> pionki;
    if (gracz)
    {
        ruchy = warcaby.ZnajdzRuch_G();
    }
    else
    {
        ruchy = warcaby.ZnajdzRuch_K();
    }
    
    for (auto& move : ruchy)
    {
        Warcaby kopia = warcaby;
        kopia.WykonajRuch(move, gracz);
        pionki.push_back({kopia, move, 0, nullptr});
    }
    return pionki;
}


int oplacalnosc(const Warcaby::Board& plansza)
{
    int waga = 0, komputer = 0, gracz = 0;

    for (int i = 0; i < Warcaby::size; ++i)
    {
        for (int j = 0; j < Warcaby::size; ++j)
        {
            if (plansza[i][j][0] == 'q' || plansza[i][j][0] == 'Q') 
            {
                komputer++;                           //zliczanie pionkow komputera
                if (plansza[i][j][0] == 'q')          // pionek maly 5 (komputer)
                {
                    waga += 2;
                }
                if (plansza[i][j][0] == 'Q')          // pionek duzy 10 (komputer)
                {
                    waga += 5;
                }
                if (i == 0 || j == 0 || i == (Warcaby::size - 1) || j == (Warcaby::size - 1))         //pionek przy scianie (komputer)
                {
                    waga += 4;
                }
                if (i + 1 > (Warcaby::size - 1) || j + 1 > (Warcaby::size - 1) || i - 1 < 0 || j - 1 < 0)     //aby nie wyjsc poza zasieg
                {
                    continue;
                }
                if ((plansza[i + 1][j - 1][0] == 'r' || plansza[i + 1][j - 1][0] == 'R') && plansza[i - 1][j + 1] == "-")    //ewentualne bicie przez gracza
                {
                    waga -= 1;
                }
                if ((plansza[i + 1][j + 1][0] == 'r' || plansza[i + 1][j + 1][0] == 'R') && plansza[i - 1][j - 1] == "-")    //ewentualne bicie przez gracza
                {
                    waga -= 1;
                }
                if (plansza[i - 1][j - 1][0] == 'R' && plansza[i + 1][j + 1] == "-")      //ewentualne bicie przez gracza krolowka
                {
                    waga -= 1;
                }
                if (plansza[i - 1][j + 1][0] == 'R' && plansza[i + 1][j - 1] == "-")      //ewentualne bicie przez gracza krolowka
                {
                    waga -= 1;
                }

                if (i + 2 > (Warcaby::size - 1) || (j - 2) < 0)   
                {     
                    continue;
                }
                if ((plansza[i + 1][j - 1][0] == 'r' || plansza[i + 1][j - 1][0] == 'R') && plansza[i + 2][j - 2] == "-")     //bicie przez komputer
                {
                    waga += 3;
                }
                if (j + 2 > (Warcaby::size - 1))
                {
                    continue;
                }
                if ((plansza[i + 1][j + 1][0] == 'r' || plansza[i + 1][j + 1][0] == 'R') && plansza[i + 2][j + 2] == "-")     //bicie przez komputer
                {
                    waga += 3;
                }
            }
            else if (plansza[i][j][0] == 'r' || plansza[i][j][0] == 'R')           
            {
                gracz++;                 //zliczanie pionkow gracza
            }
        }
    }
    int oplacalnosc = waga + (komputer - gracz) * 10000;
    return oplacalnosc;
}

int MinMax(Warcaby pionki, int glebokosc_drzewa, int a, int b, bool gracz)
{
    if (glebokosc_drzewa == 0)
    {
        return oplacalnosc(pionki.StworzPlansze());
    }

    if (!gracz)     // przewidywany ruch kompuetra
    {
        int max = -std::numeric_limits<int>::max();
        auto nodes = makeNodes(pionki, gracz);
        for (auto& node : nodes)
        {
            int wartosc = MinMax(node.pionki, glebokosc_drzewa - 1, a, b, true);
            max = std::max(max, wartosc);
            a = std::max(a, wartosc);
            if (b <= a)
                break;
        }
        return max;
    }
    else            //przewidywany ruch gracza
    {
        int min = std::numeric_limits<int>::max();
        auto nodes = makeNodes(pionki, gracz);
        for (auto& node : nodes)
        {
            int wartosc = MinMax(node.pionki, glebokosc_drzewa - 1, a, b, false);
            min = std::min(min, wartosc);
            b = std::min(b, wartosc);
            if (b <= a)
                break;
        }
        return min;
    }
}