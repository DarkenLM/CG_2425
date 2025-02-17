# clean.ps1
# Cleans the target directory.
#
# FLAGS:
#   --all: Performs a more aggressive cleanup.

param (
    [switch]$All = $false,
    [switch]$Help = $false
)

# Function to display usage information
function Show-Usage {
    Write-Host "Usage: .\clean.ps1 [--all]"
    Write-Host "  --all: DROPPING A FUCKING NUKE."
}

if ($Help) {
    Show-Usage
    exit 0
}

# Check for --all flag
if ($All) {
    Write-Host "DROPPING Little Boy!!!"
    Write-Host "3 2 1..."
    if (Test-Path "build") {
        Write-Host "Removing build directory..."
        Remove-Item -Recurse -Force "build"
        Write-Host "Build directory annihilated."
    } else {
        Write-Host "Build directory does not exist."
    }
} else {
    Write-Host "Performing standard cleanup..."
    if (Test-Path "build") {
        Write-Host "Removing build directory..."
        Remove-Item -Recurse -Force "build"
        Write-Host "Build directory removed."
    } else {
        Write-Host "Build directory does not exist."
    }
}