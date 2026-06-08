#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <vector>
#include <future>

using namespace std;

mutex mu1; //mutex for rng engine -> move

struct PlayerMove{
    int Pid; //PlayerId
    char move; //moves R(Rock) | P(Paper) | S(Scissor)
};

PlayerMove Player(int Pid){

    char moves[] = {'R', 'P', 'S'};

    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> uni_dist(0, 2);

    char move;

    {
        lock_guard<mutex> lock(mu1);
        move = moves[uni_dist(gen)];
        cout << "PlayerId : " << Pid << " -> " << "Move : " << move << endl;
    }

    return PlayerMove{Pid, move};

}



int main(){

    cout << "Main Thread" << endl;

    const int totalPlayers = 10;

    // vector<thread> RPSthreads; //Threads cannot store structs, replace with vector of futures
    vector<future<PlayerMove>> RPSFutures;
    vector<PlayerMove> PlayersMoves; //vector of struct PlayerMove

    for(int i = 0; i < totalPlayers; i++){
        // RPSthreads.push_back(thread(Player, i));
        RPSFutures.push_back(async(launch::async, Player, i));
    }

    for(int i = 0; i < totalPlayers; i++){
        // RPSthreads[i].join();
        PlayerMove result = RPSFutures[i].get();
        PlayersMoves.push_back(result);
        cout << "Pid : " << result.Pid << " -> " << "Move : " << result.move <<  endl;
    }

    
    cout << "Struct PlayersMoves : " << endl;
    for (const auto& player : PlayersMoves) {
        cout << "Player ID: " << player.Pid << " -> " << " Move : " << player.move << endl;
    }


    return 0;
}