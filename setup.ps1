# Codesys-Webots Bridge Setup Script
# Run this once to create output directories

New-Item -ItemType Directory -Force -Path "generated"
New-Item -ItemType Directory -Force -Path "generated\codesys"
New-Item -ItemType Directory -Force -Path "generated\webots"

Write-Host "✓ Directories created successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "Now run: .\CodesysWebotsBridge.exe schemas\puma560.json" -ForegroundColor Cyan
