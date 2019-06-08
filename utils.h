#include <bits/stdc++.h>

std::vector<std::string> split(const std::string &s, const char &delim) {
    std::vector<std::string> tokens;
    std::string cur;
    for (char c : s) {
        if (c == delim) { tokens.push_back(cur); cur.clear(); }
        else cur.push_back(c);
    }
    if (!cur.empty()) tokens.push_back(cur);
    return tokens;
}

bool startsWith(const std::string &test, const std::vector<std::string> &check) {
    for (auto &&s : check) if (test.substr(0, int(s.size())) == s) return true;
    return false;
}

std::string buyBond(int id, int price, int size) {
    std::string ret = std::string("ADD ") + std::to_string(id) + " BOND BUY " + std::to_string(price) + std::string(" ") + std::to_string(size);
    std::cout << ret << std::endl;
    return ret;
}

std::string sellBond(int id, int price, int size) {
    std::string ret = std::string("ADD ") + std::to_string(id) + " BOND SELL " + std::to_string(price) + std::string(" ") + std::to_string(size);
    std::cout << ret << std::endl;
    return ret;
}


std::string buyStock(int id, int price, int size, std::string stock) {
    std::string ret = "ADD " + std::to_string(id) + " " + stock + " BUY " + std::to_string(price) + std::string(" ") + std::to_string(size);
    std::cout << ret << std::endl;
    return ret;
}

std::string sellStock(int id, int price, int size, std::string stock) {
    std::string ret = "ADD " + std::to_string(id) + " " + stock + " SELL " + std::to_string(price) + std::string(" ") + std::to_string(size);
    std::cout << ret << std::endl;
    return ret;
}

std::string buyConvert(int id, int size, std::string stock) {
    std::string ret = "CONVERT " + std::to_string(id) + " " + stock + " BUY " + std::to_string(size);
    std::cout << ret << std::endl;
    return ret;
}

std::string sellConvert(int id, int size, std::string stock) {
    std::string ret = "CONVERT " + std::to_string(id) + " " + stock + " SELL " + std::to_string(size);
    std::cout << ret << std::endl;
    return ret;
}

std::string cancel(int id) {
    std::cout << "Cancelling:  " << id << std::endl;
    std::string ret = "CANCEL " + std::to_string(id);
    return ret;
}

std::pair<std::string, int> getBuyPrice(std::string line) {
    std::vector<std::string> tokens = split(line, ' ');
    std::string stock = tokens[1];
    bool isBuy = false;
    int price = INT_MIN;
    for (int i = 2; i < int(tokens.size()); i++) {
        if (tokens[i] == "BUY") isBuy = true;
        else if (tokens[i] == "SELL") isBuy = false;
        else if (isBuy) price = std::max(price, std::stoi(split(tokens[i], ':')[1]));
    }
    return std::make_pair(stock, price);
}

std::pair<std::string, int> getSellPrice(std::string line) {
    std::vector<std::string> tokens = split(line, ' ');
    std::string stock = tokens[1];
    bool isSell = false;
    int price = INT_MAX;
    for (int i = 2; i < int(tokens.size()); i++) {
        if (tokens[i] == "BUY") isSell = false;
        else if (tokens[i] == "SELL") isSell = true;
        else if (isSell) price = std::min(price, std::stoi(split(tokens[i], ':')[1]));
    }
    return std::make_pair(stock, price);
}
