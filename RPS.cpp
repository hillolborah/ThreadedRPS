#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <vector>

using namespace std;


char Player(int PlayerNo){

    char moves[] = {'R', 'P', 'S'};

    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> move(0, 2);

    cout << "PlayerNo : " << PlayerNo << endl;
    return moves[move(gen)];

}


int main(){

    cout << "main thread" << endl;

    vector<thread> RPSthreads;
    for(int i = 0; i < 10; i++){
        RPSthreads.push_back(thread(Player, i));
        RPSthreads[i].join();
    }




    return 0;
}