#include <bits/stdc++.h>
#include "configuration.h"
#include "utils.h"
using namespace std;

unordered_set<int> buys, sells;

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
    queue<string> q;
    while (true) {
        while (true) {
            try {
                string line = conn.read_from_exchange();
                while (!q.empty()) {
                    conn.send_to_exchange(q.front());
                    q.pop();
                }
                vector<string> tokens = split(line, ' ');
                if (!startsWith(tokens[0], {"BOOK", "TRADE"})) {
                    cout << line << endl;
                    if (startsWith(tokens[0], {"FILL"})) {
                        int id = stoi(tokens[1]);
                        int sz = stoi(tokens[5]);
                        if (buys.count(id)) {
                            q.push(buyBond(currentId, 999, sz));
                            buys.insert(currentId++);
                        } else {
                            q.push(sellBond(currentId, 1001, sz));
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
                    }
                }
            } catch (runtime_error &e) {
                cout << "CRASH" << endl;
                clock_t time_end;
                time_end = clock() + 100 * CLOCKS_PER_SEC / 1000;
                while (clock() < time_end);
            }
        }
    }
    return 0;
}
