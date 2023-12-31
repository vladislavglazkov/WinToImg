# DISCLAIMER

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# OVERVIEW
The "WTI" package allows to run apps and capture its contents off-screen by maintaining and managing a hidden RDP session.

# PREREQUISITES
- **Windows Server only**
- Local RDS host should be serviceable and at least one additional client license should be available

# INSTALLATION
- Run the WTI-Installer1.msi file from the package. **Important: installation must be run by a user privileged to create other domain users (it's not possible to run the installation as a standard user and pass UAC check later)**
- Specify the domain name, name for the new user and virtual screen size

# HOW TO USE

In the package, there is a .NET Framework library *WTIConnect.dll*, implementing the namespace *WTI* with the single static class *WTIConnect* having a static method defined as follows:

*static async Task<Bitmap> GetBitmap(string cmdline,int xSize,int ySize,int delay)*


You need to pass
- the command-line string used to create the new application
- requested horizontal size of the window 
- requested vertical size of the window 
- the delay since the window would have been detected to the moment it would be captured (if set too low, not all the contents might draw up properly)

The standard *System.Drawing.Bitmap* object, containing client area of the requested process's window, will be returned. Do not forget to dispose of it after it has been used.

# LIMITATIONS AND KNOWN BUGS

- Error handling mostly absent
- Attempts to capture unsuitable windows may result in undefined behaviour, including return of black image / bitmap on uninitialized memory and Task being never finished
- Occasionally the uninstall process might get stuck. Suggest you close the WindowsService1.exe process manually

# IMPORTANT NOTES

- **Software, using WTIConnect.dll must be run in a strictly x64 mode**
- In case the requested process renders more than one window, any of them might be captured. In case the window is rendered by the child process, it will not be captured.
- In no event will the console window be captured
- The created process is terminated after the window is captures. However, if child processes had been created, this will not be cleaned up
- The created user's credentials will be used to start the requested process. Ensure it is sufficiently privileged.
- The requested window size is the size of the full window, however, only the client area will be added, therefore, the real image size will be slightly smaller. (Requesting precise client area size - to be added later)

