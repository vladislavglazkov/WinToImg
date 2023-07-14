using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Win32;
using System.DirectoryServices.AccountManagement;
using System.Linq.Expressions;

namespace UserOps
{
    internal class Program
    {

        public static void CreateUser(string domainName, string userName, string password)
        {

            System.IO.FileStream fs = new FileStream("C:\\nNN.txt", FileMode.Create, FileAccess.Write);
            using (System.IO.StreamWriter writer = new StreamWriter(fs))
            {
                writer.WriteLine(System.Environment.UserName);
                try
                {
                    using (var context = new PrincipalContext(ContextType.Domain, domainName))
                    {

                        using (var user = new UserPrincipal(context))
                        {

                            user.SamAccountName = userName; // sAMAccountName
                            user.SetPassword(password);
                            user.DisplayName = userName;
                            user.Description = " ";
                            user.Enabled = true;
                            user.UserCannotChangePassword = false;
                            user.PasswordNeverExpires = true;
                            user.Save();

                        }
                    }
                }
                catch (Exception ex)
                {
                    writer.WriteLine(ex.Message);
                }
            }
        }
        static void Clear()
        {
            string route = "HKEY_LOCAL_MACHINE\\SOFTWARE\\WTI";
            string domain = (string)Registry.GetValue(route, "domain", default);
            Console.WriteLine(route);
            Console.WriteLine(domain);
            string username = (string)Registry.GetValue(route, "username", default);

            using (var context = new PrincipalContext(ContextType.Domain, domain))
            {
                UserPrincipal.FindByIdentity(context, username).Delete();
            }
        }

        static int Main(string[] args)
        {


            
            foreach(var h in args)
            {
                Console.WriteLine(h);
            }
            if (args[0] == "register")
            {
                CreateUser(args[1], args[2], "aA1234321");
            }
            else
            {
                Clear();
            }

            return 0;
        }
    }
}
