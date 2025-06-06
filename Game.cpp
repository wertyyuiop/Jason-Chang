#include "Game.h"
#include "Deck.h"
#include "ScoreCalculator.h"

#include <bits/stdc++.h>
using namespace std;

Game::Game()
    : shop(data)    // shop 用同一個 json reference
{}

void Game::start() {
    // 1. 先從檔案載入 data
    ifstream fin("players.json");
    if (!fin.is_open()) {
        // 如果沒有檔案，就先塞一個空的 array
        data = json::array();
    } else {
        data = json::parse(fin);
    }
    fin.close();

    // 2. 將 JSON 中的玩家解到 players vector（只讀 name + money）
    loadPlayers();

    // 3. 進入主選單迴圈
    while (true) {
        showMainMenu();
        int choice;
        cin >> choice;
        if (choice < 0 || choice > 2) continue;
        if (choice == 0) {
            // 離開遊戲前要把所有修改後的 JSON 存回去
            writeData();
            cout << "遊戲結束，謝謝遊玩！\n";
            exit(0);
        }
        handleMainChoice(choice);
    }
}

void Game::showMainMenu() {
    system("clear");
    cout << "=== Poker Game 主選單 ===\n";
    cout << "1. 登入並開始遊戲\n";
    cout << "2. 查看排行榜\n";
    cout << "0. 結束遊戲\n";
    cout << "請輸入選項：";
}

void Game::handleMainChoice(int choice) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清掉殘留的換行
    if (choice == 1) {
        loginAndPlay();
    } else if (choice == 2) {
        showLeaderBoard();
        cout << "按下 Enter 返回。\n";
        cin.get();
    }
}

// 由 data 讀到 players vector 中（只保 name + money）
void Game::loadPlayers() {
    players.clear();
    for (auto &entry : data) {
        std::string nm = entry["name"];
        int m  = (entry.contains("money") ? (int)entry["money"] : 0);
        players.emplace_back(nm, m);
    }
}

// 把 data 寫回 players.json
void Game::writeData() {
    ofstream fout("players.json");
    fout << data.dump(2);
    fout.close();
}

// 找 JSON array 裡面 name 相同的 index，找不到回傳 -1
int Game::findPlayerIndex(const std::string &name) {
    for (int i = 0; i < (int)data.size(); i++) {
        if (data[i]["name"] == name) return i;
    }
    return -1;
}

// 玩家登入後：先進商店，再真正 playStage，玩完之後結算 + 存檔
void Game::loginAndPlay() {
    cout << "請輸入玩家名稱：";
    string name;
    cin >> name;

    // 1. 檢查 JSON 裡有沒有這個玩家
    int idx = findPlayerIndex(name);
    if (idx < 0) {
        // JSON 裡沒有，先塞一筆空白進去
        json newEntry = {
            {"name", name},
            {"maxScore", 0},
            {"money", 0},
            {"inventory", json::array()}
        };
        data.push_back(newEntry);
        idx = (int)data.size() - 1;
    }

    // 2. 用 JSON 裡面的 money 建構 Player
    int initialMoney = data[idx]["money"];
    Player p(name, initialMoney);
    // 把這個玩家目前持有的 inventory 也同步給 Player (假設 Player 有方法拿 inventory)
    if (data[idx].contains("inventory")) {
        for (auto &it : data[idx]["inventory"]) {
            p.addToInventory(it.get<std::string>());
        }
    }

    // 3. 強制先進商店
    shop.visit(p);

    // 4. 重置遊戲內部統計
    p.resetStats();

    // 5. 開始真正的 playStage
    playStage(p);

    // 6. 決算：先顯示一些結算資訊（score, play/discard 統計 ...）
    settlementStage(p);

    // 7. 換算貨幣： money_awarded = final_score / 10
    int finalScore   = p.score();
    int moneyAwarded = finalScore / 10;
    cout << "本局最終分數：" << finalScore << "，兌換遊戲幣：" << moneyAwarded << "\n";
    cout << "按 Enter 繼續...";
    string s; getline(cin, s);

    // 8. 更新 JSON 中這筆玩家的 money, maxScore, inventory
    //    a. money += moneyAwarded
    data[idx]["money"] = data[idx]["money"].get<int>() + moneyAwarded;
    //    b. inventory 同步回 JSON
    data[idx]["inventory"] = json::array();
    for (auto &it : p.getInventory()) {
        data[idx]["inventory"].push_back(it);
    }
    //    c. 如果本局分數 > maxScore，就更新
    if (finalScore > data[idx]["maxScore"].get<int>()) {
        data[idx]["maxScore"] = finalScore;
    }

    // 9. 把整張 JSON 寫回檔案
    writeData();

    // 10. 同步更新 players vector 中的 money（下一次登入才會看到）
    for (auto &pl : players) {
        if (pl.name() == name) {
            pl.setMoney(data[idx]["money"].get<int>());
            break;
        }
    }
}

// 顯示排行榜，依照 maxScore 排序
void Game::showLeaderBoard() {
    system("clear");
    cout << "=== 排行榜（依 maxScore 由高到低） ===\n";

    // 先把 (maxScore, name) 全蒐集到 vector<pair><>
    vector<pair<int, string>> v;
    for (auto &entry : data) {
        v.push_back({ entry["maxScore"].get<int>(), entry["name"].get<std::string>() });
    }
    // 排序
    sort(v.begin(), v.end(), [](auto &a, auto &b) {
        return a.first > b.first;
    });

    // 印出來
    for (int i = 0; i < (int)v.size(); i++) {
        cout << "Rank " << (i + 1) << "： " << v[i].second
             << "  (Score: " << v[i].first << ")\n";
    }
    cout << "===============================\n";
}

// 真正的遊戲流程（從發牌到出牌 / 棄牌 / 提示等）
// 以下程式邏輯跟你在之前的版本類似，只是把商店、登入切開在外面
void Game::playStage(Player &p) {
    Deck deck; 
    deck.shuffle();

    // 每局開始都拿 8 張
    p.resetStats();
    p.hand().clear();        // 假設 Hand 有 clear()，或者手動移除
    for (int i = 0; i < 8; i++) {
        p.hand().add(deck.draw());
    }

    int playRounds    = 0;
    int discardRounds = 0;
    string s;

    while (playRounds < 4 && !p.hand().getCards().empty()) {
        system("clear");
        cout << "============= 遊戲中 =============\n";
        cout << "玩家：" << p.name() << "\n";
        cout << "目前累計分數：" << p.score() << "\n";
        cout << "已出牌次數：" << p.play() << "  已棄牌次數：" << p.discard() << "\n";
        cout << "=================================\n\n";

        // 顯示手牌（前 8 張，分兩排印出 ASCII Art）
        auto &handCards = p.hand().getCards();
        if (handCards.size() < 8) {
            // 通常 play 階段結束後不會進到這裡，但保險起見：
            while (handCards.size() < 8 && !deck.empty()) {
                p.hand().add(deck.draw());
            }
        }

        // 印兩排共 8 張
        {
            // 先把 0~3 放 top4，4~7 放 bot4
            vector<Card> top4(handCards.begin(), handCards.begin() + 4);
            vector<Card> bot4(handCards.begin() + 4, handCards.begin() + 8);

            vector<vector<string>> topArts, botArts;
            for (int i = 0; i < 4; i++) {
                topArts.push_back(top4[i].getAsciiArt());
                botArts.push_back(bot4[i].getAsciiArt());
            }
            int W = (int)topArts[0][0].length();

            // 印第一排
            for (int row = 0; row < 6; row++) {
                for (int c = 0; c < 4; c++) {
                    cout << topArts[c][row] << "  ";
                }
                cout << "\n";
            }
            // 標示索引 (0~3)
            for (int c = 0; c < 4; c++) {
                string idxStr = "(" + to_string(c) + ")";
                int padL = (W - (int)idxStr.length()) / 2;
                int padR = W - (int)idxStr.length() - padL;
                cout << string(padL - 5, ' ') << idxStr << string(padR - 5, ' ') << "  ";
            }
            cout << "\n\n";

            // 第二排
            for (int row = 0; row < 6; row++) {
                for (int c = 0; c < 4; c++) {
                    cout << botArts[c][row] << "  ";
                }
                cout << "\n";
            }
            // 標示索引 (4~7)
            for (int c = 0; c < 4; c++) {
                int idx = 4 + c;
                string idxStr = "(" + to_string(idx) + ")";
                int padL = (W - (int)idxStr.length()) / 2;
                int padR = W - (int)idxStr.length() - padL;
                cout << string(padL - 5, ' ') << idxStr << string(padR - 5, ' ') << "  ";
            }
            cout << "\n\n";
        }

        // 本輪指令選單
        cout << "(1) 出牌  (2) 棄牌  (3) 排序  (4) 結束回合  (5) 提示\n";
        cout << "請輸入：";
        int cmd; 
        cin >> cmd;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (cmd == 1 && playRounds < 4) {
            // 出牌：一次可以出任意張數 (1~5)，重複執行挑牌並移出
            cout << "輸入要出的牌索引（0-based，用空白分開）：";
            getline(cin, s);
            stringstream ss(s);
            vector<int> chosenIdx;
            int idxCard;
            while (ss >> idxCard) {
                chosenIdx.push_back(idxCard);
            }
            // 從後往前移除
            sort(chosenIdx.begin(), chosenIdx.end(), greater<int>());
            vector<Card> playSet;
            for (int id : chosenIdx) {
                playSet.push_back(p.hand().getCards()[id]);
                p.hand().removeAt(id);
            }
            // 計算分數
            string   comboName = ScoreCalculator::check(playSet);
            int      sc        = ScoreCalculator::calculate(playSet);
            cout << "牌型： " << comboName << "，得分： " << sc << "\n";
            p.recordPlay();
            p.recordScore(sc);
            p.recordPlayType(playSet);

            // 補牌：把手上補到 8 張（若 deck 裡沒就忽略）
            for (int k = 0; k < (int)chosenIdx.size(); k++) {
                if (!deck.empty()) 
                    p.hand().add(deck.draw());
            }

            playRounds++;
            cout << "按 Enter 繼續。\n";
            cin.get();

        } else if (cmd == 2) {
            // 棄牌：最多 3 次，每次可以丟任意數量張數
            if (discardRounds >= 3) {
                cout << "您已經棄牌 3 次，無法再棄牌！按 Enter 繼續。\n";
                cin.get();
                continue;
            }
            cout << "輸入要棄的牌索引（0-based，用空白分開）：";
            getline(cin, s);
            stringstream ss(s);
            vector<int> toDiscard;
            int idxCard;
            while (ss >> idxCard) {
                toDiscard.push_back(idxCard);
            }
            sort(toDiscard.begin(), toDiscard.end(), greater<int>());
            for (int id : toDiscard) {
                p.hand().removeAt(id);
            }
            // 棄掉多少張，就從 deck 補回多少張
            for (int k = 0; k < (int)toDiscard.size(); k++) {
                if (!deck.empty())
                    p.hand().add(deck.draw());
            }
            p.recordDiscard();
            discardRounds++;

        } else if (cmd == 3) {
            // 排序手牌：1=依點數，2=依花色
            cout << "(1) 依點數排序  (2) 依花色排序：";
            int m; 
            cin >> m;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (m == 1) p.hand().sortByValue();
            else         p.hand().sortBySuit();

        } else if (cmd == 5) {
            // 提示：嘗試所有 1~5 張組合，算出最高分的那一組
            vector<Card> &hand = p.hand().getCards();
            int bestScore = 0;
            vector<Card> bestCombo;
            int n = (int)hand.size();
            for (int k = 1; k <= min(5, n); k++) {
                vector<bool> sel(n, false);
                fill(sel.begin(), sel.begin() + k, true);
                do {
                    vector<Card> cand;
                    for (int i = 0; i < n; i++) {
                        if (sel[i]) cand.push_back(hand[i]);
                    }
                    int sc = ScoreCalculator::calculate(cand);
                    if (sc > bestScore) {
                        bestScore = sc;
                        bestCombo = cand;
                    }
                } while (prev_permutation(sel.begin(), sel.end()));
            }
            cout << "=== 本局提示 ===\n";
            if (bestCombo.empty()) {
                cout << "無法組成有分數的組合！\n";
            } else {
                cout << "推薦出牌： ";
                for (auto &c : bestCombo) cout << c.toString() << " ";
                cout << "\n對應牌型： " << ScoreCalculator::check(bestCombo)
                     << "，預估得分： " << bestScore << "\n";
            }
            cout << "按 Enter 繼續。\n";
            cin.get();
        } else {
            // cmd == 4 或者 其它都跳出本局
            break;
        }
    }
}

// 遊戲結束後的結算畫面，只負責印出 Player 裡自己記錄的統計
void Game::settlementStage(Player &p) {
    system("clear");
    p.printStats();
    cout << "按 Enter 繼續。\n";
}

// 不管有沒有牌，用到時請確保 Deck 有 empty() / draw()
