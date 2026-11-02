# AstroM-WebCS
Web interface for control system of SAO RAS comples «Astro-M»

## Building
Clone the repo with used submodules:
```bash
git clone --recurse-submodules --shallow-submodules -j 8 https://github.com/t1m-k/astrom-webcs.git
```
If cloned without submodules, get them by:
```bash
git submodule update --init --recursive -j 8 --depth 1
```
Flags `--shallow-submodules` and `--depth 1` are optional, used not to clone all commit history of submodules.

Once successfully cloned, just do nothing, because it is developing now.
