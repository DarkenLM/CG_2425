# build.ps1
# Configures the projects as defined by their CMakeLists.
#
# FLAGS:
#   --debug, -d: Builds the project in debug mode.
#   --verbose, -v: Enables verbose output during the configuration.

param (
    [switch]$Debug = $false,
    [switch]$Verbose = $false
)

# Function to display usage information
function Show-Usage {
    Write-Host "Usage: .\build.ps1 [--debug] [--verbose]"
    Write-Host "  --debug, -d: Configures the project in debug mode."
    Write-Host "  --verbose, -v: Enables verbose output during the configuration."
}

# Check for --debug or -d flag
$BuildType = if ($Debug) { "DEBUG" } else { "RELEASE" }
Write-Host "Running in $BuildType mode."

# Check for --verbose or -v flag
if ($Verbose) {
    $FLAGS = "--trace-expand"
} else {
    $FLAGS = ""
}

# Configure the project
Write-Host "Configuring the project..."
cmake $FLAGS -B build -DCMAKE_BUILD_TYPE=$BuildType

if ($LASTEXITCODE -ne 0) {
    Write-Host "Error: CMake configuration failed."
    exit $LASTEXITCODE
}

Write-Host "Configuration completed successfully."