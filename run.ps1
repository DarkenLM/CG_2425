###################################################################################
# run.ps1
# Runs the built executables.
#
# FLAGS:
#   --debug, -d: Rebuilds the project in debug mode (does nothing with --no-make).
#   --no-make: Runs the executable directly.
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
    $command = ".\build\out\bin\Debug\$executable $arguments"
    Invoke-Expression $command
}

# Function to run with CMake
function rwc {
    if (Test-Param "--debug" -or Test-Param "-d") {
        Write-Host "Running in DEBUG mode."
        cmake -B build -DCMAKE_BUILD_TYPE=Debug
        cmake --build build --target $argsHash["_"]
        Run
    } else {
        Write-Host "Running in RELEASE mode."
        cmake -B build -DCMAKE_BUILD_TYPE=Release
        Run
    }
}

# Main script logic
$argsHash = Get-Args -Arguments $args

if (Test-Param "--no-make") {
    Run
} else {
    rwc
}