#include <limits>
#include <algorithm>

#include "Warcaby.h"
#include "MinMax.hpp"

Warcaby::Warcaby()
: pPlansza()
, pRuch_G(true)
, pPionki_G(12)
, pPionki_K(12)
{
    // Wypelnienie tablicy
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            pPlansza[i][j] = "-";
        }
    }
    // pionki komputera
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if ((i + j) % 2 == 1)
            {
                pPlansza[i][j] = "q";
            }
        }
    }
    // pionki gracza
    for (int i = 5; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if ((i + j) % 2 == 1)
            {
                pPlansza[i][j] = "r";
            }
        }
    }
}
const Warcaby::Board& Warcaby::StworzPlansze() const
{
    return pPlansza;
}
void Warcaby::Rysuj_plansze() const
{
    std::cout << std::endl;
    std::cout << "  __________________" << std::endl;
    for (int i = 0; i < size; ++i)
    {
        std::cout << i << " |";
        for (int j = 0; j < size; ++j)
        {
            std::cout << pPlansza[i][j] << " ";
        }
        std::cout << "|"<< std::endl;
    }
    std::cout << "  ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺" << std::endl;
    for (int i = 0; i < size; ++i)
    {
        if (i == 0)
            std::cout << "   ";
        std::cout << i << " "; 
    }
    std::cout << std::endl;
}

void Warcaby::graj()
{
    while (true)
    {
        Rysuj_plansze();
        if (pRuch_G)
        {
            std::cout << std::endl;
            std::cout << "Twoj ruch: " << std::endl;
            auto Ruchy = ZnajdzRuch_G();
            if (Ruchy.size() == 0)
            {
                if (pPionki_K > pPionki_G)
                {
                    std::cout << "Brak ruchu, przegrales." << std::endl;
                }
                else
                {
                    std::cout << "Brak ruchu, remis." << std::endl;
                }
                break;
            }
            Move Nastepny_Ruch;
            std::cout << "Wybierz pionek i wykonaj ruch: " << std::endl;
            if (!(std::cin >> Nastepny_Ruch.PoprzednieI >> Nastepny_Ruch.PoprzednieJ))
            {
                std::cout << "Zle dane." << std::endl;
                break;
            }
            if (!(std::cin >> Nastepny_Ruch.NoweI >> Nastepny_Ruch.NoweJ))
            {
                std::cout << "Zle dane." << std::endl;
                break;
            }
            if (std::find(Ruchy.begin(), Ruchy.end(), Nastepny_Ruch) == Ruchy.end())
            {
                std::cout << "Niedozwolony ruch." << std::endl;
                continue;
            }

            WykonajRuch(Nastepny_Ruch, true);
            Zliczanie();
        }
        else
        {
            auto PierwszyRuch_K = makeNodes(*this, false);

            if (PierwszyRuch_K.size() == 0)
            {
                if (pPionki_K < pPionki_G)
                {
                    std::cout << "Brak ruchu, Wygrales" << std::endl;
                }
                else
                {
                    std::cout << "Brak ruchu, Remis" << std::endl;
                }
            }

            int lepszy_wynik = -std::numeric_limits<int>::max();
            Move NajlepszyRuch;
            for (auto& node : PierwszyRuch_K)
            {
                int wartosc = MinMax(node.pionki, 3, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), true);      //poziom trudnosci - wybor glebokosci przeszukiwania
                if (wartosc > lepszy_wynik)
                {
                    lepszy_wynik = wartosc;
                    NajlepszyRuch = node.ruch;
                }
            }

            if (lepszy_wynik == -std::numeric_limits<int>::max())
            {
                std::cout << "Wygrales." << std::endl;
                break;
            }
            WykonajRuch(NajlepszyRuch, false);
        }

        if (pPionki_G == 0)
        {
            std::cout << "Przegrales" << std::endl;
            break;
        }
        else if (pPionki_K == 0)
        {
            std::cout << "Wygrales" << std::endl;
            break;
        }

        pRuch_G = !pRuch_G;
    }
}

bool Warcaby::SprawdzenieRuchu(Warcaby::Move Ruch, bool gracz) const
{
    if (Ruch.NoweI >= size || Ruch.NoweI < 0)
    {
        return false;
    }
    if (Ruch.NoweJ >= size || Ruch.NoweJ < 0)
    {
        return false;
    }
    if (pPlansza[Ruch.PoprzednieI][Ruch.PoprzednieJ] == "-")
    {    
        return false;
    }
    if (pPlansza[Ruch.NoweI][Ruch.NoweJ] != "-")
    {
        return false;
    }
    if (!gracz && (pPlansza[Ruch.PoprzednieI][Ruch.PoprzednieJ][0] == 'r' || pPlansza[Ruch.PoprzednieI][Ruch.PoprzednieJ][0] == 'R'))
    {
        return false;
    }
    if (gracz && (pPlansza[Ruch.PoprzednieI][Ruch.PoprzednieJ][0] == 'q' || pPlansza[Ruch.PoprzednieI][Ruch.PoprzednieJ][0] == 'Q'))
    {
        return false;
    }
    return true;
}

bool Warcaby::SprawdzenieBicia(Warcaby::Move Ruch, int WzdluzPion, int WzdluzPoziom, bool gracz) const
{
    if (!SprawdzenieRuchu(Ruch, gracz))
    {
        return false;
    }
    if (pPlansza[WzdluzPion][WzdluzPoziom] == "-")
    {
        return false;
    }
    if (!gracz && (pPlansza[WzdluzPion][WzdluzPoziom][0] == 'q' || pPlansza[WzdluzPion][WzdluzPoziom][0] == 'Q'))
    {
        return false;
    }
    if (gracz && (pPlansza[WzdluzPion][WzdluzPoziom][0] == 'r' || pPlansza[WzdluzPion][WzdluzPoziom][0] == 'R'))
    {
        return false;
    }
    return true;
}
std::vector<Warcaby::Move> Warcaby::ZnajdzRuch_G() const
{
    std::vector<Move> Ruchy;
    std::vector<Move> Bicia;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (pPlansza[i][j][0] == 'r')
            {
                if (SprawdzenieRuchu({i, j, i - 1, j - 1}, true))
                {
                    Ruchy.push_back({i, j, i - 1, j - 1});
                }
                if (SprawdzenieRuchu({i, j, i - 1, j + 1}, true))
                {
                    Ruchy.push_back({i, j, i - 1, j + 1});
                }
                if (SprawdzenieBicia({i, j, i - 2, j - 2}, i - 1, j - 1, true))
                {
                    Bicia.push_back({i, j, i - 2, j - 2});
                }
                if (SprawdzenieBicia({i, j, i - 2, j + 2}, i - 1, j + 1, true))
                {
                    Bicia.push_back({i, j, i - 2, j + 2});
                }
            }
            else if (pPlansza[i][j][0] == 'R')
            {
                if (SprawdzenieRuchu({i, j, i + 1, j + 1}, true))
                {
                    Ruchy.push_back({i, j, i + 1, j + 1});
                }
                if (SprawdzenieRuchu({i, j, i + 1, j - 1}, true))
                {
                    Ruchy.push_back({i, j, i + 1, j - 1});
                }
                if (SprawdzenieRuchu({i, j, i - 1, j + 1}, true))
                {
                    Ruchy.push_back({i, j, i - 1, j + 1});
                }
                if (SprawdzenieRuchu({i, j, i - 1, j - 1}, true))
                {
                    Ruchy.push_back({i, j, i - 1, j - 1});
                }
                if (SprawdzenieBicia({i, j, i + 2, j + 2}, i + 1, j + 1, true))
                {
                    Bicia.push_back({i, j, i + 2, j + 2});
                }
                if (SprawdzenieBicia({i, j, i + 2, j - 2}, i + 1, j - 1, true))
                {
                    Bicia.push_back({i, j, i + 2, j - 2});
                }
                if (SprawdzenieBicia({i, j, i - 2, j + 2}, i - 1, j + 1, true))
                {
                    Bicia.push_back({i, j, i - 2, j + 2});
                }
                if (SprawdzenieBicia({i, j, i - 2, j - 2}, i - 1, j - 1, true))
                {
                    Bicia.push_back({i, j, i - 2, j - 2});
                }
            }
        }
    }
    
    if (Bicia.size() == 0)
    {
        return Ruchy;
    }
    return Bicia;
}

std::vector<Warcaby::Move> Warcaby::ZnajdzRuch_K() const
{
    std::vector<Move> Ruchy;
    std::vector<Move> Bicia;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (pPlansza[i][j][0] == 'q')
            {
                if (SprawdzenieRuchu({i, j, i + 1, j + 1}, false))
                {
                    Ruchy.push_back({i, j, i + 1, j + 1});
                }
                if (SprawdzenieRuchu({i, j, i + 1, j - 1}, false))
                {
                    Ruchy.push_back({i, j, i + 1, j - 1});
                }
                if (SprawdzenieBicia({i, j, i + 2, j + 2}, i + 1, j + 1, false))
                {
                    Bicia.push_back({i, j, i + 2, j + 2});
                }
                if (SprawdzenieBicia({i, j, i + 2, j - 2}, i + 1, j - 1, false))
                {
                    Bicia.push_back({i, j, i + 2, j - 2});
                }
            }
            else if (pPlansza[i][j][0] == 'Q')
            {
                if (SprawdzenieRuchu({i, j, i + 1, j + 1}, false))
                {
                    Ruchy.push_back({i, j, i + 1, j + 1});
                }
                if (SprawdzenieRuchu({i, j, i + 1, j - 1}, false))
                {
                    Ruchy.push_back({i, j, i + 1, j - 1});
                }
                if (SprawdzenieRuchu({i, j, i - 1, j + 1}, false))
                {
                    Ruchy.push_back({i, j, i - 1, j + 1});
                }
                if (SprawdzenieRuchu({i, j, i - 1, j - 1}, false))
                {
                    Ruchy.push_back({i, j, i - 1, j - 1});
                }
                if (SprawdzenieBicia({i, j, i + 2, j + 2}, i + 1, j + 1, false))
                {
                    Bicia.push_back({i, j, i + 2, j + 2});
                }
                if (SprawdzenieBicia({i, j, i + 2, j - 2}, i + 1, j - 1, false))
                {
                    Bicia.push_back({i, j, i + 2, j - 2});
                }
                if (SprawdzenieBicia({i, j, i - 2, j + 2}, i - 1, j + 1, false))
                {
                    Bicia.push_back({i, j, i - 2, j + 2});
                }
                if (SprawdzenieBicia({i, j, i - 2, j - 2}, i - 1, j - 1, false))
                {
                    Bicia.push_back({i, j, i - 2, j - 2});
                }
            }
        }
    }
    
    if (Bicia.size() == 0)
    {
        return Ruchy;
    }
    return Bicia;
}

void Warcaby::WykonajRuch(Move Ruch, bool player)
{
    char uzytkownik = pPlansza[Ruch.PoprzednieI][Ruch.PoprzednieJ][0];
    if (player)
    {
        if (Ruch.NoweI == 0)
        {
            uzytkownik = 'R';
        }
    }
    else
    {
        if (Ruch.NoweI == (size - 1))
        {
            uzytkownik = 'Q';
        }
    }
    
    int RoznicaI = Ruch.PoprzednieI - Ruch.NoweI;
    int RoznicaJ = Ruch.PoprzednieJ - Ruch.NoweJ;

    if (RoznicaI == -2 && RoznicaJ == 2)
    {
        pPlansza[Ruch.PoprzednieI + 1][Ruch.PoprzednieJ - 1] = "-";
    }
    else if (RoznicaI == 2 && RoznicaJ == 2)
    {
        pPlansza[Ruch.PoprzednieI - 1][Ruch.PoprzednieJ - 1] = "-";
    }
    else if (RoznicaI == 2 && RoznicaJ == -2)
    {
        pPlansza[Ruch.PoprzednieI - 1][Ruch.PoprzednieJ + 1] = "-";
    }
    else if (RoznicaI == -2 && RoznicaJ == -2)
    {
        pPlansza[Ruch.PoprzednieI + 1][Ruch.PoprzednieJ + 1] = "-";
    }

    pPlansza[Ruch.PoprzednieI][Ruch.PoprzednieJ] = "-";
    pPlansza[Ruch.NoweI][Ruch.NoweJ] = uzytkownik ;
}

void Warcaby::Zliczanie()
{
    pPionki_K = 0;
    pPionki_G = 0;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (pPlansza[i][j][0] == 'q' || pPlansza[i][j][0] == 'Q')
            {
                pPionki_K++;
            }
            else if (pPlansza[i][j][0] == 'r' || pPlansza[i][j][0] == 'R')
            {
                pPionki_G++;
            }
        }
    }
}