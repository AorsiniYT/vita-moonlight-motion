# Vita Moonlight Motion

**Version 0.11.4**

This project is a PlayStation Vita port of Moonlight, with major improvements and updates.

## What's New in 0.11.4

- Host status indicator dots: green (online), yellow (IP change pending), red (offline/disconnected).
- Robust host status checking with background scan thread and mDNS sniffer.
- Manual refresh of host status with TRIANGLE (△) in main menu and device search.
- IP change confirmation dialog now always shows old and new IP, and always asks for confirmation.
- Host status and menu only refresh when status actually changes or user requests it.
- Improved debug logging for all host and menu transitions.
- UI and logic are now robust even if host fields are empty or uninitialized.
- All UI messages for search device and refresh are now in English.
- Added screenshots to documentation (see docs/):
  - keyboard.png: Floating keyboard in a Steam app using Moonlight.
  - ip1.png: Host waiting for IP update (yellow).
  - ip2.png: Host online (green).
  - ip3.png: Host offline/disconnected (red).
  - ip4.png: IP change confirmation dialog (shows old/new IP).
  - ip5.png: Search device function showing a found local device.

---

## Credits

- Current improvements and maintenance: **AorsiniYT**
- Based on the work of:
  - [MetalfaceScout/vita-moonlight-motion](https://github.com/MetalfaceScout/vita-moonlight-motion)
  - [xyzz/vita-moonlight](https://github.com/xyzz/vita-moonlight)
  - [moonlight-stream/moonlight-common-c](https://github.com/moonlight-stream/moonlight-common-c)

---

## Build Requirements

- **VitaSDK** installed and configured on your system ([guide here](https://vitasdk.org/)).
- **Submodules updated:**
  ```sh
  git submodule update --init
  ```

---

## Quick Build

1. Install dependencies with [vdpm](https://github.com/vitasdk/vdpm) if you haven't already.
2. Make sure VitaSDK is installed and in your $PATH.
3. Run:
   ```sh
   ./makepsv
   ```
   This will generate a VPK file ready to install on your PS Vita.

---

## Manual Build (optional)

If you prefer to build manually:

```sh
# If you do git pull, make sure to update submodules first
 git submodule update --init
 mkdir build && cd build
 cmake ..
 make
```

---

## Assets

Screenshots and documentation are available in the `docs/` folder.

- `keyboard.png`: Floating keyboard in a Steam app using Moonlight.
- `ip1.png`: Host waiting for IP update (yellow).
- `ip2.png`: Host online (green).
- `ip3.png`: Host offline/disconnected (red).
- `ip4.png`: IP change confirmation dialog (shows old/new IP).
- `ip5.png`: Search device function showing a found local device.

---

## Contributing

1. Fork this repository
2. Write code
3. Send Pull Requests

---

## Note about colors in vita2d

> **Important:** The vita2d library interprets colors in BGRA format (not RGBA). For example:
> - 0xFF00FFFF will appear **yellow** (not cyan)
> - 0xFFFFFF00 will appear **blue** (not yellow)
> - 0xFF00FF00 will appear **green** (correct)
> - 0xFFFF0000 will appear **red** (correct)
>
> If the color does not look as expected, swap the byte order (use BGR instead of RGB).

---

Thanks to all contributors and the Moonlight community!
