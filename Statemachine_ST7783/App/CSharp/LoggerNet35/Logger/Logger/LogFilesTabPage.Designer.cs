namespace Logger {
	partial class LogFilesTabPage {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.logFileValuesListView = new System.Windows.Forms.ListView();
			this.Values = new System.Windows.Forms.ColumnHeader();
			this.logFilesListView = new System.Windows.Forms.ListView();
			this.File = new System.Windows.Forms.ColumnHeader();
			this.SuspendLayout();
			// 
			// logFileValuesListView
			// 
			this.logFileValuesListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.logFileValuesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Values});
			this.logFileValuesListView.Location = new System.Drawing.Point(3, 197);
			this.logFileValuesListView.Name = "logFileValuesListView";
			this.logFileValuesListView.Size = new System.Drawing.Size(603, 155);
			this.logFileValuesListView.TabIndex = 16;
			this.logFileValuesListView.UseCompatibleStateImageBehavior = false;
			this.logFileValuesListView.View = System.Windows.Forms.View.Details;
			// 
			// Values
			// 
			this.Values.Text = "Values";
			this.Values.Width = 594;
			// 
			// logFilesListView
			// 
			this.logFilesListView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.logFilesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.File});
			this.logFilesListView.Location = new System.Drawing.Point(3, 3);
			this.logFilesListView.Name = "logFilesListView";
			this.logFilesListView.Size = new System.Drawing.Size(603, 188);
			this.logFilesListView.TabIndex = 15;
			this.logFilesListView.UseCompatibleStateImageBehavior = false;
			this.logFilesListView.View = System.Windows.Forms.View.Details;
			this.logFilesListView.SelectedIndexChanged += new System.EventHandler(this.logFilesListView_SelectedIndexChanged);
			this.logFilesListView.DoubleClick += new System.EventHandler(this.logFilesListView_DoubleClick);
			// 
			// File
			// 
			this.File.Text = "File";
			this.File.Width = 441;
			// 
			// LogFilesTabPage
			// 
			this.BackColor = System.Drawing.Color.White;
			this.Controls.Add(this.logFileValuesListView);
			this.Controls.Add(this.logFilesListView);
			this.Name = "LogFilesTabPage";
			this.Size = new System.Drawing.Size(609, 425);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ListView logFileValuesListView;
		private System.Windows.Forms.ColumnHeader Values;
		private System.Windows.Forms.ListView logFilesListView;
		private System.Windows.Forms.ColumnHeader File;
	}
}
