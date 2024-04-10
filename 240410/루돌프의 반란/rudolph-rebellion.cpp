#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

struct info {
	int x, y, num;
	bool dead; //탈락
	int tmp; //기절
	int score;
};
info santa[31];
int map[51][51];  //루돌프=-1, 산타=번호
int sr[4] = { -1,0,1,0 };
int sc[4] = { 0,1,0,-1 };
int lr[8] = { -1,-1,0,1,1,1,0,-1 };
int lc[8] = { 0,-1,-1,-1,0,1,1,1 };
int n,m,p,c,d,rr,rc;
typedef pair<int, info> iin;
typedef pair<int, int> ii;

bool cmp(iin a, iin b) {
	if (a.first == b.first) {
		if (a.second.x == b.second.x) {
			return a.second.y > b.second.y;
		}
		return a.second.x > b.second.x;
	}
	return a.first < b.first;
}

void collision(int who, int dir, int santanum) {
	//루돌프가 움직여서 충돌
	if (who == -1) {
		santa[santanum].score += c;
		int nr = santa[santanum].x+c*lr[dir];
		int nc = santa[santanum].y+c*lc[dir];
		if (nr > 0 && nr <= n && nc > 0 && nc <= n) {
			//이미 산타가 있어
			if (map[nr][nc] > 0) {
				//상호작용
				/*
				cout << "상호작용전\n";
				for (int k = 1; k <= n; k++) {
					for (int l = 1; l <= n; l++) {
						cout << map[k][l] << " ";
					}
					cout << "\n";
				}
				*/
				int tmpr = nr + lr[dir];
				int tmpc = nc += lc[dir];
				int c = 1;
				while (map[tmpr][tmpc] > 0) {
					tmpr += lr[dir];
					tmpc += lc[dir];
					c++;
					cout << c;
				}
				if (tmpr > 0 && tmpr <= n && tmpc > 0 && tmpc <= n) {
					for (int a = 0; a < c; a++) {
						map[tmpr][tmpc] = map[tmpr - lr[dir]][tmpc - lc[dir]];
						santa[map[tmpr][tmpc]].x = tmpr;
						santa[map[tmpr][tmpc]].y = tmpc;
						tmpr -= lr[dir];
						tmpc -= lc[dir];
						/*
						cout << "상호작용후\n";
						for (int k = 1; k <= n; k++) {
							for (int l = 1; l <= n; l++) {
								cout << map[k][l] << " ";
							}
							cout << "\n";
						}
						*/
					}
					map[tmpr][tmpc] = santanum;
					map[santa[santanum].x][santa[santanum].y] = -1;
					rr = santa[santanum].x;
					rc = santa[santanum].y;
					santa[santanum].x = tmpr;
					santa[santanum].y = tmpc;
					/*
					cout << "상호작용후\n";
					for (int k = 1; k <= n; k++) {
						for (int l = 1; l <= n; l++) {
							cout << map[k][l] << " ";
						}
						cout << "\n";
					}
					*/
				}
				else {
					santa[map[tmpr - lr[dir]][tmpc - lc[dir]]].dead = true;
					tmpr -= lr[dir];
					tmpc -= lc[dir];
					for (int a = 1; a < c; a++) {
						//cout << "move santa" << map[tmpr - sr[dir]][tmpc - sc[dir]] << "\n";
						map[tmpr][tmpc] = map[tmpr - lr[dir]][tmpc - lc[dir]];
						santa[map[tmpr][tmpc]].x = tmpr;
						santa[map[tmpr][tmpc]].y = tmpc;
						tmpr -= lr[dir];
						tmpc -= lc[dir];
					}
					map[nr][nc] = santanum;
					map[santa[santanum].x][santa[santanum].y] = -1;
					rr = santa[santanum].x;
					rc = santa[santanum].y;
					santa[santanum].x = nr;
					santa[santanum].y = nc;
				}
			}
			else {
				map[santa[santanum].x][santa[santanum].y] = 0;
				map[nr][nc] = santanum;
				santa[santanum].x = nr;
				santa[santanum].y = nc;
				//cout << nr << " " << nc<<map[nr][nc]<<"\n";
				/*
				for (int k = 1; k <= n; k++) {
					for (int l = 1; l <= n; l++) {
						cout << map[k][l] << " ";
					}
					cout << "\n";
				}
				santa[santanum].x = nr;
				santa[santanum].y = nc;
				*/
			}
			
		}
		//게임판 밖으로 나가서 탈락
		else {
			map[rr][rc] = 0;
			map[santa[santanum].x][santa[santanum].y] = -1;
			rr = santa[santanum].x;
			rc = santa[santanum].y;
			santa[santanum].dead = true;
		}
	}
	//산타가 움직여서 충돌
	else {
		//cout << "santa collision\n";
		santa[santanum].score += d;
		int nr = santa[santanum].x + d * sr[dir];
		int nc = santa[santanum].y + d * sc[dir];
		if (nr > 0 && nr <= n && nc > 0 && nc <= n) {
			//이미 산타가 있어
			if (map[nr][nc] > 0) {
				//상호작용
				//cout << "already santa\n";
				int tmpr = nr + sr[dir];
				int tmpc = nc + sc[dir];
				int c = 1;
				while (map[tmpr][tmpc] > 0) {
					//cout << "while move santa\n";
					tmpr += sr[dir];
					tmpc += sc[dir];
					c++;
				}
				if (tmpr > 0 && tmpr <= n && tmpc > 0 && tmpc <= n) {
					for (int a = 0; a < c; a++) {
						//cout << "move santa"<<map[tmpr - sr[dir]][tmpc - sc[dir]]<<"\n";
						map[tmpr][tmpc] = map[tmpr - sr[dir]][tmpc - sc[dir]];
						santa[map[tmpr][tmpc]].x = tmpr;
						santa[map[tmpr][tmpc]].y = tmpc;
						tmpr -= sr[dir];
						tmpc -= sc[dir];
					}
					map[nr][nc] = santanum;
					map[santa[santanum].x][santa[santanum].y] = -1;
					rr = santa[santanum].x;
					rc = santa[santanum].y;
					santa[santanum].x = nr;
					santa[santanum].y = nc;
				}
				else {
					santa[map[tmpr - sr[dir]][tmpc - sc[dir]]].dead = true;
					tmpr -= sr[dir];
					tmpc -= sc[dir];
					for (int a = 1; a < c; a++) {
						//cout << "move santa" << map[tmpr - sr[dir]][tmpc - sc[dir]] << "\n";
						map[tmpr][tmpc] = map[tmpr - sr[dir]][tmpc - sc[dir]];
						santa[map[tmpr][tmpc]].x = tmpr;
						santa[map[tmpr][tmpc]].y = tmpc;
						tmpr -= sr[dir];
						tmpc -= sc[dir];
					}
					map[nr][nc] = santanum;
					map[santa[santanum].x][santa[santanum].y] = -1;
					rr = santa[santanum].x;
					rc = santa[santanum].y;
					santa[santanum].x = nr;
					santa[santanum].y = nc;
				}
			}
			else {
				map[santa[santanum].x][santa[santanum].y] = -1;
				rr = santa[santanum].x;
				rc = santa[santanum].y;
				map[nr][nc] = santanum;
				santa[santanum].x = nr;
				santa[santanum].y = nc;
			}
		}
		//게임판 밖으로 나가서 탈락
		else {
			map[santa[santanum].x][santa[santanum].y] = -1;
			rr = santa[santanum].x;
			rc = santa[santanum].y;
			santa[santanum].dead = true;
		}
	}
}
void move_lu(int turn) {
	int clr = rr;
	int clc = rc;
	vector<iin> v;
	for (int i = 1; i <= p; i++) {
		//죽은 산타는 비교안함
		if (!santa[i].dead) {
			int len = (clr - santa[i].x) * (clr - santa[i].x) + (clc - santa[i].y) * (clc - santa[i].y);
			//cout << "len" << len << " " << i << "\n";
			v.push_back({ len,santa[i] });
		}
	}
	sort(v.begin(), v.end(),cmp);
	if (!v.empty()) {
		//cout << "closest santa=" << v[0].second.num<<"\n";
		//루돌프 8가지 방향 중 산타에 젤 가까워지는 방향 탐색
		vector<ii> ld;
		for (int i = 0; i < 8; i++) {
			int nclr = clr + lr[i];
			int nclc = clc + lc[i];
			if (nclr > 0 && nclr <= n && nclc > 0 && nclc <= n) {
				int len = (nclr - santa[v[0].second.num].x) * (nclr - santa[v[0].second.num].x) + (nclc - santa[v[0].second.num].y) * (nclc - santa[v[0].second.num].y);
				ld.push_back({ len,i });
			}
		}
		sort(ld.begin(), ld.end());
		
		if (!ld.empty()) {
			map[rr][rc] = 0;
			rr = clr + lr[ld[0].second];
			rc = clc + lc[ld[0].second];

			//이동하려는 자리에 산타가 존재한다면
			if (map[rr][rc] > 0) {
				//cout << "rou move collision\n";
				santa[map[rr][rc]].tmp = turn + 1;
				//cout << map[rr][rc]<<"기절"<<"\n";
				collision(-1, ld[0].second, map[rr][rc]);
				map[rr][rc] = -1;
			}
			else {
				//cout << "rou move\n";
				map[rr][rc] = -1;
				/*
				for (int k = 1; k <= n; k++) {
					for (int l = 1; l <= n; l++) {
						cout << map[k][l] << " ";
					}
					cout << "\n";
				}
				*/
			}
		}
	}
	
}
void move_santa(int turn) {
	for (int i = 1; i <= p; i++) {
		if (turn>santa[i].tmp && !santa[i].dead) {
			//cout << santa[i].tmp<<" "<<i << "산타 움직임\n";
			int currentlength= (rr - santa[i].x) * (rr - santa[i].x) + (rc - santa[i].y) * (rc - santa[i].y);
			vector<ii> v;
			for (int j = 0; j < 4; j++) {
				int nr = santa[i].x + sr[j];
				int nc = santa[i].y + sc[j];
				if (nr > 0 && nr <= n && nc > 0 && nc <= n) {
					//산타 움직이는데 산타있으면 안움직임
					if (map[nr][nc] > 0) {
						continue;
					}
					int len = (nr - rr) * (nr - rr) + (nc - rc) * (nc - rc);
					if (len < currentlength) {
						v.push_back({ len,j });
					}
				}
				else { continue; }
			}
			sort(v.begin(), v.end());
			if (!v.empty()) {
				//산타가 움직여서 루돌프랑 충돌
				if (map[santa[i].x + sr[v[0].second]][santa[i].y + sc[v[0].second]] == -1) {
					map[santa[i].x][santa[i].y] = 0;
					santa[i].x += sr[v[0].second];
					santa[i].y += sc[v[0].second];
					santa[i].tmp = turn + 1;
					collision(i, (v[0].second+2)%4, i);
				}
				else {
					map[santa[i].x][santa[i].y] = 0;
					santa[i].x += sr[v[0].second];
					santa[i].y += sc[v[0].second];
					map[santa[i].x][santa[i].y] = santa[i].num;
				}
			}
		}
	}

}
int main() {
	cin >> n >> m >> p >> c >> d;

	//루돌프 초기화
	cin >> rr >> rc;
	map[rr][rc] = -1;

	//산타들 초기화
	int sa, sb, sc;
	for (int i = 0; i < p; i++) {
		cin >> sa >> sb >> sc;
		santa[sa].x = sb;
		santa[sa].y = sc;
		santa[sa].num = sa;
		santa[sa].dead = false;
		santa[sa].tmp = 0;
		santa[sa].score = 0;
		map[sb][sc] = sa;
	}

	for (int i = 1; i <= m; i++) {
		//cout << "move lu\n";
		move_lu(i);
		/*
		for (int k = 1; k <= n; k++) {
			for (int l = 1; l <= n; l++) {
				cout << map[k][l] << " ";
			}
			cout << "\n";
		}
		*/
		//cout << "move sa\n";
		move_santa(i);
		/*/
		for (int k = 1; k <= n; k++) {
			for (int l = 1; l <= n; l++) {
				cout << map[k][l] << " ";
			}
			cout << "\n";
		}
		*/
		bool flag = false;
		for (int j = 1; j <= p; j++) {
			if (!santa[j].dead) {
				flag = true;
				santa[j].score++;
			}
		}
		//산타 다 죽은 상태임. 더 게임 돌 필요x
		if (!flag) {
			break;
		}
		/*
		for (int k = 1; k <= n; k++) {
			for (int l = 1; l <= n; l++) {
				cout << map[k][l] << " ";
			}
			cout << "\n";
		}
		*/
	}
	
	for (int i = 1; i <= p; i++) {
		cout << santa[i].score << " ";
	}
	return 0;
}