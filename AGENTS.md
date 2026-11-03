# Agent guide

ESPHome firmware for two home devices, built with Docker. The human-facing docs are in
[config/README.md](config/README.md).

| Device YAML                 | Node name           | Board                              | What it does                                         |
|-----------------------------|---------------------|------------------------------------|------------------------------------------------------|
| `config/starry-sky.yaml`    | `starry-sky-nash`   | `esp32-s3-devkitc-1`, 16 MB, PSRAM | BLE bridge to a CHINLY star ceiling kit (RGBW + motor) |
| `config/sounder.yaml`       | `sleepy-master`     | `esp32dev`                         | DFPlayer Mini rain/soundscape player                 |

## How the config is split

- `config/<device>.yaml` holds the features. It includes `<device>/config.yaml` first, then `base/common.yaml`.
- `config/<device>/config.yaml` holds identity and hardware: `name`, `friendly_name`, board, pins, the BLE MAC
  reference. Device-only values belong here, not in `secrets.yaml` or `common.yaml`.
- `config/base/common.yaml` holds what every device shares: Wi-Fi (from the git-ignored `base/wifi_networks.yaml`),
  fallback AP, captive portal, logger, Wi-Fi signal sensors, `api` and `ota`.
- `config/secrets.yaml` is shared and git-ignored. Its keys are listed in `config/secrets.yaml.example`; keep that
  file in sync when you add or remove a secret.
- `config/stars/starsBLE.h` builds the 20-byte BLE frames (`StarrySky::turnOn`, `turnOff`, ...). It is pulled in through
  `esphome: includes:`. Keep it header-only and free of ESPHome dependencies so the tests build with plain g++.

## Commands

```bash
docker compose run --rm stars config starry-sky.yaml    # validate (paths are relative to config/)
docker compose run --rm stars compile starry-sky.yaml   # build
g++ -std=c++17 -Wall tests/starrySky.cpp -o tests/starrySky && ./tests/starrySky
```

Validate both device YAMLs after any change to `base/common.yaml`.

## Rules

- Never print, copy or commit the contents of `secrets.yaml`, `base/wifi_networks.yaml`, the `.device-*` files or
  `config/starrySky/` (legacy secrets). They are git-ignored. Redact keys and passwords in any output you show.
- Don't change `name` in a device's `config.yaml`. OTA and the `<name>.local` address depend on it.
- Don't change `api: encryption: key`. Home Assistant would need to adopt the device again. OTA uses a bare
  `encryption:` block that reuses the API key.
- Keep entity names and IDs as they are. Home Assistant history and dashboards depend on them.
- Don't flash, upload or run OTA (`run`, `upload`, `--device`) unless the user asks for it. `config` and `compile` are
  safe.
- `config/.esphome/` is ESPHome's build cache. Don't edit it. `config/.git` belongs to the ESPHome Device Builder;
  leave it alone.
