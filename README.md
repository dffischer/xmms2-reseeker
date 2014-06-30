# XMMS2-Reseeker

Audiobooks on your XMMS2 playlist? Want to hear that part of the song right again? As long as this small client runs, it seeks back a few seconds whenever the player resumes from a pause so that you can pickup where you paused listening again.

## Installation

Archlinux users have [a package in the AUR](https://aur.archlinux.org/packages/xmms2-reseeker/).

This project uses [waf](https://code.google.com/p/waf/) to compile and install. To start it automatically at xmms2d launch, place a link in the configuration directory.

```bash
waf configure install
ln -s /usr/share/xmms2/scripts/startup.d/xmms2-reseeker.sh $XMMS2_USER/.config/xmms2/startup.d/
```

## Customization

You can also copy the startup script instead of linking it. Add a custom amount of seconds to the end of the `exec` call to alter the default of 5 seconds to skip back.
