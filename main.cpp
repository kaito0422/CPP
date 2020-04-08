#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include <iterator>
#include <time.h>

#define MAX_THREAD_NUM		64
#define MAX_ROUTE_LENGTH	7
#define MIN_ROUTE_LENGTH	2

typedef unsigned int IdNum;

struct User {
	IdNum id;
	std::list<IdNum> outHist;
	int inNum = 0;
	int outNum = 0;
};

struct Node {
	IdNum id;
	Node *front;
	int level;
};

unsigned int userNum = 0;
int thread_num = 0;
int processed_num = 0;
std::map<IdNum, User> user_map;
std::set<IdNum> redundent_user;
std::map<int, std::list<std::list<IdNum> > > out_res;
std::list<std::list<IdNum> > out_list;

void dfs(IdNum uid, std::list<int> &ulist)
{
//	std::cout << uid << ' ' << ulist.size() << std::endl;
	//if (uid == 24240, 18787) {
	//	int size = out_list.size();
	//	std::cout << size << std::endl;
	//}

	User user = user_map[uid];
	std::list<IdNum> &hist = user.outHist;

	/* 遍历当前uid的所有outHist */
	for (std::list<IdNum>::iterator itor = hist.begin(); itor != hist.end(); ++itor) {
		IdNum id = *itor;

		/* 如果一个outHist里面的id为冗余的，则不参与搜索（这里的if条件为非冗余） */
		if (redundent_user.find(id) == redundent_user.end()) {
			std::list<int>::iterator reuser_itor;
			reuser_itor = ulist.end();
			bool isFound = false;
			/* 查找最近的2~7的历史中是否有该id */
			for (int i = 0; i < MAX_ROUTE_LENGTH && reuser_itor != ulist.begin(); i++) {
				reuser_itor--;
				// 找到重复的id
				if (*reuser_itor == id && i >= MIN_ROUTE_LENGTH) {
					isFound = true;
					std::list<IdNum> tmp_list;
					for (; reuser_itor != ulist.end(); ++reuser_itor)
						tmp_list.push_back(*reuser_itor);
					out_list.push_back(tmp_list);
					break;
				}
			}
			// 没有找到重复的
			if (!isFound) {
				if (user_map.find(id) != user_map.end() && ulist.size() < MAX_ROUTE_LENGTH) {
					ulist.push_back(id);
					dfs(id, ulist);
					ulist.pop_back();
				}
			}
		}
	}
}

int main()
{
	clock_t start_t, end_t;

	start_t = clock();
	/* 处理输入文件 */
	std::ifstream inputFile("test_data.txt");
	std::string line;
	while (std::getline(inputFile, line)) {
		/* 读文件必须执行，不能改变 */
		std::istringstream ss(line);
		IdNum localId, peerId;
		unsigned int amount;
		char ch;
		ss >> localId >> ch >> peerId >> ch >> amount;

		user_map[localId].id = localId;
		user_map[peerId].inNum++;
		user_map[localId].outNum++;
		user_map[localId].outHist.push_back(peerId);
	}
	end_t = clock();
	std::cout << "read file time: " << (end_t - start_t) / CLOCKS_PER_SEC << std::endl;

	start_t = clock();
	/* 除去那些只有转入或者只有转出的账户信息 */
	std::ofstream outputFile("kaito_stat.txt");
	int max_out_user = -1, max_out_num = -1, max_in_user = -1, max_in_num = -1;
	for (std::map<IdNum, User>::iterator u = user_map.begin(); u != user_map.end();) {
		outputFile << u->first << ": " << u->second.outNum << std::endl;
		if (max_out_num < u->second.outNum) {
			max_out_user = u->first;
			max_out_num = u->second.outNum;
		}
		if (max_in_num < u->second.inNum) {
			max_in_user = u->first;
			max_in_num = u->second.inNum;
		}
		if (u->second.inNum == 0 || u->second.outNum == 0) {
			redundent_user.insert(u->first);
			user_map.erase(u++);
		}
		else {
			++u;
		}
	}
	outputFile << "max_out_user: " << max_out_user << "\t max_out_num: " << max_out_num << std::endl;
	outputFile << "max_in_user: " << max_in_user << "\t max_in_num: " << max_in_num << std::endl;
	end_t = clock();
	std::cout << "erase redundent elements time: " << (end_t - start_t) / CLOCKS_PER_SEC << std::endl;
	std::cout << "effective user number: " << user_map.size() << std::endl << std::endl;

	start_t = clock();
	for (std::map<IdNum, User>::iterator u = user_map.begin(); u != user_map.end();) {
		std::list<int> ulist;
		ulist.push_back(u->first);
		std::cout << u->first << std::endl;
		dfs(u->first, ulist);
		redundent_user.insert(u->first);
		user_map.erase(u++);
	}
	end_t = clock();

	std::cout << "search time: " << (end_t - start_t) / CLOCKS_PER_SEC << std::endl;
	std::cout << "item number: " << out_list.size() << std::endl;

	system("pause");
	return 0;
}