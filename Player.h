//
// Created by 陈子昂 on 2024/2/13.
//

#include <string>
using namespace std;
#ifndef DOUDIZHU_PLAYER_H
#define DOUDIZHU_PLAYER_H


class Player {
    int cards_count;
    bool is_landlord;
    string cards;
public:
    Player();
    ~Player();
    Player & set_cards(string cards);
    const string & get_cards() const;
    Player & sort_cards();

};


#endif //DOUDIZHU_PLAYER_H
