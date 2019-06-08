#include <bits/stdc++.h>
#include "configuration.h"
using namespace std;

int main(int argc, char *argv[]) {
    assert(argc <= 3);
    // Be very careful with this boolean! It switches between test and prod
    bool test_mode = argc <= 1 || stoi(argv[1]);
    int test_exchange_index = argc <= 2 ? 2 : stoi(argv[2]);
    Configuration config(test_mode, test_exchange_index);
    Connection conn(config);

    conn.send_to_exchange(string("HELLO ") + config.team_name);
    cout << conn.read_from_exchange() << endl;
    conn.send_to_exchange(string("ADD 5 BOND BUY 1002 50"));
    cout << conn.read_from_exchange() << endl;
    // std::vector<std::string> data;
    // data.push_back(std::string("HELLO"));
    // data.push_back(config.team_name);
    
    //   A common mistake people make is to conn.send_to_exchange() > 1
    //   time for every conn.read_from_exchange() response.
    //   Since many write messages generate marketdata, this will cause an
    //   exponential explosion in pending messages. Please, don't do that!
    
    // conn.send_to_exchange(join(" ", data));
    // std::string line = conn.read_from_exchange();
    // std::cout << "The exchange replied: " << line << std::endl;
    return 0;
}
