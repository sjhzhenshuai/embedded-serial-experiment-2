param(
  [string]$KeilUv4 = ""
)
$ErrorActionPreference = "Stop"
$project = Join-Path $PSScriptRoot "MDK-ARM\STM32_USART_DMA_Interrupt.uvprojx"
$outHex = Join-Path $PSScriptRoot "MDK-ARM\Objects\STM32_USART_DMA_Interrupt.hex"
if ([string]::IsNullOrWhiteSpace($KeilUv4)) {
  $cmd = Get-Command UV4.exe -ErrorAction SilentlyContinue
  if ($cmd) { $KeilUv4 = $cmd.Source }
}
if ([string]::IsNullOrWhiteSpace($KeilUv4) -or !(Test-Path -LiteralPath $KeilUv4)) {
  throw "未找到 Keil UV4.exe。请安装 Keil MDK 后运行：powershell -ExecutionPolicy Bypass -File .\build_hex.ps1 -KeilUv4 'C:\Keil_v5\UV4\UV4.exe'"
}
& $KeilUv4 -b $project -j0
if (!(Test-Path -LiteralPath $outHex)) { throw "Keil 返回后未找到 HEX：$outHex" }
Write-Host "HEX 已生成：$outHex"
