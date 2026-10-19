#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <queue>

using namespace geode::prelude;

struct DelayedInput {
    bool isPress;
    int playerAction;
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
    log::info("Predictive Input Buffer loaded successfully.");
}

class $modify(PlayerObject) {
    void pushButton(PlayerButton btn) {
        int requiredTicks = getRequiredTicks(m_isDart);
        if (requiredTicks == 0) {
            PlayerObject::pushButton(btn);
            return;
        }

        DelayedInput delayed;
        delayed.isPress = true;
        delayed.playerAction = static_cast<int>(btn);
        delayed.targetTick = g_currentTick + requiredTicks;
        g_inputQueue.push(delayed);
    }

    void releaseButton(PlayerButton btn) {
        int requiredTicks = getRequiredTicks(m_isDart);
        if (requiredTicks == 0) {
            PlayerObject::releaseButton(btn);
            return;
        }

        DelayedInput delayed;
        delayed.isPress = false;
        delayed.playerAction = static_cast<int>(btn);
        delayed.targetTick = g_currentTick + requiredTicks;
        g_inputQueue.push(delayed);
    }

    void update(float dt) {
        PlayerObject::update(dt);
        g_currentTick++;

        while (!g_inputQueue.empty() && g_inputQueue.front().targetTick <= g_currentTick) {
            DelayedInput delayed = g_inputQueue.front();
            g_inputQueue.pop();
            
            PlayerButton btn = static_cast<PlayerButton>(delayed.playerAction);
            if (delayed.isPress) {
                PlayerObject::pushButton(btn);
            } else {
                PlayerObject::releaseButton(btn);
            }
        }
    }
};
