using System;
using System.Collections.Generic;
using System.Linq;
//using System.Threading.Tasks;
using System.Windows.Forms;

namespace Logger
{
    static class Program
    {
        /// <summary>
        /// Der Haupteinstiegspunkt für die Anwendung.
        /// </summary>
        [STAThread]
        static void Main()
        {			
			DataLogger.Instance = new DataLogger();
			SingleFileRequest.Instance = new SingleFileRequest();
			MultiFileRequest.Instance = new MultiFileRequest();
            SingleFileUploadRequest.Instance = new SingleFileUploadRequest();

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Main());
        }
    }
}
