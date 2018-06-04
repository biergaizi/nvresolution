#DeleteGithub
=========
**Deleted**. This project is moved to [notabug.org](https://notabug.org/niconiconi/nvresolution), welcome to join the free world!

NVResolution: NVIDIA Video BIOS Hack

Nowadays, many people use their computers with wide-screen monitors.
But, these resolutions aren't the VESA standards, the video drivers/framebuffers deal with them by themselves.

Unfortunately, NVIDIA's proprietary drivers for Linux don't provide framebuffer drivers. We have to use VESA compatible framebuffer and standard VESA resolution only. Therefore, we get poor resolutions when we using NVIDIA's proprietary drivers. Or, we could use Nouveau and get great resolutions with nouveaufb, but the performance is poor.

No, you can't use both of them. So, Nvidia users on GNU/Linux Platform are painful.

But, nvresolution changed the situation. nvresolution is a tool to hotpatch the video BIOS of the NVIDIA GPU.
It modifies the video BIOS, replaces a resolution mode into your specificed resolution, in order to use the native resolutions of your monitors.

See Also
-------------
* my [nvesabios](https://github.com/biergaizi/nvesabios) tool - It shows the old way to analytics and modify the VBIOS.
