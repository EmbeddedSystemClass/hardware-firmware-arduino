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
			//logFilesListView.Items.Add("Test2.csv");
			//logFilesListView.Items.Add("Test3.CSV");
			//logFilesListView.Items.Add("Test3.Csv");
			//files.Add("Test1");
			//files.Add("Test2.csv");
			//files.Add("Test3.CSV");
			//files.Add("Test3.Csv");
		}

		private void refreshButton_Click(object sender, EventArgs e) {
			logFilesListView.Items.Clear();
			if (DataLogger.Instance.TryGetDirectory(out files)) {
				foreach (string fileName in files) {                    
                    logFilesListView.Items.Add(fileName);
				}
			}
		}		

		private void logFilesListView_DoubleClick(object sender, EventArgs e) {			
			if (logFilesListView.SelectedItems.Count > 0) {
				logFileValuesListView.Items.Clear();
                List<string> lines;
				string s = logFilesListView.SelectedItems[0].Text;
				if (DataLogger.Instance.TryGetFile(s, out lines)) {
					foreach (string line in lines) {
						logFileValuesListView.Items.Add(line);
					}
				}
			}
		}

		private void saveButton_Click(object sender, EventArgs e) {			
			FolderBrowserDialog fbd = new FolderBrowserDialog();
			if (fbd.ShowDialog() == DialogResult.OK) {
				foreach (int itemIndex in logFilesListView.SelectedIndices) {
					List<string> fileLines = new List<string>();
					string fileName = removeExtension(files[itemIndex], ".csv");
					if (DataLogger.Instance.TryGetFile(fileName, out fileLines)) {						
						string destFileName = getFreeFileName(
							fbd.SelectedPath + "\\" + fileName, ".csv"
						);

						StreamWriter file = System.IO.File.CreateText(destFileName);
						foreach (string line in fileLines) {
							file.WriteLine(line);
						}
						file.Flush();
						file.Close();
					}
				}
			}			
		}

		private static string getFreeFileName(string fileName, string extension) {
			int n = 0;
			string s = fileName + extension;
			while (System.IO.File.Exists(s)) {
				n++;
				s = fileName + n.ToString() + extension;
			}
			return fileName + n.ToString() + extension;
		}

		private static string removeExtension(string fileName, string extension) {
			int p = fileName.IndexOf(extension, StringComparison.OrdinalIgnoreCase);
			if (p > 0) {
				fileName = fileName.Remove(p, extension.Length);
			}
			return fileName;
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
