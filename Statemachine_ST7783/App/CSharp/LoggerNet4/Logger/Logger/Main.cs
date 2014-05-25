using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace Logger
{

    public partial class Main : Form
    {

        public Main()
        {
            InitializeComponent();

            portComboBox.Items.AddRange(SerialPort.GetPortNames());

            updateUi();

            for (int i = 0; i < 24; i++)
            {
                chart1.Series[0].Points.AddXY(i, 0);
            }
            
        }

        private bool tryGetPortName(out string portName)
        {
            portName = portComboBox.Text;

            if (portName.Length == 0)
            {
                MessageBox.Show("select com port", "Serial Port");
                return false;
            }

            return true;
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

        private void setDateTimeButton_Click(object sender, EventArgs e)
        {
            string portName;
            if (tryGetPortName(out portName))
            {
                // frame signature
                byte[] signature = { 0xCC, 0x33, 0x55, 0xAA };

                // set time command
                byte[] time = { /*0:set time cmd*/ 0, /*1:hh*/ 0, /*2:mm*/ 0, /*3:ss*/ 0, /*4:empty*/ 0, /*5:empty*/ 0, /*6:checksum*/ 0 };

                time[1] = (byte)DateTime.Now.Hour;
                time[2] = (byte)DateTime.Now.Minute;
                time[3] = (byte)DateTime.Now.Second;
                time[6] = getCheckSum(time, 0, 6);

                // set date command
                byte[] date = { /*0:set date cmd*/ 1, /*1:yy*/ 0, /*2:yy*/ 0, /*3:mm*/ 0, /*4:dd   */ 0, /*5:empty*/ 0, /*6:Checksum*/ 0 };
                date[1] = (byte)(DateTime.Now.Year / 1000);
                date[2] = (byte)(DateTime.Now.Year % 1000);
                date[3] = (byte)DateTime.Now.Month;
                date[4] = (byte)DateTime.Now.Day;
                date[6] = getCheckSum(time, 0, 6);

                // serial communication
                SerialPort serial = new SerialPort(portName, 9600);
                serial.DtrEnable = false;
                serial.Open();

                serial.Write(signature, 0, 4);
                serial.Write(time, 0, 7);
                serial.Write(signature, 0, 4);
                serial.Write(date, 0, 7);

                serial.Close();
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
            string portName;
            if (tryGetPortName(out portName))
            {
                // frame signature
                byte[] signature = { 0xCC, 0x33, 0x55, 0xAA };
                // get temperature command
                byte[] getTemperature = { /*0:get temperature*/ 2, /*1:empty*/  0, /*2:empty*/ 0, /*3:empty*/ 0, /*4:empty*/ 0, /*5:empty*/ 0, /*6:Checksum*/ 2 };

                // serial communication
                SerialPort serial = new SerialPort(portName, 9600);
                serial.DtrEnable = false;
                serial.Open();
                serial.Write(signature, 0, 4);
                serial.Write(getTemperature, 0, 7);

                System.Threading.Thread.Sleep(100);

                // read temperature
                for (int i = 0; i < 100; i++)
                {
                    if (serial.BytesToRead > 0)
                    {
                        tempLabel.Text = serial.ReadChar().ToString();
                        break;
                    }
                    else
                    {
                        tempLabel.Text = "failed";
                    }
                }

                serial.Close();
            }
        }


        private void getTemperatureLogButton_Click(object sender, EventArgs e)
        {
            string portName;
            if (tryGetPortName(out portName))
            {
                // frame signature
                byte[] signature = { 0xCC, 0x33, 0x55, 0xAA };
                // get temperature command
                byte[] getTemperature = { /*0:get log*/ 3, /*1:empty*/  0, /*2:empty*/ 0, /*3:empty*/ 0, /*4:empty*/ 0, /*5:empty*/ 0, /*6:Checksum*/ 2 };

                // serial communication
                SerialPort serial = new SerialPort(portName, 9600);
                serial.DtrEnable = false;
                serial.Open();
                serial.Write(signature, 0, 4);
                serial.Write(getTemperature, 0, 7);

                System.Threading.Thread.Sleep(100);

                List<TemperatureItem> logItems = new List<TemperatureItem>();
                // read temperature
                for (int i = 0; i < 100; i++)
                {
                    if (serial.BytesToRead > 0)
                    {
                        for (int n = 0; n < serial.BytesToRead; n++)
                        {
                            logItems.Add(new TemperatureItem()
                            {
                                Id = logItems.Count + 1,
                                Temperature = (byte)serial.ReadByte()
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

                serial.Close();

                chart1.Series[0].Points.Clear();
                foreach (TemperatureItem item in logItems) {
                    chart1.Series[0].Points.AddXY(item.Id, item.Temperature);
	            }                
            }
        }

        private void portComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateUi();
        }

        private void updateUi()
        {
            bool enabled = portComboBox.SelectedIndex >= 0;
            setDateTimeButton.Enabled = enabled;
            getTemperatureLogButton.Enabled = enabled;
            getTemperatureButton.Enabled = enabled;
            setDateTimeButton.Enabled = enabled;
            resetButton.Enabled = enabled;
        }
    }

    public class TemperatureItem
    {
        public TemperatureItem()
        {
        }
        public int Id { get; set; }
        public byte Temperature { get; set; }
    }
}
