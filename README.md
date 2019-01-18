# Exefs patches

Patches are meant to be used with [Atmosphère](https://github.com/Atmosphere-NX/Atmosphere) loader.
Simply copy the relevant folder into `/atmosphere/exefs_patches`

## wildcard_whitelist_libwebapplet

This patch enables navigation to any URL in the [online webapplet (100A)](https://switchbrew.org/wiki/Internet_Browser#010000000000100A).
Example of usage is launching the Puyo Puyo Tetris jp demo or game ("ぷよぷよ™テトリス®Ｓ"), click on the "http://sega.jp" link in the game manual, and clicking an external link or setting up a redirection of the "sega.jp" homepage with your own DNS or Proxy.
Detailed instructions can be found [here](https://switchbrew.org/wiki/Internet_Browser#WebApplet_launch_with_Tetris).

### Versions supported

Fw 5.1, 6.0 and 6.1.

### Notes

Webapplets that are subject to go online load a whitelist text file that details which URL(s) it can navigate to (iframe are not affected). The whitelist file format is a plain text list of regular expressions separated by line-feeds ("\n"), a line starting by `"---- "` will blacklist all url matching the following regex. The online webapplet mounts the caller app own manual-type NCA and reads the following file inside: `"/accessible-urls/accessible-urls.txt"`, which is meant to embed the whitelist rules.

The patch operates in whitelist loading function by replacing the `"---- "` string used as an exclusion pattern by `"|.*?\n"` which acts as a wildcard for regular expressions, and then appends that wildcard pattern to all lines loaded, meanwhile also disabling the blacklist handling. This effectively allows any URL, given that at least one line was present in the whitelist file.

# Applet calls poc

`libnx_hb` contains structs and functions that should allow to display arbitrary urls with the WifiWebAuthApplet and the regular online Webapplet.

Special thanks and credits for the WifiWebAuthApplet poc goes to [misson20000](https://github.com/misson20000) for their [ilia](https://github.com/misson20000/ilia) and the ipc exchange logs of WifiWebAuthApplet launch made with it.

**Notes:***
	* The online webapplet launch is not functional yet and is likely to crash as it will try to fetch the whitelist from a manual NCA on the title you're currently running. It will require patching or installation of custom NSPs.
	* The WifiWebAuthApplet will not display videos on >= 3.0.0 fws. More information on the subject can be found [here](https://switchbrew.org/wiki/Internet_Browser#WifiWebAuthApplet).