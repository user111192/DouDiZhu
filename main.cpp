#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <cstdio>
#include <queue>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <random>
#include <stack>
#include <set>
#include <list>
#include "libs/constants.h"
#include "Player.h"
#include "libs/doudizhu.h"



using namespace std;
const int MAX = 1010;
const long long inf = (1ll << 60);
typedef long long ll;
int n;
string cards, under_cards;
Player players[4];
map<char, int> size_map {{'3',1},{'4',2},{'5',3},{'6',4},{'7',5},{'8',6},{'9',7},{'T',8},{'J',9},{'Q',10},{'K',11},
                         {'A', 12},{'2', 13}, {'X', 14}, {'D', 15}};

void sleep(double t) {
    clock_t start = clock();
    while (clock() < start+t*CLOCKS_PER_SEC);
}

void add_cards(char ch, int count=1) {
    assert(count >= 1);
    for (int i=1;i<=count;i++) {
        cards += ch;
    }
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

bool card_comp(char x, char y) {
    return size_map[x] < size_map[y];
}

bool card_comp_g(char x, char y) {
    return size_map[x] > size_map[y];
}

void load() {
    if (cards.size() != 54) {
        cards.clear();
        for (int i = 3; i <= 9; i++) {
            add_cards(i + '0', 4);
        }
        add_cards('T', 4);
        add_cards('J', 4);
        add_cards('Q', 4);
        add_cards('K', 4);
        add_cards('A', 4);
        add_cards('2', 4);
        add_cards('X');
        add_cards('D');
    }
    if (!BypassCardNumCheck)
        assert(cards.size() == 54);
    if (!DontShuffleCards) {
        random_device rd;
        default_random_engine e(rd());
        shuffle(cards.begin(), cards.end(), e);
    }

}

int prev_player(int x) {
    if (x==1) return 3;
    return x-1;
}
int next_player(int x) {
    if (x==3) return 1;
    return x+1;
}

bool can_hit(string my_card, string target) {
    if (target == "XD") return false;
    if (my_card == "XD") return true;
    if (GetCardType(target) == "tnt") {
        if (GetCardType(my_card) != "tnt")
            return false;
        if (card_comp_g(my_card[0], target[0])) return true;
        else return false;
    }
    if (GetCardType(my_card) == "tnt" && GetCardType(target) != "tnt") {
        return true;
    }
    if (GetCardType(my_card) != GetCardType(target)) {
        return false;
    }
    string c_type = GetCardType(target);
    if (c_type == "single" || c_type == "double" || c_type == "triple") {
        return card_comp_g(my_card[0], target[0]);
    }
    if (c_type == "3_with_1") {
        int tm = 0, tt = 0;
        if (my_card[0] == my_card[1]) tm = 1;
        else tm = 2;
        if (target[0] == target[1]) tt = 1;
        else tt = 2;
        char mx, my, tx, ty;
        if (tm==1) {
            mx = my_card[0];
            my = my_card[3];
        } else {
            mx = my_card[1];
            my = my_card[0];
        }
        if (tt==1) {
            tx = target[0];
            ty = target[3];
        } else {
            tx = target[1];
            ty = target[0];
        }
        if (card_comp_g(mx, tx)) return true;
        if (card_comp(mx, tx)) return false;
        return card_comp_g(my, ty);
    }
    if (c_type == "3_with_2") {
        int tm = 0, tt = 0;
        if (my_card[1] == my_card[2]) tm = 1;
        else tm = 2;
        if (target[1] == target[2]) tt = 1;
        else tt = 2;
        int mx, my, tx, ty;
        if (tm==1) {
            mx = my_card[0];
            my = my_card[3];
        } else {
            mx = my_card[2];
            my = my_card[0];
        }
        if (tt==1) {
            tx = target[0];
            ty = target[3];
        } else {
            tx = target[2];
            ty = target[0];
        }
        if (card_comp_g(mx, tx)) return true;
        if (card_comp(mx, tx)) return false;
        return card_comp_g(my, ty);
    }
    return true;
}

bool can_hit1(string my_cards, string target) {
    if (target == "XD") return false;
    // todo
    return true;
}

bool HasCards(string input, string my_cards) {
    int missing_count = 0;
    for (auto & c: input) {
        if (my_cards.find(c) == string::npos) {
            missing_count++;
            cout << "缺少牌 " << c << endl;
        } else {
            my_cards[my_cards.find(c)] = '?';
        }
    }
    return !missing_count;
}

string EraseCards(string input, const string & my_cards) {
    if (!HasCards(input, my_cards)) return "error";
    int missing_count = 0;
    string copy = my_cards;
    for (auto c: input) {
        copy.erase(copy.find(c),1);
    }
    return copy;
}

string GetCardType(string input) {
    if (input == "pass") return "pass";
    if (input == "XD") return "super_tnt";
    if (input.size() == 4 && input[0] == input[1] && input[1] == input[2] && input[2] == input[3]) return "tnt";
    if (input.size() == 1) return "single";
    if (input.size() == 2 && input[0] == input[1]) return "double";
    if (input.size() == 3 && input[0] == input[1] && input[1] == input[2]) return "triple";
    if (input.size() == 4) {
        bool p1 = input[1] == input[2] && input[2] == input[3];
        bool p2 = input[0] == input[1] && input[1] == input[2];
        if (p1 || p2) {
            return "3_with_1";
        }
    }
    if (input.size() == 5) {
        bool p1 = input[0] == input[1] && input[2] == input[3] && input[3] == input[4];
        bool p2 = input[0] == input[1] && input[1] == input[2] && input[3] == input[4];
        if (p1 || p2) {
            return "3_with_2";
        }
    }
    if (false) {
        // todo
        return "shun_zi";
    }
    if (false) {
        // todo
        return "lian_dui";
    }
    if (false) {
        // todo
        return "plane";
    }
    return "invalid";
}

bool CardIsValid(string input) {
    return GetCardType(input) != "invalid";
}


int main() {
    clear_screen();
    cout << "欢乐斗地主 v1.0" << endl;
    load();
    cout << "卡牌: " << cards << endl;
    cout << "====================" << endl;
    cout << "=      发牌        =" << endl;
    cout << "====================" << endl;
    under_cards = cards.substr(51);
    cards.erase(51,54);
    cout << "底牌: " << under_cards << endl;
    cout << "开始发牌！" << endl;
    sleep(1.25);
    for (int i=1;i<=3;i++) {
        clear_screen();
        cout << "====================" << endl;
        cout << "=      发牌        =" << endl;
        cout << "====================" << endl;

        cout << "发牌进度：玩家 " << i << "/3" << endl;
        sleep(0.3);
        players[i].set_cards(cards.substr((i-1)*17,17));
        assert(players[i].get_cards().size() == 17);
        for (int j=1;j<=17;j++) {
            printf("\r发牌进度: %d/17 当前手牌: %s",j,players[i].get_cards().substr(0,j).c_str());
            fflush(stdout);
            // cout << "发牌进度：" << j << "/17 当前手牌: " << players[i].get_cards().substr(0,j+1) << "\r" << flush;
            if (DontShuffleCards) {
                sleep(0.02);
            } else {
                sleep(0.25);
            }
        }
        cout << endl;
        players[i].sort_cards();
        cout << "发牌完成！" << "当前手牌: " << players[i].get_cards() << endl;
        sleep(2);

    }
    clear_screen();

    cout << "====================" << endl;
    cout << "=      叫地主      =" << endl;
    cout << "====================" << endl;

    int landlord_index = 0;
    int jdz[4]{};

    for (int i=1;i<=3;i++) {
        clear_screen();
        cout << "====================" << endl;
        cout << "=      叫地主      =" << endl;
        cout << "====================" << endl;

        if (i > 1) {
            cout << "玩家1: " << (jdz[1]?"叫地主":"不叫") << endl;
        } else {
            cout << endl;
        }
        if (i > 2) {
            cout << "玩家2: " << (jdz[2]?"叫地主":"不叫") << endl;
        } else {
            cout << endl;
        }
        cout << "当前：玩家 " << i << "/3" << endl;
        cout << "当前手牌: " << players[i].get_cards() << endl;
        if (!landlord_index) {
            char ch = '?';
            bool input_valid = false;
            while (!input_valid) {
                cout << "叫地主: y\t不叫: n" << endl;
                cout << "请输入: ";
                cin >> ch;
                cin.ignore(255, '\n');
                ch = tolower(ch);
                switch (ch) {
                    case 'y':
                        landlord_index = i;
                        jdz[i] = 1;
                    case 'n':
                        input_valid = true;
                        break;
                    case '?':
                        cout << "提示: \t叫地主: y\n不叫: n\t显示手牌: c" << endl;
                        break;
                    case 'c':
                        cout << "当前手牌: " << players[i].get_cards() << endl;
                        break;
                    default:
                        cout << "输入无效！" << endl;
                        break;
                }
            }
        } else {
            char ch = '?';
            bool input_valid = false;
            while (!input_valid) {
                cout << "抢地主: y\t不抢: n" << endl;
                cout << "请输入: ";
                cin >> ch;
                cin.ignore(255, '\n');
                ch = tolower(ch);
                switch (ch) {
                    case 'y':
                        landlord_index = i;
                        jdz[i] = 2;
                    case 'n':
                        input_valid = true;
                        break;
                    case '?':
                        cout << "提示: \t抢地主: y\n不抢: n\t显示手牌: c" << endl;
                        break;
                    case 'c':
                        cout << "当前手牌: " << players[i].get_cards() << endl;
                        break;
                    default:
                        cout << "输入无效！" << endl;
                        break;
                }
            }
        }
    }

    if (landlord_index == 0) {
        main();
        exit(0);
    }

    clear_screen();
    cout << "====================" << endl;
    cout << "=       出牌       =" << endl;
    cout << "====================" << endl;
    cout << "地主: 玩家" << landlord_index << endl;
    cout << "底牌: " << under_cards << endl;
    players[landlord_index].set_cards(players[landlord_index].get_cards() + under_cards);
    players[landlord_index].sort_cards();
    bool end = false;
    int player = landlord_index, winner = 0;
    string recent1, recent2;
    string rec1_t = "pass", rec2_t = "pass";
    while (!end) {
        clear_screen();
        cout << "====================" << endl;
        cout << "=       出牌       =" << endl;
        cout << "====================" << endl;
        cout << "轮到玩家" << player << "了" << endl;
        cout << "上家剩余:\t" << players[prev_player(player)].get_cards().size() << endl;
        cout << "下家剩余:\t" << players[next_player(player)].get_cards().size() << endl;
        cout << "你剩余:\t\t" << players[player].get_cards().size() << endl;
        players[player].sort_cards();
        cout << "当前手牌:\t" << players[player].get_cards() << endl;
        if ((recent1 == "pass" || recent1.empty()) && (recent2 == "pass" || recent2.empty())) {
            if (!recent2.empty()) {
                cout << "玩家" << prev_player(prev_player(player)) << "出了 " << recent2 << endl;
            } else {
                cout << endl;
            }
            if (!recent1.empty()) {
                cout << "玩家" << prev_player(player) << "出了 " << recent1 << endl;
            } else {
                cout << endl;
            }
            cout << "提示: \t\t出牌: 输入你要出的牌\n显示手牌: c" << endl;
            string input;
            bool valid = false;
            while (!valid) {
                cout << "请输入你要出的牌: ";
                cin >> input;
                if (input == "c") {
                    cout << "当前手牌:\t" << players[player].get_cards() << endl;
                } else if (input == "pass") {
                    cout << "当前不可以不出" << endl;
                } else {
                    sort(input.begin(), input.end(), card_comp);
                    if (HasCards(input, players[player].get_cards())) {
                        if (CardIsValid(input)) {
                            valid = true;
                        } else {
                            cout << "你输入的牌无效！" << endl;
                        }
                    } else {
                        cout << "你没有这些牌！" << endl;
                    }
                }
            }
            cout << "你出了: " << input << endl;
            recent2 = recent1;
            rec2_t = rec1_t;
            recent1 = input;
            rec1_t = GetCardType(input);
            if (input != "pass") {
                players[player].set_cards(EraseCards(input, players[player].get_cards()));
            }
            sleep(2);
        } else {
            if (!recent2.empty()) {
                cout << "玩家" << prev_player(prev_player(player)) << "出了 " << recent2 << endl;
            } else {
                cout << endl;
            }
            if (!recent1.empty()) {
                cout << "玩家" << prev_player(player) << "出了 " << recent1 << endl;
            } else {
                cout << endl;
            }
            if (can_hit1(players[player].get_cards(),recent1)) {
                cout << endl;
                cout << "提示: \t\t出牌: 输入你要出的牌\n不出: pass\t显示手牌: c" << endl;
                string input;
                bool valid = false;
                while (!valid) {
                    cout << "请输入你要出的牌: ";
                    cin >> input;
                    if (input == "c") {
                        cout << "当前手牌:\t" << players[player].get_cards() << endl;
                    } else if (input == "pass") {
                        cout << "你不出" << endl;
                        valid = true;
                    } else {
                        sort(input.begin(), input.end(), card_comp);
                        if (HasCards(input, players[player].get_cards())) {
                            if (CardIsValid(input)) {
                                if (can_hit(input, (recent1!="pass")?recent1:recent2)) {
                                    valid = true;
                                } else {
                                    cout << "你的牌没有别人的大! " << endl;
                                }
                            } else {
                                cout << "你输入的牌无效！" << endl;
                            }
                        } else {
                            cout << "你没有这些牌！" << endl;
                        }
                    }
                }
                cout << "你出了: " << input << endl;
                recent2 = recent1;
                rec2_t = rec1_t;
                recent1 = input;
                rec1_t = GetCardType(input);
                if (input != "pass") {
                    players[player].set_cards(EraseCards(input, players[player].get_cards()));
                }
                sleep(2);
            } else {
                cout << "你没有牌大于上家" << endl;
                cout << "不出: pass" << endl;
                recent2 = recent1;
                rec2_t = rec1_t;
                recent1 = "pass";
                rec1_t = "pass";
                sleep(3);
            }
        }
        if (players[player].get_cards().empty()) {
            // 如果打完了
            if (landlord_index == player) {
                winner = 1;
            } else {
                winner = 2;
            }
            end = true;
            break;
        }
        fflush(stdout);
        player = next_player(player);
    }
    clear_screen();
    cout << "====================" << endl;
    cout << "=       结果       =" << endl;
    cout << "====================" << endl;
    if (winner == 1) {
        cout << "结果: 地主获胜" << endl;
        cout << "获胜玩家: " << landlord_index << endl;
        cout << "失败玩家: " << prev_player(landlord_index) << "," << next_player(landlord_index) << endl;
    } else if (winner == 2) {
        cout << "结果: 农民获胜" << endl;
        cout << "获胜玩家: " << prev_player(landlord_index) << "," << next_player(landlord_index) << endl;
        cout << "失败玩家: " << landlord_index << endl;
    } else {
        assert(0);
        cout << "结果: 未知" << endl;
    }
    return 0;
}