using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;

namespace Logger {
	public partial class CurrentTabPage : LoggerTabPage {		

		public CurrentTabPage() {
			InitializeComponent();
			if(DataLogger.Instance != null) {
				DataLogger.Instance.OnConnectionChanged += onConnectionChanged;
			}
		}

		private void onConnectionChanged(object sender, EventArgs e) {
			sensorView.Columns.Clear();
			sensorView.Columns.Add("DateTime");
			foreach (SensorItem sensor in DataLogger.Sensors) {
				sensorView.Columns.Add(sensor.Name);
			}
		}

		private void refresh(object sender, EventArgs e) {
			sensorView.SuspendLayout();
			ListViewItem item = sensorView.Items.Add(DateTime.Now.ToLongTimeString());
			foreach (SensorItem sensor in DataLogger.Sensors) {
				int value = 0;
				if (DataLogger.Instance.TryGetCurrentValue(sensor.Id, out value)) {
					item.SubItems.Add(value.ToString());
				} else {
					item.SubItems.Add("n/a");
				}
			}			
			sensorView.EnsureVisible(sensorView.Items.Count - 1);
			sensorView.ResumeLayout();
			updateUi();
		}

		private void clear(object sender, EventArgs e) {
			sensorView.Items.Clear();
			updateUi();
		}

		private void save(object sender, EventArgs e) {
			SaveFileDialog sfd = new SaveFileDialog();
			sfd.Filter = "*.csv | *.csv";
			if (sfd.ShowDialog() == DialogResult.OK) {
				StreamWriter sw = new StreamWriter(sfd.FileName);				
				foreach (ListViewItem item in sensorView.Items) {
					StringBuilder sb = new StringBuilder();					
					foreach (ListViewItem.ListViewSubItem value in item.SubItems) {						
						sb.Append(value.Text);
						sb.Append(";");
					}			
					sw.WriteLine(sb.ToString());
				}
				sw.Flush();
				sw.Close();
			}
		}

		private void updateUi() {
			SaveFileButton.Instance.Enabled = sensorView.Items.Count > 0;
		}

		public override void OnActivate() {
			RefreshButton.Instance.AddTo(Main.Instance.ToolStrip);
			RefreshButton.Instance.Refresh += refresh;

			ClearButton.Instance.AddTo(Main.Instance.ToolStrip);
			ClearButton.Instance.Click += clear;

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

			SaveFileButton.Instance.RemoveFrom(Main.Instance.ToolStrip);
			base.OnDeactivate();
		}
	}	
}
