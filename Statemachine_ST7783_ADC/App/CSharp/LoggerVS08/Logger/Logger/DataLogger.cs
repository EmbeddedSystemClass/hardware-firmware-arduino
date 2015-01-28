using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

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
							files.Add(s.Substring(0, 12));
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
				
				XModem.XModem xModem = new XModem.XModem(port);
				xModem.PacketReceived += new EventHandler(xModem_PacketReceived);
				byte[] xFile = xModem.XModemReceive(true);

				port.Close();				
				
				if (xFile != null)
				{
					StringBuilder sb = new StringBuilder();
					for (int i = 0; i < xFile.Length; i++) {
						sb.Append((char)xFile[i]);
					}
					
					string[] strings = sb.ToString().Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

					foreach (string line in strings) {
						string[] values = line.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
						if (values.Length == 3) {
							for (int i = 1; i < 3; i++) {
								int x = 0;
								if (int.TryParse(values[i], out x)) {
									if (x > 127) {
										x = x - 256;
									}
									values[i] = x.ToString();
								}								
							}							
							lines.Add(String.Format("{0}; {1}; {2}", values[0], values[1], values[2]));							
						} else {
							lines.Add(line);
						}
					}					
				}										
			}
			
			return lines != null && lines.Count > 0;
		}

		void xModem_PacketReceived(object sender, EventArgs e) {
			if (ProgressBar.Instance.ProgressBar.InvokeRequired) {
				ProgressBar.Instance.ProgressBar.Invoke((System.Windows.Forms.MethodInvoker)
					delegate() {
						xModem_PacketReceived(sender, e);
					}
				);
				return;
			}
			ProgressBar.Instance.ProgressBar.Value = (ProgressBar.Instance.ProgressBar.Value + 1) % 100;
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

				System.Threading.Thread.Sleep(200);

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

		public bool TryGetDayLog(byte sensorId, out List<TemperatureItem> logItems) {
			logItems = null;

			SerialPort port;
			

			if (tryGetPort(out port)) {				
				// get temperature log command
				byte[] getTemperature = { /*0:get log*/ 3, sensorId, 0, 0, 0, 0, 0, 0, 0, /*checksum*/ 3 };

				// serial communication               
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(getTemperature, 0, DATASIZE);

				System.Threading.Thread.Sleep(200);

				XModem.XModem xModem = new XModem.XModem(port);
				//xModem.PacketReceived += new EventHandler(xModem_PacketReceived);
				byte[] xFile = xModem.XModemReceive(true);

				if (xFile != null) {
					logItems = new List<TemperatureItem>();
					for (int i = 0; i < 24; i++) {
						logItems.Add(
							new TemperatureItem() {
								Id = logItems.Count + 1,
								Temperature = xFile[i]
							}
						);
					}
				}

				port.Close();

				
			}

			return logItems != null && logItems.Count > 0;
		}

		public bool TryGetSensors(out List<SensorItem> sensors) {
			sensors = new List<SensorItem>();
			//SerialPort port;
			
			// Demo
			sensors.Add(new SensorItem() { Id = 0, Name = "T1" });
			sensors.Add(new SensorItem() { Id = 1, Name = "T2" });

			//if (tryGetPort(out port)) {
			//    byte[] sensorsCmd = { /*0:get sensors cmd*/ 0, 0, 0, 0, 0, 0, 0, 0, 0, /*9:checksum*/ 0 };
				
			//    // serial communication
			//    port.Write(signature, 0, SIGNATURESIZE);
			//    port.Write(sensorsCmd, 0, DATASIZE);
			//    System.Threading.Thread.Sleep(100);

			//    // read sensor id's				
			//    for (long i = 0; i < 100; i++) {
			//        if (port.BytesToRead > 0) {
			//            string s = port.ReadLine();
			//            if (s.Length > 0) {
			//                if (s.Contains("EOF"))
			//                    break;
			//                sensors.Add(
			//                    new SensorItem() { Id = sensors.Count, Name = s.Substring(0, s.Length - 1) }
			//                );
			//            }
			//        }
			//    }

			//    port.Close();
			//}
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
			IsConnected = TryGetSensors(out sensors);
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
