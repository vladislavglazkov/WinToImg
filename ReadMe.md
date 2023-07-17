# COPYRIGHT NOTICE

Copyright 2023 Vladislav Glazkov

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

# DISCLAIMER

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# OVERVIEW
The "WTI" package allows to run apps and capture its contents off-screen by maintaining and managing a hidden RDP session.

# PREREQUISITES
- Windows Server only
- RDS server should be serviceable and at least one additional license should be available

# INSTALLATION
- Run the WTI-Installer1.msi file from the package. **Important: installation must be run by a user privileged to create other domain users (it's not possible to run the installation as a standard user and pass UAC check later)**
- Specify the domain name, name for the new user and virtual screen size

# HOW TO USE

In the package, there is a .NET Framework library *WTIConnect.dll*, implementing the namespace *WTIConnect* with the single static class *WTI* having a static method defined as follows:

*static async Task<Bitmap> GetBitmap(string cmdline,int xSize,int ySize,int delay)*


You need to pass
- the command-line string used to create the new application
- requested horizontal size of the window 
- requested vertical size of the window 
- the delay since the window would have been detected to the moment it would be captured (if set too low, not all the contents might draw up properly)

# LIMITATIONS AND KNOWN BUGS

- Error handling mostly absent
- Attempts to capture unsuitable windows may result in undefined behaviour, including return of black image / bitmap on uninitialized memory and Task being never finished
- Occasionally the uninstall process might get stuck. Suggest you close the WindowsService1.exe process manually

# IMPORTANT NOTES

- **Software, using WTIConnect.dll must be run in a strictly x64 mode**
- In case the requested process renders more than one window, any of them might be captured
- In no event will the console window be captured
- The created process is terminated after the window is captures. However, if child processes had been created, this will not be cleaned up
- 
