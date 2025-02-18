###################################################################################
# run.ps1
# Runs the built executables.
#
# FLAGS:
#   --debug, -d: Rebuilds the project in debug mode.
###################################################################################

# Function to parse arguments
function Get-Args {
    param (
        [string[]]$Arguments
    )
    $argsHash = @{}
    for ($i = 0; $i -lt $Arguments.Length; $i++) {
        if ($Arguments[$i] -match '^--') {
            $argsHash[$Arguments[$i]] = $true
        } elseif ($Arguments[$i] -match '^-') {
            $argsHash[$Arguments[$i]] = $true
        } else {
            $argsHash["_"] = $Arguments[$i]
        }
    }
    return $argsHash
}

# Function to check if a parameter exists
function Test-Param {
    param (
        [string]$Param
    )
    return $argsHash.ContainsKey($Param)
}

# Function to run the executable
function Run {
    $executable = $argsHash["_"]
    $arguments = $argsHash.Keys | Where-Object { $_ -ne "_" } | ForEach-Object { "$_ $($argsHash[$_])" }
    $command = ".\build\out\debug\bin\$executable $arguments"
    Write-Host "Running: $command"
    Invoke-Expression $command
}

# Function to build and run the project
function BuildAndRun {
    if (Test-Param "--debug" -or Test-Param "-d") {
        Write-Host "Building in DEBUG mode."
        cmake -B build -DCMAKE_BUILD_TYPE="Debug"
        cmake --build build --target $argsHash["_"]
    } else {
        Write-Host "Building in RELEASE mode."
        cmake -B build -DCMAKE_BUILD_TYPE="Release"
        cmake --build build --target $argsHash["_"]
    }
    Run
}

# Main script logic
$argsHash = Get-Args -Arguments $args

if (-not $argsHash.ContainsKey("_")) {
    Write-Host "Error: No executable specified."
    Write-Host "Usage: .\run.ps1 <executable> [--debug]"
    exit 1
}

BuildAndRun