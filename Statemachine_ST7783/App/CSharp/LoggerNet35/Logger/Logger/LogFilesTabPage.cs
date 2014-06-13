using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;

namespace Logger {
	public partial class LogFilesTabPage : LoggerTabPage {
		private List<string> files;

		public LogFilesTabPage() {
			InitializeComponent();

			files = new List<string>();
			//logFilesListView.Items.Add("Test1");
			//logFilesListView.Items.Add("Test2");
			//logFilesListView.Items.Add("Test3");
		}

		private void refreshButton_Click(object sender, EventArgs e) {
			logFilesListView.Clear();
			List<string> files;
			if (DataLogger.Instance.TryGetDirectory(out files)) {
				foreach (string fileName in files) {
					logFilesListView.Items.Add(fileName);
				}
			}
		}		

		private void logFilesListView_DoubleClick(object sender, EventArgs e) {			
			if (logFilesListView.SelectedItems.Count > 0) {
				logFileValuesListView.Clear();
				files.Clear();
				string s = logFilesListView.SelectedItems[0].Text;
				if (DataLogger.Instance.TryGetFile(s, out files)) {
					foreach (string line in files) {
						logFileValuesListView.Items.Add(line);
					}
				}
			}
		}

		private void saveButton_Click(object sender, EventArgs e) {
			if (logFilesListView.SelectedIndices.Count == 1) {
				SaveFileDialog sfd = new SaveFileDialog();
				sfd.Filter.Insert(0, "*.csv|*.csv");
				sfd.Filter = "csv files (*.csv)|*.txt|All files (*.*)|*.*";
				sfd.FilterIndex = 1;
				sfd.RestoreDirectory = true;
				if (sfd.ShowDialog() == DialogResult.OK) {
					foreach (int itemIndex in logFilesListView.SelectedIndices) {
						List<string> fileLines = new List<string>();
						string fileName = files[logFilesListView.SelectedIndices[0]];
						if (DataLogger.Instance.TryGetFile(fileName, out fileLines)) {
							StreamWriter file = System.IO.File.CreateText(
								sfd.FileName + "\\" + fileName
							);
							foreach (string line in fileLines) {
								file.Write(line);
							}
							file.Flush();
							file.Close();
						}
					}
				}
			} else {
				FolderBrowserDialog fbd = new FolderBrowserDialog();
				if (fbd.ShowDialog() == DialogResult.OK) {
					foreach (int itemIndex in logFilesListView.SelectedIndices) {
						List<string> fileLines = new List<string>();
						string fileName = files[itemIndex];
						if (DataLogger.Instance.TryGetFile(fileName, out fileLines)) {
							StreamWriter file = System.IO.File.CreateText(
								fbd.SelectedPath + "\\" + fileName
							);
							foreach (string line in fileLines) {
								file.Write(line);
							}
							file.Flush();
							file.Close();
						}
					}
				}
			}
		}

		private void logFilesListView_SelectedIndexChanged(object sender, EventArgs e) {
			SaveFileButton.Instance.Enabled = logFilesListView.SelectedIndices.Count > 0;
		}

		public override void OnActivate() {
			RefreshButton.Instance.AddTo(Main.Instance.ToolStrip);
			RefreshButton.Instance.Click += refreshButton_Click;

			SaveFileButton.Instance.AddTo(Main.Instance.ToolStrip);
			SaveFileButton.Instance.Click += saveButton_Click;

			SaveFileButton.Instance.Enabled = logFilesListView.SelectedIndices.Count > 0;
			base.OnActivate();
		}		

		public override void OnDeactivate() {
			RefreshButton.Instance.RemoveFrom(Main.Instance.ToolStrip);
			RefreshButton.Instance.Click -= refreshButton_Click;

			SaveFileButton.Instance.RemoveFrom(Main.Instance.ToolStrip);
			SaveFileButton.Instance.Click -= saveButton_Click;
			base.OnDeactivate();
		}		
	}

	public class SaveFileButton : ToolStripButton {
		private ToolStripSeparator separatorButton;

		public static SaveFileButton Instance = new SaveFileButton();

		public void AddTo(ToolStrip toolStrip) {
			toolStrip.Items.Add(separatorButton);
			toolStrip.Items.Add(this);
		}

		public void RemoveFrom(ToolStrip toolStrip) {
			toolStrip.Items.Remove(separatorButton);
			toolStrip.Items.Remove(this);
		}

		public SaveFileButton() {
			separatorButton = new ToolStripSeparator();
			Image = ImageResource.Save16x16;
		}
	}
}
