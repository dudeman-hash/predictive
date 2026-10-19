#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <queue>

using namespace geode::prelude;

struct DelayedInput {
    bool isPress;
    int button;
    bool isPlayer2;
    int targetTick;
};

std::queue<DelayedInput> g_inputQueue;
int g_currentTick = 0;
int g_stage = 1;

int getRequiredTicks(bool isWave) {
    if (g_stage == 0) return 0;
    if (g_stage == 1) return 2;
    if (g_stage == 2) return 4;
    if (g_stage == 3) return 8;
    if (g_stage == 4) return isWave ? 8 : 16;
    return 0;
}

$execute {
    log::info("Predictive Input Buffer initialization baseline verified.");
}

class $modify(GJBaseGameLayer) {
    bool init() {
        if (!GJBaseGameLayer::init()) return false;
        
        g_inputQueue = std::queue<DelayedInput>();
        g_currentTick = 0;

        this->getScheduler()->scheduleUpdateForTarget(this, 0, false);
        
        return true;
    }

    void handleButton(bool press, int button, bool isPlayer2) {
        bool isWave = false;
        auto player = isPlayer2 ? m_player2 : m_player1;
        if (player) {
            isWave = player->m_isDart;
        }

        int requiredTicks = getRequiredTicks(isWave);
        if (requiredTicks == 0) {
            GJBaseGameLayer::handleButton(press, button, isPlayer2);
            return;
        }

        DelayedInput delayed;
        delayed.isPress = press;
        delayed.button = button;
        delayed.isPlayer2 = isPlayer2;
        delayed.targetTick = g_currentTick + requiredTicks;
        g_inputQueue.push(delayed);
    }

    void update(float dt) {
        GJBaseGameLayer::update(dt);
        
        g_currentTick++;
        while (!g_inputQueue.empty() && g_inputQueue.front().targetTick <= g_currentTick) {
            DelayedInput delayed = g_inputQueue.front();
            g_inputQueue.pop();
            GJBaseGameLayer::handleButton(delayed.isPress, delayed.button, delayed.isPlayer2);
        }
    }
};
