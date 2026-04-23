# Sensor Diagnostic Log

This project does not use UART for diagnostics.

The firmware writes a one-shot ASCII diagnostic record into internal flash after
the first BLE connection. The text lives in the internal NVS region:

- Base address: `0x48000`
- Region size: `0x4000`

Those values come from:

- [simple_peripheral.syscfg](./simple_peripheral.syscfg)
- [Release/syscfg/ti_utils_build_linker.cmd.genmap](./Release/syscfg/ti_utils_build_linker.cmd.genmap)

## What gets written

The firmware stores text in this format:

```text
SENSOR_DIAG_LOG_V1
TEST AFE init=1 id=24 st=07 fifo=4 int=0 samp=2 red=12345 ir=23456
TEST ADXL init=1 id=F7 st=07 int=0 xyz=12,-4,980 dyn=18 base=960400
```

Interpretation:

- `id`
  - AFE should be `24` for `MAX86140`
  - ADXL should be `F7` for `ADXL366`
- `st`
  - bit `0x1`: SPI opened
  - bit `0x2`: part ID matched
  - bit `0x4`: live data seen
  - bit `0x8`: interrupt edge seen
- `fifo`, `samp`, `red`, `ir`
  - AFE transport and sample evidence
- `xyz`, `dyn`, `base`
  - ADXL raw sample and motion evidence

## How to export the log

1. Build and flash the project.
2. Connect once over BLE so the one-shot log is written.
3. In CCS, open the Memory Browser.
4. Read memory starting at `0x48000`.
5. Save at least the first `512` bytes as a binary file, for example `sensor_diag_dump.bin`.
6. Run:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\extract_sensor_diag_log.ps1 -DumpPath .\sensor_diag_dump.bin -OutPath .\sensor_diag_log.txt
```

That produces a normal text file containing the latest diagnostic record.
