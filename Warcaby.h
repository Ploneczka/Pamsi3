#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <string>


class Warcaby
{
    public:
        static constexpr std::size_t size = 8;
        typedef std::array<std::array<std::string, size>, size> Board;

        struct Move
        {
            int PoprzednieI, PoprzednieJ, NoweI, NoweJ;
            bool operator==(const Move& o) 
            { 
                return NoweI == o.NoweI && NoweJ == o.NoweJ && PoprzednieI == o.PoprzednieI && PoprzednieJ == o.PoprzednieJ;
            }
        };
    public:
        const Board& StworzPlansze() const;
        Warcaby();
        void graj();
        void WykonajRuch(Move Ruch, bool gracz);   
        std::vector<Move> ZnajdzRuch_K() const;
        std::vector<Move> ZnajdzRuch_G() const;
    private:
        void Rysuj_plansze() const;
        bool SprawdzenieRuchu(Move Ruch, bool gracz) const;
        bool SprawdzenieBicia(Move Ruch, int WzdluzPion, int WzdluzPoziom, bool gracz) const;
        void Zliczanie();
    private:
        Board pPlansza;
        bool pRuch_G;
        unsigned int pPionki_G;
        unsigned int pPionki_K;
};
