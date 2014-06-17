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
			chart.Series[0].Points.Clear();
			for (int i = 0; i < 24; i++) {
				chart.Series[0].Points.AddXY(i, 0);
			}
		}

		private void refresh(object sender, EventArgs e) {
			chart.Series.Clear();

			foreach (SensorButton button in SensorButton.Instances) {
				if(button.Checked) {
					chart.Series.Add(button.Name);
					List<TemperatureItem> items;
					if (DataLogger.Instance.TryGetRAMlog(button.SensorId, out items)) {
						foreach (TemperatureItem tItem in items) {
							chart.Series[0].Points.AddXY(tItem.Id, tItem.Temperature);
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
