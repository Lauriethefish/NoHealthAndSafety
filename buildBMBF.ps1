# Builds a .zip file for loading with BMBF
& $PSScriptRoot/build.ps1

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/libno-health-and-safety.so", "./libs/arm64-v8a/libbeatsaber-hook_1_0_9.so", "./libs/arm64-v8a/libcodegen_0_5_3.so", "./bmbfmod.json" -DestinationPath "./no-health-and-safety_v0.1.1.zip" -Update
}
