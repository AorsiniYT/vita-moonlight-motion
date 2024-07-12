# Motion Branch

This branch is for fully emulating motion controls for the vita, using DS5 emulation.


# Vita Moonlight

This is a vita port of Moonlight Embedded.
Moonlight is an open source implementation of NVIDIA GameStream.

## Modified

[xyzz]: https://github.com/xyzz/vita-moonlight

[Originally by xyzz][xyzz], this fork of vita-moonlight contains changes by youbobcat to add motion controls.
I've added another option to allow for a double tap to sprint (Press L3).
It also contains bugfixes and aims to be compatible with Linux hosts running Sunshine.

This is also untested and I'm new to developing, so use with caution!

If somehow these features are wanted in the original project, please reach out and I'll be happy to submit pull requests/modify whatever.

## Planned Features (In order of feasibility)

- Adjustable double tap to sprint delay
- Manual option to choose remote or local connection
- Fix for quitting apps
- Fix for pairing to host
- Per app configuration
- In app button remapping

## Changes

- Update moonlight-common-c and enet
- moonlight-common-c now uses pthreads for threading (fix re-entry crash)
- enet has additional definitions for networking
- Disabled video slicing (for now)
- Increase video decoder buffer size (fix for crashing with VA-API)

## Known Issues

- Quitting an app directly after streaming does not work
- Losing connection, then trying to reconnect is buggy
- Pairing from "Resume from <>" dialog does not save paired info
- Pairing from search devices and manual works, but doesn't update the menu requiring a restart

## Documentation

More information can find [moonlight-docs][1], [moonlight-embedded][2], and our [wiki][3].
If you need more help, join the #vita-help channel in [discord][4].

[1]: https://github.com/moonlight-stream/moonlight-docs/wiki
[2]: https://github.com/irtimmer/moonlight-embedded/wiki
[3]: https://github.com/xyzz/vita-moonlight/wiki
[4]: https://discord.gg/atkmxxT

# Build deps

You can install build dependencies with [vdpm](https://github.com/vitasdk/vdpm).

# Build Moonlight

```
# if you do git pull, make sure submodules are updated first
git submodule update --init
mkdir build && cd build
cmake ..
make
```

# Assets

- Icon - [moonlight-stream][moonlight] project logo
- Livearea background - [Moonlight Reflection][reflection] Public domain

[moonlight]: https://github.com/moonlight-stream
[reflection]: http://www.publicdomainpictures.net/view-image.php?image=130014&picture=moonlight-reflection

## See also

[Moonlight-common-c](https://github.com/moonlight-stream/moonlight-common-c) is the shared codebase between different Moonlight implementations

## Contribute

1. Fork us
2. Write code
3. Send Pull Requests
