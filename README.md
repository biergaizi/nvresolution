nvresolution
=========

NVResolution: NVIDIA Video BIOS Hack

Nowadays, many people use their computers with wide-screen monitors.
But, these resolutions aren't the VESA standards, the drivers deal with them by themselves.

Unfortunately, in a text-mode framebuffer, NVIDIA's proprietary drivers for Linux supported the VESA resolutions only. Therefore, we get poor resolutions when we using NVIDIA's proprietary drivers.

nvresolution is a tool to modify the video BIOS of the NVIDIA GPU.
It modifies the video BIOS, replaces a resolution mode into your specificed resolution, in order to use the native resolutions of your monitors.

Usage
--------
    Usage: ./nvresolution [width] [height]

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
