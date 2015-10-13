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
				string s = logFilesListView.SelectedItems[0].Text;
				enableCtrls(false);
				ProgressBar.Instance.Value = 0;
				ProgressBar.Instance.Style = ProgressBarStyle.Blocks;
				ProgressBar.Instance.Cancel += new EventHandler(cancel_FileRequest);
				ProgressBar.Instance.Visible = true;								
				SingleFileRequest.Instance.OnAsyncReady += singleFileRequest_OnAsyncReady;
				SingleFileRequest.Instance.Begin(s);
			}
		}		
		
		private void saveButton_Click(object sender, EventArgs e) {			
			FolderBrowserDialog fbd = new FolderBrowserDialog();
			if (fbd.ShowDialog() == DialogResult.OK) {
				enableCtrls(false);
				ProgressBar.Instance.Value = 0;
				ProgressBar.Instance.Style = ProgressBarStyle.Blocks;
				ProgressBar.Instance.Cancel += new EventHandler(cancel_FileRequest);
				ProgressBar.Instance.Visible = true;				
				SaveFileButton.Instance.Enabled = false;
				RefreshButton.Instance.Enabled = false;
				HomeButton.Instance.Enabled = false;
				
				List<string> fileNames = new List<string>();
				foreach (int itemIndex in logFilesListView.SelectedIndices) {					
					string fileName = removeExtension(files[itemIndex], ".csv");
					fileNames.Add(fileName);
				}
				MultiFileRequest.Instance.OnAsyncReady += multiFileRequest_OnAsyncReady;	
				MultiFileRequest.Instance.Begin(fileNames, fbd.SelectedPath);
			}			
		}

		void singleFileRequest_OnAsyncReady(object sender, EventArgs e) {
			if (InvokeRequired) {
				Invoke((System.Windows.Forms.MethodInvoker)
						delegate() {
							singleFileRequest_OnAsyncReady(sender, e);
						}
					);
				return;
			}
			logFilesListView.Enabled = true;
			SingleFileRequest.Instance.OnAsyncReady -= singleFileRequest_OnAsyncReady;			
			ProgressBar.Instance.Visible = false;

			if (SingleFileRequest.Instance.Lines != null && SingleFileRequest.Instance.Lines.Count > 0) {
				foreach (string line in SingleFileRequest.Instance.Lines) {
					logFileValuesListView.Items.Add(line);
				}
			}

			enableCtrls(true);
		}		

		void multiFileRequest_OnAsyncReady(object sender, EventArgs e) {
			if (InvokeRequired) {
				Invoke((System.Windows.Forms.MethodInvoker)
					delegate() {
						multiFileRequest_OnAsyncReady(sender, e);
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

		void cancel_FileRequest(object sender, EventArgs e) {
			ProgressBar.Instance.Visible = false;
			SingleFileRequest.Instance.OnAsyncReady -= singleFileRequest_OnAsyncReady;
			SingleFileRequest.Instance.Abort();

			MultiFileRequest.Instance.OnAsyncReady -= multiFileRequest_OnAsyncReady;
			MultiFileRequest.Instance.Abort();

			enableCtrls(true);
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

		private void enableCtrls(bool bEnable) {
			logFilesListView.Enabled = bEnable;
			SaveFileButton.Instance.Enabled = bEnable;
			RefreshButton.Instance.Enabled = bEnable;
			HomeButton.Instance.Enabled = bEnable;
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

	class MultiFileRequest {
		private List<string> fileNames;
		private string destinationPath;
        private CancellationTokenSource cancellationToken = new CancellationTokenSource();

		public static MultiFileRequest Instance;

		public event EventHandler OnAsyncReady;

		public void Begin(List<string> fileNames, string destinationPath) {
			this.fileNames = fileNames;
			this.destinationPath = destinationPath;
            cancellationToken = new CancellationTokenSource();
			ThreadPool.QueueUserWorkItem(callMethod);
		}

		public void Abort() {
            cancellationToken.Cancel();
		}

		private void callMethod(object state) {
			try {
				foreach (string fileName in fileNames) {
                    if (cancellationToken.IsCancellationRequested)
                    {
                        break;
                    }

					List<string> fileLines = new List<string>();
					if (DataLogger.Instance.TryGetFile(fileName, out fileLines, cancellationToken)) {
						string destFileName = getFreeFileName(
							destinationPath + "\\" + fileName.Replace(".TXT", ""), ".csv"
						);

						StreamWriter file = System.IO.File.CreateText(destFileName);
						foreach (string line in fileLines) {
							file.WriteLine(line);
						}
						file.Flush();
						file.Close();
					}
				}
			} catch /*(Exception e)*/ {
				
			}

            if (!cancellationToken.IsCancellationRequested && OnAsyncReady != null)
            {
				OnAsyncReady(this, EventArgs.Empty);
			}
		}

		private static string getFreeFileName(string fileName, string extension) {
			int n = 0;
			string s = fileName + "-" + n.ToString() + extension;
			while (System.IO.File.Exists(s)) {
				n++;
				s = fileName + "-" + n.ToString() + extension;
			}
			return s;
		}
	}

	public class SingleFileRequest {
        private CancellationTokenSource cancellationToken = new CancellationTokenSource();
        
		public string FileName;
		public List<string> Lines;        

		public static SingleFileRequest Instance;

		public event EventHandler OnAsyncReady;

		public void Begin(string fileName) {
			FileName = fileName;
            cancellationToken = new CancellationTokenSource();
			ThreadPool.QueueUserWorkItem(callMethod);
		}

		public void Abort() {
            cancellationToken.Cancel();
		}

		private void callMethod(object state) {
			try {
				Instance.Lines = new List<string>();
                DataLogger.Instance.TryGetFile(Instance.FileName, out Instance.Lines, cancellationToken);
			} catch /*(Exception e)*/ {
				
			}

            if (!cancellationToken.IsCancellationRequested && OnAsyncReady != null)
            {
				OnAsyncReady(this, EventArgs.Empty);
			}
		}
	}
}
