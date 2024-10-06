//
// Created by 陈子昂 on 2024/2/13.
//

#include "Player.h"
#include "libs/doudizhu.h"
using namespace std;

Player::Player() {

}

Player::~Player() {

}

// 设置手牌
Player & Player::set_cards(string cards1) {
    cards = cards1;
    return *this;
}

// 获取手牌
const string & Player::get_cards() const {
    return cards;
}


// 整理手牌
Player & Player::sort_cards() {
    sort(cards.begin(), cards.end(), card_comp);
    return *this;
}
