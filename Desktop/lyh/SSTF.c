#include<math.h>
#include"joint.h"
#include"request.h"

req* request;
struct Bus bus;
int TOTAL_STATION=5,STRATEGY=1,DISTANCE=2,stop=0,totDistance=20,i=0;
int *clockwise;
int *counterclockwise;
int getBy_aim[31];//处理完顺便服务的aim后去完成原定的目标

int regain_aim(int pos,int *isLifted_aim){//重新获得目标,isLifted_aim不为0->目标为isLifted_aim
    int min_distance = 10000,aim=-1;
    if(i!=0) {
        aim = isLifted_aim[i-1];
        isLifted_aim[i-1]=0;
        i--;
    }
    else{ 
        for(req* p = request->next; p != NULL; p = p->next){
            if(abs(p->aim - pos) < min_distance) {
                aim = p->aim;
                min_distance = abs(p->aim-pos);
            }
        }
    }
    return aim;
}

int direction_decision(int aim){//传入车位置和目标站点，决定此时执行请求的方向
    int aim_pos = aim * DISTANCE;
    if(bus.position > aim_pos){
        if( (DISTANCE * TOTAL_STATION) + aim_pos - bus.position > bus.position - aim_pos) return 0;//逆时针更近
        else return 1;
    }
    else{
        if( (DISTANCE * TOTAL_STATION) + bus.position - aim_pos > aim_pos - bus.position) return 0;//逆时针更近
        else return 1;
    }
}

int getBy(int totDistance, int distance){//返回当前bus->aim
    req* end = request->next;
    while(end->next != NULL) end = end->next;
    int flag = direction_decision(end->aim);
    int endPos = (bus.aim - 1) * 3;
    int aimPos = (end->aim - 1) * 3;
    if(bus.direction == flag && abs(endPos - bus.position) < abs(aimPos - bus.position) && bus.position != endPos){
        return bus.aim;
    }
    else return 0;
}

void SSTF(){
    /*
        1>到站->是否是目标(否->return,是->执行clock下的函数)
        2>有请求->对尾结点判断是否顺便服务
    */
    req* end = request->next;
    while(end->next != NULL) end = end->next;
    if(request->next == NULL) return;//停止在某一站点，无请求
    else{
        if(bus.position % DISTANCE == 0){//到站
            int station = bus.position / DISTANCE;
            if(counterclockwise[station] == 1 || bus.target[station] == 1 || clockwise[station] == 1){//到目标地点
                if(bus.isStop == 1){
                    delAimx(request->next, station, 1);
                    delAimx(request->next, station, 2);
                    delAimx(request->next, station, 3);
                    int new_aim = regain_aim(station, getBy_aim);
                    if(new_aim != -1){
                        bus.aim = new_aim;
                        bus.direction = direction_decision(new_aim);
                        if(counterclockwise[station] == 1) counterclockwise[station] = 0;
                        if(clockwise[station] == 1) clockwise[station] = 0;
                        if(bus.target[station] == 1) bus.target[station] = 0;
                    }
                    else {
                        bus.aim = -1;
                        bus.direction = -1;
                    }
                    bus.isStop = 0;
                }
            else bus.isStop = 1;
            }
        }
        getBy_aim[i] = getBy(DISTANCE*TOTAL_STATION, DISTANCE);
        if(getBy_aim[i] == 0) return;
        else{
            bus.aim = end->aim;
            i++;
        } 
    }    
    return;
}