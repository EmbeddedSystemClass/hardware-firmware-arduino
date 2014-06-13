using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace Logger {
	public partial class TemperatureTabPage : LoggerTabPage {		

		public TemperatureTabPage() {
			InitializeComponent();
		}

		void refreshButton_Click(object sender, EventArgs e) {			
			int temperature;
			if (DataLogger.Instance.TryGetCurrentTemperature(out temperature)) {
				temperatureLabel.Text = string.Format("%d °C", temperature);
			} else {
				temperatureLabel.Text = "failed";
			}
		}

		public override void OnActivate() {
			RefreshButton.Instance.AddTo(Main.Instance.ToolStrip);
			RefreshButton.Instance.Click += refreshButton_Click;
			base.OnActivate();
		}

		public override void OnDeactivate() {
			RefreshButton.Instance.RemoveFrom(Main.Instance.ToolStrip);
			RefreshButton.Instance.Click -= refreshButton_Click;
			base.OnDeactivate();
		}
	}	
}
