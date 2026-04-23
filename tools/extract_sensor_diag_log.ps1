param(
    [Parameter(Mandatory = $true)]
    [string]$DumpPath,

    [string]$OutPath = "sensor_diag_log.txt"
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $DumpPath)) {
    throw "Dump file not found: $DumpPath"
}

$bytes = [System.IO.File]::ReadAllBytes((Resolve-Path -LiteralPath $DumpPath))

if ($bytes.Length -eq 0) {
    throw "Dump file is empty: $DumpPath"
}

$endIndex = [Array]::IndexOf($bytes, [byte]0)
if ($endIndex -lt 0) {
    $endIndex = $bytes.Length
}

$text = [System.Text.Encoding]::ASCII.GetString($bytes, 0, $endIndex).Trim()

if ([string]::IsNullOrWhiteSpace($text)) {
    throw "No ASCII diagnostic payload found in $DumpPath"
}

[System.IO.File]::WriteAllText($OutPath, $text + [Environment]::NewLine, [System.Text.Encoding]::ASCII)
Write-Host "Wrote diagnostic log to $OutPath"
