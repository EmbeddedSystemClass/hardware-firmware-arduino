using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace Logger {
	public partial class TemperatureChartTabPage : LoggerTabPage {
		public TemperatureChartTabPage() {
			InitializeComponent();

			resetTemperatureChart();
		}

		private void resetTemperatureChart() {
			temperatureChart.Series[0].Points.Clear();
			for (int i = 0; i < 24; i++) {
				temperatureChart.Series[0].Points.AddXY(i, 0);
			}
		}

		private void refreshButton_Click(object sender, EventArgs e) {
			temperatureChart.Series[0].Points.Clear();
			List<TemperatureItem> items;
			if (DataLogger.Instance.TryGetDaylog(out items)) {
				foreach (TemperatureItem item in items) {
					temperatureChart.Series[0].Points.AddXY(item.Id, item.Temperature);
				}
			}
		}

		private void printMenuItem_Click(object sender, EventArgs e) {
			temperatureChart.Printing.PageSetup();
			temperatureChart.Printing.PrintPreview();
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
