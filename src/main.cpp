#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <queue>

using namespace geode::prelude;

// Struct to hold input data
struct DelayedInput {
    bool isPress;
    int button;
    bool isPlayer2;
    int targetTick;
};

// Global variables for tracking mod state
std::queue<DelayedInput> g_inputQueue;
int g_currentTick = 0;
int g_stage = 1; // 1, 2, 3, 4, 0 (OFF)

int getRequiredTicks(bool isWave) {
    if (g_stage == 0) return 0; // OFF
    if (g_stage == 1) return 2;
    if (g_stage == 2) return 4;
    if (g_stage == 3) return 8;
    if (g_stage == 4) return isWave ? 8 : 16;
    return 0;
}

class $modify(MyBaseGameLayer, GJBaseGameLayer) {
    // This is the core logic function updated to support both Windows and iOS compilation
    void handleButton(bool press, int button, bool isPlayer2) {
        // Check if we are currently using a wave vehicle
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

    // Process commands loop updated to tick at 240Hz safely on mobile engines
    void processCommands(float dt) {
        GJBaseGameLayer::processCommands(dt);
        g_currentTick++;

        while (!g_inputQueue.empty() && g_inputQueue.front().targetTick <= g_currentTick) {
            DelayedInput delayed = g_inputQueue.front();
            g_inputQueue.pop();
            GJBaseGameLayer::handleButton(delayed.isPress, delayed.button, delayed.isPlayer2);
        }
    }
};

// Listen for F6 key logic handles custom stage cycling safely on keyboard/touch combinations
class $modify(MyCCLayer, CCLayer) {
    void keyDown(enumKeyCodes key) {
        if (key == enumKeyCodes::KEY_F6) {
            g_stage = (g_stage + 1) % 5;
            std::string stageText = "Stage: " + std::to_string(g_stage);
            if (g_stage == 0) stageText = "Stage: OFF (Raw Input)";
            FLAlertLayer::create("Predictive Input", stageText, "OK")->show();
            return;
        }
        CCLayer::keyDown(key);
    }
};

