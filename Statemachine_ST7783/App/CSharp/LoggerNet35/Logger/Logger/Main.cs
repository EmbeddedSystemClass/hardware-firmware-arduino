using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
//using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace Logger
{
    public partial class Main : Form
    {
		// frame data size
		private const int DATASIZE = 10;
		// frame signature size
		private const int SIGNATURESIZE = 4;
		// frame signature
		private byte[] signature = { 0xCC, 0x33, 0x55, 0xAA };

        public Main()
        {
            InitializeComponent();

            portComboBox.Items.AddRange(SerialPort.GetPortNames());

            updateUi();

			resetTemperatureChart();            
            
        }		

        private byte getCheckSum(byte[] buffer, int offset, int count)
        {
            byte sum = 0;
            for (int i = offset; i < offset + count; i++)
            {
                sum += buffer[i];
            }
            return sum;
        }

		private void getFile(string fileName) {
			SerialPort port;

			if (tryGetPort(out port)) {
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
				List<string> items = new List<string>();
				for (int i = 0; i < 1000; i++) {
					if (port.BytesToRead > 0) {
						string s = port.ReadLine();
						if (s.Length > 0) {
							if (s.Contains("EOF"))
								break;
							items.Add(s);
						}

					}
				}
				port.Close();

				// populate list view
				logFileValuesListView.Items.Clear();
				foreach (string item in items) {
					logFileValuesListView.Items.Add(item);
				}
			}
		}

		private void resetTemperatureChart() {
			temperatureChart.Series[0].Points.Clear();
			for (int i = 0; i < 24; i++) {
				temperatureChart.Series[0].Points.AddXY(i, 0);
			}
		}

		private bool tryGetPortName(out string portName) {
			portName = portComboBox.Text;

			if (portName.Length == 0) {
				MessageBox.Show("No port selected", "Serial Port");
				return false;
			}

			return true;
		}

		private bool tryGetPort(out SerialPort port) {
			port = null;
			string portName = portComboBox.Text;

			if (portName.Length > 0) {
				port = new SerialPort(portName, 9600);
				port.DtrEnable = false;
				port.Open();
				return true;
			}

			return false;
		}

		private void updateUi() {
			bool enabled = portComboBox.SelectedIndex >= 0;
			setDateTimeButton.Enabled = enabled;
			getTemperatureLogButton.Enabled = enabled;
			getTemperatureButton.Enabled = enabled;
			setDateTimeButton.Enabled = enabled;
			resetButton.Enabled = enabled;
			getDirectoryButton.Enabled = enabled;
		}

        private void setDateTimeButton_Click(object sender, EventArgs e)
        {
			SerialPort port;
			            
            if (tryGetPort(out port))
            {
                // set time command
				byte[] time = { /*0:set time cmd*/ 0, /*1:hh*/ 0, /*2:mm*/ 0, /*3:ss*/ 0, /*4:empty*/ 0, /*5:empty*/ 0, /*6:empty*/ 0, /*7:empty*/ 0, /*8:empty */ 0, /*9:checksum*/ 0 };

                time[1] = (byte)DateTime.Now.Hour;
                time[2] = (byte)DateTime.Now.Minute;
                time[3] = (byte)DateTime.Now.Second;
                time[6] = getCheckSum(time, 0, 10);

                // set date command
				byte[] date = { /*0:set date cmd*/ 1, /*1:yy*/ 0, /*2:yy*/ 0, /*3:mm*/ 0, /*4:dd   */ 0, /*5:empty*/ 0, /*6:empty*/ 0, /*7:empty*/ 0, /*8:empty */ 0, /*9:checksum*/ 0 };
                date[1] = (byte)(DateTime.Now.Year / 1000);
                date[2] = (byte)(DateTime.Now.Year % 1000);
                date[3] = (byte)DateTime.Now.Month;
                date[4] = (byte)DateTime.Now.Day;
                date[6] = getCheckSum(time, 0, 10);

                // serial communication
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(time, 0, DATASIZE);
				port.Write(signature, 0, 4);
				port.Write(date, 0, DATASIZE);

				port.Close();
            }
        }

        private void resetButton_Click(object sender, EventArgs e)
        {
            string portName;
            if (tryGetPortName(out portName))
            {
                SerialPort serial = new SerialPort(portName, 9600);
                serial.DtrEnable = true;
                serial.Open();
                serial.Close();
            }
        }

        private void getTemperatureButton_Click(object sender, EventArgs e)
        {
			SerialPort port;

			if (tryGetPort(out port))
            {
                // get temperature command
				byte[] getTemperature = { /*0:get temperature*/ 2, 0, 0, 0, 0, 0, 0, 0, 0, /*checksum*/ 2 };

                // serial communication                
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(getTemperature, 0, DATASIZE);

                System.Threading.Thread.Sleep(100);

                // read temperature
                for (int i = 0; i < 100; i++)
                {
                    if (port.BytesToRead > 0)
                    {
                        tempLabel.Text = port.ReadChar().ToString();
                        break;
                    }
                    else
                    {
                        tempLabel.Text = "failed";
                    }
                }

                port.Close();
            }
        }


        private void getTemperatureLogButton_Click(object sender, EventArgs e)
        {
            SerialPort port;

			if (tryGetPort(out port))            
            {
                // get temperature log command
				byte[] getTemperature = { /*0:get log*/ 3, 0, 0, 0, 0, 0, 0, 0, 0, /*checksum*/ 3 };

                // serial communication               
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(getTemperature, 0, DATASIZE);

                System.Threading.Thread.Sleep(100);

                // read temperature log
                List<TemperatureItem> logItems = new List<TemperatureItem>();
                for (int i = 0; i < 100; i++)
                {
					if (port.BytesToRead > 0)
                    {
						for (int n = 0; n < port.BytesToRead; n++)
                        {
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
                    else
                    {
                        tempLabel.Text = "failed";
                    }
                }

				port.Close();

                temperatureChart.Series[0].Points.Clear();
                foreach (TemperatureItem item in logItems) {
                    temperatureChart.Series[0].Points.AddXY(item.Id, item.Temperature);
	            } 
            }
        }               		

		private void getDirectoryButton_Click(object sender, EventArgs e) {
			SerialPort port;

			if (tryGetPort(out port))
			{
				// get dir command
				byte[] getDir = { /*0:get dir*/ 4, 0, 0, 0, 0, 0,  0, 0, 0, /* checksum */ 4 };
				
				// serial communication				
				port.Write(signature, 0, SIGNATURESIZE);
				port.Write(getDir, 0, DATASIZE);

				System.Threading.Thread.Sleep(250);

				// read directory
				List<string> files = new List<string>();				
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

				// populate list view
				logFilesListView.Items.Clear();
				foreach (string item in files) {
					logFilesListView.Items.Add(item);
				}
			}
		}				

		private void printMenuItem_Click(object sender, EventArgs e) {
			temperatureChart.Printing.PageSetup();
			temperatureChart.Printing.PrintPreview();
		}

		private void portComboBox_SelectedIndexChanged(object sender, EventArgs e) {
			updateUi();
		}

		private void logFilesListView_DoubleClick(object sender, EventArgs e) {
			if (logFilesListView.SelectedItems.Count > 0) {
				string s = logFilesListView.SelectedItems[0].Text;
				getFile(s);
			}
		}
    }

    public class TemperatureItem
    {        
        public int Id { get; set; }
        public byte Temperature { get; set; }
    }
}
