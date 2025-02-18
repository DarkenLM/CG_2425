# build.ps1
# Configures and executes the projects as defined by their CMakeLists.
#
# FLAGS:
#   --debug, -d: Builds the project in debug mode.
#   --verbose, -v: Enables verbose output during the build.
#   --clean: Cleans the build directory before configuring and building.

param (
    [switch]$Debug = $false,
    [switch]$Verbose = $false,
    [switch]$Clean = $false
)

# Function to display usage information
function Show-Usage {
    Write-Host "Usage: .\build.ps1 [--debug] [--verbose] [--clean]"
    Write-Host "  --debug, -d: Builds the project in debug mode."
    Write-Host "  --verbose, -v: Enables verbose output during the build."
    Write-Host "  --clean: Cleans the build directory before configuring and building."
}

$BuildType = if ($Debug) { "DEBUG" } else { "RELEASE" }
Write-Host "Running in $BuildType mode."

# Check for --verbose or -v
if ($Verbose) {
    $FLAGS = "--verbose"
} else {
    $FLAGS = ""
}

# Check for --clean
if ($Clean) {
    Write-Host "Cleaning build directory..."
    Remove-Item -Recurse -Force -Path .\build -ErrorAction SilentlyContinue
}

# Configure the project
Write-Host "Configuring the project..."
cmake -B build -DCMAKE_BUILD_TYPE=$BuildType 

if ($LASTEXITCODE -ne 0) {
    Write-Host "Error: CMake configuration failed."
    exit $LASTEXITCODE
}

# Build the project
Write-Host "Building the project..."
cmake --build build $FLAGS

if ($LASTEXITCODE -ne 0) {
    Write-Host "Error: Build failed."
    exit $LASTEXITCODE
}

Write-Host "Build completed successfully."