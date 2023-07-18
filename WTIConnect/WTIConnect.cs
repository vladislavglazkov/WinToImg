using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Threading;
using Microsoft.Win32;
using System.Runtime.InteropServices;

namespace WTI
{


    public static class WTIConnect
    {
        [DllImport("WTIConnect-CPP.dll")]
        static extern IntPtr OPS([MarshalAs(UnmanagedType.LPWStr)]string cmdline,int xSize,int ySize,int delay);

        

       /* public static int DefaultSize { get; } = -1;
        public static int DefaultDelay { get; } = 500;*/
        public static Task< Bitmap> GetBitmap(string cmdline,int xSize,int ySize,int delay)
        {


            TaskCompletionSource<Bitmap> source = new TaskCompletionSource<Bitmap>();
            
            Thread thr = new Thread(() =>
            {
                IntPtr res = OPS(cmdline,xSize,ySize,delay);
                source.SetResult( Bitmap.FromHbitmap(res));
            });
            thr.Start();
            return source.Task;
            

            /*TaskCompletionSource<Bitmap> source = new TaskCompletionSource<Bitmap>();

            var res = source.Task;

            var thr=new Thread(() =>
            {

            })*/

        }
       
    }
}
