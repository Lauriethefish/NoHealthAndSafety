#include "main.hpp"

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
MAKE_HOOK_FIND_CLASS_INSTANCE(HealthWarningFlowCoordinator_DidActivate, "", "HealthWarningFlowCoordinator", "DidActivate", void, Il2CppObject* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    getLogger().info("Moving to next scene to bypass health and safety");
    Il2CppObject* gameScenesManager = THROW_UNLESS(il2cpp_utils::GetFieldValue(self, "_gameScenesManager"));
    if(!gameScenesManager) {getLogger().info("nullA");}
    Il2CppObject* initData = THROW_UNLESS(il2cpp_utils::GetFieldValue(self, "_initData"));
        if(!initData) {getLogger().info("nullB");}

    Il2CppObject* nextScenesTransitionSetupData = THROW_UNLESS(il2cpp_utils::GetFieldValue(initData, "nextScenesTransitionSetupData"));
        if(!nextScenesTransitionSetupData) {getLogger().info("nullC");}

    const MethodInfo* methodInfo = THROW_UNLESS(il2cpp_utils::FindMethodUnsafe("", "GameScenesManager", "ReplaceScenes", 4));

    // TODO: This only workds if you manually disable type checking in il2cpp-utils-methods.hpp, relying on this is really bad
    // Pretty sure it's an il2cpp utils bug that prevents you from passing in the arguments I'm giving
    THROW_UNLESS(il2cpp_utils::RunMethod(gameScenesManager, methodInfo, nextScenesTransitionSetupData, 0.0f, (Il2CppObject*) nullptr, (Il2CppObject*) nullptr));
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
    INSTALL_HOOK(getLogger(), HealthWarningFlowCoordinator_DidActivate);
    getLogger().info("Installed all hooks!");
}