#include "main.hpp"

#include "GlobalNamespace/HealthWarningFlowCoordinator.hpp"
#include "GlobalNamespace/GameScenesManager.hpp"
#include "GlobalNamespace/HealthWarningFlowCoordinator_InitData.hpp"
using namespace GlobalNamespace;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Move to the next scene upon health and safety loading
MAKE_HOOK_OFFSETLESS(HealthWarningFlowCoordinator_DidActivate, void, HealthWarningFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    getLogger().info("Moving to next scene to bypass health and safety");
    self->gameScenesManager->ReplaceScenes(self->initData->nextScenesTransitionSetupData, 0.0f, nullptr, nullptr);
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    getLogger().info("Installing hooks...");

    // Install our hooks
    INSTALL_HOOK_OFFSETLESS(getLogger(), HealthWarningFlowCoordinator_DidActivate, il2cpp_utils::FindMethodUnsafe("", "HealthWarningFlowCoordinator", "DidActivate", 3));
    getLogger().info("Installed all hooks!");
}