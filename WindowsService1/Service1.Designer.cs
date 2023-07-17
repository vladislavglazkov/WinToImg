using System;
using System.Diagnostics;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;
using static WindowsService1.Service1;
using System.IO.MemoryMappedFiles;
using System.Drawing.Imaging;
using Microsoft.Win32;
namespace WindowsService1
{
    partial class Service1
    {


        int running = 0;
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        /// System.Diagnostics.EventLog
        /// 


        private static string GetRegParam(string param)
        {
            string route = "HKEY_LOCAL_MACHINE\\SOFTWARE\\WTI";
            return (string)Registry.GetValue(route, param, default);

            
        }
        private string username;
        static System.Diagnostics.EventLog eventLog1;
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            this.ServiceName = "WTIService";
            eventLog1 = new System.Diagnostics.EventLog();
            
            if (!System.Diagnostics.EventLog.SourceExists("MySource"))
            {
                System.Diagnostics.EventLog.CreateEventSource(
                    "MySource", "MyNewLog");
            }
            eventLog1.Source = "MySource";
            eventLog1.Log = "MyNewLog";
            eventLog1.Clear();



        }
        AxMSTSCLib.AxMsRdpClient9NotSafeForScripting rdpConnection;
        //int setRun;
       
        void MyThreadStartMethod()
        {
            if (running==0)
                return;
            /*eventLog1.WriteEntry("At Least");
            Form form1 = new Form();
            form1.Show();
            var rdpConnection = new AxMSTSCLib.AxMsRdpClient9NotSafeForScripting();
            form1.Controls.Add(rdpConnection);
            rdpConnection.Server = "winserver.localops";
            rdpConnection.Domain = "WINSERVER";
            rdpConnection.UserName = "vladi";
            rdpConnection.AdvancedSettings9.ClearTextPassword = "1234321aA";
            rdpConnection.AdvancedSettings9.EnableCredSspSupport = true;

            rdpConnection.Connect();
            rdpConnection.OnConnected += RdpConnection_OnConnected;
            rdpConnection.OnConnecting += RdpConnection_OnConnecting;
            eventLog1.WriteEntry("So-so");*/


            rdpConnection = new AxMSTSCLib.AxMsRdpClient9NotSafeForScripting();

            Console.WriteLine("Outer thread started");
            ApplicationContext context = new ApplicationContext();

            eventLog1.WriteEntry("Before lock");

            //timer.Elapsed += OPS;
           
                eventLog1.WriteEntry("In locksep");
            /*if (form==null)
            {
                eventLog1.WriteEntry("new form");
                form = new Form();
                Application.Run();
            }*/

            Form form = new Form();
            form.Controls.Add(rdpConnection);
            
            MethodInvoker min = new MethodInvoker(() => { Console.WriteLine("OPS"); });
            form.Visible = false;
            form.ShowInTaskbar = false;
            var hh = form.Handle;
            eventLog1.WriteEntry(hh.ToString());
            form.BeginInvoke(min);
            form.Show();

            rdpConnection.Server = GetRegParam("domain");
            rdpConnection.Domain = rdpConnection.Server;
            rdpConnection.UserName = GetRegParam("username");
            this.username= rdpConnection.UserName;
            rdpConnection.DesktopHeight = int.Parse(GetRegParam("Y"));
            rdpConnection.DesktopWidth = int.Parse(GetRegParam("X"));
            rdpConnection.AdvancedSettings9.ClearTextPassword = (string)(Registry.GetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\WTI\\PRIVILEGED","password",default));
            rdpConnection.AdvancedSettings9.EnableCredSspSupport = true;
            rdpConnection.OnConnected += RdpConnection_OnConnected;
            rdpConnection.OnConnecting += RdpConnection_OnConnecting;
            rdpConnection.OnDisconnected += RdpConnection_OnDisconnected;
            //rdpConnection.AdvancedSettings9.
            //rdpConnection.ClientSize = new System.Drawing.Size(500, 500);
            rdpConnection.Connect();




            Application.Run();
        }

        private void RdpConnection_OnDisconnected(object sender, AxMSTSCLib.IMsTscAxEvents_OnDisconnectedEvent e)
        {
            eventLog1.WriteEntry("DISCON "+e.discReason);
            if (e.discReason != 2)
            {
                MyThreadStartMethod();
            }

        }

        private void RdpConnection_OnConnecting(object sender, EventArgs e)
        {
            eventLog1.WriteEntry("That's something");
        }

        protected override void OnStop()
        {
            running = 0;

           
               /* if (form != null)
                {
                    form.Dispose();
                    form = null;
                }*/
            
            if (rdpConnection != null)
            {
                rdpConnection.Disconnect();
            }
            base.OnStop();
            
        }

        protected override void OnStart(string[] args)
        {

            running = 1;
            var t = new Thread(MyThreadStartMethod);
            t.SetApartmentState(ApartmentState.STA);
            t.Start();
            eventLog1.WriteEntry("In OnStart.");
            
        }


        [DllImport("wtsapi32.dll", SetLastError = true)]
        public static extern bool WTSQueryUserToken(int sessionId, out IntPtr phToken);



       /* [StructLayout(LayoutKind.Sequential)]
        public struct HANDLE
        {
            public IntPtr hHandle;
            public IntPtr pHandle;
            public int dwAccessMask;
        }*/

        // Process information structure
        [StructLayout(LayoutKind.Sequential)]
        public struct PROCESS_INFORMATION
        {
            public IntPtr hProcess;
            public IntPtr hThread;
            public int dwProcessId;
            public int dwThreadId;
        }

        // Startup information structure
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct STARTUPINFO
        {
            public int cb;
            public string lpReserved;
            public string lpDesktop;
            public string lpTitle;
            public int dwX;
            public int dwY;
            public int dwXSize;
            public int dwYSize;
            public int dwXCountChars;
            public int dwYCountChars;
            public int dwFillAttribute;
            public int dwFlags;
            public short wShowWindow;
            public short cbReserved2;
            public IntPtr lpReserved2;
            public IntPtr hStdInput;
            public IntPtr hStdOutput;
            public IntPtr hStdError;
        }

        // Import the CreateProcessAsUser function
        [DllImport("advapi32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        public static extern bool CreateProcessAsUser(
            IntPtr hToken,
            string lpApplicationName,
            string lpCommandLine,
            IntPtr lpProcessAttributes,
            IntPtr lpThreadAttributes,
            bool bInheritHandles,
            int dwCreationFlags,
            IntPtr lpEnvironment,
            string lpCurrentDirectory,
            ref STARTUPINFO lpStartupInfo,
            out PROCESS_INFORMATION lpProcessInformation
        );


        [DllImport("BringBitmap.dll")]
        public static extern int GetSessionId([MarshalAs(UnmanagedType.LPWStr)]string name);


        private void RdpConnection_OnConnected(object sender, EventArgs e)
        {
            //Thread.Sleep(5000);

            
            
            

             var thr = new Thread(() => {

                 Thread.Sleep(5000);
                 int sessionId = GetSessionId(username);
                 eventLog1.WriteEntry($"For username {username} SessionID obtained: {sessionId}");
                 eventLog1.WriteEntry("Hey-hey");
                 token = new IntPtr();
                 int error;
                 bool res = WTSQueryUserToken(sessionId, out token);
                 error = Marshal.GetLastWin32Error();
                 if (res == false)
                 {
                     eventLog1.WriteEntry("Error on UserToken Request: " + error.ToString());
                 }
                 else
                 {
                     eventLog1.WriteEntry("Error on UserToken Request: none");

                 }

                 Saver(); });
             thr.Start();


            
        }


        



        [DllImport("user32.dll")]
        public static extern bool EnumThreadWindows(int dwThreadId, EnumThreadDelegate lpfn, IntPtr lParam);

        public delegate bool EnumThreadDelegate(IntPtr hWnd, IntPtr lParam);


        private static bool CallbackFunction(IntPtr hWnd, IntPtr lParam)
        {
            // Do something with the window handle

            // Continue enumeration
            eventLog1.WriteEntry("WinEnum: "+hWnd.ToString());
            return true;
        }


        // DllImport statement for ImpersonateLoggedOnUser
        [DllImport("advapi32.dll", SetLastError = true)]
        public static extern bool ImpersonateLoggedOnUser(IntPtr token);



        [DllImport("kernel32.dll", SetLastError = true)]
        static extern uint WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);

        const uint INFINITE = 0xFFFFFFFF;
        const uint WAIT_FAILED = 0xFFFFFFFF;
        const uint WAIT_OBJECT_0 = 0x00000000;



        /*[DllImport("C:\\BringBitmap.dll", SetLastError = true)]
        static extern IntPtr OPS(IntPtr handle,int thrid);*/


        IntPtr token = new IntPtr();

        [DllImport("BringBitmap.dll", SetLastError = true)]
        extern static void StartListen(IntPtr token);

        void Saver()
        {

            eventLog1.WriteEntry($"Before {token}");

            //bool res=WTSQueryUserToken(2, out token);

            eventLog1.WriteEntry($"After {token}");


            StartListen(token);


            /*var res=ImpersonateLoggedOnUser(token);
            if (res == false)
            {
                eventLog1.WriteEntry("Impersonation error: " + Marshal.GetLastWin32Error().ToString());
            }
            else
            {
                eventLog1.WriteEntry("Impersonation error: none");
            }*/

            


           /* IntPtr finres=OPS(token,thrid);
            eventLog1.WriteEntry("RECEIVEED");
            Bitmap bmp=Bitmap.FromHbitmap(finres);
            eventLog1.WriteEntry("RECEIVEED THEREAFTER");

            eventLog1.WriteEntry(bmp.Height.ToString());
            eventLog1.WriteEntry(bmp.Width.ToString());
            bmp.Save("C:\\temp\\image.bmp", ImageFormat.Bmp);*/


            /*res = CreateProcessAsUser(token, "C:\\WinCap.exe", thrid.ToString()+" "+ops.ToString("N"), IntPtr.Zero, IntPtr.Zero, false, 0, IntPtr.Zero, null, ref info, out pinfo);
            WaitForSingleObject(pinfo.hProcess, INFINITE);
            eventLog1.WriteEntry("Proc finished");*/

            //EnumThreadWindows(12536, CallbackFunction, IntPtr.Zero);

            /*var ress=GetHandle(thrid);
            System.Drawing.Bitmap bmp = Bitmap.FromHbitmap(ress);
            bmp.Save("C:\\ops.bmp");*/
        }



        private void Tm_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            

        }

        #endregion
    }
}
