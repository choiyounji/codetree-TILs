#include<iostream>
#include<queue>

using namespace std;

#define max_l 41  //맵 최대 크기
#define max_n 31 //기사 최대 수

int l, n, q;
int map[max_l][max_l];
struct info {
	int r, c, h, w, k;
};
info knight[max_n];
info tmpknight[max_n];
int originK[max_n];
int damage[max_n];
bool ismoved[max_n];

int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,-0,-1 };

bool tryMove(int num, int dir) {
	for (int i = 1; i <= n; i++) {
		damage[i] = 0;
		ismoved[i] = false;
		tmpknight[i].r = knight[i].r;
		tmpknight[i].c = knight[i].c;
	}

	queue<int> q;

	q.push(num);
	ismoved[num] = true;

	while (!q.empty()) {
		int x = q.front();
		q.pop();

		tmpknight[x].r += dr[dir];
		tmpknight[x].c += dc[dir];

		//경계 벗어나면 못움직임
		if (tmpknight[x].r<1 || tmpknight[x].c<1 || tmpknight[x].r + knight[x].h - 1>l || tmpknight[x].c + knight[x].w - 1>l) {
			return false;
		}

		// 대상 조각이 다른 조각이나 장애물과 충돌하는지 검사합니다.
		for (int i = tmpknight[x].r; i <= tmpknight[x].r + knight[x].h - 1; i++) {
			for (int j = tmpknight[x].c; j <= tmpknight[x].c + knight[x].w - 1; j++) {
				if (map[i][j] == 1)
					damage[x]++;
				if (map[i][j] == 2)
					return false;
			}
		}

		//다른 조각 마주치면 얘네도 같이 이동
		for (int i = 1; i <= n; i++) {
			if (ismoved[i] || knight[i].k <= 0) {
				continue;
			}
			
			if (knight[i].r > tmpknight[x].r + knight[x].h - 1 || tmpknight[x].r > knight[i].r + knight[i].h - 1) {
				continue;
			}
			if (knight[i].c > tmpknight[x].c + knight[i].w - 1 || tmpknight[x].c > knight[i].c + knight[i].w - 1) {
				continue;
			}

			ismoved[i] = true;
			q.push(i);
		}
	}

	damage[num] = 0;
	return true;
}

void move(int num, int dir) {

	//죽은 나이트는 안움직임
	if (!knight[num].k > 0) {
		return;
	}

	//이동 가능하면 위치랑 체력 업데이트
	if (tryMove(num, dir)) {
		for (int i = 1; i <= n; i++) {
			knight[i].r = tmpknight[i].r;
			knight[i].c = tmpknight[i].c;
			knight[i].k -= damage[i];
		}
	}
}

int main() {
	cin >> l >> n >> q;
	for (int i = 1; i <= l; i++) {
		for (int j = 1; j <= l; j++) {
			cin >> map[i][j];
		}
	}

	for (int i = 1; i <= n; i++) {
		cin >> knight[i].r >> knight[i].c >> knight[i].h >> knight[i].w >> knight[i].k;
		originK[i] = knight[i].k;
	}
	for (int i = 0; i < q; i++) {
		int num, dir;
		cin >> num >> dir;
		move(num, dir);
	}

	int ans = 0;

	for (int i = 1; i <= q; i++) {
		if (knight[i].k > 0) {
			ans += originK[i] - knight[i].k;
		}
	}
	cout << ans;
	return 0;
}