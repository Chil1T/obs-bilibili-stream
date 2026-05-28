# OBS Bilibili Stream Fork Plan

## Goal

Build and publish a community-friendly fork of `Zarosmm/obs-bilibili-stream` that reduces manual setup after opening a Bilibili live room and provides an automatic Windows installer.

## Current Fork Scope

- Automatically write the returned Bilibili RTMP server and stream key into OBS streaming settings after `startLive` succeeds.
- Keep a fallback dialog that separates RTMP address and stream key, supports selection, and has individual copy buttons.
- Package Windows releases as both:
  - `.zip` for manual OBS plugin installation.
  - `.exe` installer for automatic installation into `C:\ProgramData\obs-studio\plugins`.
- Preserve GPL license and upstream attribution.

## Implementation Notes

- OBS service settings use `rtmp_custom` with `server`, `key`, and `use_auth=false`.
- The plugin calls `obs_frontend_set_streaming_service()` and `obs_frontend_save_streaming_service()` after Bilibili returns RTMP data.
- If OBS service creation fails, the plugin still stores the RTMP data in its config and shows the split-copy dialog.
- Windows installer generation uses Inno Setup in CI through `.github/actions/package-plugin/action.yaml`.

## Release Convention

- Keep semantic version tags such as `2.1.1` or prerelease tags such as `2.1.1-beta1`.
- GitHub Actions creates draft releases for valid version tags.
- Release assets should include checksums, Windows zip, Windows installer exe, macOS pkg/tarball, and Linux deb/tarball where the platform builds succeed.

## Manual OBS Verification

1. Install the Windows `.exe` artifact while OBS is closed.
2. Start OBS and confirm the `Bilibili直播` menu appears.
3. Log in, update room settings, and click `开始直播`.
4. Confirm OBS `设置 -> 直播` is changed to a custom RTMP service using the Bilibili server and stream key.
5. Confirm the success dialog displays address and stream key separately and each copy button places only the intended value on the clipboard.

## Known Constraints

- Bilibili API behavior and OBS service APIs must be validated inside a real OBS runtime.
- GitHub publishing still requires configuring an `origin` remote owned by the fork maintainer and pushing tags.
- macOS signing/notarization depends on repository secrets; unsigned local macOS artifacts may still build, but notarized releases need Apple credentials.
