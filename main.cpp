#include<cstdio>
#include<cmath>
#include<cstdlib>
#include<time.h>
#include<string>
#include<queue>
#define LAMBDA 11//每分钟进出大楼人数的平均值
#define GOING_ON 1
#define GOING_DOWN 0
#define WAITING 3
#define MAX_RIDER 10
#define EIGHT 0
#define NINE 60
#define ELEVEN 180
#define TWELVE 240
#define THIRTEEN 300
#define FOURTEEN 360
#define EIGHTEEN 600
#define NINETEEN 660
#define TWENTY_TWO 840
#define ELEVATOR_NUMBER 6


using namespace std;

typedef struct node1 {
	int nowFloor;
	int endFloor;
	int startWaitTime;
	float endWaitTime;
	int on_down;
	struct node1(int a, int b, int c, int d, int e) {//构造函数
		nowFloor = a;
		endFloor = b;
		startWaitTime = c;
		endWaitTime = d;
		on_down = e;
	}
	struct node1() {//默认构造函数
		nowFloor = 0;
		endFloor = 0;
		startWaitTime = 0;
		endWaitTime = -1;
		on_down = -1;
	};
	bool operator<(const node1& s)const {
		return startWaitTime > s.startWaitTime;
	}
}clerk;


typedef struct node2 {
	int nowFloor;
	int hithest;
	int nowRiders;
	int type;	//电梯当前状态
	bool ifwaiting;
	float haswaitedtime;
}elevator;
priority_queue<clerk, vector<clerk>, less<clerk>> floors[22][22];//在某层到某层
elevator elevators[ELEVATOR_NUMBER];
int map[22][22];//某层楼到另一层楼的人数
int stop[ELEVATOR_NUMBER][22];//某一部电梯里面到某个楼层的人数

double P_Random();
int U_Random();
int possion();
bool Iffloorempty(int n) {
	for (int i = 0; i < 22; i++) {
		if (!floors[n-1][i].empty())
			return false;
	}
	return true;
}

bool Iflowempty(int n) {//判断是否这层楼有人等待往下走
	for (int i = 0; i < n; i++) {
		if (!floors[n-1][i].empty())
			return false;
	}
	return true;
}

bool Ifhighempty(int n) {//判断这一层楼是否有人等待往上走
	for (int i = n; i < 22; i++) {
		if (!floors[n-1][i].empty())
			return false;
	}
	return true;
}


void Headle(clerk);

int Onnumber(int n) {
	int result = 0;
	for (int i = n + 1; i <= 22; i++)
		result += map[n - 1][i - 1];
	return result;
}
int T_Random();

int Calctime(int, int, int);



bool IfBuildingEmpty() {
	for(int i=1;i<22;i++)
		for (int j = 0; j < 22; j++) {
			if (!floors[i][j].empty())
				return false;
		}
	return true;
}

	double alltime = 0;//所有人的等待时间
	int nowtime = EIGHT;
	int visitorsflow = 0;//一天之内进出写字楼的总人数
	int satisfaction = 0;
	int dissatisfaction = 0;
	int elsenumber = 0;//除去满意和不满意的人
int main() {
	int innumber, outnumber;
	int needorder;//当前乘客时间最近的电梯
	srand((unsigned)time(NULL));
	memset(map, 0, sizeof(map));
	memset(stop, 0, sizeof(stop));
	for (int i = 0; i < ELEVATOR_NUMBER; i++) {
		elevators[i].nowFloor = 1;
		elevators[i].nowRiders = 0;
		elevators[i].type = WAITING;
		elevators[i].haswaitedtime = 0;
		elevators[i].hithest = -1;
		elevators[i].ifwaiting = false;
	}
		while (nowtime < TWENTY_TWO) {	
				int destination;
				int begin ;
				int ty;
			if (nowtime < NINE || (nowtime >= ELEVEN && nowtime < FOURTEEN)) {//上班时间
				visitorsflow += 22;
				innumber = 0;
				outnumber = 0;				
				for (int i = 0; i < 22; i++) {
					if (outnumber >= 2) {//只有上楼的人
						ty = GOING_ON;
						begin = 1;
						destination = 1;
						while (destination == 1)destination = U_Random();
						floors[0][destination - 1].push(clerk(1, destination, nowtime, -1, GOING_ON));
						map[0][destination - 1]++;
						innumber++;
					}
					else if (innumber >= 20) {//只有下楼的人
						ty = GOING_DOWN;
						begin = 1;
						destination = 1;
						while (begin == 1)begin = U_Random();
						floors[begin - 1][0].push(clerk(begin, 1, nowtime, -1, GOING_DOWN));
						map[begin - 1][0]++;
						outnumber++;
					}
					else {
						ty = T_Random();
						if (ty == GOING_ON) {
							begin = 1;
							destination=1;
							while (destination == 1)destination = U_Random();
							floors[0][destination-1].push(clerk(1, destination, nowtime, -1, GOING_ON));
							map[0][destination - 1]++;
							innumber++;
						}
						else if (ty == GOING_DOWN) {
							begin = 1;
							destination = 1;
							while (begin == 1)begin = U_Random();
							floors[begin - 1][0].push(clerk(begin, 1, nowtime, -1, GOING_DOWN));
							map[begin - 1][0]++;
							outnumber++;
						}
					}
					//产生一个乘客后马上响应
					needorder = Calctime(begin, destination, ty);
					if (elevators[needorder].type == WAITING) {
							if (begin < elevators[needorder].nowFloor)
								elevators[needorder].type = GOING_DOWN;
							else if (begin >= elevators[needorder].nowFloor)
								elevators[needorder].type = GOING_ON;
							if (ty == GOING_DOWN) {
									elevators[needorder].hithest = begin;
							}
							if (ty == GOING_ON) {
								elevators[needorder].hithest = destination;
							}
							elevators[needorder].ifwaiting = false;
					}
					else if (elevators[needorder].type == GOING_DOWN) {//电梯正在往下走
						if (ty == GOING_DOWN && begin > elevators[needorder].nowFloor) {//乘客往下，但是错过电梯
							if (elevators[needorder].hithest < begin)
								elevators[needorder].hithest = begin;
						}
					}
					else if (elevators[needorder].type == GOING_ON) {//电梯往上走
							 if (ty == GOING_DOWN) {
								 if (elevators[i].hithest < begin)
									 elevators[i].hithest = begin;
							}
					}	
				}
			}
			else if ((nowtime >= ELEVEN && nowtime < TWELVE) || (nowtime >= EIGHTEEN && nowtime < NINETEEN)) {//下班时间
				visitorsflow += 22;
				innumber = 0;
				outnumber = 0;
				for (int i = 0; i < 22; i++) {
					if (innumber >= 2) {//只有下楼的人
						ty = GOING_DOWN;
						begin = 1;
						destination = 1;
						while (begin == 1)begin = U_Random();
						floors[begin - 1][0].push(clerk(begin, 1, nowtime, -1, GOING_DOWN));
						map[begin - 1][0]++;
						outnumber++;
					}
					else if (outnumber >= 20) {//只有上楼的人
					ty = GOING_ON;
					begin = 1;
					destination = 1;
					while (destination == 1)destination = U_Random();
					floors[0][destination -1].push(clerk(1, destination, nowtime, -1, GOING_ON));
					map[0][destination - 1]++;
					innumber++;
					}
					else {
						ty = T_Random();
						if (ty == GOING_DOWN) {
							destination = 1;
							begin = 1;
							while (begin == 1)begin = U_Random();
							floors[begin - 1][0].push(clerk(begin, 1, nowtime, -1, GOING_DOWN));
							map[begin - 1][0]++;
							outnumber++;
						}
						else if (ty == GOING_ON) {
							begin = 1;
							destination = 1;
							while (destination == 1)destination = U_Random();
							floors[0][destination - 1].push(clerk(1, destination, nowtime, -1, GOING_ON));
							map[0][destination - 1]++;
							innumber++;
						}
					}
					//产生一个乘客后马上响应
					needorder = Calctime(begin, destination, ty);
					if (elevators[needorder].type == WAITING) {
						if (begin < elevators[needorder].nowFloor)
							elevators[needorder].type = GOING_DOWN;
						else if (begin >= elevators[needorder].nowFloor)
							elevators[needorder].type = GOING_ON;
						if (ty == GOING_DOWN) {
							elevators[needorder].hithest = begin;
						}
						if (ty == GOING_ON) {
							elevators[needorder].hithest = destination;
						}
						elevators[needorder].ifwaiting = false;
					}
					else if (elevators[needorder].type == GOING_DOWN) {//电梯正在往下走
						//下楼顺路
						if (ty == GOING_DOWN && begin <= elevators[needorder].nowFloor) {
							if (elevators[needorder].hithest < begin)
								elevators[needorder].hithest = begin;
						}
						if (ty == GOING_DOWN && begin > elevators[needorder].nowFloor) {//乘客往下，但是错过电梯
							if (elevators[needorder].hithest < begin)
								elevators[needorder].hithest = begin;
						}
					}
					else if (elevators[needorder].type == GOING_ON) {//电梯往上走
						if (ty == GOING_DOWN) {
							if (elevators[i].hithest < begin)
								elevators[i].hithest = begin;
						}
					}
				}
			}
			else {//泊松分布
				int nowclerkon = P_Random();
				int nowclerkdown = P_Random();
				visitorsflow += nowclerkdown + nowclerkon;
				innumber = 0;
				outnumber = 0;
				for (int i = 0; i < nowclerkdown + nowclerkon; i++) {
					if (innumber >= nowclerkon) {//只有下楼的人
						ty = GOING_DOWN;
						begin = 1;
						destination = 1;
						while (begin == 1)begin = U_Random();
						floors[begin - 1][0].push(clerk(begin, 1, nowtime, -1, GOING_DOWN));
						map[begin - 1][0]++;
						outnumber++;
					}
					else if (outnumber >= nowclerkdown) {//只有上楼的人
						ty = GOING_ON;
						begin = 1;
						destination = 1;
						while (destination == 1)destination = U_Random();
						floors[0][destination - 1].push(clerk(1, destination, nowtime, -1, GOING_ON));
						map[0][destination - 1]++;
						innumber++;
					}
					else {
						ty = T_Random();
						if (ty == GOING_DOWN) {
							begin = 1;
							destination = 1;
							while (begin == 1)begin = U_Random();
							floors[begin - 1][0].push(clerk(begin, 1, nowtime, -1, GOING_DOWN));
							map[begin - 1][0]++;
							outnumber++;
						}
						else if (ty == GOING_ON) {
							begin = 1;
							destination = 1;
							while (destination == 1)destination = U_Random();
							floors[0][destination - 1].push(clerk(1, destination, nowtime, -1, GOING_ON));
							map[0][destination - 1]++;
							innumber++;
						}
					}
					//产生一个乘客后马上响应
					needorder = Calctime(begin, destination, ty);
					if (elevators[needorder].type == WAITING) {
						if (begin < elevators[needorder].nowFloor)
							elevators[needorder].type = GOING_DOWN;
						else if (begin >= elevators[needorder].nowFloor)
							elevators[needorder].type = GOING_ON;
						if (ty == GOING_DOWN) {
							elevators[needorder].hithest = begin;
						}
						if (ty == GOING_ON) {
							elevators[needorder].hithest = destination;
						}
						elevators[needorder].ifwaiting = false;
					}
					else if (elevators[needorder].type == GOING_DOWN) {//电梯正在往下走
						//下楼顺路
						if (ty == GOING_DOWN && begin <= elevators[needorder].nowFloor) {
							if (elevators[needorder].hithest < begin)
								elevators[needorder].hithest = begin;
						}
						if (ty == GOING_DOWN && begin > elevators[needorder].nowFloor) {//乘客往下，但是错过电梯
							if (elevators[needorder].hithest < begin)
								elevators[needorder].hithest = begin;
						}
					}
					else if (elevators[needorder].type == GOING_ON) {//电梯往上走
						if (ty == GOING_DOWN) {
							if (elevators[i].hithest < begin)
								elevators[i].hithest = begin;
						}
					}
				}
			}
			//两个0.5秒里面电梯的运动状态改变
			for (int loop2 = 0; loop2 < 2; loop2++) {
				for (int loop1 = 0; loop1 < ELEVATOR_NUMBER; loop1++) {
					if (elevators[loop1].type != WAITING) {
						//前一个0.5秒电梯没有停下来
						if (!elevators[loop1].ifwaiting) {
							//电梯往下不在一楼
							if (elevators[loop1].type == GOING_DOWN && elevators[loop1].nowFloor != 1) {
								//电梯下楼可以顺路带人，可以在顶楼
								if (!floors[elevators[loop1].nowFloor - 1][0].empty() && elevators[loop1].nowRiders < MAX_RIDER) {
									while (elevators[loop1].nowRiders < MAX_RIDER && !floors[elevators[loop1].nowFloor - 1][0].empty()) {
										elevators[loop1].nowRiders++;
										map[elevators[loop1].nowFloor - 1][0]--;
										clerk nw = floors[elevators[loop1].nowFloor - 1][0].top();
										nw.endWaitTime = nowtime + loop2 * 0.5;
										floors[elevators[loop1].nowFloor - 1][0].pop();
										stop[loop1][0]++;
										Headle(nw);
									}
									elevators[loop1].ifwaiting = true;
									elevators[loop1].haswaitedtime = 0.5;
								}
								//电梯不停下来
								else {
									elevators[loop1].nowFloor--;
								}
							}
							//电梯往下并且在一楼
							else if (elevators[loop1].type == GOING_DOWN && elevators[loop1].nowFloor == 1) {
								elevators[loop1].nowRiders = 0;
								elevators[loop1].type = GOING_ON;
								//寻找最先来等着上楼的人
								while (elevators[loop1].nowRiders < MAX_RIDER && !Ifhighempty(1)) {
									elevators[loop1].nowRiders++;
									int mintime = 100000;
									int end;
									for (int j = 1; j < 22; j++) {
										if (!floors[0][j].empty()) {
											if (floors[0][j].top().startWaitTime <= mintime) {
												mintime = floors[0][j].top().startWaitTime;
												end = j + 1;
											}
										}
									}
									if (elevators[loop1].hithest < end)
										elevators[loop1].hithest = end;
									map[0][end - 1]--;
									clerk nw = floors[0][end - 1].top();
									nw.endWaitTime = nowtime + loop2 * 0.5;
									floors[0][end - 1].pop();
									stop[loop1][end - 1]++;
									Headle(nw);
								}
								elevators[loop1].ifwaiting = true;
								elevators[loop1].haswaitedtime = 0.5;
							}
							//电梯网上并且在一楼
							else if (elevators[loop1].nowFloor == 1 && elevators[loop1].type == GOING_ON) {
								elevators[loop1].ifwaiting = true;
								elevators[loop1].haswaitedtime = 0.5;
								while (elevators[loop1].nowRiders < MAX_RIDER && !Ifhighempty(1)) {
									elevators[loop1].nowRiders++;
									int mintime = 100000;
									int end;
									for (int j = 1; j < 22; j++) {
										if (!floors[0][j].empty()) {
											if (floors[0][j].top().startWaitTime <= mintime) {
												mintime = floors[0][j].top().startWaitTime;
												end = j + 1;
											}
										}
									}
									if (elevators[loop1].hithest < end)
										elevators[loop1].hithest = end;
									map[0][end - 1]--;
									clerk nw = floors[0][end - 1].top();
									nw.endWaitTime = nowtime + loop2 * 0.5;
									floors[0][end - 1].pop();
									stop[loop1][end - 1]++;
									Headle(nw);
								}
							}
							//电梯网上并且不在顶楼
							else if (elevators[loop1].nowFloor > 1 && elevators[loop1].type == GOING_ON && elevators[loop1].nowFloor < elevators[loop1].hithest) {
								if (stop[loop1][elevators[loop1].nowFloor - 1] == 0) {
									elevators[loop1].nowFloor++;
								}
								else {
									elevators[loop1].ifwaiting = true;
									elevators[loop1].haswaitedtime = 0.5;
									elevators[loop1].nowRiders -= stop[loop1][elevators[loop1].nowFloor - 1];
									//////////////////内存越界导致nowtime被修改//////////
									stop[loop1][elevators[loop1].nowFloor - 1] = 0;
								}
							}
							//电梯往上并且到了顶楼
							else if (elevators[loop1].nowFloor == elevators[loop1].hithest && elevators[loop1].type == GOING_ON) {
								elevators[loop1].type = GOING_DOWN;
								elevators[loop1].ifwaiting = true;
								elevators[loop1].haswaitedtime = 0.5;
								stop[loop1][elevators[loop1].hithest - 1] = 0;
								elevators[loop1].nowRiders = 0;
								while (elevators[loop1].nowRiders < MAX_RIDER && map[elevators[loop1].nowFloor - 1][0]>0) {
									elevators[loop1].nowRiders++;
									map[elevators[loop1].nowFloor - 1][0]--;
									clerk nw = floors[elevators[loop1].nowFloor - 1][0].top();
									nw.endWaitTime = nowtime + loop2 * 0.5;
									floors[elevators[loop1].nowFloor - 1][0].pop();
									stop[loop1][0]++;
									Headle(nw);
								}
							}

						}
						//前一个0.5秒已经停下来了
						else {
							//电梯往下停下来，但是不是在一楼
							if (elevators[loop1].type == GOING_DOWN && elevators[loop1].nowFloor != 1) {
								elevators[loop1].haswaitedtime += 0.5;
								if (elevators[loop1].haswaitedtime >= 2) {
									elevators[loop1].ifwaiting = false;
									elevators[loop1].haswaitedtime = 0;
									elevators[loop1].nowFloor--;
								}
							}
							//电梯往下停下来了，而且是在一楼
							//电梯往上停下来了，在一楼
							else if (elevators[loop1].type == GOING_ON && elevators[loop1].nowFloor == 1) {
								elevators[loop1].haswaitedtime += 0.5;
								if (elevators[loop1].haswaitedtime >= 2) {
									elevators[loop1].haswaitedtime = 0;
									elevators[loop1].ifwaiting = false;
									elevators[loop1].nowFloor++;
								}
							}
							//电梯往上停下来了，不再最终目的地
							else if (elevators[loop1].type == GOING_ON && elevators[loop1].hithest != elevators[loop1].nowFloor) {
								elevators[loop1].haswaitedtime += 0.5;
								if (elevators[loop1].haswaitedtime >= 2) {
									elevators[loop1].haswaitedtime = 0;
									elevators[loop1].ifwaiting = false;
									elevators[loop1].nowFloor++;
								}
							}
							//电梯往下停下在最终目的地
							else if (elevators[loop1].hithest == elevators[loop1].nowFloor && elevators[loop1].type == GOING_DOWN) {
								elevators[loop1].haswaitedtime += 0.5;
								if (elevators[loop1].haswaitedtime >= 2) {
									elevators[loop1].haswaitedtime = 0;
									elevators[loop1].ifwaiting = false;
									elevators[loop1].hithest = 0;
									elevators[loop1].nowFloor--;
								}
							}
						}
					}
				}
			}
			nowtime++;
		}
		//没有新的乘客产生
		while (!IfBuildingEmpty()) {
			//从最高层开始运
			for (int ems = 22; ems > 1; ems--) {	
				if(!floors[ems-1][0].empty())
				for (int loop = 0; loop < ELEVATOR_NUMBER; loop++)
						elevators[loop].hithest = ems;
				//只要最高层还有人，就运行
				while (!floors[ems - 1][0].empty()) {
					//两个0.5秒里面电梯的运动状态改变
					for (int loop2 = 0; loop2 < 2; loop2++) {
						for (int loop1 = 0; loop1 < ELEVATOR_NUMBER; loop1++) {
							if (elevators[loop1].type != WAITING) {
								//前一个0.5秒电梯没有停下来
								if (!elevators[loop1].ifwaiting) {
									//电梯往下不在一楼
									if (elevators[loop1].type == GOING_DOWN && elevators[loop1].nowFloor != 1) {
										//电梯下楼可以顺路带人，可以在顶楼
										if (!floors[elevators[loop1].nowFloor - 1][0].empty()&& elevators[loop1].nowRiders < MAX_RIDER) {
											while (elevators[loop1].nowRiders < MAX_RIDER && !floors[elevators[loop1].nowFloor - 1][0].empty()) {
												elevators[loop1].nowRiders++;
												map[elevators[loop1].nowFloor - 1][0]--;
												clerk nw = floors[elevators[loop1].nowFloor - 1][0].top();
												nw.endWaitTime = nowtime + loop2 * 0.5;
												floors[elevators[loop1].nowFloor - 1][0].pop();
												stop[loop1][0]++;
												Headle(nw);
											}
											elevators[loop1].ifwaiting = true;
											elevators[loop1].haswaitedtime = 0.5;
										}
										//电梯不停下来
										else {
											elevators[loop1].nowFloor--;
										}
									}
									//电梯往下并且在一楼
									else if (elevators[loop1].type == GOING_DOWN && elevators[loop1].nowFloor == 1) {
										elevators[loop1].nowRiders = 0;
										elevators[loop1].type = GOING_ON;
										//寻找最先来等着上楼的人
										while (elevators[loop1].nowRiders < MAX_RIDER && !Ifhighempty(1)) {
											elevators[loop1].nowRiders++;
											int mintime = 100000;
											int end;
											for (int j = 1; j < 22; j++) {
												if (!floors[0][j].empty()) {
													if (floors[0][j].top().startWaitTime <= mintime) {
														mintime = floors[0][j].top().startWaitTime;
														end = j + 1;
													}
												}
											}
											if (elevators[loop1].hithest < end)
												elevators[loop1].hithest = end;
											map[0][end - 1]--;
											clerk nw = floors[0][end - 1].top();
											nw.endWaitTime = nowtime + loop2 * 0.5;
											floors[0][end - 1].pop();
											stop[loop1][end - 1]++;
											Headle(nw);
										}
										elevators[loop1].ifwaiting = true;
										elevators[loop1].haswaitedtime = 0.5;
									}
									//电梯网上并且在一楼
									else if (elevators[loop1].nowFloor == 1 && elevators[loop1].type == GOING_ON) {
										elevators[loop1].nowRiders = 0;
										elevators[loop1].ifwaiting = true;
										elevators[loop1].haswaitedtime = 0.5;
										while (elevators[loop1].nowRiders < MAX_RIDER && !Ifhighempty(1)) {
											elevators[loop1].nowRiders++;
											int mintime = 100000;
											int end;
											for (int j = 1; j < 22; j++) {
												if (!floors[0][j].empty()) {
													if (floors[0][j].top().startWaitTime <= mintime) {
														mintime = floors[0][j].top().startWaitTime;
														end = j + 1;
													}
												}
											}
											if (elevators[loop1].hithest < end)
												elevators[loop1].hithest = end;
											map[0][end - 1]--;
											clerk nw = floors[0][end - 1].top();
											nw.endWaitTime = nowtime + loop2 * 0.5;
											floors[0][end - 1].pop();
											stop[loop1][end - 1]++;
											Headle(nw);
										}
									}
									//电梯网上并且不在顶楼
									else if (elevators[loop1].nowFloor > 1 && elevators[loop1].type == GOING_ON && elevators[loop1].nowFloor < elevators[loop1].hithest) {
										if (stop[loop1][elevators[loop1].nowFloor - 1] == 0) {
											elevators[loop1].nowFloor++;
										}
										else {
											elevators[loop1].ifwaiting = true;
											elevators[loop1].haswaitedtime = 0.5;
											elevators[loop1].nowRiders -= stop[loop1][elevators[loop1].nowFloor - 1];
											//////////////////内存越界导致nowtime被修改//////////
											stop[loop1][elevators[loop1].nowFloor - 1] = 0;
										}
									}
									//电梯往上并且到了顶楼
									else if (elevators[loop1].nowFloor == elevators[loop1].hithest && elevators[loop1].type == GOING_ON) {
										elevators[loop1].type = GOING_DOWN;
										elevators[loop1].ifwaiting = true;
										elevators[loop1].haswaitedtime = 0.5;
										stop[loop1][elevators[loop1].hithest - 1] = 0;
										elevators[loop1].nowRiders = 0;
										while (elevators[loop1].nowRiders < MAX_RIDER && map[elevators[loop1].nowFloor - 1][0]>0) {
											elevators[loop1].nowRiders++;
											map[elevators[loop1].nowFloor - 1][0]--;
											clerk nw = floors[elevators[loop1].nowFloor - 1][0].top();
											nw.endWaitTime = nowtime + loop2 * 0.5;
											floors[elevators[loop1].nowFloor - 1][0].pop();
											stop[loop1][0]++;
											Headle(nw);
										}
									}

								}
								//前一个0.5秒已经停下来了
								else {
									//电梯往下停下来，但是不是在一楼
									if (elevators[loop1].type == GOING_DOWN && elevators[loop1].nowFloor != 1) {
										elevators[loop1].haswaitedtime += 0.5;
										if (elevators[loop1].haswaitedtime >= 2) {
											elevators[loop1].ifwaiting = false;
											elevators[loop1].haswaitedtime = 0;
											elevators[loop1].nowFloor--;
										}
									}
									//电梯往下停下来了，而且是在一楼
									//电梯往上停下来了，在一楼
									else if (elevators[loop1].type == GOING_ON && elevators[loop1].nowFloor == 1) {
										elevators[loop1].haswaitedtime += 0.5;
										if (elevators[loop1].haswaitedtime >= 2) {
											elevators[loop1].haswaitedtime = 0;
											elevators[loop1].ifwaiting = false;
											elevators[loop1].nowFloor++;
										}
									}
									//电梯往上停下来了，不再最终目的地
									else if (elevators[loop1].type == GOING_ON && elevators[loop1].hithest != elevators[loop1].nowFloor) {
										elevators[loop1].haswaitedtime += 0.5;
										if (elevators[loop1].haswaitedtime >= 2) {
											elevators[loop1].haswaitedtime = 0;
											elevators[loop1].ifwaiting = false;
											elevators[loop1].nowFloor++;
										}
									}
									//电梯往下停下在最终目的地
									else if (elevators[loop1].hithest == elevators[loop1].nowFloor&&elevators[loop1].type==GOING_DOWN) {
										elevators[loop1].haswaitedtime += 0.5;
										if (elevators[loop1].haswaitedtime >= 2) {
											elevators[loop1].haswaitedtime = 0;
											elevators[loop1].ifwaiting = false;
											elevators[loop1].hithest = 0;
											elevators[loop1].nowFloor--;
										}
									}
								}
							}
						}
					}
					nowtime++;
				}
			}
		}
		double a1;//平均等待时间
		double a2;//平均满意率
		a1 = (double)alltime / visitorsflow;
		a2 = (double)satisfaction / visitorsflow;
		printf("平均等待时间%.6lf分钟\n", a1);
		printf("平均满意率%.6lf%%\n", a2 * 100);
		return 0;
}
//计算不同电梯去接clerk的时间.返回所需时间最小的电梯编号
int Calctime(int nwf, int edf,int on_down) {
	float mintime = 100000;
	int order;
	float n;//记录最小时间
	int origrider;//记录原来电梯上有的乘客数量
	int m[ELEVATOR_NUMBER][22];//保存stop数组
	int k[22][22];//保存map数组
	clerk fuben[1000];//储存弹出来的乘客，方便还原
	int fubennumber;//记录弹出来的乘客数量
	int origdestination;
	for (int i = 0; i < ELEVATOR_NUMBER; i++) {
		if (elevators[i].type == WAITING) {
			return i;
		}
	}
	for (int i = 0; i < ELEVATOR_NUMBER; i++) {
		for (int j = 0; j < 22; j++)
			m[i][j] = stop[i][j];
	}
	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++)
			k[i][j] = map[i][j];
	}
	for (int i = 0; i < ELEVATOR_NUMBER; i++) {
		n = 0;
		fubennumber = 0;
		origrider = elevators[i].nowRiders;
		origdestination = elevators[i].hithest;
		if (elevators[i].type == WAITING) {//有空闲的电梯
			n = abs(nwf - elevators[i].nowFloor) * 0.5;
		}
		//电下，人下，人在电梯下
		else if (elevators[i].type == GOING_DOWN && nwf <= elevators[i].nowFloor&&on_down==GOING_DOWN) {
			for (int j = elevators[i].nowFloor; j > nwf; j--) {
				if (elevators[i].nowRiders >= MAX_RIDER)//这层楼没有人上电梯并且没有人需要下电梯或者只有人上电梯但是电梯人已经满了，就不停下来
					n += 0.5;
				else {
						//考虑上电梯的人对电梯是否在某个楼层停止有影响
						while (elevators[i].nowRiders < MAX_RIDER &&!Iflowempty(j)) {
							//找出来开始等待时间最小并且的乘客
							elevators[i].nowRiders++;
							fubennumber++;
							fuben[fubennumber - 1] = floors[j - 1][0].top();
							stop[i][0]++;
							map[j - 1][0]--;
							floors[j - 1][0].pop();
						}
					n += 2;
				}
			}
		}
		//电下，人下，人在电梯上
		else if (elevators[i].type==GOING_DOWN&&on_down==GOING_DOWN&& nwf > elevators[i].nowFloor) {
			for (int j = elevators[i].nowFloor; j > 1; j--) {
				if ( elevators[i].nowRiders >= MAX_RIDER)
					n += 0.5;
				else {
					//考虑上电梯的人对电梯是否在某个楼层停止有影响
					while (elevators[i].nowRiders < MAX_RIDER &&!Iflowempty(j)) {
						//找出来开始等待时间最小并且的乘客
						elevators[i].nowRiders++;
						fubennumber++;
						fuben[fubennumber - 1] = floors[j - 1][0].top();
						stop[i][0]++;
						floors[j - 1][0].pop();
						map[j - 1][0]--;
					}
					n += 2;
				}
			}
			n += 2;
			//在一楼上电梯的人
			elevators[i].nowRiders = 0;
			while (elevators[i].nowRiders < 10 && !Ifhighempty(1)) {
				int temmintime = TWENTY_TWO;
				int tem;//最先等待人想要去的楼层
				for (int loop1 = 22; loop1 > 1; loop1--) {
					if (!floors[0][loop1 - 1].empty() && temmintime >= floors[0][loop1 - 1].top().startWaitTime) {
						tem = loop1;
						temmintime = floors[0][loop1 - 1].top().startWaitTime;
					}
				}
				elevators[i].nowRiders++;
				fubennumber++;
				fuben[fubennumber - 1] = floors[0][tem - 1].top();
				stop[i][fuben[fubennumber - 1].endFloor - 1]++;
				floors[0][tem - 1].pop();
				map[0][tem - 1]--;
			}
			//电梯上行
			for (int j = 1; j < nwf; j++) {
				if (stop[i][j - 1] == 0 && (Ifhighempty(j) || elevators[i].nowRiders >= MAX_RIDER))
					n += 0.5;
				else {
					elevators[i].nowRiders = elevators[i].nowRiders - stop[i][j - 1];
					stop[i][j - 1] = 0;
					n += 2;
				}
			}
		}
		//电上，人上，人在电梯下
		else if (elevators[i].type == GOING_ON && on_down == GOING_ON && nwf < elevators[i].nowFloor) {
				for (int j = elevators[i].nowFloor; j < elevators[i].hithest; j++) {
					if (stop[i][j - 1] == 0) {
						n += 0.5;
							}
					else {
						n += 2;
						elevators[i].nowRiders -= stop[i][j - 1];
						stop[i][j - 1] = 0;
					}
				}
				//在最高层上人
				n += 2;
				elevators[i].nowRiders = 0;
				while (elevators[i].nowRiders < 10 && !floors[elevators[i].hithest-1][0].empty()) {
					map[elevators[i].hithest - 1][0]--;
					fubennumber++;
					clerk nw = floors[elevators[i].hithest - 1][0].top();
					stop[i][0]++;
					elevators[i].nowRiders++;
					floors[elevators[i].hithest - 1][0].pop();
					fuben[fubennumber - 1] = nw;
				}
				//电梯下行
				for (int j = elevators[i].hithest; j > nwf; j--) {
					if (elevators[i].nowRiders < 10 && !floors[j - 1][0].empty()) {
						n += 2;
						while (elevators[i].nowRiders < 10 && !floors[j - 1][0].empty()) {
							map[j - 1][0]--;
							fubennumber++;
							clerk nw = floors[j - 1][0].top();
							stop[i][0]++;
							elevators[i].nowRiders++;
							floors[j - 1][0].pop();
							fuben[fubennumber - 1] = nw;
						}
					}
					else
						n += 0.5;
				}
		}
		//电上人下，人在电梯上      待修改
		else if (elevators[i].type == GOING_ON && on_down == GOING_DOWN && nwf > elevators[i].nowFloor) {
			//最后电梯停在人的上面
			if (elevators[i].hithest >= nwf) {
				//上行
				for (int j = elevators[i].nowFloor; j <elevators[i].hithest; j++) {
					if (stop[i][j - 1] == 0) {
						n += 0.5;
					}
					else {
						n += 2;
						elevators[i].nowRiders -= stop[i][j - 1];
						stop[i][j - 1] = 0;
					}
				}
				//在最高层上人
				n += 2;
				elevators[i].nowRiders = 0;
				while (elevators[i].nowRiders < 10 && !floors[elevators[i].hithest - 1][0].empty()) {
					map[elevators[i].hithest - 1][0]--;
					fubennumber++;
					clerk nw = floors[elevators[i].hithest - 1][0].top();
					stop[i][0]++;
					elevators[i].nowRiders++;
					floors[elevators[i].hithest - 1][0].pop();
					fuben[fubennumber - 1] = nw;
				}
				//电梯下行
				for (int j = elevators[i].hithest; j > nwf; j--) {
					if (elevators[i].nowRiders < 10 && !floors[j - 1][0].empty()) {
						n += 2;
						while (elevators[i].nowRiders < 10 && !floors[j - 1][0].empty()) {
							map[j - 1][0]--;
							fubennumber++;
							clerk nw = floors[j - 1][0].top();
							stop[i][0]++;
							elevators[i].nowRiders++;
							floors[j - 1][0].pop();
							fuben[fubennumber - 1] = nw;
						}
					}
					else
						n += 0.5;
				}
			}
			//最后电梯停在人的下面
			else if (elevators[i].hithest < nwf) {
				for (int j = elevators[i].nowFloor; j < nwf; j++) {
					if (stop[i][j - 1] == 0) {
						n += 0.5;
					}
					else {
						n += 2;
						elevators[i].nowRiders -= stop[i][j - 1];
						stop[i][j - 1] = 0;
					}
				}
			}
		}
		//电上人下，人在电梯下
		else if (elevators[i].type == GOING_ON && on_down == GOING_DOWN && nwf <= elevators[i].nowFloor) {
				for (int j = elevators[i].nowFloor; j <= elevators[i].hithest; j++) {
					if (stop[i][j - 1] == 0) {
						n += 0.5;
					}
					else {
						n += 2;
						elevators[i].nowRiders -= stop[i][j - 1];
						stop[i][j - 1] = 0;
					}
				}
				//在最高层上人
				elevators[i].nowRiders = 0;
				while (elevators[i].nowRiders < 10 && !floors[elevators[i].hithest - 1][0].empty()) {
					map[elevators[i].hithest - 1][0]--;
					fubennumber++;
					clerk nw = floors[elevators[i].hithest - 1][0].top();
					stop[i][0]++;
					elevators[i].nowRiders++;
					floors[elevators[i].hithest - 1][0].pop();
					fuben[fubennumber - 1] = nw;
				}
				//电梯下行
				for (int j = elevators[i].hithest; j > nwf; j--) {
					if (elevators[i].nowRiders < 10 && !floors[j - 1][0].empty()) {
						n += 2;
						while (elevators[i].nowRiders < 10 && !floors[j - 1][0].empty()) {
							map[j - 1][0]--;
							fubennumber++;
							clerk nw = floors[j - 1][0].top();
							stop[i][0]++;
							elevators[i].nowRiders++;
							floors[j - 1][0].pop();
							fuben[fubennumber - 1] = nw;
						}
					}
					else
						n += 0.5;
				}
}
//电下人上
		else if (elevators[i].type == GOING_DOWN && on_down == GOING_ON) {
		for (int j = elevators[i].nowFloor; j >= 0; j--) {
			if (stop[i][j - 1] == 0 && (Iflowempty(j) || elevators[i].nowRiders >= MAX_RIDER))
				n += 0.5;
			else {
				elevators[i].nowRiders = elevators[i].nowRiders - stop[i][j - 1];
				stop[i][j - 1] = 0;
				//考虑上电梯的人对电梯是否在某个楼层停止有影响
				while (elevators[i].nowRiders < MAX_RIDER && !Iflowempty(j)) {
					//找出来开始等待时间最小并且的乘客
					elevators[i].nowRiders++;
					fubennumber++;
					fuben[fubennumber - 1] = floors[j - 1][0].top();
					stop[i][0]++;
					floors[j - 1][0].pop();
					map[j - 1][0]--;
				}
				n += 2;
			}
		}
		//在一楼上电梯的人
		elevators[i].nowRiders = 0;
		while (elevators[i].nowRiders < 10 && !Ifhighempty(1)) {
			int temmintime = TWENTY_TWO;
			int tem;//最先等待人想要去的楼层
			for (int loop1 = 22; loop1 > 1; loop1--) {
				if (!floors[0][loop1 - 1].empty() && temmintime >= floors[0][loop1 - 1].top().startWaitTime) {
					tem = loop1;
					temmintime = floors[0][loop1 - 1].top().startWaitTime;
				}
			}
			elevators[i].nowRiders++;
			fubennumber++;
			fuben[fubennumber - 1] = floors[0][tem - 1].top();
			stop[i][fuben[fubennumber - 1].endFloor - 1]++;
			floors[0][tem - 1].pop();
			map[0][tem - 1]--;
		}
		//电梯上行
		for (int j = 1; j < nwf; j++) {
			if (stop[i][j - 1] == 0 && (Ifhighempty(j) || elevators[i].nowRiders >= MAX_RIDER))
				n += 0.5;
			else {
				elevators[i].nowRiders = elevators[i].nowRiders - stop[i][j - 1];
				stop[i][j - 1] = 0;
				n += 2;
			}
		}
}
		elevators[i].nowRiders = origrider;
		for (int ii = 0; ii < 22; ii++) {
			for (int ji = 0; ji < 22; ji++)
				map[ii][ji] = k[ii][ji];
		}
		for (int loop = 0; loop < fubennumber; loop++) {
			int a = fuben[loop].nowFloor;
			int b = fuben[loop].endFloor;
			floors[a - 1][b - 1].push(fuben[loop]);
		}
		if (n < mintime) {
			mintime = n;
			order = i;//电梯编号
		}
		elevators[i].hithest = origdestination;
	}
	for (int i1 = 0; i1 < ELEVATOR_NUMBER; i1++) {
		for (int j1 = 0; j1 < 22; j1++)
			stop[i1][j1] = m[i1][j1];
	}
	return order;
}

double P_Random() {//生成一个0~1之间的随机数
	double f;
	f = (float)(rand());
	return f / RAND_MAX;
}


int possion() {
	int k = 0;
	double u;
	long double p = 1.0;
	long double l = exp(-LAMBDA);
	while (p > l) {
		u = P_Random();
		p *= u;
		k++;
	}
	return k-1;
}


int U_Random() {//生成一个1到22之间的随机数
	int d;
	d = (int)(rand() % 22) + 1;
	return d;
}


int T_Random() {//产生0 1之中的一个随机数
	int t;
	t = (int)(rand() % 2);
	return t;
 }

void Headle(clerk nw) {
	float period = nw.endWaitTime - nw.startWaitTime;
	alltime += period;
	if (period < 3)
		satisfaction++;
	else if (period > 6)
		dissatisfaction++;
	else
		elsenumber++;
	return;
}


