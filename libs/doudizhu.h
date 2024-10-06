//
// Created by 陈子昂 on 2024/2/14.
//

#ifndef DOUDIZHU_DOUDIZHU_H
#define DOUDIZHU_DOUDIZHU_H
void sleep(double t);
void add_cards(char ch, int count);
void clear_screen();
bool card_comp(char x, char y);
bool card_comp_g(char x, char y);
int prev_player(int x);
int next_player(int x);
bool can_hit1(string my_cards, string target);
bool HasCards(string input, string my_cards);
string EraseCards(string input, const string & my_cards);
bool CardIsValid(string input);
string GetCardType(string input);
#endif //DOUDIZHU_DOUDIZHU_H
