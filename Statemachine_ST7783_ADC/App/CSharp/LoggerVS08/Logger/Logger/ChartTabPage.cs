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
					series.Points.AddXY(n, 0);
				}
			}
		}

		private void refresh(object sender, EventArgs e) {
			chart.SuspendLayout();

			for (int i = 0; i < SensorButton.Instances.Count; i++) {
				SensorButton sensorButton = SensorButton.Instances[i];
				if (sensorButton.Checked) {
					List<TemperatureItem> temperaturItems;
					if (DataLogger.Instance.TryGetDayLog(sensorButton.SensorId, out temperaturItems)) {
						Series series = chart.Series[i];
						series.Enabled = true;
						series.Points.Clear();
						for (int n = 0; n < 24; n++) {
							series.Points.AddXY(23-n, (double)temperaturItems[n].Temperature);
						}
					}
				} else {
					chart.Series[i].Enabled = false;
				}
			}
			
			ChartArea chartArea = chart.ChartAreas[0];
			chartArea.RecalculateAxesScale();


			double[] scale = { 5, 10, 25, 50, 75, 150 };

			double max1 = chart.Series[0].Points.FindMaxByValue().YValues[0];
			double max2 = chart.Series[1].Points.FindMaxByValue().YValues[0];

			double min1 = chart.Series[0].Points.FindMinByValue().YValues[0];
			double min2 = chart.Series[1].Points.FindMinByValue().YValues[0];

			double m1 = System.Math.Max(System.Math.Abs(min1), max1);
			double m2 = System.Math.Max(System.Math.Abs(min2), max2);

			bool isNeg = min1 < 0 || min2 < 0;

			for (int i = 0; i < scale.Length; i++) {
				if (scale[i] > m1) {
					chartArea.AxisY.Maximum = scale[i];
					if (isNeg) {
						chartArea.AxisY.Minimum = -scale[i];
					}
					break;
				}
			}

			for (int i = 0; i < scale.Length; i++) {
				if (scale[i] > m2) {
					chartArea.AxisY2.Maximum = scale[i];
					if (isNeg) {
						chartArea.AxisY2.Minimum = -scale[i];
					}
					break;
				}
			}

			updateUi();

			chart.ResumeLayout();
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

		void printChart(object sender, EventArgs e) {
			chart.Printing.PrintDocument.DefaultPageSettings.Landscape = true;			
			chart.Printing.PrintPreview();
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

			PrintButton.Instance.AddTo(Main.Instance.ToolStrip);
			PrintButton.Instance.Click += printChart;

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
			SaveFileButton.Instance.Click -= save;

			PrintButton.Instance.Click -= save;
			PrintButton.Instance.RemoveFrom(Main.Instance.ToolStrip);
			
			base.OnDeactivate();
		}
	}

	public class PrintButton : ToolStripButton {
		private ToolStripSeparator separatorButton;

		public static PrintButton Instance = new PrintButton();

		public void AddTo(ToolStrip toolStrip) {
			toolStrip.Items.Add(separatorButton);
			toolStrip.Items.Add(this);
		}

		public void RemoveFrom(ToolStrip toolStrip) {
			toolStrip.Items.Remove(separatorButton);
			toolStrip.Items.Remove(this);
		}

		public PrintButton() {
			separatorButton = new ToolStripSeparator();
			Image = ImageResource.Print16x16;
		}
	}
}
