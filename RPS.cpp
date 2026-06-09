#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <vector>
#include <future>
#include <algorithm>

using namespace std;

mutex mu1; //mutex for rng engine -> move

struct PlayerMove{
    const int Pid; //PlayerId
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

//Moderator function -> takes in PlayersMoves, match making and deterministic logic
//MatchMaking function -> line 52 and line 57 loop made into a callable function

void Mod(vector<PlayerMove> Players){
    cout << "Struct PlayersMoves : " << endl;
    // for (const auto& player : Players) {
    //     cout << "Player ID: " << player.Pid << " -> " << " Move : " << player.move << endl;
    // }

    if(Players.empty()) return;

    vector<size_t> idx_vect(Players.size());
    for(size_t i = 0; i < idx_vect.size(); i++){
        idx_vect[i] = i;
    }

    static random_device rd;
    static mt19937 gen(rd());
    shuffle(idx_vect.begin(), idx_vect.end(), gen);


    for(size_t randIdx : idx_vect){
        const auto& player = Players[randIdx];
        cout << "Pid : " << player.Pid << " -> Move : " << player.move << endl;
    }


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

    Mod(PlayersMoves);

    return 0;
}