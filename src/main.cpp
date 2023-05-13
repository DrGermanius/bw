#include <BWAPI.h>
#include <BWAPI/Client.h>
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char *argv[]) {
    size_t gameCount = 0;

    // if we are not currently connected to BWAPI, try to reconnect
    while (!BWAPI::BWAPIClient.connect()) {
        std::this_thread::sleep_for(std::chrono::milliseconds{1000});
    }

    // if we have connected to BWAPI
    while (BWAPI::BWAPIClient.isConnected()) {
        // the starcraft exe has connected but we need to wait for the game to start
        std::cout << "Waiting for game start\n";
        while (BWAPI::BWAPIClient.isConnected() && !BWAPI::Broodwar->isInGame()) {
            BWAPI::BWAPIClient.update();
        }

        // Check to see if Starcraft shut down somehow
        if (BWAPI::BroodwarPtr == nullptr) { break; }

        // If we are successfully in a game, call the module to play the game
        if (BWAPI::Broodwar->isInGame()) {
            if (!BWAPI::Broodwar->isReplay()) {
                std::cout << "Playing Game " << gameCount++ << " on map " << BWAPI::Broodwar->mapFileName() << "\n";
            } else {
                std::cout << "Parsing Replay " << gameCount++ << " on map " << BWAPI::Broodwar->mapFileName() << "\n";
            }
        }
    }

    return 0;
}