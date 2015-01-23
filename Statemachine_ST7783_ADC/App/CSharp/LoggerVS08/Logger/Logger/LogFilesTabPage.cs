using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Threading;

namespace Logger {
	public partial class LogFilesTabPage : LoggerTabPage {
		private List<string> files;

		public LogFilesTabPage() {
			InitializeComponent();

			files = new List<string>();
			//logFilesListView.Items.Add("Test1");			
			//files.Add("Test1");			
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
				ProgressBar.Instance.Style = ProgressBarStyle.Blocks;
				ProgressBar.Instance.Visible = true;
				if (DataLogger.Instance.TryGetFile(s, out lines)) {
					foreach (string line in lines) {
						logFileValuesListView.Items.Add(line);
					}
				}
				ProgressBar.Instance.Visible = false;
			}
		}

		private void saveButton_Click(object sender, EventArgs e) {			
			FolderBrowserDialog fbd = new FolderBrowserDialog();
			if (fbd.ShowDialog() == DialogResult.OK) {
				ProgressBar.Instance.Style = ProgressBarStyle.Blocks;
				ProgressBar.Instance.Visible = true;
				SaveFileButton.Instance.Enabled = false;
				RefreshButton.Instance.Enabled = false;
				HomeButton.Instance.Enabled = false;
				logFilesListView.Enabled = false;

				List<string> fileNames = new List<string>();
				foreach (int itemIndex in logFilesListView.SelectedIndices) {					
					string fileName = removeExtension(files[itemIndex], ".csv");
					fileNames.Add(fileName);
				}
				FileRequest fileRequest = new FileRequest();
				fileRequest.Begin(fileNames, fbd.SelectedPath);
				fileRequest.OnAsyncReady += new EventHandler(fileRequest_OnAsyncReady);				
			}			
		}

		void fileRequest_OnAsyncReady(object sender, EventArgs e) {
			if (InvokeRequired) {
				Invoke((System.Windows.Forms.MethodInvoker)
					delegate() {
						fileRequest_OnAsyncReady(sender, e);
					}
				);
				return;				
			}
			
			RefreshButton.Instance.Enabled = true;
			SaveFileButton.Instance.Enabled = true;
			HomeButton.Instance.Enabled = true;
			logFilesListView.Enabled = true;			
			ProgressBar.Instance.Visible = false;
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

			ProgressBar.Instance.AddTo(Main.Instance.ToolStrip, true);
			ProgressBar.Instance.Visible = false;
			base.OnActivate();
		}		

		public override void OnDeactivate() {
			RefreshButton.Instance.RemoveFrom(Main.Instance.ToolStrip);
			RefreshButton.Instance.Click -= refreshButton_Click;

			SaveFileButton.Instance.RemoveFrom(Main.Instance.ToolStrip);
			SaveFileButton.Instance.Click -= saveButton_Click;

			ProgressBar.Instance.RemoveFrom(Main.Instance.ToolStrip);
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

	class FileRequest {
		private List<string> fileNames;
		private string destinationPath;

		public event EventHandler OnAsyncReady;

		public Exception Exception;

		public void Begin(List<string> fileNames, string destinationPath) {
			this.fileNames = fileNames;
			this.destinationPath = destinationPath;
			ThreadPool.QueueUserWorkItem(callMethod);
		}

		private void callMethod(object state) {
			try {
				foreach (string fileName in fileNames) {
					List<string> fileLines = new List<string>();
					if (DataLogger.Instance.TryGetFile(fileName, out fileLines)) {
						string destFileName = getFreeFileName(
							destinationPath + "\\" + fileName, ".csv"
						);

						StreamWriter file = System.IO.File.CreateText(destFileName);
						foreach (string line in fileLines) {
							file.WriteLine(line);
						}
						file.Flush();
						file.Close();
					}
				}
			} catch (Exception e) {
				this.Exception = e;				
			}			

			OnAsyncReady(this, EventArgs.Empty);
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
	}
}
