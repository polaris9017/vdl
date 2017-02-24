#vdl

[![Build Status](https://ci.appveyor.com/api/projects/status/cul1hdta39iys0wl/branch/master?svg=true)](https://ci.appveyor.com/project/qscx9512/vdl/branch/master) [![GitHub license](https://img.shields.io/badge/license-Apache%202-blue.svg)](https://raw.githubusercontent.com/qscx9512/vdl/master/LICENSE)

VLive Video Downloader(a.k.a vdl) is video downloader inspired by [youtube-dl](https://github.com/rg3/youtube-dl)



##Changelog

 - 2.0.0-rc1 (2017.02.08)
1. Change libraries (PugiXML -> Xerxes)   
2. Improve Code Structure
3. Support command line



##Features

 - Download specific video from VLive homepage
 - Download video from VLive channel

##Usage

> vdl [-v | -d] `<url>`

**Options**
-v: Show Version
-d `<URL>`: Download video from URL

or execute directly.



##Planned

 - Support download from
   - Naver TVCast
   - Daum TVPot
   - Facebook
   - Instagram

and more...!



##How to Build

###1. For Windows
---------
**Requires Visual Studio 2015 or latest releases**

1. Clone this git repository to your computer
> git clone https://github.com/qscx9512/vdl.git vdl

2. Open project (vdl.sln)

3. Set Configuration as 'Release' and Platform as 'x86'

4. Build it!

###2. For Mac
---------
coming soon

###3. For Linux
---------
coming soon



##License

This program is licensed under the Apache Software License, Version 2.0.
 
 Used library for this program follows by...



 - Cpr (https://github.com/whoshuu/cpr)
 
 Copyright (c) 2017 Huu Nguyen, licensed under the MIT License
 - Xerces-C++ (https://xerces.apache.org/xerces-c)
 
 Copyright (c) 2015 The Apache Software Foundation, licensed under the Apache Software License, Version 2.0
 - JsonCpp (http://jsoncpp.sourceforge.net)
 
 Copyright (c) 2010 by Baptiste Lepilleur, licensed under the MIT License
 - ICU - International Components for Unicode (http://site.icu-project.org)
 
 Copyright (c) 2016 The Unicode(R) Consortium, licensed under the Unicode license
 - libcurl (https://curl.haxx.se)
 
 Copyright (c) 1996 - 2017, Daniel Stenberg, daniel@haxx.se, and many contributors,  licensed under a MIT/X derivate license. See [here](https://curl.haxx.se/docs/copyright.html).


##Contact

If you have any problems, suggestions or questions, please contact to moonrise917@gmail.com

Your contribution will be great help for developing this.
