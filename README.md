nvresolution
=========

NVResolution: NVIDIA Video BIOS Hack

Nowadays, many people use their computers with wide-screen monitors.
But, these resolutions aren't the VESA standards, the drivers deal with them by themselves.

Unfortunately, in a text-mode framebuffer, NVIDIA's proprietary drivers for Linux supported the VESA resolutions only. Therefore, we get poor resolutions when we using NVIDIA's proprietary drivers.

nvresolution is a tool to modify the video BIOS of the NVIDIA GPU.
It modifies the video BIOS, replaces a resolution mode into your specificed resolution, in order to use the native resolutions of your monitors.

Build into GRUB 2
-------------------

* Create a sub-directory called "grub-extras" of your grub2 checkout
* git checkout grub, at your nvresolution checkout
* copy nvresolution checkout into grub-extras
* Export GRUB\_CONTRIB environment variable to point to grub-extras.
* Build GRUB as usual (./autogen.sh; make).

```
git clone git://git.savannah.gnu.org/grub.git
cd grub
git clone git://github.com/biergaizi/nvresolution.git
cd nvresolution
git checkout grub
cd ../
mkdir grub-extras
cp nvresolution grub-extras/ -R
export GRUB_CONTRIB="$(pwd)/grub-extras/"
./autogen.sh
./configure
make
sudo make install
# grub-install to install GRUB to MBR and /boot
```

Usage
--------
    Usage: ./nvresolution [width] [height]

    root is required.

Check If It Works
------------------
1. make
2. Install `hwinfo`
3. `hwinfo --framebuffer`, get a list of available resolution.
4. `./nvresolution [width] [height]`
5. `hwinfo --frambuffer` to see whether the new resolution appears in the list.
    * If so, following the next steps to hack the resolution every time you boot.
    * Otherwise, please report an issue.

How To Get a High Resolution
--------------------------------

1. Build nvresolution into GRUB
2. Install GRUB
3. Execute nvresolution every time you boot (add `nvresolution 1440 900` to grub.cfg)
3. Check whether you have `uvesafb` kernel module
   * https://wiki.archlinux.org/index.php/Uvesafb
   * http://wiki.gentoo.org/wiki/Uvesafb
4. If uvesafb is built into the kernel,
    * add kernel parameter: `video=uvesafb:1440x900-32,mtrr:3,ywrap`, don't forget to replace 1440x900 to your resolution.
5. If uvesafb is a kernel module,
    * `echo "options uvesafb mode_option=1440x900-32 scroll=ywrap" > /etc/modprobe.d/uvesafb.conf`

If you can't rebuild GRUB:

1. `make static`, you got a static version of nvresolution
2. https://wiki.archlinux.org/index.php/Uvesafb#Uvesafb\_and\_915resolution
   * Don't forget to replace all 915resolution (and its usage) into nvresolution

Supported Hardwares
-------------------
It should work on GTX 400 and newer GPUs. But only tested with my
GTX 460.

It must unlock PAM registers to modify the VBIOS memory. The method to unlocking
PAM registers is depends on CPUs.

It should works on Intel Core i-series processors Gen1/Gen2/LGA2011. But only tested
with my Core i7 2600K (belongs to Gen2).

Core i-series processors Gen3 and newer processors are not supported.
AMD processors are not supported, because I don't have manuals or hardwares.

Pull Requests are welcomed.

Safety
---------
It modifies the video BIOS?! Yes, but the video BIOS it modifies which is mapped into the RAM. It means the modifications of the BIOS are transient. The modification will lose after shutting down.
There is no risk of permanent modification of the BIOS. It also means that nvresolution must be run every time the computer boots.

If it breaks your graphics, just reboot your computers.

But, please keep in mind:

This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Credits
---------
* 915Resolution - The source of all the resolution hacks
  http://915resolution.mango-lang.org/

* DieBuche - The original work on hacking legacy NVIDIA/ATI VBIOS
  http://www.insanelymac.com/forum/topic/211294-information-on-vesa-modes-in-atinvidia-bios/

* P.S: my `nvesabios` tool
  https://github.com/biergaizi/nvesabios

* Clover
  http://sourceforge.net/projects/cloverefiboot/

* pene - Reverse Engineering on NVIDIA VBIOS (READ IT IF YOU WANT TO JOIN ME)
  http://www.projectosx.com/forum/index.php?showtopic=2562&st=2840&p=22683&#entry22683

* Chameleon
  http://forge.voodooprojects.org/p/chameleon

* dmazar - Providing the information about unlocking PAM on newer CPUs
  http://www.insanelymac.com/forum/topic/284296-solved-resolution-hack-for-1st-gen-intel-hd-graphics/

* GNU GRUB
  http://www.gnu.org/software/grub/

* VBIOS images submitted by Hackintosh users
