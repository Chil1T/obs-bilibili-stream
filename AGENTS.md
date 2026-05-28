# Repository Guide

This repository is a GPL-licensed fork of `Zarosmm/obs-bilibili-stream`.

## Scope
- Keep upstream attribution visible in README and release notes.
- Keep changes focused on OBS/Bilibili streaming workflow, packaging, and installer quality.
- Prefer small patches that can still be compared against upstream.

## Build And Package
- Windows local build uses CMake preset `windows-x64`.
- CI build and release packaging are driven by `.github/workflows/push.yaml`.
- Windows releases should provide both a manual `.zip` and an automatic `.exe` installer.

## Validation
- For code changes, run the smallest available CMake configure/build check for the touched platform.
- For release changes, inspect GitHub Actions YAML and packaging scripts for artifact names before tagging.
- If a full OBS runtime check is unavailable, state that explicitly and list the manual OBS verification path.

## Done Criteria
- The plugin still builds for the target platform.
- Starting Bilibili live writes RTMP settings to OBS when possible.
- The fallback dialog shows RTMP address and stream key separately with individual copy actions.
- Windows release artifacts include an installer that places files under OBS plugin directory.
