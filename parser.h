#include <bits/stdc++.h>
using namespace std;

map<string, pair<map<int, int>, map<int, int> > > books;
map<string, vector<pair<int, int> > > trades;
map<string, bool> open;


int getvl(string s, int ind){
	int a = 0;
	for(int i = ind; i<s.size(); i++){
		if(s[i]<'0' || s[i]>'9') return a;
		else{ a = a*10 + s[i]-'0';}
	}
	return a;
}

pair<pair<int, int>, pair<int, int> > getsums(string s){ 
	map<int, int> buy =  books[s].first;
	map<int, int> sell =  books[s].second;
	long long int tsell = 0, tbuy = 0;
	int nsell = 0, nbuy = 0;
	for(auto it=buy.begin(); it!=buy.end(); ++it) {
		tbuy+= (it->second)*(it->first); nsell +=it->second;
	}
	for(auto it=sell.begin(); it!=sell.end(); ++it) {
		tsell+= (it->second)*(it->first); nsell +=it->second;
	}
	return {{tbuy, nbuy}, {tsell, nsell}};
}

int fairvaluebook(string s){
	pair<pair<int, int>, pair<int, int> > tpp = getsum(s);
	pair<int, int> buy = tpp.first;
	pair<int, int> sell = tpp.second;
	int fairval = ((buy.first)/((double) buy.second) + (sell.first)/((double) sell.second))/2;
	return fairval;
}

int fairvaluetrades(string s){
	int tt = 0;
	long long int ttl = 0;
	vector<pair<int, int> > tv = trades[s];
	for(int i = 0; i<tv.size(); i++){
		if(tt+tv[i].second>50){
			ttl+=tv[i].first*(50-tt);
			tt = 50;
			break;
		}
		else{
			ttl+=tv[i].first*tv[i].second;
			tt+=tv[i].second;
		}
		
	}
	if(tt<50){ return fairvaluebook(s); }
	else{return ttl/(double tt);}
}

int fairvalue(string s) {
	return fairvaluetrades(s);
}
vector<pair<int, int> > vBuy(int fairval, string s, int num) {
	vector<pair<int, int> > ords;
	for(auto it = (books[s].first).rbegin(); it!=(books[s].first).rend(); ++it) {
		if(it->first>fairval) {
			if(it->second > num) {ords.push_back({it->first, num}); break;}
			else{ords.push_back({it->first, it->second}); nums-=it->second;}
		}
		else{
			break;
		}
	}
	return ords;
}

vector<pair<int, int> > vSell(int fairval, string s, int num) {
	vector<pair<int, int> > ords;
	for(auto it = (books[s].first).begin(); it!=(books[s].first).end(); ++it) {
		if(it->first<fairval) {
			if(it->second > num) {ords.push_back({it->first, num}); break;}
			else{ords.push_back({it->first, it->second}); num-=it->second;}
		}
		else{
			break;
		}
	}
	return ords;
}

int fairvalueV() {
	int fairval = fairvaluebook("VALEBZ");

}
/*vector<string> getVOrders(int id) {
	int tmpbz = fairvaluebook("VALBZ"), tmpvl = fairvaluebook("VALE");
	if(tmpbz>tmpvl) {


	}

}*/
pair<int, int> addp(string vl) {
	for(int i = 0; i<vl.size(); i++){
		if(vl[i]==':') {
			return {getvl(vl, 0), getvl(vl, i+1)};
		}
	}
	return {0, 0};
}

void buy(vector<string> vect, string s, int ind) {
	for(int i = ind; i<vect.size(); i++){
		if(vect[i]=="SELL") {
			return;
		}
		else{
			pair<int, int> tmp = addp(vect[i]);
			(books[s].first)[tmp.first]+=tmp.second;
			//cout<<"BUY: "<<addp(vect[i]).first<<" "<<addp(vect[i]).second<<'\n';
		}
	}
}

void sell(vector<string> vect, string s, int ind) {
	for(int i = ind; i<vect.size(); i++){
		pair<int, int> tmp = addp(vect[i]);
		(books[s].second)[tmp.first]+=tmp.second;
		//cout<<addp(vect[i]).first<<" "<<addp(vect[i]).second<<'\n';
	}
}

void mkopen(vector<string> vect, int ind, bool vl) {
	for(int i = ind; i<vect.size(); i++) {
		open[vect[i]] = vl;
	}
}

void parse(string s){
 	istringstream iss(s);
	vector<string> results((istream_iterator<string>(iss)),
                                 istream_iterator<string>());
    if(results[0]=="BOOK"){
    	//cout<<results.size();
    	((books[results[1]]).first).clear();
    	((books[results[1]]).second).clear();
    	for(int i = 2; i<results.size(); i++){
    		cout<<results[i]<<'\n';
    		if(results[i]=="BUY") {
    			buy(results, results[1], i+1);

    		}
    		else if(results[i]=="SELL") {
    			sell(results, results[1], i+1);
    		}
    	}
    }

    if(results[0]=="TRADE") {
    	trades[results[1]].push_back({getvl(results[2], 0), getvl(results[3], 0)});
    }
    if(results[0]=="OPEN") {
    	mkopen(results, 1, 1);
    }
    if(results[0]=="CLOSE") {
    	mkopen(results, 1, 0);
    }
    if(results[0]=="FILL") {

    }


    return;
}

/*if(valbz[1]<vale[0]) {
	buy valbz, sell vale
}
if(vale[1]<valbz[0]) {
	buy vale, sell valebz
}*/
// int main() {
// 	string curr;
// 	getline(cin, curr);
// 	parse(curr);
// 	getline(cin, curr);
// 	parse(curr);
// 	cout<<"dn\n";
// 	for(auto it = books.begin(); it!=books.end(); ++it){
// 		cout<<it->first<<'\n';
// 		for(auto it2 = ((it->second).first).begin(); it2!=((it->second).first).end(); ++it2){
// 			cout<<it2->first<<" "<<it2->second<<'\n';
// 		}
// 		cout<<"SELL\n";
// 		for(auto it2 = ((it->second).second).begin(); it2!=((it->second).second).end(); ++it2){
// 			cout<<it2->first<<" "<<it2->second<<'\n';
// 		}
// 	}
// 	return 0;
// }