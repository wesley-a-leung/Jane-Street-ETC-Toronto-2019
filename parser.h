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
		tbuy+= (it->second)*(it->first); nbuy +=it->second;
	}
	for(auto it=sell.begin(); it!=sell.end(); ++it) {
		tsell+= (it->second)*(it->first); nsell +=it->second;
	}
	// cout<<"tbuy: "<<tbuy<<" nbuy: "<<nbuy<<"tsell: "<<tsell<<" nsell: "<<nsell<<"\n";
	return {{tbuy, nbuy}, {tsell, nsell}};
}

int fairvaluebook(string s){
	pair<pair<int, int>, pair<int, int> > tpp = getsums(s);
	pair<int, int> buy = tpp.first;
	pair<int, int> sell = tpp.second;
	if (buy.first== 0 || buy.first == 0 || buy.second == 0 || sell.second == 0) return -1;
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
	// cout<<"fairvaltt: "<<tt<<" fairval ttl"<<ttl<<'\n';
	if(tt<50){ return fairvaluebook(s); }
	else{return ttl/((double) tt);}
}

int fairvalue(string s) {
	return fairvaluetrades(s);
}
vector<pair<int, int> > vBuy(int fairval, string s, int nums) {
	vector<pair<int, int> > ords;
	for(auto it = (books[s].first).rbegin(); it!=(books[s].first).rend(); ++it) {
		if(it->first>fairval) {
			if(it->second > nums) {ords.push_back(make_pair(it->first, nums)); break;}
			else{ords.push_back(make_pair(it->first, it->second)); nums-=it->second;}
		}
		else{
			break;
		}
	}
	return ords;
}

vector<pair<int, int> > vSell(int fairval, string s, int num) {
	vector<pair<int, int> > ords;
	for(auto it = (books[s].second).begin(); it!=(books[s].second).end(); ++it) {
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
int botm(string s, int num) {
	map<int, int> buy =  books[s].first;
	long long int tt = 0;
	for(auto it = buy.rbegin(); it!=buy.rend(); ++it){
		if(num<it->second){
			num-=it->second;
			tt+= (it->first)*(it->second);
		}
		else{
			tt+=(it->first)*(num);
			num = 0;
		}
	}
	if(num>0){return -1;}
	return tt;
}
int topm(string s, int num) {
	map<int, int> sell =  books[s].second;
	long long int tt = 0;
	for(auto it = sell.begin(); it!=sell.end(); ++it){
		if(num<it->second){
			num-=it->second;
			tt+= (it->first)*(it->second);
		}
		else{
			tt+=(it->first)*(num);
			num = 0;
		}
	}
	if(num>0){return -1;}
	return tt;
}
//3 bond
//2 gs
//3 ms
//2 wfc 
int gETCb(int num) {
	int bnd = botm("BOND", 3*num);
	if(bnd<0){ return -1;}
	int gs = botm("GS", 2*num);
	if(gs<0){ return -1; }
	int ms = botm("MS", 3*num);
	if(ms<0) {return -1; }
	int wtc = botm("WFC", 2*num);
	if(wtc<0){return -1; }
	return bnd+gs+ms+wtc;
}
int gETCt(int num) {
	int bnd = topm("BOND", 3*num);
	if(bnd<0){ return -1;}
	int gs = topm("GS", 2*num);
	if(gs<0){ return -1; }
	int ms = topm("MS", 3*num);
	if(ms<0) {return -1; }
	int wtc = topm("WFC", 2*num);
	if(wtc<0){return -1; }
	return bnd+gs+ms+wtc;
}
vector<pair<string, vector< pair<int, int> > > > gvals(int vl, int num) {
	vector<pair<string, pair<int, int> > > tmp;
	if(vl==-1) {
		tmp.push_back({"BOND", vSell(99999999, "BOND", 3*num)});
		tmp.push_back({"GS", vSell(99999999, "GS", 2*num)});
		tmp.push_back({"MS", vSell(99999999, "MS", 3*num)});
		tmp.push_back({"WFC", vSell(99999999, "WFC", 2*num)});
		tmp.push_back({"XLF", vBuy(0, "XLF", 10*num)});
	}
	else if(vl==1) {
		tmp.push_back({"BOND", vBuy(0, "BOND", 3*num)});
		tmp.push_back({"GS", vBuy(0, "GS", 2*num)});
		tmp.push_back({"MS", vBuy(0, "MS", 3*num)});
		tmp.push_back({"WFC",vBuy(0, "WFC", 2*num)});
		tmp.push_back({"XLF", vSell(99999999, "XLF", 10*num)});
	}
	return tmp;

}
int tradeETF(int thresh, int num) {
	int st = 1, wfb = gETCb(num), wft = gETCt(num), ETFb = botm("XLF", 10*num), ETFt = topm("XLF", 10*num);
	if(!(wfb==-1 || ETFt ==-1)){
		if(wfb-ETFt > thresh){ return 1; } //we can sell wf for more than we can buy etf -> buy etf, sell wf
	}
	else if((!(ETFb==-1 || wft==-1)) &&(ETFb-wft > thresh)) { return -1;} //we can sell etf for more than we can buy wf ->buy wf, sell etf
	else{return 0; } //it doesn't meet the threshold
}




int fairvalueV() {
	int fairval = fairvaluebook("VALEBZ");
	return fairval;

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
    		// cout<<results[i]<<'\n';
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