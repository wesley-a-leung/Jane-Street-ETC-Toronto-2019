#include <bits/stdc++.h>
#include "configuration.h"
#include "utils.h"
using namespace std;

unordered_set<int> buys, sells;
unordered_map<int, string> pending;
unordered_map<string, int> buyPrice, sellPrice;

int main(int argc, char *argv[]) {
    assert(argc <= 3);
    // Be very careful with this boolean! It switches between test and prod
    bool test_mode = argc <= 1 || stoi(argv[1]);
    int test_exchange_index = argc <= 2 ? 2 : stoi(argv[2]);
    Configuration config(test_mode, test_exchange_index);
    Connection conn(config);

    conn.send_to_exchange(string("HELLO ") + config.team_name);
    cout << conn.read_from_exchange() << endl;

    int currentId = 1;
    pair<int, int> lastVAL = make_pair(-1, -1);
    queue<int> q;
    for (int cnt = 0; ; cnt++) {
        try {
            string line = conn.read_from_exchange();
            while (!q.empty()) {
                conn.send_to_exchange(pending[q.front()]);
                q.pop();
            }
            vector<string> tokens = split(line, ' ');
            if (tokens[0] != "BOOK" && tokens[0] != "TRADE") {
                if (tokens[0] != "TRADE") cout << line << endl;
                if (tokens[0] == "FILL") {
                    int id = stoi(tokens[1]);
                    int sz = stoi(tokens[5]);
                    string stock = tokens[2];
                    if (stock == "BOND") {
                        if (buys.count(id)) {
                            pending[currentId] = buyBond(currentId, 999, sz);
                            q.push(currentId);
                            buys.insert(currentId++);
                        } else {
                            pending[currentId] = sellBond(currentId, 1001, sz);
                            q.push(currentId);
                            sells.insert(currentId++);
                        }
                    }
                } else if (tokens[0] == "OUT") {
                    int id = stoi(tokens[1]);
                    buys.erase(id);
                    sells.erase(id);
                    if (id == lastVAL.first) lastVAL.first = -1;
                    if (id == lastVAL.second) lastVAL.second = -1;
                } else if (tokens[0] == "OPEN") {
                    conn.send_to_exchange(buyBond(currentId, 999, 100));
                    buys.insert(currentId++);
                    conn.send_to_exchange(sellBond(currentId, 1001, 100));
                    sells.insert(currentId++);
                } else if (tokens[0] == "CLOSE") {
                    return 0;
                } else if (tokens[0] == "FILL") {
                    int id = stoi(tokens[1]);
                    pending.erase(id);                    
                } else if (tokens[0] == "REJECT") {
                    int id = stoi(tokens[1]);
                    q.push(id);
                }
            } else if (tokens[0] == "BOOK") {
                pair<string, int> bp = getBuyPrice(line);
                pair<string, int> sp = getSellPrice(line);
                buyPrice[bp.first] = bp.second;
                sellPrice[sp.first] = sp.second;
            }
            if (sellPrice["VALE"] < buyPrice["VALBZ"]) {
                if (lastVAL == make_pair(-1, -1)) {
                    pending[currentId] = buyStockConvert(currentId, sellPrice["VALE"], 10, "VALE");
                    lastVAL.first = currentId;
                    q.push(currentId++);
                    pending[currentId] = sellStockConvert(currentId, buyPrice["VALBZ"], 10, "VALBZ");
                    lastVAL.second = currentId;
                    q.push(currentId++);
                } else {
                    cancel(lastVAL.first);
                    cancel(lastVAL.second);
                }
            }
        } catch (runtime_error &e) {
            cout << "CRASH" << endl;
            return 0;
        }
    }
    return 0;
}
