using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace Logger {
	public class DataLogger {

		private string portName;

		// frame data size
		public const int DATASIZE = 10;
		// frame signature size
		public const int SIGNATURESIZE = 4;
		// frame signature
		public byte[] signature = { 0xCC, 0x33, 0x55, 0xAA };

		public static DataLogger Instance { get; set; }

		public DataLogger() {
			Instance = this;
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

				System.Threading.Thread.Sleep(250);

				// read directory
				files = new List<string>();
				for (int i = 0; i < 1000; i++) {
					if (port.BytesToRead > 0) {
						string s = port.ReadLine();
						if (s.Length > 0) {
							if (s.Contains("EOF"))
								break;
							files.Add(s);
						}

					}
				}
				port.Close();
			}
			
			return files != null && files.Count > 0;
		}

		public bool TryGetFile(string fileName, out List<string> lines) {
			lines = null;

			SerialPort port;

			if (tryGetPort(out port)) {
				lines = new List<string>();

				// get file command
				char[] getFile = { 
					/*0:get file*/ (char)5, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, /* checksum */ (char)5 
				};
				Array.Copy(fileName.ToCharArray(), 0, getFile, 1, 8);

				// serial communication				
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(getFile, 0, DATASIZE);

				System.Threading.Thread.Sleep(250);

				// read file				
				for (long i = 0; i < 1000000; i++) {
					if (port.BytesToRead > 0) {
						string s = port.ReadLine();
						if (s.Length > 0) {
							if (s.Contains("EOF"))
								break;
							lines.Add(s);
						}
					}
				}
				port.Close();						
			}
			
			return lines != null && lines.Count > 0;
		}

		public bool TryGetCurrentTemperature(out int temperature) {
			temperature = -9999;

			SerialPort port;
			
			if (tryGetPort(out port)) {
				// get temperature command
				byte[] getTemperature = { /*0:get temperature*/ 2, 0, 0, 0, 0, 0, 0, 0, 0, /*checksum*/ 2 };

				// serial communication                
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(getTemperature, 0, DATASIZE);

				System.Threading.Thread.Sleep(100);

				// read temperature, 100 attempts
				for (int i = 0; i < 99; i++) {
					if (port.BytesToRead > 0) {
						temperature = port.ReadChar();
						break;				
					}
				}

				port.Close();
			}

			return temperature > -129 && temperature < 129;
		}

		public bool TryGetDaylog(out List<TemperatureItem> logItems) {
			logItems = null;

			SerialPort port;
			

			if (tryGetPort(out port)) {
				// get temperature log command
				byte[] getTemperature = { /*0:get log*/ 3, 0, 0, 0, 0, 0, 0, 0, 0, /*checksum*/ 3 };

				// serial communication               
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(getTemperature, 0, DATASIZE);

				System.Threading.Thread.Sleep(100);

				// read temperature log
				logItems = new List<TemperatureItem>();
				for (int i = 0; i < 99; i++) {
					if (port.BytesToRead > 0) {
						for (int n = 0; n < port.BytesToRead; n++) {
							logItems.Add(new TemperatureItem()
								{
									Id = logItems.Count + 1,
									Temperature = (byte)port.ReadByte()
								}
							);
						}

						if (logItems.Count >= 24)
							break;					
					}
				}

				port.Close();

				
			}

			return logItems != null && logItems.Count > 0;
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
				port.Write(time, 0, DATASIZE);				
				port.Close();
            }
        }

		public void SetDate(DateTime dateTime) {
			SerialPort port;

			if (tryGetPort(out port)) 
			{		
				byte[] date = { /*0:set date cmd*/ 1, /*1:yy*/ 0, /*2:yy*/ 0, /*3:mm*/ 0, /*4:dd   */ 0, /*5:empty*/ 0, /*6:empty*/ 0, /*7:empty*/ 0, /*8:empty */ 0, /*9:checksum*/ 0 };
				date[1] = (byte)(dateTime.Year / 1000);
				date[2] = (byte)(dateTime.Year % 1000);
				date[3] = (byte)dateTime.Month;
				date[4] = (byte)dateTime.Day;
				date[6] = getCheckSum(date, 0, 10);

				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(date, 0, DATASIZE);

				port.Close();
			}
		}

		public void Connect(string portName) {
			this.portName = portName;
			IsConnected = true;
			OnConnectionChanged(this, EventArgs.Empty);
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

			if (!string.IsNullOrEmpty(portName)) {
				port = new SerialPort(portName, 9600);
				port.DtrEnable = false;
				port.Open();
				return true;
			}

			return false;
		}
    }

    public class TemperatureItem
    {        
        public int Id { get; set; }
        public byte Temperature { get; set; }
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
