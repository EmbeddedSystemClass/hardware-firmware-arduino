using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace Logger {
	public partial class Main : Form {
		
		public Main() {
			InitializeComponent();

			portComboBox.Items.AddRange(SerialPort.GetPortNames());

		}

		private bool tryGetPortName(out string portName) {
			portName = portComboBox.Text;

			if (portName.Length == 0) {
				MessageBox.Show("select com port", "Serial Port");
				return false;
			}

			return true;
		}

		private byte getCheckSum(byte[] buffer, int count) {
			byte sum = 0;
			for (int i = 0; i < count; i++) {
				sum += buffer[i];
			}
			return sum;
		}

		private void setDateTimeButton_Click(object sender, EventArgs e) {
			string portName;
			if(tryGetPortName(out portName)) {
				byte[] time = { /*0:Signature*/ 0xCC, 0x33, 0x55, 0xAA, /*4:time*/ 0, /*5:hour*/  0, /*6:minute*/ 0, /*7:second*/ 0, /*8*/     0, /*9*/ 0 };
				byte[] date = { /*0:Signature*/ 0xCC, 0x33, 0x55, 0xAA, /*4:date*/ 1, /*5:year*/ 19, /*6:year*/  70, /*7:month*/  0, /*8:day*/ 0, /*9*/ 0 };

				time[5] = (byte)DateTime.Now.Hour;
				time[6] = (byte)DateTime.Now.Minute;
				time[7] = (byte)DateTime.Now.Second;
				time[9] = getCheckSum(time, 8);

				SerialPort serial = new SerialPort(portName, 9600);
				serial.DtrEnable = false;
				serial.Open();
				serial.Write(time, 0, 9);
				serial.Write(date, 0, 9);
				serial.Close();
			}
		}

		private void resetButton_Click(object sender, EventArgs e) {
			string portName;
			if (tryGetPortName(out portName)) {
				SerialPort serial = new SerialPort(portName, 9600);
				serial.DtrEnable = true;
				serial.Open();
				serial.Close();
			}
		}
	}
}
