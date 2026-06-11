#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <vector>
#include <future>
#include <algorithm>

using namespace std;


struct PlayerMove{
    const int Pid; //PlayerId
    char move; //moves R(Rock) | P(Paper) | S(Scissor)
};

PlayerMove Player(int Pid){

    char moves[] = {'R', 'P', 'S'};

    thread_local random_device rd;
    thread_local mt19937 gen(rd());
    uniform_int_distribution<> uni_dist(0, 2);

    char move = moves[uni_dist(gen)];

    return PlayerMove{Pid, move};

}

//Moderator function -> takes in PlayersMoves, match making and deterministic logic

int Fight(char m1, char m2){//0->draw 1->m1 2->m2
    if (m1 == m2) return 0;
    if((m1 == 'R' && m2 == 'S') || (m1 == 'P' && m2 == 'R') || (m1 == 'S' && m2 == 'P')){
        return 1;
    }
    return 2;
}

void Mod(vector<PlayerMove> Players){
    cout << "Mod function" << endl;
    // for (const auto& player : Players) {
    //     cout << "Player ID: " << player.Pid << " -> " << " Move : " << player.move << endl;
    // }

    vector<size_t> idx_vect(Players.size());
    vector<PlayerMove> NextPass;


    if(Players.empty()) return;
    if (Players.size() == 1) {
        cout << "Winner Pid : " << Players[0].Pid << endl;
        return;
    }

    for(size_t i = 0; i < idx_vect.size(); i++){
        idx_vect[i] = i;
    }

    static random_device rd;
    static mt19937 gen(rd());
    shuffle(idx_vect.begin(), idx_vect.end(), gen);


    // for(size_t randIdx : idx_vect){
    //     const auto& player = Players[randIdx];
    //     cout << "Pid : " << player.Pid << " -> Move : " << player.move << endl;
    // }


    for(size_t i = 0; i < idx_vect.size(); i+=2){//iterative +2, match two consecutive players from the vector
        if(i+1 >= idx_vect.size()){
            const auto& OddBall = Players[idx_vect[i]];
            cout << "Last  player odd : " << OddBall.Pid << endl;
            NextPass.push_back(OddBall);
            break;
        }

        PlayerMove p1 = Players[idx_vect[i]];
        PlayerMove p2 = Players[idx_vect[i+1]];
        cout << "Pid : " << p1.Pid << " vs " << "Pid : " << p2.Pid << endl;

        int result = Fight(p1.move, p2.move);

        if(result == 0){ //ToDo: update logic to send both to NextPass?
            cout << "Draw" << endl;
            idx_vect.push_back(idx_vect[i]);
            idx_vect.push_back(idx_vect[i+1]);
        }else if(result == 1){
            cout << "Winner : Pid " << p1.Pid << endl;
            NextPass.push_back(p1);
        }else{
            cout << "Winner : Pid " << p2.Pid << endl;
            NextPass.push_back(p2);
        }

        Mod(NextPass); //recursive matches

    }


}

int main(){

    cout << "Main Thread" << endl;

    const int totalPlayers = 1;

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
        // cout << "Pid : " << result.Pid << " -> " << "Move : " << result.move <<  endl;
    }

    Mod(PlayersMoves);

    return 0;
}