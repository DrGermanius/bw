#include <BWAPI.h>
#include <BWAPI/Client.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "bot/Bot.h"

void Play();

void Init();

int main(int argc, char *argv[]) {
    size_t gameCount = 0;
    Bot bot;

    std::thread t1(Init);

    while (!BWAPI::BWAPIClient.connect()) {
        std::this_thread::sleep_for(std::chrono::milliseconds{1000});
    }

    while (BWAPI::BWAPIClient.isConnected()) {
        std::cout << "Waiting for game start\n";
        while (BWAPI::BWAPIClient.isConnected() && !BWAPI::Broodwar->isInGame()) {
            BWAPI::BWAPIClient.update();
        }

        if (BWAPI::BroodwarPtr == nullptr) { break; }

        if (BWAPI::Broodwar->isInGame() && !BWAPI::Broodwar->isReplay()) {

            std::cout << "Playing Game " << gameCount++ << " on map " << BWAPI::Broodwar->mapFileName() << "\n";
            Play();
        }
    }

    return 0;
}

void Init() {
    std::this_thread::sleep_for(std::chrono::milliseconds{1000});
    system("init.bat");
}

void Play() {
    Bot bot;

    while (BWAPI::BWAPIClient.isConnected() && BWAPI::Broodwar->isInGame()) {
        for (const BWAPI::Event &e: BWAPI::Broodwar->getEvents()) {
            switch (e.getType()) {
                case BWAPI::EventType::MatchStart: {
                    bot.onStart();
                    break;
                }
                case BWAPI::EventType::MatchFrame: {
                    bot.onFrame();
                    break;
                }
                case BWAPI::EventType::MatchEnd: {
                    bot.onEnd(e.isWinner());
                    break;
                }
                case BWAPI::EventType::UnitShow: {
                    bot.onUnitShow(e.getUnit());
                    break;
                }
                case BWAPI::EventType::UnitHide: {
                    bot.onUnitHide(e.getUnit());
                    break;
                }
                case BWAPI::EventType::UnitCreate: {
                    bot.onUnitCreate(e.getUnit());
                    break;
                }
                case BWAPI::EventType::UnitMorph: {
                    bot.onUnitMorph(e.getUnit());
                    break;
                }
                case BWAPI::EventType::UnitDestroy: {
                    bot.onUnitDestroy(e.getUnit());
                    break;
                }
                case BWAPI::EventType::UnitRenegade: {
                    bot.onUnitRenegade(e.getUnit());
                    break;
                }
                case BWAPI::EventType::UnitComplete: {
                    bot.onUnitComplete(e.getUnit());
                    break;
                }
                case BWAPI::EventType::SendText: {
                    bot.onSendText(e.getText());
                    break;
                }
                default: {
                    break;
                }
            }
        }

        BWAPI::BWAPIClient.update();
        if (!BWAPI::BWAPIClient.isConnected()) {
            std::cout << "Disconnected\n";
            break;
        }
    }

    std::cout << "Game Over\n";
}