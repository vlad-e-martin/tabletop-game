#include <gui/GameEnvironment.h>

int main() {
    // Add 2 goblins to the game environment
    std::vector<GUI::NpcType> npcList = { GUI::NpcType::Goblin, GUI::NpcType::Goblin };
    GUI::GameEnvironment gameEnv;

    gameEnv.beginGameCycle(GUI::CharType::Wizard, npcList);

    return EXIT_SUCCESS;
}