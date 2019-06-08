#include <bits/stdc++.h>
#include "configuration.h"
using namespace std;

bool startsWith(const string &test, const vector<string> &check) {
    for (auto &&s : check) if (test.substr(0, int(s.size())) == s) return true;
    return false;
}

int main(int argc, char *argv[]) {
    assert(argc <= 3);
    // Be very careful with this boolean! It switches between test and prod
    bool test_mode = argc <= 1 || stoi(argv[1]);
    int test_exchange_index = argc <= 2 ? 2 : stoi(argv[2]);
    Configuration config(test_mode, test_exchange_index);
    Connection conn(config);

    conn.send_to_exchange(string("HELLO ") + config.team_name);
    cout << conn.read_from_exchange() << endl;
    conn.send_to_exchange(string("ADD 1 BOND BUY 999 100"));
    while (true) {
        string line = conn.read_from_exchange();
        if (!startsWith(line, {"BOOK", "TRADE", "OPEN", "CLOSE"})) {
            cout << line << endl;
            break;
        }
    }
    conn.send_to_exchange(string("ADD 2 BOND SELL 1001 100"));
    while (true) {
        string line = conn.read_from_exchange();
        if (!startsWith(line, {"BOOK", "TRADE", "OPEN", "CLOSE"})) {
            cout << line << endl;
            break;
        }
    }
    while (true) {
        try {
            string line = conn.read_from_exchange();
            if (!startsWith(line, {"BOOK", "TRADE", "OPEN", "CLOSE"})) {
                cout << line << endl;
            }
        } catch (runtime_error &e) {
            assert(false, "Exchange Crashed");
            return 0;
        }
    }
    return 0;
}
