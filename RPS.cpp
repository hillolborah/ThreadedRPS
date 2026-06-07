#include <iostream>
#include <thread>
#include <mutex>
#include <random>

using namespace std;


char Player(){

    char moves[] = {'R', 'P', 'S'};

    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> move(0, 2);

    return moves[move(gen)];

}


int main(){

    char move = Player();

    cout << move << endl;


    return 0;
}