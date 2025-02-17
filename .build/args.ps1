# has_param function to check if certain flags exist in the arguments
function has_param {
    param (
        [int]$n,        # number of terms to look for
        [array]$terms,   # terms to search in arguments
        [array]$arguments  # array of arguments
    )

    for ($i = 0; $i -lt $n; $i++) {
        $shift
    }
    
    foreach ($arg in $arguments) {
        foreach ($term in $terms) {
            if ($arg -eq $term) {
                return $true
            }
        }
    }

    return $false
}

# get_args function to extract arguments that are not flags
function get_args {
    param (
        [ref]$outputArgs,  # array to hold the extracted args
        [array]$allArgs    # all the arguments passed to the function
    )

    foreach ($arg in $allArgs) {
        if ($arg -notmatch '^-.+') {
            $outputArgs.Value += $arg
        }
    }
}
