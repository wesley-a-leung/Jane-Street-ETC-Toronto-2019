#include <bits/stdc++.h>
#include "configuration.h"
#include "utils.h"
#include "parser.h"
using namespace std;

unordered_set<int> buys, sells;
unordered_map<int, string> pending;
double tol = 0.005;

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
    queue<int> q;

    unordered_map<string, int> stockToSellID({
			{"GS", 0},
			{"MS", 0},
			{"WFC", 0}
  	});
    unordered_map<string, int> stockToBuyID({
			{"GS", 0},
			{"MS", 0},
			{"WFC", 0}
  	});
    
    for (int cnt = 0; ; cnt++) {
        try {
            string line = conn.read_from_exchange();
            while (!q.empty()) {
                conn.send_to_exchange(pending[q.front()]);
                q.pop();
            }
            vector<string> tokens = split(line, ' ');
            if (!startsWith(tokens[0], {"BOOK", "TRADE"})) {
                if (!startsWith(tokens[0], {"REJECT"})) cout << line << endl;
                if (startsWith(tokens[0], {"FILL"})) {
                    int id = stoi(tokens[1]);
                    int sz = stoi(tokens[5]);
                    if (buys.count(id)) {
                        pending[currentId] = buyBond(currentId, 999, sz);
                        q.push(currentId);
                        buys.insert(currentId++);
                    } else if (sells.count(id)) {
                        pending[currentId] = sellBond(currentId, 1001, sz);
                        q.push(currentId);
                        sells.insert(currentId++);
                    }
                } else if (startsWith(tokens[0], {"OUT"})) {
                    int id = stoi(tokens[1]);
                    buys.erase(id);
                    sells.erase(id);
                } else if (startsWith(tokens[0], {"OPEN"})) {
                    conn.send_to_exchange(buyBond(currentId, 999, 100));
                    buys.insert(currentId++);
                    conn.send_to_exchange(sellBond(currentId, 1001, 100));
                    sells.insert(currentId++);
                } else if (startsWith(tokens[0], {"CLOSE"})) {
                    cout << "CLOSE" << endl;
                    return 0;
                } else if (startsWith(tokens[0], {"ACK"})) {
                    int id = stoi(tokens[1]);
                    pending.erase(id);                    
                } else if (startsWith(tokens[0], {"REJECT"})) {
                    int id = stoi(tokens[1]);
                    q.push(id);
                }
            } else if (startsWith(tokens[0], {"BOOK", "TRADE"})){
                cout << "On the line" << line << endl;
                parse(line);
                int fairval = tradeETF(0, 0);
                cout << "current fair value " << fairval << endl;
                if (fairval != -1 && (tokens[1] == "XLF")) {
                    cout << "stockToSellID" << stockToSellID[tokens[1]] << endl;
                    if (stockToSellID[tokens[1]]) {
                        conn.send_to_exchange(cancel(stockToSellID[tokens[1]]));
                    }
                    conn.send_to_exchange(sellStock(currentId, (int) fairval * (1 + tol), 5, tokens[1]));
                    stockToSellID[tokens[1]] = currentId;
                    currentId++;

                    cout << "stockToBuyID" << stockToBuyID[tokens[1]] << endl;
                    if (stockToBuyID[tokens[1]]) {
                        conn.send_to_exchange(cancel(stockToBuyID[tokens[1]]));
                    }
                    conn.send_to_exchange(buyStock(currentId, (int) fairval * (1 - tol), 5, tokens[1]));
                    stockToBuyID[tokens[1]] = currentId;
                    currentId++;
                }
            }

        } catch (runtime_error &e) {
            cout << "CRASH" << endl;
            return 0;
        }
    }
    return 0;
}
