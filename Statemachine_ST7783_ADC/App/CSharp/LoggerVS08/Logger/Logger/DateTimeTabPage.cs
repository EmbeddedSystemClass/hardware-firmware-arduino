using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Logger {
	public partial class DateTimeTabPage : LoggerTabPage {
		public DateTimeTabPage() {
			InitializeComponent();
		}

		private void updateTimer_Tick(object sender, EventArgs e) {
			systemDateLabel.Text = DateTime.Now.Date.ToLongDateString();
			systemTimeLabel.Text = DateTime.Now.ToString("HH:mm:ss");
		}

		private void setDateButton_Click(object sender, EventArgs e) {
			DataLogger.Instance.SetDate(datePicker.Value);
		}

		private void setTimeButton_Click(object sender, EventArgs e) {
			DataLogger.Instance.SetTime(timePicker.Value);
		}

		private void setSystemDateButton_Click(object sender, EventArgs e) {
			DataLogger.Instance.SetDate(DateTime.Now);
		}

		private void setSystemTimeButton_Click(object sender, EventArgs e) {
			DataLogger.Instance.SetTime(DateTime.Now);
		}

		private void setDateTime_Click(object sender, EventArgs e) {
			DataLogger.Instance.SetTime(timePicker.Value);
			DataLogger.Instance.SetDate(datePicker.Value);
		}

		private void setSystemDateTime_Click(object sender, EventArgs e) {
			DataLogger.Instance.SetTime(DateTime.Now);
			DataLogger.Instance.SetDate(DateTime.Now);
		}
	}
}
