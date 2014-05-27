namespace Logger
{
    partial class Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.components = new System.ComponentModel.Container();
			System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
			System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
			System.Windows.Forms.DataVisualization.Charting.Title title2 = new System.Windows.Forms.DataVisualization.Charting.Title();
			this.setDateTimeButton = new System.Windows.Forms.Button();
			this.portComboBox = new System.Windows.Forms.ComboBox();
			this.resetButton = new System.Windows.Forms.Button();
			this.portComboLabel = new System.Windows.Forms.Label();
			this.getTemperatureButton = new System.Windows.Forms.Button();
			this.tempLabel = new System.Windows.Forms.Label();
			this.getTemperatureLogButton = new System.Windows.Forms.Button();
			this.temperatureChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
			this.chartMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.printMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.getDirectoryButton = new System.Windows.Forms.Button();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.logChartTabPage = new System.Windows.Forms.TabPage();
			this.homeTabPage = new System.Windows.Forms.TabPage();
			this.logFilesTabPage = new System.Windows.Forms.TabPage();
			this.logFilesListView = new System.Windows.Forms.ListView();
			this.logFileValuesListView = new System.Windows.Forms.ListView();
			this.File = new System.Windows.Forms.ColumnHeader();
			this.Values = new System.Windows.Forms.ColumnHeader();
			((System.ComponentModel.ISupportInitialize)(this.temperatureChart)).BeginInit();
			this.chartMenuStrip.SuspendLayout();
			this.tabControl1.SuspendLayout();
			this.logChartTabPage.SuspendLayout();
			this.homeTabPage.SuspendLayout();
			this.logFilesTabPage.SuspendLayout();
			this.SuspendLayout();
			// 
			// setDateTimeButton
			// 
			this.setDateTimeButton.Location = new System.Drawing.Point(9, 33);
			this.setDateTimeButton.Name = "setDateTimeButton";
			this.setDateTimeButton.Size = new System.Drawing.Size(95, 23);
			this.setDateTimeButton.TabIndex = 1;
			this.setDateTimeButton.Text = "Set DateTime";
			this.setDateTimeButton.UseVisualStyleBackColor = true;
			this.setDateTimeButton.Click += new System.EventHandler(this.setDateTimeButton_Click);
			// 
			// portComboBox
			// 
			this.portComboBox.FormattingEnabled = true;
			this.portComboBox.Location = new System.Drawing.Point(41, 6);
			this.portComboBox.Name = "portComboBox";
			this.portComboBox.Size = new System.Drawing.Size(63, 21);
			this.portComboBox.TabIndex = 0;
			this.portComboBox.SelectedIndexChanged += new System.EventHandler(this.portComboBox_SelectedIndexChanged);
			// 
			// resetButton
			// 
			this.resetButton.Location = new System.Drawing.Point(9, 91);
			this.resetButton.Name = "resetButton";
			this.resetButton.Size = new System.Drawing.Size(95, 23);
			this.resetButton.TabIndex = 2;
			this.resetButton.Text = "Reset";
			this.resetButton.UseVisualStyleBackColor = true;
			this.resetButton.Click += new System.EventHandler(this.resetButton_Click);
			// 
			// portComboLabel
			// 
			this.portComboLabel.AutoSize = true;
			this.portComboLabel.Location = new System.Drawing.Point(6, 9);
			this.portComboLabel.Name = "portComboLabel";
			this.portComboLabel.Size = new System.Drawing.Size(29, 13);
			this.portComboLabel.TabIndex = 3;
			this.portComboLabel.Text = "Port:";
			// 
			// getTemperatureButton
			// 
			this.getTemperatureButton.Location = new System.Drawing.Point(9, 62);
			this.getTemperatureButton.Name = "getTemperatureButton";
			this.getTemperatureButton.Size = new System.Drawing.Size(95, 23);
			this.getTemperatureButton.TabIndex = 4;
			this.getTemperatureButton.Text = "Get Temp";
			this.getTemperatureButton.UseVisualStyleBackColor = true;
			this.getTemperatureButton.Click += new System.EventHandler(this.getTemperatureButton_Click);
			// 
			// tempLabel
			// 
			this.tempLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.tempLabel.Location = new System.Drawing.Point(110, 62);
			this.tempLabel.Name = "tempLabel";
			this.tempLabel.Size = new System.Drawing.Size(95, 23);
			this.tempLabel.TabIndex = 5;
			this.tempLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// getTemperatureLogButton
			// 
			this.getTemperatureLogButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.getTemperatureLogButton.Location = new System.Drawing.Point(454, 6);
			this.getTemperatureLogButton.Name = "getTemperatureLogButton";
			this.getTemperatureLogButton.Size = new System.Drawing.Size(95, 23);
			this.getTemperatureLogButton.TabIndex = 7;
			this.getTemperatureLogButton.Text = "Get Log";
			this.getTemperatureLogButton.UseVisualStyleBackColor = true;
			this.getTemperatureLogButton.Click += new System.EventHandler(this.getTemperatureLogButton_Click);
			// 
			// chart1
			// 
			this.temperatureChart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.temperatureChart.BorderlineColor = System.Drawing.Color.Black;
			this.temperatureChart.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
			chartArea2.AxisX.MajorGrid.Enabled = false;
			chartArea2.AxisX.Title = "Hours";
			chartArea2.AxisY.TextOrientation = System.Windows.Forms.DataVisualization.Charting.TextOrientation.Horizontal;
			chartArea2.AxisY.Title = "°C";
			chartArea2.Name = "ChartArea1";
			this.temperatureChart.ChartAreas.Add(chartArea2);
			this.temperatureChart.ContextMenuStrip = this.chartMenuStrip;
			this.temperatureChart.Location = new System.Drawing.Point(6, 6);
			this.temperatureChart.Name = "chart1";
			series2.ChartArea = "ChartArea1";
			series2.Name = "Series1";
			this.temperatureChart.Series.Add(series2);
			this.temperatureChart.Size = new System.Drawing.Size(442, 371);
			this.temperatureChart.TabIndex = 8;
			this.temperatureChart.Text = "chart1";
			title2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
			title2.Name = "Title1";
			title2.Text = "Temperature";
			this.temperatureChart.Titles.Add(title2);
			// 
			// chartMenuStrip
			// 
			this.chartMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.printMenuItem});
			this.chartMenuStrip.Name = "chartMenuStrip";
			this.chartMenuStrip.Size = new System.Drawing.Size(100, 26);
			// 
			// printMenuItem
			// 
			this.printMenuItem.Name = "printMenuItem";
			this.printMenuItem.Size = new System.Drawing.Size(99, 22);
			this.printMenuItem.Text = "Print";
			this.printMenuItem.Click += new System.EventHandler(this.printMenuItem_Click);
			// 
			// getDirectoryButton
			// 
			this.getDirectoryButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.getDirectoryButton.Location = new System.Drawing.Point(454, 6);
			this.getDirectoryButton.Name = "getDirectoryButton";
			this.getDirectoryButton.Size = new System.Drawing.Size(95, 23);
			this.getDirectoryButton.TabIndex = 11;
			this.getDirectoryButton.Text = "Get Dir";
			this.getDirectoryButton.UseVisualStyleBackColor = true;
			this.getDirectoryButton.Click += new System.EventHandler(this.getDirectoryButton_Click);
			// 
			// tabControl1
			// 
			this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl1.Controls.Add(this.homeTabPage);
			this.tabControl1.Controls.Add(this.logChartTabPage);
			this.tabControl1.Controls.Add(this.logFilesTabPage);
			this.tabControl1.Location = new System.Drawing.Point(12, 12);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(563, 409);
			this.tabControl1.TabIndex = 12;
			// 
			// logChartTabPage
			// 
			this.logChartTabPage.Controls.Add(this.temperatureChart);
			this.logChartTabPage.Controls.Add(this.getTemperatureLogButton);
			this.logChartTabPage.Location = new System.Drawing.Point(4, 22);
			this.logChartTabPage.Name = "logChartTabPage";
			this.logChartTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.logChartTabPage.Size = new System.Drawing.Size(555, 383);
			this.logChartTabPage.TabIndex = 0;
			this.logChartTabPage.Text = "Log Chart";
			this.logChartTabPage.UseVisualStyleBackColor = true;
			// 
			// homeTabPage
			// 
			this.homeTabPage.Controls.Add(this.setDateTimeButton);
			this.homeTabPage.Controls.Add(this.getTemperatureButton);
			this.homeTabPage.Controls.Add(this.portComboLabel);
			this.homeTabPage.Controls.Add(this.tempLabel);
			this.homeTabPage.Controls.Add(this.portComboBox);
			this.homeTabPage.Controls.Add(this.resetButton);
			this.homeTabPage.Location = new System.Drawing.Point(4, 22);
			this.homeTabPage.Name = "homeTabPage";
			this.homeTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.homeTabPage.Size = new System.Drawing.Size(555, 383);
			this.homeTabPage.TabIndex = 1;
			this.homeTabPage.Text = "Home";
			this.homeTabPage.UseVisualStyleBackColor = true;
			// 
			// logFilesTabPage
			// 
			this.logFilesTabPage.Controls.Add(this.logFileValuesListView);
			this.logFilesTabPage.Controls.Add(this.logFilesListView);
			this.logFilesTabPage.Controls.Add(this.getDirectoryButton);
			this.logFilesTabPage.Location = new System.Drawing.Point(4, 22);
			this.logFilesTabPage.Name = "logFilesTabPage";
			this.logFilesTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.logFilesTabPage.Size = new System.Drawing.Size(555, 383);
			this.logFilesTabPage.TabIndex = 2;
			this.logFilesTabPage.Text = "Log files";
			this.logFilesTabPage.UseVisualStyleBackColor = true;
			// 
			// logFilesListView
			// 
			this.logFilesListView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.logFilesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.File});
			this.logFilesListView.Location = new System.Drawing.Point(3, 6);
			this.logFilesListView.Name = "logFilesListView";
			this.logFilesListView.Size = new System.Drawing.Size(445, 188);
			this.logFilesListView.TabIndex = 12;
			this.logFilesListView.UseCompatibleStateImageBehavior = false;
			this.logFilesListView.View = System.Windows.Forms.View.Details;
			this.logFilesListView.DoubleClick += new System.EventHandler(this.logFilesListView_DoubleClick);
			// 
			// logFileValuesListView
			// 
			this.logFileValuesListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.logFileValuesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Values});
			this.logFileValuesListView.Location = new System.Drawing.Point(6, 200);
			this.logFileValuesListView.Name = "logFileValuesListView";
			this.logFileValuesListView.Size = new System.Drawing.Size(442, 177);
			this.logFileValuesListView.TabIndex = 13;
			this.logFileValuesListView.UseCompatibleStateImageBehavior = false;
			this.logFileValuesListView.View = System.Windows.Forms.View.Details;
			// 
			// File
			// 
			this.File.Text = "File";
			this.File.Width = 441;
			// 
			// Values
			// 
			this.Values.Text = "Values";
			this.Values.Width = 438;
			// 
			// Main
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(587, 433);
			this.Controls.Add(this.tabControl1);
			this.Name = "Main";
			this.Text = "Logger";
			((System.ComponentModel.ISupportInitialize)(this.temperatureChart)).EndInit();
			this.chartMenuStrip.ResumeLayout(false);
			this.tabControl1.ResumeLayout(false);
			this.logChartTabPage.ResumeLayout(false);
			this.homeTabPage.ResumeLayout(false);
			this.homeTabPage.PerformLayout();
			this.logFilesTabPage.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button setDateTimeButton;
        private System.Windows.Forms.ComboBox portComboBox;
        private System.Windows.Forms.Button resetButton;
        private System.Windows.Forms.Label portComboLabel;
        private System.Windows.Forms.Button getTemperatureButton;
        private System.Windows.Forms.Label tempLabel;
        private System.Windows.Forms.Button getTemperatureLogButton;
        private System.Windows.Forms.DataVisualization.Charting.Chart temperatureChart;
		private System.Windows.Forms.ContextMenuStrip chartMenuStrip;
		private System.Windows.Forms.ToolStripMenuItem printMenuItem;
		private System.Windows.Forms.Button getDirectoryButton;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage homeTabPage;
		private System.Windows.Forms.TabPage logChartTabPage;
		private System.Windows.Forms.TabPage logFilesTabPage;
		private System.Windows.Forms.ListView logFileValuesListView;
		private System.Windows.Forms.ListView logFilesListView;
		private System.Windows.Forms.ColumnHeader File;
		private System.Windows.Forms.ColumnHeader Values;
    }
}

