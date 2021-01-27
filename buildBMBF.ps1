# Builds a .zip file for loading with BMBF
& $PSScriptRoot/build.ps1

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/libno-health-and-safety.so", "./bmbfmod.json" -DestinationPath "./no-health-and-safety_v0.1.1.zip" -Update
}
