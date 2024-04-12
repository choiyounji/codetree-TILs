#include<iostream>

using namespace std;

#define max_n 51
#define max_p 31
typedef pair<int, int> ii;
typedef pair<int, ii> iii;

int n, m, p, c, d;

ii santa[max_p];    //산타들 위치
bool alive[max_p];  //산타들 생존여부
int points[max_p];   //산타들 점수
int stun[max_p]; //기절 체크용

ii rudolf;   //루돌프 위치

int board[max_n][max_n];   //게임판, 루돌프=-1

//상우하좌
int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,0,-1 };

bool is_range(int x, int y) {
	return 1 <= x && x <= n && 1 <= y && y <= n;
}

int main() {
	//초기화
	cin >> n >> m >> p >> c >> d;
	cin >> rudolf.first >> rudolf.second;
	board[rudolf.first][rudolf.second] = -1;

	for (int i = 1; i <= p; i++) {
		int num;
		cin >> num;
		cin >> santa[num].first >> santa[num].second;
		board[santa[num].first][santa[num].second] = num;
		alive[num] = true;
	}

	//게임 시작
	for (int t = 1; t <= m; t++) {
		int closestX = 10000, closestY = 10000, closestNum = 0;

		//루돌프에서 가장 가까운 산타 찾기
		for (int i = 1; i <= p; i++) {
			//죽은 산타는 고려안함
			if (!alive[i]) {
				continue;
			}
			iii currentBest = { (closestX - rudolf.first) * (closestX - rudolf.first) + (closestY - rudolf.second) * (closestY - rudolf.second), {-closestX, -closestY} };
			iii currentValue = { (santa[i].first - rudolf.first) * (santa[i].first - rudolf.first) + (santa[i].second - rudolf.second) * (santa[i].second - rudolf.second), {-santa[i].first,-santa[i].second} };

			if (currentValue < currentBest) {
				closestX = santa[i].first;
				closestY = santa[i].second;
				closestNum = i;
			}
		}

		//루돌프 가장 가까운 산타쪽으로 한칸 이동
		if (closestNum) {
			ii originRudolf = rudolf;

			int moveX = 0;
			if (closestX > rudolf.first) {
				moveX = 1;
			}
			else if (closestX < rudolf.first) {
				moveX = -1;
			}

			int moveY = 0;
			if (closestY > rudolf.second) {
				moveY = 1;
			}
			else if (closestY < rudolf.second) {
				moveY = -1;
			}

			rudolf.first += moveX;
			rudolf.second += moveY;
			board[originRudolf.first][originRudolf.second] = 0;

			//루돌프 이동으로 충돌 발생
			if (rudolf.first == closestX && rudolf.second == closestY) {
				int firstX = closestX + moveX * c;
				int firstY = closestY + moveY * c;
				int lastX = firstX;
				int lastY = firstY;
				stun[closestNum] = t + 1; //이때까지 기절

				//이동한 위치에 산타가 이미 있을 경우
				while (is_range(lastX, lastY) && board[lastX][lastY] > 0) {
					lastX += moveX;
					lastY += moveY;
				}
				while (!(lastX == firstX && lastY == firstY)) {
					int beforeX = lastX - moveX;
					int beforeY = lastY - moveY;

					//if (!is_range(beforeX, beforeY)) break;

					int num = board[beforeX][beforeY];
					if (!is_range(lastX, lastY)) {
						alive[num] = false;
					}
					else {
						board[lastX][lastY] = board[beforeX][beforeY];
						santa[num] = { lastX,lastY };
					}
					lastX = beforeX;
					lastY = beforeY;
				}

				points[closestNum] += c;
				santa[closestNum] = { firstX,firstY };
				if (is_range(firstX, firstY)) {
					board[firstX][firstY] = closestNum;
				}
				else {
					alive[closestNum] = false;
				}
			}
		}
		
		board[rudolf.first][rudolf.second] = -1;

		//산타들 루돌프쪽으로 이동
		for (int i = 1; i <= p; i++) {
			//죽었거나 기절한 산타는 패스
			if (!alive[i] || stun[i] >= t) {
				continue;
			}

			int minDist = (santa[i].first - rudolf.first) * (santa[i].first - rudolf.first) + (santa[i].second - rudolf.second) * (santa[i].second - rudolf.second);
			int moveDir = -1;

			for (int dir = 0; dir < 4; dir++) {
				int nx = santa[i].first + dr[dir];
				int ny = santa[i].second + dc[dir];

				//게임판 밖이나 이미 산타 있는 곳으로는 산타 이동 안함
				if (!is_range(nx, ny) || board[nx][ny] > 0) {
					continue;
				}

				int dist = (nx - rudolf.first) * (nx - rudolf.first) + (ny - rudolf.second) * (ny - rudolf.second);
				if (dist < minDist) {
					minDist = dist;
					moveDir = dir;
				}
			}

			//산타 이동하기로함
			if (moveDir != -1) {
				int nx = santa[i].first + dr[moveDir];
				int ny = santa[i].second + dc[moveDir];

				//산타 이동으로 루돌프랑 충돌
				if (nx == rudolf.first && ny == rudolf.second) {
					//이때까지 기절
					stun[i] = t + 1;

					//루돌프랑 충돌하면 반대방향으로 이동하니까 -
					int moveX = -dr[moveDir];
					int moveY = -dc[moveDir];

					int firstX = nx + moveX * d;
					int firstY = ny + moveY * d;
					int lastX = firstX;
					int lastY = firstY;

					//d가 1이면 산타 걍 원래 자리에 있는거나 다름없음
					if (d == 1) {
						points[i] += d;
					}
					else {
						while (is_range(lastX, lastY) && board[lastX][lastY] > 0) {
							lastX += moveX;
							lastY += moveY;
						}
						while (!(lastX == firstX and lastY == firstY)) {
							int beforeX = lastX - moveX;
							int beforeY = lastY - moveY;

							if (!is_range(beforeX, beforeY)) {
								break;
							}

							int num = board[beforeX][beforeY];

							if (!is_range(lastX, lastY)) {
								alive[num] = false;
							}
							else {
								board[lastX][lastY] = board[beforeX][beforeY];
								santa[num] = { lastX, lastY };
							}

							lastX = beforeX;
							lastY = beforeY;
						}
						points[i] += d;
						board[santa[i].first][santa[i].second] = 0;
						santa[i] = { firstX,firstY };
						if (is_range(firstX, firstY)) {
							board[firstX][firstY] = i;
						}
						else {
							alive[i] = false;
						}
					}
				}
				//산타 이동하려는 자리에 암것도 없음
				else {
					board[santa[i].first][santa[i].second] = 0;
					santa[i] = { nx,ny };
					board[nx][ny] = i;
				}
			}
		}
		for (int i = 1; i <= p; i++) {
			if (alive[i]) {
				points[i]++;
			}
		}
	}
	for (int i = 1; i <= p; i++) {
		cout << points[i] << " ";
	}

	return 0;
}