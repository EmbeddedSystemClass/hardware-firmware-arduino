using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Windows.Forms.DataVisualization.Charting;

namespace Logger {
	public partial class ChartTabPage : LoggerTabPage {
		public ChartTabPage() {
			InitializeComponent();

			resetTemperatureChart();
		}

		private void resetTemperatureChart() {
			for (int i = 0; i < 2; i++) {
				Series series = chart.Series[i];
				series.Points.Clear();
				for (int n = 0; n < 24; n++) {
					series.Points.AddXY(n + 1, 0);
				}
			}
		}

		private void refresh(object sender, EventArgs e) {
			for (int i = 0; i < SensorButton.Instances.Count; i++) {
				SensorButton sensorButton = SensorButton.Instances[i];
				if(sensorButton.Checked) {
					List<TemperatureItem> temperaturItems;
					if (DataLogger.Instance.TryGetDayLog(sensorButton.SensorId, out temperaturItems)) {						
						Series series = chart.Series[i];
						series.Points.Clear();
						for (int n = 0; n < 24; n++) {							
							series.Points.AddXY(n + 1, (double)temperaturItems[n].Temperature);
						}
					}
				}
			}

			updateUi();
		}

		private void printMenuItem_Click(object sender, EventArgs e) {
			chart.Printing.PageSetup();
			chart.Printing.PrintPreview();
		}

		private void clear(object sender, EventArgs e) {
			chart.Series.Clear();
			updateUi();
		}

		private void save(object sender, EventArgs e) {
			SaveFileDialog sfd = new SaveFileDialog();
			sfd.Filter = "*.csv | *.csv";
			if (sfd.ShowDialog() == DialogResult.OK) {
				StreamWriter sw = new StreamWriter(sfd.FileName);
				foreach (Series series in chart.Series) {
					StringBuilder sb = new StringBuilder();
					sb.Append(series.Name);
					sb.Append(";");
					foreach (var point in series.Points) {
						sb.Append(point.YValues[0]);
						sb.Append(";");
					}
					sw.WriteLine(sb.ToString());
				}
				sw.Flush();
				sw.Close();
			}
		}

		private void updateUi() {
			SaveFileButton.Instance.Enabled = chart.Series.Count > 0;
		}

		public override void OnActivate() {
			RefreshButton.Instance.TimerButtonEnabled = false;
			RefreshButton.Instance.AddTo(Main.Instance.ToolStrip);
			RefreshButton.Instance.Refresh += refresh;

			ClearButton.Instance.AddTo(Main.Instance.ToolStrip);
			ClearButton.Instance.Click += clear;

			SensorButton.AddTo(Main.Instance.ToolStrip);

			SaveFileButton.Instance.AddTo(Main.Instance.ToolStrip);
			SaveFileButton.Instance.Click += save;

			updateUi();

			base.OnActivate();
		}

		public override void OnDeactivate() {
			RefreshButton.Instance.TimerButtonEnabled = true;
			RefreshButton.Instance.RemoveFrom(Main.Instance.ToolStrip);
			RefreshButton.Instance.Refresh -= refresh;

			ClearButton.Instance.RemoveFrom(Main.Instance.ToolStrip);
			ClearButton.Instance.Click -= clear;

			SensorButton.RemoveFrom(Main.Instance.ToolStrip);

			SaveFileButton.Instance.RemoveFrom(Main.Instance.ToolStrip);
			
			base.OnDeactivate();
		}
	}
}
