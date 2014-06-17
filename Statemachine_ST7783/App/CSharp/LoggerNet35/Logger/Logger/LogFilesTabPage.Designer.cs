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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.logFilesListView = new System.Windows.Forms.ListView();
            this.File = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.logFileValuesListView = new System.Windows.Forms.ListView();
            this.Values = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.logFilesListView);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.logFileValuesListView);
            this.splitContainer1.Size = new System.Drawing.Size(609, 425);
            this.splitContainer1.SplitterDistance = 212;
            this.splitContainer1.TabIndex = 19;
            // 
            // logFilesListView
            // 
            this.logFilesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.File});
            this.logFilesListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.logFilesListView.Location = new System.Drawing.Point(0, 0);
            this.logFilesListView.Name = "logFilesListView";
            this.logFilesListView.Size = new System.Drawing.Size(609, 212);
            this.logFilesListView.TabIndex = 16;
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
            // logFileValuesListView
            // 
            this.logFileValuesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Values});
            this.logFileValuesListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.logFileValuesListView.Location = new System.Drawing.Point(0, 0);
            this.logFileValuesListView.Name = "logFileValuesListView";
            this.logFileValuesListView.Size = new System.Drawing.Size(609, 209);
            this.logFileValuesListView.TabIndex = 19;
            this.logFileValuesListView.UseCompatibleStateImageBehavior = false;
            this.logFileValuesListView.View = System.Windows.Forms.View.Details;
            // 
            // Values
            // 
            this.Values.Text = "Values";
            this.Values.Width = 594;
            // 
            // LogFilesTabPage
            // 
            this.BackColor = System.Drawing.Color.White;
            this.Controls.Add(this.splitContainer1);
            this.Name = "LogFilesTabPage";
            this.Size = new System.Drawing.Size(609, 425);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ListView logFilesListView;
        private System.Windows.Forms.ColumnHeader File;
        private System.Windows.Forms.ListView logFileValuesListView;
        private System.Windows.Forms.ColumnHeader Values;

    }
}
