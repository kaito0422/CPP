#include <iostream>
#include <string>
#include <iomanip>
#include <list>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

int p[600][600];
bool flag[600][600];
int n;
int direct[4][2] = {
	{-1, 0},
	{0, 1},
	{1, 0},
	{0, -1}
};

struct Node {
	int x, y;
};

int  bfs(int x, int y)
{
	int count = 1;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			flag[i][j] = 0;
	std::list<struct Node> lst_node;
	lst_node.push_back({x, y});
	flag[x][y] = 1;
	while (lst_node.size()) {
		struct Node tmp = lst_node.front();
		lst_node.pop_front();
		int x_tmp, y_tmp;
		x_tmp = tmp.x;
		y_tmp = tmp.y;
		for (int i = 0; i < 4; i++) {
			int xx, yy;
			xx = x_tmp + direct[i][0];
			yy = y_tmp + direct[i][1];
			if (flag[xx][yy] || !(xx >= 0 && yy >= 0 && xx < n && yy < n))
				continue;
			else if(!flag[xx][yy] && p[xx][yy] == p[x][y]){
				flag[xx][yy] = 1;
				p[xx][yy] = 0;
				count++;
				lst_node.push_back({ xx, yy });
			}
		}
	}
	p[x][y] = 0;
	return count;
}

int main()
{
	int m;
	std::cin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			std::cin >> p[i][j];

	while (m--) {
		int x, y;
		std::cin >> x >> y;
		int color = p[x][y];
		if (color == 0)
			continue;
		if (p[x][y] == 0) {
			std::cout << "empty!" << std::endl;
		}
		if (!(p[x][y] != p[x - 1][y] && p[x][y] != p[x + 1][y]
			&& p[x][y] != p[x][y - 1] && p[x][y] != p[x][y + 1])) {
			std::cout << bfs(x, y) << std::endl;
		}
		else {
			std::cout << "only one!" << std::endl;
			continue;
		}

		for (int i = 0; i < n; i++) {
			int index;
			for (int j = n - 1; j >= 0; j--) {
				if (p[j][i] != 0)
					continue;
				index = j;
				for (int t = index - 1; t >= 0; t--) {
					if (p[t][i] != 0) {
						p[index][i] = p[t][i];
						p[t][i] = 0;
						break;
					}
				}
			}
		}

		for (int i = 0; i < n; i++) {
			if (p[n - 1][i] == 0) {
				for (int ii = i; ii < n - 1; ii++)
					for (int jj = 0; jj < n; jj++)
						p[jj][ii] = p[jj][ii + 1];
				for (int ii = 0; ii < n; ii++)
					p[ii][n - 1] = 0;
			}
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				std::cout << p[i][j] << ' ';
			std::cout << std::endl;
		}
	}


	system("pause");
	return 0;
}