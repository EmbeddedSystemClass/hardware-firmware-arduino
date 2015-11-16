using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace Logger {
	public class DataLogger {

		private string portName;
		private static List<SensorItem> sensors;

		// frame data size
		public const int DATASIZE = 10;
		// frame signature size
		public const int SIGNATURESIZE = 4;
		// frame signature
		public byte[] signature = { 0xCC, 0x33, 0x55, 0xAA };

		public static DataLogger Instance { get; set; }

		public static List<SensorItem> Sensors { get { return sensors; } }

		public DataLogger() {			
		}		

		public bool IsConnected { get; set; }

		public event EventHandler OnConnectionChanged;

		public bool TryGetDirectory(out List<string> files) {
			files = null;
			SerialPort port;

			if (tryGetPort(out port)) {
				// get dir command
				byte[] getDir = { /*0:get dir*/ 4, 0, 0, 0, 0, 0, 0, 0, 0, /* checksum */ 4 };

				// serial communication				
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(getDir, 0, DATASIZE);

                const int BUFFER_SIZE = 10000;
                byte[] buffer = new byte[BUFFER_SIZE];

                int n1 = 0;
                int n2 = 0;
                if (Receive(port, buffer, 0, BUFFER_SIZE, 0, "<DIR>", "</DIR>", out n1, out n2, new CancellationTokenSource()) > 0)
                {
                    files = new List<string>();

                    StringBuilder sb = new StringBuilder();
                    for (int i = n1; i < n2; i++)
                    {
                        char c = (char)buffer[i];
                        if (buffer[i] == 13)
                        {
                            files.Add(sb.ToString());
                            sb.Clear();
                        }
                        else if (buffer[i] == 10)
                        {
                            // ignore
                        }
                        else
                        {
                            sb.Append(c);
                        }
                    }

                    files.Sort();
                }

				port.Close();
			}
			
			return files != null && files.Count > 0;
		}

        public bool TryGetFile(string fileName, out List<string> lines, CancellationTokenSource cancellationToken)
        {
			lines = null;
            
			SerialPort port;

            if (tryGetPort(out port))
            {
                lines = new List<string>();

                // get file command
                char[] getFile = { 
					/*0:get file*/ (char)5, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, /* checksum */ (char)5 
				};
                Array.Copy(fileName.ToCharArray(), 0, getFile, 1, 8);

                // serial communication				
                port.Write(signature, 0, SIGNATURESIZE);
                port.Write(getFile, 0, DATASIZE);

                const int BUFFER_SIZE = 30 * 1024;
                byte[] buffer = new byte[BUFFER_SIZE];

                // receive file size

                int fileSize = 0;
                int n1 = 0;
                int n2 = 0;
                int read = Receive(port, buffer, 0, BUFFER_SIZE, 0, "<SIZE>", "</SIZE>", out n1, out n2, cancellationToken);
                if (read > 0)
                {
                    fileSize = buffer[n1 + 3] << 24 | buffer[n1 + 2] << 16 | buffer[n1 + 1] << 8 | buffer[n1];
                    fileSize -= ("<FILE</FILE>").Length;
                }

                // receive file data

                if(fileSize > 0) 
                {
                    if (Receive(port, buffer, read, BUFFER_SIZE, fileSize, "<FILE>", "</FILE>", out n1, out n2, cancellationToken) > 0)
                    {
                        StringBuilder sb = new StringBuilder();
                        for (int i = n1; i < n2; i++)
                        {
                            sb.Append((char)buffer[i]);
                        }

                        string[] strings = sb.ToString().Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

                        foreach (string line in strings)
                        {
                            string[] values = line.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                            if (values.Length == 3)
                            {
                                for (int i = 1; i < 3; i++)
                                {
                                    // Workaround: old files contains 255 in place of -1
                                    int x = 0;
                                    if (int.TryParse(values[i], out x))
                                    {
                                        if (x > 127)
                                        {
                                            x = x - 256;
                                        }
                                        values[i] = x.ToString();
                                    }
                                }
                                lines.Add(String.Format("{0}; {1}; {2}", values[0], values[1], values[2]));
                            }
                            else
                            {
                                lines.Add(line);
                            }
                        }	
                    }
                }

                port.Close();
            }

            return lines != null && lines.Count > 0;
		}

        public bool TryGetFile1(string fileName, out List<string> lines, CancellationTokenSource cancellationToken)
        {
            lines = null;
            bool bReady = false;

            SerialPort port;

            if (tryGetPort(out port))
            {
                lines = new List<string>();

                // get file command
                char[] getFile = { 
					/*0:get file*/ (char)5, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, /* checksum */ (char)5 
				};
                Array.Copy(fileName.ToCharArray(), 0, getFile, 1, 8);

                // serial communication				
                port.Write(signature, 0, SIGNATURESIZE);
                port.Write(getFile, 0, DATASIZE);

                int fileSize = 0;
                int read = 0;
                int lastRead = 0;
                const int BUFFER_SIZE = 30 * 1024;
                char[] buffer = new char[BUFFER_SIZE];

                // receive file size
                {
                    DateTime timeout = DateTime.UtcNow.Add(TimeSpan.FromMilliseconds(250));

                    while (timeout.Subtract(DateTime.UtcNow).Ticks > 0)
                    {
                        try
                        {
                            read += port.Read(buffer, read, BUFFER_SIZE - read);

                            int n1 = 0;
                            int n2 = 0;
                            if (FindData(buffer, BUFFER_SIZE, "<SIZE>", "</SIZE>", out n1, out n2))
                            {
                                fileSize = buffer[n1 + 3] << 24 | buffer[n1 + 2] << 16 | buffer[n1 + 1] << 8 | buffer[n1];
                                fileSize -= ("<FILE</FILE>").Length;
                                break;
                            }
                        }
                        catch
                        {
                            //ignore throw;
                        }
                    }
                }

                // receive file data

                if (fileSize > 0)
                {
                    DateTime timeout = DateTime.UtcNow.Add(TimeSpan.FromMilliseconds(250));

                    while (timeout.Subtract(DateTime.UtcNow).Ticks > 0 && !cancellationToken.IsCancellationRequested)
                    {
                        try
                        {
                            read += port.Read(buffer, read, BUFFER_SIZE - read);

                            int n1 = 0;
                            int n2 = 0;
                            if (FindData(buffer, BUFFER_SIZE, "<FILE>", "</FILE>", out n1, out n2))
                            {

                                StringBuilder sb = new StringBuilder();
                                for (int i = n1; i < n2; i++)
                                {
                                    if (buffer[i] < 127)    // only temperature values less 127°C
                                        sb.Append(buffer[i]);
                                }

                                string[] strings = sb.ToString().Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

                                foreach (string line in strings)
                                {
                                    string[] values = line.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                                    if (values.Length == 3)
                                    {
                                        for (int i = 1; i < 3; i++)
                                        {
                                            // Workaround: old files contains 255 in place of -1
                                            int x = 0;
                                            if (int.TryParse(values[i], out x))
                                            {
                                                if (x > 127)
                                                {
                                                    x = x - 256;
                                                }
                                                values[i] = x.ToString();
                                            }
                                        }
                                        lines.Add(String.Format("{0}; {1}; {2}", values[0], values[1], values[2]));
                                    }
                                    else
                                    {
                                        lines.Add(line);
                                    }
                                }

                                ReportProgress(100);
                                bReady = true;
                                break;  // while
                            }

                            if (lastRead != read)
                            {
                                timeout = DateTime.UtcNow.Add(TimeSpan.FromMilliseconds(250));
                                lastRead = read;

                                ReportProgress((int)((float)read / fileSize * 100));
                            }

                            Thread.Sleep(1);
                        }
                        catch (Exception e)
                        {
                            break;
                        }
                    }
                }

                port.Close();
            }

            return bReady && lines != null && lines.Count > 0;
        }

        private static void ReportProgress(int progress)
        {
            if (ProgressBar.Instance.ProgressBar.InvokeRequired)
            {
                ProgressBar.Instance.ProgressBar.Invoke((System.Windows.Forms.MethodInvoker)
                    delegate()
                    {
                        ProgressBar.Instance.ProgressBar.Value = progress <= 100 ? progress : 100;
                    }
                );
            }
        }

        public int Receive(SerialPort port, byte[] buffer, int offset, int bufferSize, int refSize, 
            string startTag, string stoppTag, out int startIndex, out int stoppIndex, CancellationTokenSource cancellationToken)
        {
            startIndex = -1;
            stoppIndex = -1;

            DateTime timeout = DateTime.UtcNow.Add(TimeSpan.FromMilliseconds(250));

            int read = offset;
            int lastRead = 0;
            bool ready = false;

            while (timeout.Subtract(DateTime.UtcNow).Ticks > 0 && !cancellationToken.IsCancellationRequested)
            {
                try
                {
                    read += port.Read(buffer, read, bufferSize - read);

                    if (FindData(buffer, bufferSize, startTag, stoppTag, out startIndex, out stoppIndex))
                    {
                        ready = true;
                        if (refSize > 0)
                        {
                            ReportProgress(100);
                        } 
                        break;  // while
                    }

                    if (lastRead != read)
                    {
                        timeout = DateTime.UtcNow.Add(TimeSpan.FromMilliseconds(250));
                        lastRead = read;
                        if (refSize > 0)
                        {
                            ReportProgress((int)((float)read / refSize * 100));
                        }                        
                    }

                    Thread.Sleep(1);
                }
                catch/*(Exception e)*/
                {
                    break;
                }
            }

            return ready ? read : 0;
        }

        bool FindData(char[] buffer, int nSize, string startTag, string stoppTag, out int startIndex, out int stoppIndex)
        {
            startIndex = -1;
            stoppIndex = -1;

            if (startTag.Length == 0 || stoppTag.Length == 0)
            {
                return false;
            }

            string sBuffer = new string(buffer);

            startIndex = sBuffer.IndexOf(startTag);
            stoppIndex = sBuffer.IndexOf(stoppTag);

            if (startIndex >= 0)
            {
                startIndex += startTag.Length;
            }
            
            return stoppIndex > 0 && stoppIndex >= startIndex;
        }

        bool FindData(byte[] buffer, int nSize, string startTag, string stoppTag, out int startIndex, out int stoppIndex)
        {
            startIndex = 0;
            stoppIndex = 0;

            if (startTag.Length == 0 || stoppTag.Length == 0)
            {
                return false;
            }

            for (int i = 0; i < nSize; i++)
            {
                int j = 0;
                for (; j < startTag.Length; j++)
                {
                    if ((char)buffer[i + j] != startTag[j])
                    {
                        j = -1;
                        break;
                    }
                }
                if (j > 0)
                {
                    startIndex = i;
                    break;
                }
            }

            for (int i = 0; i < nSize; i++)
            {
                int j = 0;
                for (; j < stoppTag.Length; j++)
                {
                    if ((char)buffer[i + j] != stoppTag[j])
                    {
                        j = -1;
                        break;
                    }
                }
                if (j > 0)
                {
                    stoppIndex = i;
                    break;
                }
            }

           
            if (startIndex >= 0)
            {
                startIndex += startTag.Length;
            }

            return stoppIndex > 0 && stoppIndex >= startIndex;
        }

		public bool TryGetCurrentValue(byte sensorId, out int temperature) {
			temperature = -9999;
			// TODO: implement ID query
			SerialPort port;
			
			if (tryGetPort(out port)) {
				// get temperature command
				byte[] getTemperature = { /*0:get temperature*/ 2, sensorId, 0, 0, 0, 0, 0, 0, 0, /*checksum*/ 2 };

				// serial communication                
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(getTemperature, 0, DATASIZE);


                const int BUFFER_SIZE = 100;
                byte[] buffer = new byte[BUFFER_SIZE];

                int n1 = 0;
                int n2 = 0;
                if (Receive(port, buffer, 0, BUFFER_SIZE, 0, "<TEMP>", "</TEMP>", out n1, out n2, new CancellationTokenSource()) > 0)
                {
                    temperature = (sbyte)buffer[n1];
                }               

				port.Close();
			}

			return temperature > -129 && temperature < 129;
		}

		public bool TryGetDayLog(byte sensorId, out List<TemperatureItem> logItems) {
			logItems = null;

			SerialPort port;

			if (tryGetPort(out port)) {				
				// get temperature log command
				byte[] getTemperature = { /*0:get log*/ 3, sensorId, 0, 0, 0, 0, 0, 0, 0, /*checksum*/ 3 };

				// serial communication               
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(getTemperature, 0, DATASIZE);

                const int BUFFER_SIZE = 1000;
                byte[] buffer = new byte[BUFFER_SIZE];

                int n1 = 0;
                int n2 = 0;
                if (Receive(port, buffer, 0, BUFFER_SIZE, 24, "<DAYLOG>", "</DAYLOG>", out n1, out n2, new CancellationTokenSource()) > 0)
                {
                    if (n2 - n1 == 24)
                    {
                        logItems = new List<TemperatureItem>();
                        for (int i = n1; i < n2; i++)
                        {
                            logItems.Add(
                                new TemperatureItem()
                                {
                                    Id = logItems.Count + 1,
                                    Temperature = (SByte)buffer[i]
                                }
                            );
                        }
                    }
                }

				port.Close();				
			}

			return logItems != null && logItems.Count > 0;
		}

		public bool TryGetSensors(out List<SensorItem> sensors) {
			sensors = new List<SensorItem>();
			sensors.Add(new SensorItem() { Id = 0, Name = "T1" });
			sensors.Add(new SensorItem() { Id = 1, Name = "T2" });
			return sensors.Count > 0;
		}

		public void SetTime(DateTime dateTime) {
			SerialPort port;
			            
            if (tryGetPort(out port))
            {                
				byte[] time = { /*0:set time cmd*/ 0, /*1:hh*/ 0, /*2:mm*/ 0, /*3:ss*/ 0, /*4:empty*/ 0, /*5:empty*/ 0, /*6:empty*/ 0, /*7:empty*/ 0, /*8:empty */ 0, /*9:checksum*/ 0 };

				time[1] = (byte)dateTime.Hour;
				time[2] = (byte)dateTime.Minute;
				time[3] = (byte)dateTime.Second;
                time[6] = getCheckSum(time, 0, 10);
                
                // serial communication
				port.Write(signature, 0, SIGNATURESIZE);
				System.Threading.Thread.Sleep(100);
				port.Write(time, 0, DATASIZE);
				System.Threading.Thread.Sleep(100);
				port.Close();
            }
        }

		public void SetDate(DateTime dateTime) {
			SerialPort port;

			if (tryGetPort(out port)) 
			{		
				byte[] date = { /*0:set date cmd*/ 1, /*1:yy*/ 0, /*2:yy*/ 0, /*3:mm*/ 0, /*4:dd   */ 0, /*5:empty*/ 0, /*6:empty*/ 0, /*7:empty*/ 0, /*8:empty */ 0, /*9:checksum*/ 0 };
				date[1] = 0;
				date[2] = (byte)(dateTime.Year - 2000);
				date[3] = (byte)dateTime.Month;
				date[4] = (byte)dateTime.Day;
				date[6] = getCheckSum(date, 0, 10);

				port.Write(signature, 0, SIGNATURESIZE);
				System.Threading.Thread.Sleep(100);
				port.Write(date, 0, DATASIZE);
				System.Threading.Thread.Sleep(100);
				port.Close();
			}
		}

		public void Connect(string portName) {
			this.portName = portName;

			SerialPort port;

			if (tryGetPort(out port)) {
				IsConnected = TryGetSensors(out sensors);
				OnConnectionChanged(this, EventArgs.Empty);
                port.Close();
			} else {
				MessageBox.Show("Port not available");
			}
		}

		public void Disconnect(string portName) {
			this.portName = string.Empty;
			IsConnected = false;
			OnConnectionChanged(this, EventArgs.Empty);
		}

		public void ResetLooger() {
            string portName;
            if (tryGetPortName(out portName))
            {
                SerialPort serial = new SerialPort(portName, 9600);
                serial.DtrEnable = true;
                serial.Open();
                serial.Close();
            }
        }

		private byte getCheckSum(byte[] buffer, int offset, int count) {
			byte sum = 0;
			for (int i = offset; i < offset + count; i++) {
				sum += buffer[i];
			}
			return sum;
		}

		private bool tryGetPortName(out string name) {
			name = "";
			if (string.IsNullOrEmpty(portName)) {
				MessageBox.Show("No port selected", "Serial Port");
				return false;
			}

			name = portName;

			return true;
		}

		private bool tryGetPort(out SerialPort port) {
			port = null;
			bool result = false;

			try {
				if (!string.IsNullOrEmpty(portName)) {
					port = new SerialPort(portName, 9600);
					port.DtrEnable = false;
					port.Open();
					result = true;
				}
			} catch /*(Exception e)*/ {
				result = false;			
			}

			return result;
		}
    }

    public class TemperatureItem
    {        
        public int Id { get; set; }
        public int Temperature { get; set; }
    }

	public class SensorItem {
		public byte Id { get; set; }
		public string Name { get; set; }
	}

	public enum Action {
		ShowPorts,
		ShowDateTimeTab,
		ShowTemperatureTab,
		ShowTemperatureChartTab,
		ShowFilesTab,
		Reset
	}
}
